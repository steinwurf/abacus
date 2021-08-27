// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "detail/raw.hpp"
#include "metrics.hpp"

#include <algorithm>
#include <cstring>

#include <bourne/json.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

metrics::metrics(uint16_t max_metrics, uint16_t max_name_bytes,
                 const std::string& title) :
    m_max_metrics(max_metrics),
    m_max_name_bytes(max_name_bytes)
{
    assert(m_max_metrics > 0);
    assert(m_max_name_bytes > 0);
    assert(title.size() < m_max_name_bytes);

    // Allocate the memory for the counters
    std::size_t memory_needed = storage_bytes();
    m_data = static_cast<uint8_t*>(::operator new(memory_needed));

    // Zero out all memory
    std::memset(m_data, 0, memory_needed);

    // Write the header
    new (m_data) uint16_t(m_max_name_bytes);
    new (m_data + 2) uint16_t(m_max_metrics);
    new (m_data + 4) uint8_t(8);

    // Write the title
    set_metrics_title(title);
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

void metrics::set_metrics_title(const std::string& title)
{
    // Write the title
    char* title_data = detail::raw_title(m_data);
    std::memcpy(title_data, title.data(), title.size());
}

auto metrics::metric_name(std::size_t index) const -> std::string
{
    assert(is_metric_initialized(index));
    return detail::raw_name(m_data, index);
}

auto metrics::metric_value(std::size_t index) const -> uint64_t
{
    assert(is_metric_initialized(index));
    return *detail::raw_value(m_data, index);
}

auto metrics::initialize_metric(std::size_t index, const std::string& name)
    -> metric
{
    assert(!is_metric_initialized(index));

    // We check for less than since there also needs to be at least one
    // zero terminating byte
    assert(name.size() < m_max_name_bytes);

    char* name_data = detail::raw_name(m_data, index);
    uint64_t* value_data = detail::raw_value(m_data, index);

    // Copy the name
    std::memcpy(name_data, name.data(), name.size());

    // Placement new the counter value in the designated location
    new (value_data) uint64_t{0U};

    return metric{value_data};
}

void metrics::copy_storage(uint8_t* data) const
{
    std::size_t metrics_size = storage_bytes();
    std::memcpy(data, m_data, metrics_size);
}
auto metrics::storage_bytes() const -> std::size_t
{
    std::size_t names_bytes = m_max_name_bytes * m_max_metrics;
    std::size_t value_bytes = sizeof(uint64_t) * m_max_metrics;

    return detail::header_size() + m_max_name_bytes + names_bytes + value_bytes;
}

auto metrics::is_metric_initialized(std::size_t index) const -> bool
{
    return detail::is_metric_initialized(m_data, index);
}

void metrics::reset_metrics()
{
    for (std::size_t index = 0; index < m_max_metrics; ++index)

    {
        if (!is_metric_initialized(index))
        {
            continue;
        }

        reset_metric(index);
    }
}

void metrics::reset_metric(std::size_t index)
{
    assert(is_metric_initialized(index));

    uint64_t* value = detail::raw_value(m_data, index);
    *value = 0;
}

auto metrics::to_json() const -> std::string
{
    bourne::json counters = bourne::json::object();

    for (std::size_t i = 0; i < m_max_metrics; ++i)
    {
        if ((!metrics::is_metric_initialized(i)))
        {
            continue;
        }

        auto n = metrics::metric_name(i);
        auto v = metrics::metric_value(i);

        counters[n] = v;
    }

    return counters.dump();
}

}
}
