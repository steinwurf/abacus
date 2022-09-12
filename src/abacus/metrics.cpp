// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <numeric>

#include "detail/raw.hpp"
#include "metrics.hpp"

#include <endian/is_big_endian.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

metrics::metrics(const metric_info* info, std::size_t size) : m_info(info, size)
{
    assert(size > 0);
    assert(size <= std::numeric_limits<uint16_t>::max());

    uint16_t name_bytes = 0;
    uint16_t description_bytes = 0;

    // First calculate the total size of header
    std::size_t storage_bytes = detail::header_bytes();
    for (std::size_t i = 0; i < m_info.size(); i++)
    {
        const auto& info = m_info[i];
        m_name_to_index.insert({info.name, i});
        name_bytes += info.name.size();
        description_bytes += info.description.size();

        // name_size
        storage_bytes += sizeof(uint16_t);
        // description size
        storage_bytes += sizeof(uint16_t);
        // name
        storage_bytes += info.name.size();
        // description
        storage_bytes += info.description.size();
        // type
        storage_bytes += sizeof(uint8_t);
        // is_constant
        storage_bytes += sizeof(uint8_t);
    }

    // Add padding to ensure alignment for the values.
    if (storage_bytes % 8 != 0)
    {
        storage_bytes += detail::alignment_padding(storage_bytes);
    }

    // Finally, add the bytes needed for the values.
    storage_bytes +=
        8 * m_info.eight_byte_metrics_count() + m_info.one_byte_metrics_count();

    // Allocate the memory needed.
    m_data = static_cast<uint8_t*>(::operator new(storage_bytes));

    // Make sure that the data is 8byte-aligned
    assert(reinterpret_cast<uint64_t>(m_data) % 8U == 0U);

    // Zero out all memory
    std::memset(m_data, 0, storage_bytes);

    // Write the header
    // The number of the metrics
    new (m_data) uint16_t(static_cast<uint16_t>(m_info.size()));
    // The endianness of the data, 0 for little-endian, 1 for big-endian
    new (m_data + 2) uint8_t(endian::is_big_endian());
    // The total bytes used for names
    new (m_data + 4) uint16_t(name_bytes);
    // The total bytes used for descriptions
    new (m_data + 6) uint16_t(description_bytes);
    // The number of 8-byte metric values (uint64_t, int64_t and double types)
    new (m_data + 8) uint16_t(m_info.eight_byte_metrics_count());
    // The number of 1-byte metric values (bool type)
    new (m_data + 10) uint16_t(m_info.one_byte_metrics_count());

    assert((reinterpret_cast<uint64_t>(m_data + detail::values_offset(m_data)) %
            8U) == 0U);

    // Write the name sizes into memory
    uint8_t* name_sizes_ptr = m_data + detail::name_sizes_offset();

    // Write the description sizes into memory
    uint8_t* description_sizes_ptr =
        m_data + detail::description_sizes_offset(m_data);

    // Write the names into memory
    uint8_t* names_ptr = m_data + detail::names_offset(m_data);

    // Write the descriptions into memory
    uint8_t* descriptions_ptr = m_data + detail::descriptions_offset(m_data);

    // Write the types into memory
    uint8_t* types_ptr = m_data + detail::types_offset(m_data);

    // Write the is_constant into memory
    uint8_t* is_constant_ptr = m_data + detail::is_constant_offset(m_data);

    for (std::size_t i = 0; i < m_info.size(); i++)
    {
        const auto& info = m_info[i];
        uint16_t name_size = static_cast<uint16_t>(info.name.size());
        uint16_t description_size =
            static_cast<uint16_t>(info.description.size());
        std::memcpy(name_sizes_ptr, &name_size, sizeof(name_size));
        name_sizes_ptr += sizeof(name_size);

        std::memcpy(description_sizes_ptr, &description_size,
                    sizeof(description_size));
        description_sizes_ptr += sizeof(description_size);

        std::memcpy(names_ptr, info.name.data(), info.name.size());
        names_ptr += info.name.size();

        std::memcpy(descriptions_ptr, info.description.data(),
                    info.description.size());
        descriptions_ptr += info.description.size();

        std::memcpy(is_constant_ptr, &info.is_constant,
                    sizeof(info.is_constant));
        is_constant_ptr += sizeof(info.is_constant);

        auto type = metric_type::uninitialized;
        std::memcpy(types_ptr, &type, sizeof(type));
        types_ptr += sizeof(type);
    }
}

metrics::~metrics()
{
    ::operator delete(m_data);
}

auto metrics::data() const -> const uint8_t*
{
    return m_data;
}

auto metrics::metric_count() const -> std::size_t
{
    return m_info.size();
}

auto metrics::name_bytes() const -> uint16_t
{
    return detail::name_bytes(m_data);
}

auto metrics::description_bytes() const -> uint16_t
{
    return detail::descriptions_bytes(m_data);
}

auto metrics::is_metric_initialized(std::size_t index) const -> bool
{
    assert(index < metric_count());
    return detail::is_metric_initialized(m_data, index);
}

auto metrics::metric_name(std::size_t index) const -> std::string
{
    assert(index < metric_count());
    return m_info[index].name;
}

