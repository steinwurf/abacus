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

#include "metrics.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

metrics::metrics(std::vector<metric_info> info) : m_info(info)
{
    assert(m_info.size() > 0);
    assert(m_info.size() <= std::numeric_limits<uint16_t>::max());

    m_count = static_cast<uint16_t>(m_info.size());

    for (std::size_t i = 0; i < m_count; i++)
    {
        m_name_to_index.insert({m_info[i].name, i});
    }

    m_name_sizes.reserve(m_count);
    m_description_sizes.reserve(m_count);

    // Get the sizes of all the metric names
    for (auto info : m_info.eight_byte_metrics)
    {
        m_name_sizes.push_back((uint16_t)(info.name.size() + 1));
        m_description_sizes.push_back((uint16_t)(info.description.size() + 1));
    }

    // Get the sizes of all the metric descriptions
    for (auto info : m_info.one_byte_metrics)
    {
        m_name_sizes.push_back((uint16_t)(info.name.size() + 1));
        m_description_sizes.push_back((uint16_t)(info.description.size() + 1));
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
    m_storage_bytes +=
        8 * m_info.eight_byte_metrics.size() + m_info.one_byte_metrics.size();

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
    // The total bytes used for names
    new (m_data + 4) uint16_t(name_bytes);
    // The total bytes used for descriptions
    new (m_data + 6) uint16_t(description_bytes);
    // The number of 8-byte metric values (uint64_t, int64_t and double types)
    new (m_data + 8) uint16_t(m_info.eight_byte_metrics.size());
    // The number of 1-byte metric values (bool type)
    new (m_data + 10) uint16_t(m_info.one_byte_metrics.size());

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

auto metrics::eight_byte_count() const -> uint16_t
{
    return detail::eight_byte_count(m_data);
}

auto metrics::one_byte_count() const -> uint16_t
{
    return detail::one_byte_count(m_data);
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

auto metrics::metric_type(std::size_t index) const -> value_type
{
    assert(index < m_count);
    return m_info[index].type;
}

auto metrics::metric_is_constant(std::size_t index) const -> bool
{
    assert(index < m_count);
    return m_info[index].is_constant;
}

void metrics::initialize_constant(std::size_t index, uint64_t value,
                                  std::string name) const
{
    assert(index < m_count);
    assert(!is_metric_initialized(index));
    assert(metric_type(index) == value_type::unsigned_integral);
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
    uint8_t type_byte = static_cast<uint8_t>(metric_type(index));
    *types_ptr = type_byte;

    // Write the is_constant bool given to the constructor into memory
    uint8_t* is_constant_ptr =
        m_data + detail::is_constant_offset(m_data) + index;
    *(bool*)is_constant_ptr = metric_is_constant(index);

    uint64_t* value_ptr = detail::raw_value<uint64_t>(m_data, index);

    *value_ptr = value;
}

void metrics::initialize_constant(std::size_t index, int64_t value,
                                  std::string name) const
{
    assert(index < m_count);
    assert(!is_metric_initialized(index));
    assert(metric_type(index) == value_type::signed_integral);
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
    uint8_t type_byte = static_cast<uint8_t>(metric_type(index));
    *types_ptr = type_byte;

    // Write the is_constant bool given to the constructor into memory
    uint8_t* is_constant_ptr =
        m_data + detail::is_constant_offset(m_data) + index;
    *(bool*)is_constant_ptr = metric_is_constant(index);

    int64_t* value_ptr = detail::raw_value<int64_t>(m_data, index);

    *value_ptr = value;
}

void metrics::initialize_constant(std::size_t index, double value,
                                  std::string name) const
{
    assert(index < m_count);
    assert(!is_metric_initialized(index));
    assert(metric_type(index) == value_type::floating_point);
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
    uint8_t type_byte = static_cast<uint8_t>(metric_type(index));
    *types_ptr = type_byte;

    // Write the is_constant bool given to the constructor into memory
    uint8_t* is_constant_ptr =
        m_data + detail::is_constant_offset(m_data) + index;
    *(bool*)is_constant_ptr = metric_is_constant(index);

    double* value_ptr = detail::raw_value<double>(m_data, index);

    *value_ptr = value;
}

void metrics::initialize_constant(std::size_t index, bool value,
                                  std::string name) const
{
    assert(index < m_count);
    assert(!is_metric_initialized(index));
    assert(metric_type(index) == value_type::boolean);
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
    uint8_t type_byte = static_cast<uint8_t>(metric_type(index));
    *types_ptr = type_byte;

    // Write the is_constant bool given to the constructor into memory
    uint8_t* is_constant_ptr =
        m_data + detail::is_constant_offset(m_data) + index;
    *(bool*)is_constant_ptr = metric_is_constant(index);

    bool* value_ptr = detail::raw_value<bool>(m_data, index);

    *value_ptr = value;
}

void metrics::metric_value(std::size_t index, uint64_t& value) const
{
    assert(index < m_count);
    assert(is_metric_initialized(index));
    assert(metric_type(index) == value_type::unsigned_integral);

    uint64_t* value_ptr = detail::raw_value<uint64_t>(m_data, index);

    value = *value_ptr;
}

void metrics::metric_value(std::size_t index, int64_t& value) const
{
    assert(index < m_count);
    assert(is_metric_initialized(index));
    assert(metric_type(index) == value_type::signed_integral);

    int64_t* value_ptr = detail::raw_value<int64_t>(m_data, index);

    value = *value_ptr;
}

void metrics::metric_value(std::size_t index, double& value) const
{
    assert(index < m_count);
    assert(is_metric_initialized(index));
    assert(metric_type(index) == value_type::floating_point);

    double* value_ptr = detail::raw_value<double>(m_data, index);

    value = *value_ptr;
}

void metrics::metric_value(std::size_t index, bool& value) const
{
    assert(index < m_count);
    assert(is_metric_initialized(index));
    assert(metric_type(index) == value_type::boolean);

    bool* value_ptr = detail::raw_value<bool>(m_data, index);

    value = *value_ptr;
}

auto metrics::metric_index(std::string name) const -> std::size_t
{
    std::size_t index = m_name_to_index.at(name);
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

    uint16_t* scope_size_data = detail::raw_scope_size(m_data);
    *scope_size_data = scope_size;
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
        return;
    }
    std::size_t last_scope_size = m_scopes.back().size();
    uint16_t updated_scope_size = scope_size() - last_scope_size;
    m_scope = m_scope.substr(last_scope_size, updated_scope_size);
    m_scopes.pop_back();

    // Update the scope size
    uint16_t* scope_size_data = detail::raw_scope_size(m_data);
    *scope_size_data = updated_scope_size;
}

void metrics::copy_storage(uint8_t* data, std::size_t size) const
{
    assert(data != nullptr);
    assert(size == storage_bytes());
    std::memcpy(data, m_data, detail::scope_offset(m_data));
    if (scope_size() > 0U)
    {
        std::memcpy(data + detail::scope_offset(m_data), m_scope.c_str(),
                    scope_size());
    }
}

auto metrics::storage_bytes() const -> std::size_t
{

    std::size_t offset = detail::values_offset(m_data);

    assert(offset % 8 == 0);

    offset += detail::eight_byte_count(m_data) * sizeof(uint64_t) +
              detail::one_byte_count(m_data) * sizeof(bool);

    offset += scope_size() + detail::scope_alignment_padding(m_data);

    return offset;
}

void metrics::reset_metric(std::size_t index)
{
    assert(index < m_count);
    assert(is_metric_initialized(index));
    assert(!metric_is_constant(index));

    value_type type = static_cast<value_type>(*detail::raw_type(m_data, index));

    switch (type)
    {
    case value_type::boolean:
    {
        bool* value_data = detail::raw_value<bool>(m_data, index);
        bool value = false;
        *value_data = value;
        break;
    }
    case value_type::unsigned_integral:
    {
        uint64_t* value_data = detail::raw_value<uint64_t>(m_data, index);
        uint64_t value = 0U;
        *value_data = value;
        break;
    }
    case value_type::signed_integral:
    {
        int64_t* value_data = detail::raw_value<int64_t>(m_data, index);
        int64_t value = 0;
        *value_data = value;
        break;
    }
    case value_type::floating_point:
    {
        double* value_data = detail::raw_value<double>(m_data, index);
        double value = 0.0;
        *value_data = value;
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

auto metrics::to_json() const -> std::string
{
    return detail::to_json(m_data, m_scope);
}

}
}
