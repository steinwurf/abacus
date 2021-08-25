// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "view.hpp"

#include <algorithm>
#include <cstring>

#include <bourne/json.hpp>
#include <endian/is_big_endian.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

void view::set_data(uint8_t* data)
{
    assert(data != nullptr);

    m_data = data;
}

auto view::max_metrics() const -> const uint16_t
{
    assert(m_data != nullptr);

    const uint8_t* max_metrics_data = m_data + 4;
    return *reinterpret_cast<const uint16_t*>(max_metrics_data);
}

auto view::max_name_bytes() const -> const uint16_t
{
    assert(m_data != nullptr);

    const uint8_t* max_name_bytes_data = m_data + 2;
    return *reinterpret_cast<const uint16_t*>(max_name_bytes_data);
}

auto view::level() const -> const uint8_t
{
    assert(m_data != nullptr);

    const uint8_t* level_data = m_data + 1;
    return *level_data;
}

auto view::raw_title() const -> const char*
{
    assert(m_data != nullptr);

    const uint8_t* title_data = m_data + title_offset();

    return reinterpret_cast<const char*>(title_data);
}

auto view::raw_title() -> char*
{
    assert(m_data != nullptr);

    uint8_t* title_data = m_data + title_offset();

    return reinterpret_cast<char*>(title_data);
}

auto view::raw_name(std::size_t index) const -> const char*
{
    assert(m_data != nullptr);

    const uint16_t max_metrics = view::max_metrics();
    assert(index < max_metrics);

    const uint16_t max_name_bytes = view::max_name_bytes();
    const uint8_t* name_data =
        m_data + names_offset() + (index * max_name_bytes);

    return reinterpret_cast<const char*>(name_data);
}

auto view::raw_name(std::size_t index) -> char*
{
    assert(m_data != nullptr);

    const uint16_t max_metrics = view::max_metrics();
    assert(index < max_metrics);

    const uint16_t max_name_bytes = view::max_name_bytes();
    uint8_t* name_data = m_data + names_offset() + (index * max_name_bytes);

    return reinterpret_cast<char*>(name_data);
}

auto view::raw_value(std::size_t index) const -> const uint64_t*
{
    assert(m_data != nullptr);

    const uint16_t max_metrics = view::max_metrics();
    assert(index < max_metrics);

    const uint8_t* value_data =
        m_data + values_offset() + (index * sizeof(uint64_t));

    return reinterpret_cast<const uint64_t*>(value_data);
}

auto view::raw_value(std::size_t index) -> uint64_t*
{
    assert(m_data != nullptr);

    const uint16_t max_metrics = view::max_metrics();
    assert(index < max_metrics);

    uint8_t* value_data = m_data + values_offset() + (index * sizeof(uint64_t));

    return reinterpret_cast<uint64_t*>(value_data);
}

auto view::title_offset() const -> std::size_t
{
    return header_size;
}

auto view::names_offset() const -> std::size_t
{
    // Skip header + title
    return header_size + view::max_name_bytes();
}

auto view::values_offset() const -> std::size_t
{
    // Skip header + title + names
    return header_size + view::max_name_bytes() +
           (view::max_metrics() * view::max_name_bytes());
}

auto view::view_bytes() const -> std::size_t
{
    return header_size + view::max_name_bytes() +
           view::max_metrics() * (view::max_name_bytes() + sizeof(uint64_t));
}

}
}