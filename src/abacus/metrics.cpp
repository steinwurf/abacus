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

    m_name_sizes.reserve(m_count);
    m_description_sizes.reserve(m_count);

    // Get the sizes of all the metric names
    std::transform(m_info.eight_byte_metrics.begin(),
                   m_info.eight_byte_metrics.end(), m_name_sizes.begin(),
                   [](const metric_info& i) { return i.name.size() + 1; });

    std::transform(m_info.one_byte_metrics.begin(),
                   m_info.one_byte_metrics.end(),
                   m_name_sizes.begin() + m_info.eight_byte_metrics.size(),
                   [](const metric_info& i) { return i.name.size() + 1; });

    // Get the sizes of all the metric descriptions
    std::transform(m_info.eight_byte_metrics.begin(),
                   m_info.eight_byte_metrics.end(), m_description_sizes.begin(),
                   [](const metric_info& i)
                   { return i.description.size() + 1; });

    std::transform(
        m_info.one_byte_metrics.begin(), m_info.one_byte_metrics.end(),
        m_description_sizes.begin() + m_info.eight_byte_metrics.size(),
        [](const metric_info& i) { return i.description.size() + 1; });

    // Calculate the total number of bytes needed for the names
    std::size_t name_bytes = 0;
    std::size_t description_bytes = 0;
    for (std::size_t i = 0; i < m_count; ++i)
    {
        name_bytes += m_name_sizes[i];
        description_bytes += m_description_sizes[i];
    }

    // Calculate the total size of header + name_sizes + description_sizes +
    // names + descriptions + type-enum
    m_storage_bytes = detail::header_bytes() +
                      (2 * sizeof(uint16_t) * m_count) + name_bytes +
                      description_bytes + m_count;

    if (m_storage_bytes % 8 != 0)
    {
        m_storage_bytes += detail::alignment_padding(m_storage_bytes);
    }

    m_storage_bytes +=
        8 * m_info.eight_byte_metrics.size() + m_info.one_byte_metrics.size();

    m_data = static_cast<uint8_t*>(::operator new(m_storage_bytes));

    assert(reinterpret_cast<uint64_t>(m_data) % 8U == 0U);

    // Zero out all memory
    std::memset(m_data, 0, m_storage_bytes);

    // // Write the header
    new (m_data) uint16_t(m_count);
    new (m_data + 2) uint16_t(0);
    new (m_data + 4) uint16_t(name_bytes);
    new (m_data + 6) uint16_t(description_bytes);
    new (m_data + 8) uint16_t(m_info.eight_byte_metrics.size());
    new (m_data + 10) uint16_t(m_info.one_byte_metrics.size());

    assert((reinterpret_cast<uint64_t>(m_data + detail::values_offset(m_data)) %
            8U) == 0U);

    // Write the name sizes
    uint8_t* name_sizes_ptr = m_data + detail::name_sizes_offset();
    for (std::size_t i = 0; i < m_count; ++i)
    {
        std::memcpy(name_sizes_ptr, &m_name_sizes[i], sizeof(uint16_t));
        name_sizes_ptr += sizeof(uint16_t);
    }

    // Write the description sizes
    uint8_t* description_sizes_ptr =
        m_data + detail::description_sizes_offset(m_data);

    for (std::size_t i = 0; i < m_count; ++i)
    {

        std::memcpy(description_sizes_ptr, &m_description_sizes[i],
                    sizeof(uint16_t));
        description_sizes_ptr += sizeof(uint16_t);
    }

    uint8_t* types_ptr = m_data + detail::types_offset(m_data);
    for (std::size_t i = 0; i < m_count; ++i)
    {
        uint8_t type_byte = static_cast<uint8_t>(m_info[i].type);
        std::memcpy(types_ptr, &type_byte, sizeof(uint8_t));
        types_ptr += sizeof(uint8_t);
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

auto metrics::is_metric_initialized(std::size_t index) const -> bool
{
    assert(index < m_count);

    const char* name_data = detail::raw_name(m_data, index);

    // If the name is non-zero it is initialized and valid. We just check the
    // first byte to see if it's zero.
    return name_data[0] != 0;
}

auto metrics::metric_name(std::size_t index) const -> std::string
{
    assert(index < m_count);
    assert(is_metric_initialized(index));
    return detail::raw_name(m_data, index);
}

auto metrics::metric_description(std::size_t index) const -> std::string
{
    assert(index < m_count);
    assert(is_metric_initialized(index));
    return detail::raw_description(m_data, index);
}

auto metrics::metric_type(std::size_t index) const -> value_type
{
    assert(index < m_count);
    return static_cast<value_type>(*detail::raw_type(m_data, index));
}

auto metrics::metric_is_constant(std::size_t index) const -> bool
{
    assert(index < m_count);
    return m_info[index].is_constant;
}

}
}
