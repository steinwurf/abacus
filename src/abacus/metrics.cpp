// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cassert>
#include <cstdint>
#include <cstring>

#include "detail/raw.hpp"
#include "metric_flags.hpp"
#include "metric_info.hpp"
#include "metric_type.hpp"
#include "metrics.hpp"

#include <endian/is_big_endian.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

metrics::metrics(const metric_info* info, std::size_t count) :
    m_info(info, count)
{
    assert(count > 0);
    assert(count <= std::numeric_limits<uint16_t>::max());

    uint16_t name_bytes = 0;
    uint16_t description_bytes = 0;

    // First calculate the total size of header
    std::size_t meta_bytes = detail::header_bytes();
    for (std::size_t i = 0; i < m_info.count(); i++)
    {
        const auto& info = m_info[i];
        m_name_to_index.emplace(info.name, i);
        name_bytes += info.name.size();
        description_bytes += info.description.size();

        // name_size
        meta_bytes += sizeof(uint16_t);
        // description size
        meta_bytes += sizeof(uint16_t);
        // name
        meta_bytes += info.name.size();
        // description
        meta_bytes += info.description.size();
        // type
        meta_bytes += sizeof(uint8_t);
        // flags
        meta_bytes += sizeof(uint8_t);
    }
    // Add padding to ensure alignment for the values.
    std::size_t alignment = detail::alignment_padding(meta_bytes);

    std::size_t value_bytes = 0;
    // Add the bytes needed for the values.
    value_bytes +=
        8 * m_info.eight_byte_metrics_count() + m_info.one_byte_metrics_count();

    // Finally add the bytes needed for the initalized bitmap
    value_bytes += (m_info.count() + 7) / 8;

    std::size_t storage_bytes = meta_bytes + alignment + value_bytes;

    // Allocate the memory needed.
    m_meta_data = static_cast<uint8_t*>(::operator new(storage_bytes));

    // Make sure that the data is 8-byte-aligned
    assert(reinterpret_cast<uint64_t>(m_meta_data) % 8U == 0U);

    // Zero out all memory
    std::memset(m_meta_data, 0, storage_bytes);

    // Write the header
    // The endianness of the data, 0 for little-endian, 1 for big-endian
    new (m_meta_data) uint8_t(endian::is_big_endian());
    // The total bytes used for names
    new (m_meta_data + 2) uint16_t(name_bytes);
    // The total bytes used for descriptions
    new (m_meta_data + 4) uint16_t(description_bytes);
    // The number of 8-byte metric values (uint64_t, int64_t and double types)
    new (m_meta_data + 6) uint16_t(m_info.eight_byte_metrics_count());
    // The number of 1-byte metric values (bool type)
    new (m_meta_data + 8) uint16_t(m_info.one_byte_metrics_count());

    // Write the name sizes into memory
    uint8_t* name_sizes_ptr = m_meta_data + detail::name_sizes_offset();

    // Write the description sizes into memory
    uint8_t* description_sizes_ptr =
        m_meta_data + detail::description_sizes_offset(m_meta_data);

    // Write the names into memory
    uint8_t* names_ptr = m_meta_data + detail::names_offset(m_meta_data);

    // Write the descriptions into memory
    uint8_t* descriptions_ptr =
        m_meta_data + detail::descriptions_offset(m_meta_data);

    // Write the types into memory
    uint8_t* types_ptr = m_meta_data + detail::types_offset(m_meta_data);

    // Write the flags into memory
    uint8_t* flags_ptr = m_meta_data + detail::flags_offset(m_meta_data);

    for (std::size_t i = 0; i < m_info.count(); i++)
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

        std::memcpy(flags_ptr, &info.flags, sizeof(info.flags));
        flags_ptr += sizeof(info.flags);

        std::memcpy(types_ptr, &info.type, sizeof(info.type));
        types_ptr += sizeof(info.type);
    }

    m_value_data = m_meta_data + meta_bytes + alignment;

    assert(storage_bytes ==
           (detail::meta_bytes(m_meta_data) +
            detail::alignment_padding(detail::meta_bytes(m_meta_data)) +
            detail::value_bytes(m_meta_data)));
}

metrics::~metrics()
{
    ::operator delete(m_meta_data);
}

auto metrics::meta_data() const -> const uint8_t*
{
    return m_meta_data;
}

auto metrics::meta_bytes() const -> std::size_t
{
    return detail::meta_bytes(m_meta_data);
}

auto metrics::value_data() const -> const uint8_t*
{
    return m_value_data;
}

auto metrics::value_bytes() const -> std::size_t
{
    return detail::value_bytes(m_meta_data);
}

auto metrics::metric_count() const -> std::size_t
{
    return m_info.count();
}

auto metrics::is_metric_initialized(std::size_t index) const -> bool
{
    assert(index < metric_count());
    return detail::is_metric_initialized(m_meta_data, m_value_data, index);
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
    return (bool)(m_info[index].flags & metric_flags::constant);
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
    value = *static_cast<uint64_t*>(
        detail::value_ptr(m_meta_data, m_value_data, index));
}

void metrics::metric_value(std::size_t index, int64_t& value) const
{
    assert(index < metric_count());
    assert(is_metric_initialized(index));
    assert(is_metric_int64(index));
    value = *static_cast<int64_t*>(
        detail::value_ptr(m_meta_data, m_value_data, index));
}

void metrics::metric_value(std::size_t index, double& value) const
{
    assert(index < metric_count());
    assert(is_metric_initialized(index));
    assert(is_metric_float64(index));
    value = *static_cast<double*>(
        detail::value_ptr(m_meta_data, m_value_data, index));
}

void metrics::metric_value(std::size_t index, bool& value) const
{
    assert(index < metric_count());
    assert(is_metric_initialized(index));
    assert(is_metric_boolean(index));
    value = *static_cast<bool*>(
        detail::value_ptr(m_meta_data, m_value_data, index));
}

auto metrics::metric_index(const std::string& name) const -> std::size_t
{
    auto it = m_name_to_index.find(name);
    assert(it != m_name_to_index.end() && "metric name not found.");
    return it->second;
}

void metrics::reset_metric(std::size_t index)
{
    assert(index < metric_count());
    assert(is_metric_initialized(index));
    assert(!is_metric_constant(index));

    switch (detail::type(m_meta_data, index))
    {
    case metric_type::boolean:
    {
        bool* value_data = static_cast<bool*>(
            detail::value_ptr(m_meta_data, m_value_data, index));
        *value_data = false;
        break;
    }
    case metric_type::uint64:
    {
        uint64_t* value_data = static_cast<uint64_t*>(
            detail::value_ptr(m_meta_data, m_value_data, index));
        *value_data = 0U;
        break;
    }
    case metric_type::int64:
    {
        int64_t* value_data = static_cast<int64_t*>(
            detail::value_ptr(m_meta_data, m_value_data, index));
        *value_data = 0;
        break;
    }
    case metric_type::float64:
    {
        double* value_data = static_cast<double*>(
            detail::value_ptr(m_meta_data, m_value_data, index));
        *value_data = 0.0;
        break;
    }
    default:
        assert(false && "Invalid metric type.");
    }
}

void metrics::reset_metrics()
{
    memset(m_value_data, 0, detail::metrics_bytes(m_meta_data));
}

auto metrics::initialize(std::size_t index) const -> void*
{
    assert(index < metric_count());
    assert(!is_metric_initialized(index));
    // Write that the metric has been initialized into memory
    detail::set_metric_initialized(m_meta_data, m_value_data, index);
    assert(is_metric_initialized(index));
    return detail::value_ptr(m_meta_data, m_value_data, index);
}
}
}