auto metrics::metric_description(std::size_t index) const -> std::string
{
    assert(index < metric_count());
    return m_info[index].description;
}

auto metrics::is_metric_boolean(std::size_t index) const -> bool
{
    assert(index < metric_count());
    return m_info[index].type == metric_type::boolean;
}

auto metrics::is_metric_uint64(std::size_t index) const -> bool
{
    assert(index < metric_count());
    return m_info[index].type == metric_type::uint64;
}

auto metrics::is_metric_int64(std::size_t index) const -> bool
{
    assert(index < metric_count());
    return m_info[index].type == metric_type::int64;
}

auto metrics::is_metric_float64(std::size_t index) const -> bool
{
    assert(index < metric_count());
    return m_info[index].type == metric_type::float64;
}

auto metrics::is_metric_constant(std::size_t index) const -> bool
{
    assert(index < metric_count());
    return m_info[index].is_constant == qualifier::constant;
}

void metrics::initialize_constant(const std::string& name, uint64_t value) const
{
    auto index = metric_index(name);
    assert(is_metric_constant(index));
    assert(is_metric_uint64(index));
    *static_cast<uint64_t*>(initialize(index)) = value;
}

void metrics::initialize_constant(const std::string& name, int64_t value) const
{
    auto index = metric_index(name);
    assert(is_metric_constant(index));
    assert(is_metric_int64(index));
    *static_cast<int64_t*>(initialize(index)) = value;
}

void metrics::initialize_constant(const std::string& name, double value) const
{
    auto index = metric_index(name);
    assert(is_metric_constant(index));
    assert(is_metric_float64(index));
    *static_cast<double*>(initialize(index)) = value;
}

void metrics::initialize_constant(const std::string& name, bool value) const
{
    auto index = metric_index(name);
    assert(is_metric_constant(index));
    assert(is_metric_boolean(index));
    *static_cast<bool*>(initialize(index)) = value;
}

void metrics::metric_value(std::size_t index, uint64_t& value) const
{
    assert(index < metric_count());
    assert(is_metric_initialized(index));
    assert(is_metric_uint64(index));
    value = *static_cast<uint64_t*>(detail::raw_value(m_data, index));
}

void metrics::metric_value(std::size_t index, int64_t& value) const
{
    assert(index < metric_count());
    assert(is_metric_initialized(index));
    assert(is_metric_int64(index));
    value = *static_cast<int64_t*>(detail::raw_value(m_data, index));
}

void metrics::metric_value(std::size_t index, double& value) const
{
    assert(index < metric_count());
    assert(is_metric_initialized(index));
    assert(is_metric_float64(index));
    value = *static_cast<double*>(detail::raw_value(m_data, index));
}

void metrics::metric_value(std::size_t index, bool& value) const
{
    assert(index < metric_count());
    assert(is_metric_initialized(index));
    assert(is_metric_boolean(index));
    value = *static_cast<bool*>(detail::raw_value(m_data, index));
}

auto metrics::metric_index(const std::string& name) const -> std::size_t
{
    auto it = m_name_to_index.find(name);
    assert(it != m_name_to_index.end() && "metric name not found.");
    return it->second;
}

void metrics::copy_storage(uint8_t* data, std::size_t size) const
{
    assert(data != nullptr);
    assert(size == storage_bytes());
    std::memcpy(data, m_data, storage_bytes());
}

auto metrics::storage_bytes() const -> std::size_t
{
    return detail::storage_bytes(m_data);
}

void metrics::reset_metric(std::size_t index)
{
    assert(index < metric_count());
    assert(is_metric_initialized(index));
    assert(!is_metric_constant(index));

    metric_type type =
        static_cast<metric_type>(*detail::raw_type(m_data, index));

    switch (type)
    {
    case metric_type::boolean:
    {
        bool* value_data = (bool*)detail::raw_value(m_data, index);
        *value_data = false;
        break;
    }
    case metric_type::uint64:
    {
        uint64_t* value_data = (uint64_t*)detail::raw_value(m_data, index);
        *value_data = 0U;
        break;
    }
    case metric_type::int64:
    {
        int64_t* value_data = (int64_t*)detail::raw_value(m_data, index);
        *value_data = 0;
        break;
    }
    case metric_type::float64:
    {
        double* value_data = (double*)detail::raw_value(m_data, index);
        *value_data = 0.0;
        break;
    }
    case metric_type::uninitialized:
    default:
        assert(false && "Invalid metric type.");
    }
}

void metrics::reset_metrics()
{
    auto values_offset = detail::values_offset(m_data);
    memset(m_data + values_offset, 0, storage_bytes() - values_offset);
}

auto metrics::initialize(std::size_t index) const -> void*
{
    assert(index < metric_count());
    assert(!is_metric_initialized(index));

    // Write the metric type into memory, this will mark the metric as
    // initialized.
    uint8_t* types_ptr = m_data + detail::types_offset(m_data) + index;
    uint8_t type_byte = static_cast<uint8_t>(m_info[index].type);
    *types_ptr = type_byte;

    void* value_ptr = detail::raw_value(m_data, index);
    return value_ptr;
}
}
}
