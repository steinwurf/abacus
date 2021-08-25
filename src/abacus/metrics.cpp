// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "metrics.hpp"

#include <algorithm>
#include <cstring>

#include <bourne/json.hpp>
#include <endian/is_big_endian.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

metrics::metrics(uint16_t max_metrics, uint16_t max_name_bytes,
                 const std::string& title, uint8_t level) :
    m_max_metrics(max_metrics),
    m_max_name_bytes(max_name_bytes), m_level(level)
{
    assert(m_max_metrics > 0);
    assert(m_max_name_bytes > 0);
    assert(title.size() < m_max_name_bytes);

    // Allocate the memory for the counters
    std::size_t memory_needed = storage_bytes();
    m_data = static_cast<uint8_t*>(::operator new(memory_needed));
    view::set_data(m_data);

    // Zero out all memory
    std::memset(m_data, 0, memory_needed);

    // Write the header
    new (m_data) uint8_t(endian::is_big_endian());
    new (m_data + 1) uint8_t(8);
    new (m_data + 2) uint8_t(m_level);
    new (m_data + 3) uint16_t(m_max_name_bytes);
    new (m_data + 5) uint16_t(m_max_metrics);

    // Write the title
    set_metrics_title(title);
}

metrics::~metrics()
{
    ::operator delete(m_data);
}

void metrics::set_metrics_title(const std::string& title)
{
    // Write the title
    char* title_data = view::raw_title();
    std::memcpy(title_data, title.data(), m_max_name_bytes);
}

auto metrics::metric_name(std::size_t index) const -> std::string
{
    assert(is_metric_initialized(index));
    return {view::raw_name(index)};
}

auto metrics::metric_value(std::size_t index) const -> uint64_t
{
    assert(is_metric_initialized(index));
    return *view::raw_value(index);
}

auto metrics::initialize_metric(std::size_t index, const std::string& name)
    -> metric
{
    assert(!is_metric_initialized(index));

    // We check for less than since there also needs to be at least one
    // zero terminating byte
    assert(name.size() < m_max_name_bytes);

    char* name_data = view::raw_name(index);
    uint64_t* value_data = view::raw_value(index);

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

    return header_size + m_max_name_bytes + names_bytes + value_bytes;
}

auto metrics::is_metric_initialized(std::size_t index) const -> bool
{
    assert(index < m_max_metrics);
    const char* name_data = view::raw_name(index);

    // If the name is non-zero it is initialized and valid. We just check the
    // first byte to see if it's zero.
    return name_data[0] != 0;
}

auto metrics::to_json() const -> std::string
{
    bourne::json counters = bourne::json::object();

    for (std::size_t i = 0; i < m_max_metrics; ++i)
    {
        if (!is_metric_initialized(i))
        {
            continue;
        }

        auto n = view::raw_name(i);
        auto v = *view::raw_value(i);

        counters[n] = v;
    }

    return counters.dump();
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

    uint64_t* value = view::raw_value(index);
    *value = 0;
}

auto metrics::metrics_count() const -> std::size_t
{
    return m_max_metrics;
}

auto metrics::metrics_level() const -> uint8_t
{
    return m_level;
}

}
}
