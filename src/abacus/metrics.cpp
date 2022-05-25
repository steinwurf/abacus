// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>

#include "detail/info_bytes.hpp"
#include "detail/raw.hpp"
#include "metrics.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

metrics::metrics(std::vector<metric_info> info) : m_info(info)
{
    assert(m_info.size() > 0);
    assert(m_info.size() <= std::numeric_limits<uint16_t>::max());

    m_count = m_info.size();

    // Calculate the needed memory for the metrics.
    detail::info_bytes bytes = detail::info_bytes_from_info(m_info);

    m_storage_bytes = detail::header_bytes() + bytes.name_bytes +
                      bytes.description_bytes + bytes.type_bytes;

    if (m_storage_bytes % 8 != 0)
    {
        m_storage_bytes += 8 - (m_storage_bytes % 8);
    }

    m_storage_bytes += bytes.value_bytes;

    m_data = static_cast<uint8_t*>(::operator new(m_storage_bytes));

    assert(reinterpret_cast<uint64_t>(m_data) % 8U == 0U);

    // Zero out all memory
    std::memset(m_data, 0, m_storage_bytes);

    // // Write the header
    new (m_data) uint16_t(m_count);
    new (m_data + 2) uint16_t(0);
    new (m_data + 4) uint16_t(bytes.name_bytes);
    new (m_data + 6) uint16_t(bytes.description_bytes);
    new (m_data + 8) uint16_t(m_count - bytes.bools_count);
    new (m_data + 10) uint16_t(bytes.bools_count);

    assert((reinterpret_cast<uint64_t>(m_data + detail::values_offset(m_data)) %
            8U) == 0U);

    // Write the names
    uint8_t* name_ptr = m_data + detail::names_offset();
}

metrics::~metrics()
{
    ::operator delete(m_data);
}

auto metrics::metric_count() const -> std::size_t
{
    return m_count;
}

auto metrics::max_name_bytes() const -> std::size_t
{
    return m_max_name_bytes;
}

auto metrics::metric_name(std::size_t index) const -> std::string
{
    assert(index < m_count);
    return detail::raw_name(m_data, index);
}

auto metrics::metric_type(std::size_t index) const -> value_type
{
    assert(index < m_count);
    return static_cast<value_type>(*detail::raw_type(m_data, index));
}

void metrics::metric_value(bool& value, std::size_t index) const
{
    assert(index < m_count);
    assert(static_cast<value_type>(*detail::raw_type(m_data, index)) ==
           value_type::boolean);
    value = *detail::raw_value<bool>(m_data, index);
}

void metrics::metric_value(uint64_t& value, std::size_t index) const
{
    assert(index < m_count);
    assert(static_cast<value_type>(*detail::raw_type(m_data, index)) ==
           value_type::unsigned_integral);
    value = *detail::raw_value<uint64_t>(m_data, index);
}

void metrics::metric_value(int64_t& value, std::size_t index) const
{
    assert(index < m_count);
    assert(static_cast<value_type>(*detail::raw_type(m_data, index)) ==
           value_type::signed_integral);
    value = *detail::raw_value<int64_t>(m_data, index);
}

void metrics::metric_value(double& value, std::size_t index) const
{
    assert(index < m_count);
    assert(static_cast<value_type>(*detail::raw_type(m_data, index)) ==
           value_type::floating_point);
    value = *detail::raw_value<double>(m_data, index);
}

auto metrics::metric_index(const std::string& name) const -> std::size_t
{
    assert(name.size() < m_max_name_bytes);

    for (std::size_t i = 0; i < m_count; ++i)
    {
        if (metric_name(i) == name)
        {
            return i;
        }
    }

    assert(false && "Metric index was not found");

    return std::numeric_limits<std::size_t>::max();
}

auto metrics::scope() const -> std::string
{
    return m_scope;
}

auto metrics::scope_size() const -> std::size_t
{
    return m_scope.size() + 1;
}

template <typename Type>
auto metrics::add_metric(const std::string& name) -> metric<Type>
{
    assert(0 && "Unsupported metric type");

    (void)name;

    return metric<Type>{};
}

template <>
auto metrics::add_metric<bool>(const std::string& name) -> metric<bool>
{
    char* name_data = detail::raw_name(m_data, m_count);
    // Copy the name
    std::memcpy(name_data, name.data(), name.size());

    uint8_t* type_data = detail::raw_type(m_data, m_count);
    *type_data = static_cast<uint8_t>(value_type::boolean);

    bool* value_data = detail::raw_value<bool>(m_data, m_count);
    bool value = false;
    std::memcpy(value_data, &value, sizeof(bool));

    m_count++;

    return metric<bool>{value_data};
}

