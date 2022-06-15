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
}

auto view::data() const -> const uint8_t*
{
    assert(m_data != nullptr);
    return m_data;
}

auto view::max_name_bytes() const -> uint16_t
{
    return detail::max_name_bytes(m_data);
}

auto view::max_metrics() const -> uint16_t
{
    return detail::max_metrics(m_data);
}

auto view::scope_size() const -> uint8_t
{
    return detail::scope_size(m_data);
}

auto view::metric_name(std::size_t index) const -> std::string
{
    assert(has_metric(index));
    std::string name = detail::raw_name(m_data, index);
    return name;
}

auto view::metric_type(std::size_t index) const -> value_type
{
    assert(index < count());
    return static_cast<value_type>(*detail::raw_type(m_data, index));
}

void view::metric_value(bool& value, std::size_t index) const
{
    assert(index < count());
    assert(static_cast<value_type>(*detail::raw_type(m_data, index)) ==
           value_type::boolean);
    value = *detail::raw_value<bool>(m_data, index);
}

void view::metric_value(uint64_t& value, std::size_t index) const
{
    assert(index < count());
    assert(static_cast<value_type>(*detail::raw_type(m_data, index)) ==
           value_type::unsigned_integral);
    value = *detail::raw_value<uint64_t>(m_data, index);
}

void view::metric_value(int64_t& value, std::size_t index) const
{
    assert(index < count());
    assert(static_cast<value_type>(*detail::raw_type(m_data, index)) ==
           value_type::signed_integral);
    value = *detail::raw_value<int64_t>(m_data, index);
}

void view::metric_value(double& value, std::size_t index) const
{
    assert(index < count());
    assert(static_cast<value_type>(*detail::raw_type(m_data, index)) ==
           value_type::floating_point);
    value = *detail::raw_value<double>(m_data, index);
}

auto view::metric_index(const std::string& name) const -> std::size_t
{

    for (std::size_t index = 0; index < max_metrics(); ++index)
    {
        if (has_metric(index) && metric_name(index) == name)
        {
            return index;
        }
    }

    assert(false && "Metric index was not found");

    return std::numeric_limits<std::size_t>::max();
}

auto view::count() const -> std::size_t
{
    std::size_t count = 0U;

    for (std::size_t i = 0U; i < max_metrics(); ++i)
    {
        if (has_metric(i))
        {
            ++count;
        }
    }

    return count;
}

auto view::scope() const -> std::string
{
    return m_scope;
}

auto view::has_metric(std::size_t index) const -> bool
{
    return detail::has_metric(m_data, index);
}

auto view::view_bytes() const -> std::size_t
{
    std::size_t bytes_before_values = detail::header_bytes() +
                                      max_name_bytes() * max_metrics() +
                                      max_metrics();
    std::size_t scope_size_with_padding =
        scope_size() == 0U ? scope_size()
                           : scope_size() + (8U - scope_size() % 8U);
    return bytes_before_values +
           detail::alignment_padding(bytes_before_values) +
           max_metrics() * sizeof(uint64_t) + scope_size_with_padding;
}

auto view::to_json(bool top_level) const -> std::string
{
    return detail::to_json(m_data, scope(), top_level);
}

}
}
