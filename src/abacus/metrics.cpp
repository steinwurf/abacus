// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>

#include "detail/raw.hpp"
#include "metrics.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

metrics::metrics(std::size_t max_metrics, std::size_t max_name_bytes) :
    m_max_metrics(max_metrics), m_max_name_bytes(max_name_bytes)
{
    assert(m_max_metrics > 0);
    assert(m_max_metrics <= std::numeric_limits<uint16_t>::max());
    assert(m_max_name_bytes > 0);
    assert(m_max_name_bytes <= std::numeric_limits<uint16_t>::max());

    // Allocate the memory for the counters
    std::size_t memory_needed = storage_bytes();
    m_data = static_cast<uint8_t*>(::operator new(memory_needed));

    assert(reinterpret_cast<uint64_t>(m_data) % 8U == 0U);

    // Zero out all memory
    std::memset(m_data, 0, memory_needed);

    // // Write the header
    new (m_data) uint16_t(m_max_name_bytes);
    new (m_data + 2) uint16_t(m_max_metrics);
    new (m_data + 4) uint8_t(8);
    new (m_data + 5) uint8_t(0);

    assert((reinterpret_cast<uint64_t>(detail::raw_value(m_data, 0)) % 8U) ==
           0U);
}

metrics::~metrics()
{
    ::operator delete(m_data);
}

auto metrics::max_metrics() const -> std::size_t
{
    return m_max_metrics;
}

auto metrics::max_name_bytes() const -> std::size_t
{
    return m_max_name_bytes;
}

auto metrics::metric_name(std::size_t index) const -> std::string
{
    assert(has_metric(index));
    return detail::raw_name(m_data, index);
}

auto metrics::metric_value(std::size_t index) const -> uint64_t
{
    assert(has_metric(index));
    return *detail::raw_value(m_data, index);
}

auto metrics::metric_index(const std::string& name) const -> std::size_t
{
    assert(name.size() < m_max_name_bytes);

    for (std::size_t i = 0; i < m_count; ++i)
    {
        if (has_metric(i) && metric_name(i) == name)
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
    return m_scope.size();
}

auto metrics::add_metric(const std::string& name) -> metric
{
    char* name_data = detail::raw_name(m_data, m_count);
    uint64_t* value_data = detail::raw_value(m_data, m_count);

    // Copy the name
    std::memcpy(name_data, name.data(), name.size());

    // Use memcpy here for now, since placement new causes Bus Error on
    // Raspberry pi
    uint64_t value = 0U;
    std::memcpy(value_data, &value, sizeof(uint64_t));

    m_count++;

    return metric{value_data};
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

    uint8_t* scope_size_data = detail::raw_scope_size(m_data);
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
    updated_scope_size = (uint8_t)updated_scope_size;
    uint8_t* scope_size_data = detail::raw_scope_size(m_data);
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
    // Add null terminator
    data[detail::scope_offset(m_data) + scope_size()] = '\0';
}

auto metrics::storage_bytes() const -> std::size_t
{
    std::size_t values_offset =
        detail::header_bytes() + m_max_metrics * m_max_name_bytes;

    values_offset += detail::values_alignment_padding(values_offset);

    assert(values_offset % 8 == 0);

    std::size_t value_bytes = m_max_metrics * sizeof(uint64_t);

    std::size_t scope_bytes = scope_size();

    // if we have a scope, add the null terminator.
    if (scope_bytes > 0U)
    {
        scope_bytes += 1U;
    }

    return values_offset + value_bytes + scope_bytes;
}

auto metrics::has_metric(std::size_t index) const -> bool
{
    return detail::has_metric(m_data, index);
}

void metrics::reset_metrics()
{
    for (std::size_t index = 0; index < m_max_metrics; ++index)

    {
        if (!has_metric(index))
        {
            continue;
        }

        reset_metric(index);
    }
}

void metrics::reset_metric(std::size_t index)
{
    assert(has_metric(index));

    uint64_t* value_data = detail::raw_value(m_data, index);
    uint64_t value = 0U;
    std::memcpy(value_data, &value, sizeof(uint64_t));
}

auto metrics::to_json() const -> std::string
{
    return detail::to_json(m_data, m_scope);
}
}
}
