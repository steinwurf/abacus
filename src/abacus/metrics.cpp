// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <numeric>

#include "detail/raw.hpp"
#include "metrics.hpp"

#include <endian/is_big_endian.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

metrics::metrics(const metric_info* info, std::size_t size,
                 std::size_t max_scope_bytes) :
    m_info(info, size),
    m_max_scope_bytes(max_scope_bytes)
{
    assert(size > 0);
    assert(size <= std::numeric_limits<uint16_t>::max());

    m_count = static_cast<uint16_t>(m_info.size());

    for (std::size_t i = 0; i < m_count; i++)
    {
        m_name_to_index.insert({m_info[i].name, i});
    }

    m_name_sizes.reserve(m_count);
    m_description_sizes.reserve(m_count);

    // Get the sizes of all the metric names
    for (std::size_t i = 0; i < m_info.m_eight_byte_count; i++)
    {
        m_name_sizes.push_back((uint16_t)(m_info[i].name.size() + 1));
        m_description_sizes.push_back(
            (uint16_t)((m_info[i].description.size() + 1)));
    }

    // Get the sizes of all the metric descriptions
    for (std::size_t i = 0; i < m_info.m_one_byte_count; i++)
    {
        m_name_sizes.push_back(
            (uint16_t)(m_info[i + m_info.m_eight_byte_count].name.size() + 1));
        m_description_sizes.push_back((uint16_t)((
            m_info[i + m_info.m_eight_byte_count].description.size() + 1)));
    }

    // Calculate the total number of bytes needed for the names
    uint16_t name_bytes = 0;
    uint16_t description_bytes = 0;
    for (uint16_t i = 0; i < m_count; ++i)
    {
        name_bytes += m_name_sizes[i];
        description_bytes += m_description_sizes[i];
    }

    // Calculate the total memory needed.

    // First calculate the total size of header + name_sizes + description_sizes
    // + names + descriptions + type-enum + is_constant-bools
    m_storage_bytes = detail::header_bytes() +
                      (2 * sizeof(uint16_t) * m_count) + name_bytes +
                      description_bytes + 2 * m_count;

    // Add padding to ensure alignment for the values.
    if (m_storage_bytes % 8 != 0)
    {
        m_storage_bytes += detail::alignment_padding(m_storage_bytes);
    }

    // Finally, add the bytes needed for the values.
    m_storage_bytes += 8 * m_info.m_eight_byte_count + m_info.m_one_byte_count;

    m_storage_bytes +=
        detail::alignment_padding(m_storage_bytes) + m_max_scope_bytes;

    // Allocate the memory needed.
    m_data = static_cast<uint8_t*>(::operator new(m_storage_bytes));

    // Make sure that the data is 8byte-aligned to append the scope to the
    // buffer with copy_storage()
    assert(reinterpret_cast<uint64_t>(m_data) % 8U == 0U);

    // Zero out all memory
    std::memset(m_data, 0, m_storage_bytes);

    // Write the header
    //
    // The number of the metrics
    new (m_data) uint16_t(m_count);
    // The current size of the scope in bytes
    new (m_data + 2) uint16_t(0);
    // The endianness of the data, 0 for little-endian, 1 for big-endian
    new (m_data + 4) uint8_t(endian::is_big_endian());
    // The total bytes used for names
    new (m_data + 6) uint16_t(name_bytes);
    // The total bytes used for descriptions
    new (m_data + 8) uint16_t(description_bytes);
    // The number of 8-byte metric values (uint64_t, int64_t and double types)
    new (m_data + 10) uint16_t(m_info.m_eight_byte_count);
    // The number of 1-byte metric values (bool type)
    new (m_data + 12) uint16_t(m_info.m_one_byte_count);

    assert((reinterpret_cast<uint64_t>(m_data + detail::values_offset(m_data)) %
            8U) == 0U);

    // Write the name sizes into memory
    uint8_t* name_sizes_ptr = m_data + detail::name_sizes_offset();
    for (std::size_t i = 0; i < m_count; ++i)
    {
        std::memcpy(name_sizes_ptr, &m_name_sizes[i], sizeof(uint16_t));
        name_sizes_ptr += sizeof(uint16_t);
    }

    // Write the description sizes into memory
    uint8_t* description_sizes_ptr =
        m_data + detail::description_sizes_offset(m_data);

    for (std::size_t i = 0; i < m_count; ++i)
    {

        std::memcpy(description_sizes_ptr, &m_description_sizes[i],
                    sizeof(uint16_t));
        description_sizes_ptr += sizeof(uint16_t);
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
    return m_count;
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
    assert(index < m_count);
    return detail::is_metric_initialized(m_data, index);
}

auto metrics::metric_name(std::size_t index) const -> std::string
{
    assert(index < m_count);
    return m_info[index].name;
}

auto metrics::metric_description(std::size_t index) const -> std::string
{
    assert(index < m_count);
    return m_info[index].description;
}

auto metrics::is_metric_boolean(std::size_t index) const -> bool
{
    assert(index < m_count);
    return m_info[index].type == metric_type::boolean;
}

auto metrics::is_metric_uint64(std::size_t index) const -> bool
{
    assert(index < m_count);
    return m_info[index].type == metric_type::uint64;
}

auto metrics::is_metric_int64(std::size_t index) const -> bool
{
    assert(index < m_count);
    return m_info[index].type == metric_type::int64;
}

auto metrics::is_metric_float64(std::size_t index) const -> bool
{
    assert(index < m_count);
    return m_info[index].type == metric_type::float64;
}

auto metrics::metric_is_constant(std::size_t index) const -> bool
{
    assert(index < m_count);
    return static_cast<bool>(m_info[index].is_constant);
}

void metrics::initialize_constant(std::string name, uint64_t value) const
{
    std::size_t index = metric_index(name);
    assert(index < m_count);
    assert(!is_metric_initialized(index));
    assert(m_info[index].type == metric_type::uint64);
    assert(metric_is_constant(index));
    assert(name == metric_name(index));

    // Write the metric name given to the constructor into memory
    char* name_ptr = detail::raw_name(m_data, index);
    std::memcpy(name_ptr, metric_name(index).c_str(), m_name_sizes[index]);

    // Write the metric description given to the constructor into memory
    char* description_ptr = detail::raw_description(m_data, index);
    std::memcpy(description_ptr, metric_description(index).c_str(),
                m_description_sizes[index]);

    // Write the metric type given to the constructor into memory
    uint8_t* types_ptr = m_data + detail::types_offset(m_data) + index;
    uint8_t type_byte = static_cast<uint8_t>(m_info[index].type);
    *types_ptr = type_byte;

    // Write the is_constant bool given to the constructor into memory
    uint8_t* is_constant_ptr =
        m_data + detail::is_constant_offset(m_data) + index;
    *(bool*)is_constant_ptr = metric_is_constant(index);

    uint64_t* value_ptr = (uint64_t*)detail::raw_value(m_data, index);

    *value_ptr = value;
}

void metrics::initialize_constant(std::string name, int64_t value) const
{
    std::size_t index = metric_index(name);
    assert(index < m_count);
    assert(!is_metric_initialized(index));
    assert(m_info[index].type == metric_type::int64);
    assert(metric_is_constant(index));
    assert(name == metric_name(index));

    // Write the metric name given to the constructor into memory
    char* name_ptr = detail::raw_name(m_data, index);
    std::memcpy(name_ptr, metric_name(index).c_str(), m_name_sizes[index]);

    // Write the metric description given to the constructor into memory
    char* description_ptr = detail::raw_description(m_data, index);
    std::memcpy(description_ptr, metric_description(index).c_str(),
                m_description_sizes[index]);

    // Write the metric type given to the constructor into memory
    uint8_t* types_ptr = m_data + detail::types_offset(m_data) + index;
    uint8_t type_byte = static_cast<uint8_t>(m_info[index].type);
    *types_ptr = type_byte;

    // Write the is_constant bool given to the constructor into memory
    uint8_t* is_constant_ptr =
        m_data + detail::is_constant_offset(m_data) + index;
    *(bool*)is_constant_ptr = metric_is_constant(index);

    int64_t* value_ptr = (int64_t*)detail::raw_value(m_data, index);

    *value_ptr = value;
}

void metrics::initialize_constant(std::string name, double value) const
{
    std::size_t index = metric_index(name);
    assert(index < m_count);
    assert(!is_metric_initialized(index));
    assert(m_info[index].type == metric_type::float64);
    assert(metric_is_constant(index));
    assert(name == metric_name(index));

    // Write the metric name given to the constructor into memory
    char* name_ptr = detail::raw_name(m_data, index);
    std::memcpy(name_ptr, metric_name(index).c_str(), m_name_sizes[index]);

    // Write the metric description given to the constructor into memory
    char* description_ptr = detail::raw_description(m_data, index);
    std::memcpy(description_ptr, metric_description(index).c_str(),
                m_description_sizes[index]);

    // Write the metric type given to the constructor into memory
    uint8_t* types_ptr = m_data + detail::types_offset(m_data) + index;
    uint8_t type_byte = static_cast<uint8_t>(m_info[index].type);
    *types_ptr = type_byte;

    // Write the is_constant bool given to the constructor into memory
    uint8_t* is_constant_ptr =
        m_data + detail::is_constant_offset(m_data) + index;
    *(bool*)is_constant_ptr = metric_is_constant(index);

    double* value_ptr = (double*)detail::raw_value(m_data, index);

    *value_ptr = value;
}

void metrics::initialize_constant(std::string name, bool value) const
{
    std::size_t index = metric_index(name);
    assert(index < m_count);
    assert(!is_metric_initialized(index));
    assert(m_info[index].type == metric_type::boolean);
    assert(metric_is_constant(index));
    assert(name == metric_name(index));

    // Write the metric name given to the constructor into memory
    char* name_ptr = detail::raw_name(m_data, index);
    std::memcpy(name_ptr, metric_name(index).c_str(), m_name_sizes[index]);

    // Write the metric description given to the constructor into memory
    char* description_ptr = detail::raw_description(m_data, index);
    std::memcpy(description_ptr, metric_description(index).c_str(),
                m_description_sizes[index]);

    // Write the metric type given to the constructor into memory
    uint8_t* types_ptr = m_data + detail::types_offset(m_data) + index;
    uint8_t type_byte = static_cast<uint8_t>(m_info[index].type);
    *types_ptr = type_byte;

    // Write the is_constant bool given to the constructor into memory
    uint8_t* is_constant_ptr =
        m_data + detail::is_constant_offset(m_data) + index;
    *(bool*)is_constant_ptr = metric_is_constant(index);

    bool* value_ptr = (bool*)detail::raw_value(m_data, index);

    *value_ptr = value;
}

void metrics::metric_value(std::size_t index, uint64_t& value) const
{
    assert(index < m_count);
    assert(is_metric_initialized(index));
    assert(m_info[index].type == metric_type::uint64);

    uint64_t* value_ptr = (uint64_t*)detail::raw_value(m_data, index);

    value = *value_ptr;
}

void metrics::metric_value(std::size_t index, int64_t& value) const
{
    assert(index < m_count);
    assert(is_metric_initialized(index));
    assert(m_info[index].type == metric_type::int64);

    int64_t* value_ptr = (int64_t*)detail::raw_value(m_data, index);

    value = *value_ptr;
}

void metrics::metric_value(std::size_t index, double& value) const
{
    assert(index < m_count);
    assert(is_metric_initialized(index));
    assert(m_info[index].type == metric_type::float64);

    double* value_ptr = (double*)detail::raw_value(m_data, index);

    value = *value_ptr;
}

void metrics::metric_value(std::size_t index, bool& value) const
{
    assert(index < m_count);
    assert(is_metric_initialized(index));
    assert(m_info[index].type == metric_type::boolean);

    bool* value_ptr = (bool*)detail::raw_value(m_data, index);

    value = *value_ptr;
}

auto metrics::metric_index(std::string name) const -> std::size_t
{
    auto it = m_name_to_index.find(name);

    assert((it != m_name_to_index.end()) && "No metric has the given name.");

    std::size_t index = it->second;

    return index;
}

auto metrics::scope() const -> std::string
{
    return m_scope;
}

auto metrics::scope_size() const -> uint16_t
{
    return detail::scope_size(m_data);
}

void metrics::push_scope(const std::string& text)
{
    if (m_scope.size() == 0)
    {
        m_scope = text;
    }
    else
    {
        m_scope = text + "." + m_scope;
    }

    m_scopes.push_back(text);

    // Update the scope size
    uint8_t scope_size = (uint8_t)m_scope.size() + 1;

    assert(scope_size <= m_max_scope_bytes);

    uint16_t* scope_size_data = detail::raw_scope_size(m_data);
    *scope_size_data = scope_size;

    char* scope_data = detail::raw_scope(m_data);

    std::memcpy(scope_data, m_scope.c_str(), scope_size);
}

void metrics::pop_scope()
{
    assert(!m_scopes.empty());
    if (m_scopes.size() == 1)
    {
        m_scope = "";
        m_scopes.pop_back();
        uint16_t* scope_size_data = detail::raw_scope_size(m_data);
        *scope_size_data = 0U;
        char* scope_data = detail::raw_scope(m_data);
        memset(scope_data, 0, m_max_scope_bytes);
        return;
    }
    std::size_t last_scope_size = m_scopes.back().size();
    uint16_t updated_scope_size = scope_size() - last_scope_size;
    m_scope = m_scope.substr(last_scope_size, updated_scope_size);
    m_scopes.pop_back();

    // Update the scope size
    uint16_t* scope_size_data = detail::raw_scope_size(m_data);
    *scope_size_data = updated_scope_size;

    char* scope_data = detail::raw_scope(m_data);

    std::memcpy(scope_data, m_scope.c_str(), updated_scope_size);
}

void metrics::copy_storage(uint8_t* data, std::size_t size) const
{
    assert(data != nullptr);
    assert(size == storage_bytes());
    std::memcpy(data, m_data, storage_bytes());
}

auto metrics::storage_bytes() const -> std::size_t
{
    auto offset = detail::scope_offset(m_data);
    offset += detail::scope_size(m_data);
    offset += detail::alignment_padding(offset);
    return offset;
}

void metrics::reset_metric(std::size_t index)
{
    assert(index < m_count);
    assert(is_metric_initialized(index));
    assert(!metric_is_constant(index));

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
    }
}

