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

metrics::metric::metric(uint64_t* memory) : m_memory(memory)
{
    assert(m_memory != nullptr);
}

auto metrics::metric::operator=(uint64_t value) -> metric&
{
    assert(m_memory != nullptr);

    *m_memory = value;
    return *this;
}

auto metrics::metric::operator+=(uint64_t value) -> metric&
{
    assert(m_memory != nullptr);

    *m_memory += value;
    return *this;
}

auto metrics::metric::operator++() -> metric&
{
    assert(m_memory != nullptr);

    *m_memory += 1U;
    return *this;
}

auto metrics::metric::is_initialized() const -> bool
{
    return m_memory != nullptr;
}

metrics::metrics(uint64_t max_metrics, uint64_t max_name_bytes,
                 const std::string& title, uint32_t level) :
    m_max_metrics(max_metrics),
    m_max_name_bytes(max_name_bytes), m_level(level)
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
    char* title_data = raw_title();
    std::memcpy(title_data, title.data(), title.size());
}

auto metrics::metric_name(std::size_t index) const -> std::string
{
    assert(is_metric_initialized(index));
    return {raw_name(index)};
}

auto metrics::metric_value(std::size_t index) const -> uint64_t
{
    assert(is_metric_initialized(index));
    return *raw_value(index);
}

auto metrics::initialize_metric(std::size_t index, const std::string& name)
    -> metric
{
    assert(!is_metric_initialized(index));

    // We check for less than since there also needs to be at least one
    // zero terminating byte
    assert(name.size() < m_max_name_bytes);

    char* name_data = raw_name(index);
    uint64_t* value_data = raw_value(index);

    // Copy the name
    std::memcpy(name_data, name.data(), name.size());

    // Placement new the counter value in the designated location
    new (value_data) uint64_t{0U};

    return metric{value_data};
}

auto metrics::copy_storage(uint8_t* data) const
{
    std::size_t metrics_size = storage_bytes();
    std::memcpy(data, m_data, metrics_size);
}
auto metrics::storage_bytes() const -> std::size_t
{
    std::size_t names_bytes = m_max_name_bytes * m_max_metrics;
    std::size_t value_bytes = sizeof(uint64_t) * m_max_metrics;

    return header_size + names_bytes + value_bytes;
}

auto metrics::is_metric_initialized(std::size_t index) const -> bool
{
    assert(index < m_max_metrics);
    const char* name_data = raw_name(index);

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

        auto n = raw_name(i);
        auto v = *raw_value(i);

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

    uint64_t* value = raw_value(index);
    *value = 0;
}

auto metrics::metrics_count() const -> std::size_t
{
    return m_max_metrics;
}

auto metrics::metrics_level() const -> uint32_t
{
    return m_level;
}

auto metrics::raw_title() const -> const char*
{
    const uint8_t* title_data = m_data + title_offset();

    return reinterpret_cast<const char*>(title_data);
}

auto metrics::raw_title() -> char*
{
    uint8_t* title_data = m_data + title_offset();

    return reinterpret_cast<char*>(title_data);
}

auto metrics::raw_name(std::size_t index) const -> const char*
{
    assert(index < m_max_metrics);

    const uint8_t* name_data =
        m_data + names_offset() + (index * m_max_name_bytes);

    return reinterpret_cast<const char*>(name_data);
}

auto metrics::raw_name(std::size_t index) -> char*
{
    assert(index < m_max_metrics);

    uint8_t* name_data = m_data + names_offset() + (index * m_max_name_bytes);

    return reinterpret_cast<char*>(name_data);
}

auto metrics::raw_value(std::size_t index) const -> const uint64_t*
{
    assert(index < m_max_metrics);

    const uint8_t* value_data =
        m_data + values_offset() + (index * sizeof(uint64_t));

    return reinterpret_cast<const uint64_t*>(value_data);
}

auto metrics::raw_value(std::size_t index) -> uint64_t*
{
    assert(index < m_max_metrics);

    uint8_t* value_data = m_data + values_offset() + (index * sizeof(uint64_t));

    return reinterpret_cast<uint64_t*>(value_data);
}

auto metrics::title_offset() const -> std::size_t
{
    return header_size;
}

auto metrics::names_offset() const -> std::size_t
{
    // Skip header + title + level
    return header_size + m_max_name_bytes + m_level;
}

auto metrics::values_offset() const -> std::size_t
{
    // Skip header + title + level + names
    return header_size + m_level + m_max_name_bytes +
           (m_max_metrics * m_max_name_bytes);
}

}
}
