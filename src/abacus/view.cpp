// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <iostream>
#include <limits>

#include "detail/raw.hpp"
#include "view.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

void view::set_data(const uint8_t* data)
{
    assert(data != nullptr);
    m_data = data;
    m_scope =
        std::string(detail::raw_scope(m_data), detail::scope_size(m_data));

    // Fill the names and indices as keys in the map.
    for (std::size_t i = 0; i < metric_count(); i++)
    {
        if (!is_metric_initialized(i))
        {
            continue;
        }
        m_name_to_index.insert({metric_name(i), i});
    }
}

auto view::data() const -> const uint8_t*
{
    assert(m_data != nullptr);
    return m_data;
}

auto view::metric_count() const -> uint16_t
{
    return detail::metric_count(m_data);
}

auto view::scope_size() const -> uint16_t
{
    return detail::scope_size(m_data);
}

auto view::name_bytes() const -> uint16_t
{
    return detail::name_bytes(m_data);
}

auto view::description_bytes() const -> uint16_t
{
    return detail::descriptions_bytes(m_data);
}

auto view::eight_byte_count() const -> uint16_t
{
    return detail::eight_byte_count(m_data);
}

auto view::one_byte_count() const -> uint16_t
{
    return detail::one_byte_count(m_data);
}

auto view::is_metric_initialized(std::size_t index) const -> bool
{
    return detail::is_metric_initialized(m_data, index);
}

auto view::metric_name(std::size_t index) const -> std::string
{
    assert(is_metric_initialized(index));
    return detail::raw_name(m_data, index);
}

auto view::metric_description(std::size_t index) const -> std::string
{
    assert(is_metric_initialized(index));
    return detail::raw_description(m_data, index);
}

auto view::metric_type(std::size_t index) const -> value_type
{
    assert(is_metric_initialized(index));
    return static_cast<value_type>(*detail::raw_type(m_data, index));
}

auto view::metric_is_constant(std::size_t index) const -> bool
{
    assert(is_metric_initialized(index));
    return detail::is_constant(m_data, index);
}

void view::metric_value(std::size_t index, bool& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == value_type::boolean);
    value = *detail::raw_value<bool>(m_data, index);
}

void view::metric_value(std::size_t index, uint64_t& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == value_type::unsigned_integral);
    value = *detail::raw_value<uint64_t>(m_data, index);
}

void view::metric_value(std::size_t index, int64_t& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == value_type::signed_integral);
    value = *detail::raw_value<int64_t>(m_data, index);
}

void view::metric_value(std::size_t index, double& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == value_type::floating_point);
    value = *detail::raw_value<double>(m_data, index);
}

auto view::metric_index(const std::string& name) const -> std::size_t
{
    std::size_t index = m_name_to_index.at(name);
    return index;
}

auto view::scope() const -> std::string
{
    return detail::raw_scope(m_data);
}

auto view::view_bytes() const -> std::size_t
{
    std::size_t offset = detail::values_offset(m_data);

    assert(offset % 8 == 0);

    offset += detail::eight_byte_count(m_data) * sizeof(uint64_t) +
              detail::one_byte_count(m_data) * sizeof(bool);

    offset += scope_size() + detail::scope_alignment_padding(m_data);

    return offset;
}

auto view::to_json() const -> std::string
{
    return detail::to_json(m_data, scope());
}
}
}