template <>
auto metrics::add_metric<uint64_t>(const std::string& name) -> metric<uint64_t>
{
    char* name_data = detail::raw_name(m_data, m_count);
    // Copy the name
    std::memcpy(name_data, name.data(), name.size());

    uint8_t* type_data = detail::raw_type(m_data, m_count);
    *type_data = static_cast<uint8_t>(value_type::unsigned_integral);

    uint64_t* value_data = detail::raw_value<uint64_t>(m_data, m_count);
    uint64_t value = 0U;
    std::memcpy(value_data, &value, sizeof(uint64_t));

    m_count++;

    return metric<uint64_t>{value_data};
}

template <>
auto metrics::add_metric<int64_t>(const std::string& name) -> metric<int64_t>
{
    char* name_data = detail::raw_name(m_data, m_count);
    // Copy the name
    std::memcpy(name_data, name.data(), name.size());

    uint8_t* type_data = detail::raw_type(m_data, m_count);
    *type_data = static_cast<uint8_t>(value_type::signed_integral);

    int64_t* value_data = detail::raw_value<int64_t>(m_data, m_count);
    int64_t value = 0;
    std::memcpy(value_data, &value, sizeof(int64_t));

    m_count++;

    return metric<int64_t>{value_data};
}

template <>
auto metrics::add_metric<double>(const std::string& name) -> metric<double>
{
    char* name_data = detail::raw_name(m_data, m_count);
    // Copy the name
    std::memcpy(name_data, name.data(), name.size());

    uint8_t* type_data = detail::raw_type(m_data, m_count);
    *type_data = static_cast<uint8_t>(value_type::floating_point);

    double* value_data = detail::raw_value<double>(m_data, m_count);
    double value = 0.0;
    std::memcpy(value_data, &value, sizeof(double));

    m_count++;

    return metric<double>{value_data};
}

auto metrics::count() const -> std::size_t
{
    return m_count;
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
    uint8_t scope_size = (uint8_t)m_scope.size();

    uint16_t* scope_size_data = detail::raw_scope_size(m_data);
    std::memcpy(scope_size_data, &scope_size, sizeof(uint8_t));
}

void metrics::pop_scope()
{
    assert(!m_scopes.empty());
    std::size_t last_scope_size = m_scopes.back().size();
    std::size_t updated_scope_size = scope_size() - last_scope_size;
    m_scope = m_scope.substr(last_scope_size, updated_scope_size);
    m_scopes.pop_back();

    // Update the scope size
    updated_scope_size = (uint16_t)updated_scope_size;
    uint16_t* scope_size_data = detail::raw_scope_size(m_data);
    std::memcpy(scope_size_data, &updated_scope_size, sizeof(uint8_t));
}

void metrics::copy_storage(uint8_t* data) const
{
    assert(data != nullptr);
    std::memcpy(data, m_data, detail::scope_offset(m_data));
    if (scope_size() > 0U)
    {
        std::memcpy(data + detail::scope_offset(m_data), m_scope.c_str(),
                    scope_size());
    }
}

auto metrics::storage_bytes() const -> std::size_t
{
    std::size_t values_offset = detail::header_bytes() +
                                m_max_metrics * m_max_name_bytes +
                                m_max_metrics;

    values_offset += detail::alignment_padding(values_offset);

    assert(values_offset % 8 == 0);

    std::size_t value_bytes = m_max_metrics * sizeof(uint64_t);

    std::size_t scope_bytes = scope_size();

    std::size_t scope_padding = 0U;

    if (scope_bytes > 0U)
    {
        scope_padding = 8 - (scope_bytes % 8);
    }

    return values_offset + value_bytes + scope_bytes + scope_padding;
}

void metrics::reset_metrics()
{
    for (std::size_t index = 0; index < m_count; ++index)

    {
        reset_metric(index);
    }
}

void metrics::reset_metric(std::size_t index)
{
    assert(index < m_count);

    value_type type = static_cast<value_type>(*detail::raw_type(m_data, index));

    switch (type)
    {
    case value_type::boolean:
    {
        bool* value_data = detail::raw_value<bool>(m_data, index);
        bool value = false;
        std::memcpy(value_data, &value, sizeof(bool));
        break;
    }
    case value_type::unsigned_integral:
    {
        uint64_t* value_data = detail::raw_value<uint64_t>(m_data, index);
        uint64_t value = 0U;
        std::memcpy(value_data, &value, sizeof(uint64_t));
        break;
    }
    case value_type::signed_integral:
    {
        int64_t* value_data = detail::raw_value<int64_t>(m_data, index);
        int64_t value = 0;
        std::memcpy(value_data, &value, sizeof(int64_t));
        break;
    }
    case value_type::floating_point:
    {
        double* value_data = detail::raw_value<double>(m_data, index);
        double value = 0.0;
        std::memcpy(value_data, &value, sizeof(double));
        break;
    }
    }
}

auto metrics::to_json() const -> std::string
{
    return detail::to_json(m_data, m_scope);
}
}
}