void metrics::reset_metrics()
{
    for (std::size_t index = 0; index < m_count; ++index)
    {
        if (!is_metric_initialized(index))
        {
            continue;
        }
        if (!metric_is_constant(index))
        {
            reset_metric(index);
        }
    }
}

auto metrics::initialize(std::string name) const -> void*
{
    std::size_t index = metric_index(name);
    assert(index < m_count);
    assert(!is_metric_initialized(index));
    assert(!metric_is_constant(index));
    assert(name == metric_name(index));

    // Write the metric name given to the constructor into memory
    char* name_ptr = detail::raw_name(m_data, index);
    std::memcpy(name_ptr, metric_name(index).c_str(), m_name_sizes[index]);

    // Write the metric description given to the constructor into memory
    char* description_ptr = detail::raw_description(m_data, index);
    std::memcpy(description_ptr, metric_description(index).c_str(),
                m_description_sizes[index]);

    // Write the metric type given to the constructor into memory
    uint8_t* types_ptr = m_data + detail::types_offset(m_data) + index;
    uint8_t type_byte = static_cast<uint8_t>(m_info[index].type);
    *types_ptr = type_byte;

    // Write the is_constant bool given to the constructor into memory
    uint8_t* is_constant_ptr =
        m_data + detail::is_constant_offset(m_data) + index;
    *(bool*)is_constant_ptr = metric_is_constant(index);

    void* value_ptr = detail::raw_value(m_data, index);
    return value_ptr;
}
}
}
