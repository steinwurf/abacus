// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <algorithm>
#include <cstring>

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
}

auto view::max_name_bytes() const -> uint16_t
{
    return detail::max_name_bytes(m_data);
}

auto view::max_metrics() const -> uint16_t
{
    return detail::max_metrics(m_data);
}

auto view::get_title() const -> std::string
{
    std::string title = detail::raw_title(m_data);
    return title;
}

auto view::metric_name(std::size_t index) const -> std::string
{
    assert(is_metric_initialized(index));
    std::string name = detail::raw_name(m_data, index);
    return name;
}

auto view::metric_value(std::size_t index) const -> uint64_t
{
    assert(is_metric_initialized(index));
    return *detail::raw_value(m_data, index);
}

auto view::is_metric_initialized(std::size_t index) const -> bool
{
    return detail::is_metric_initialized(m_data, index);
}

auto view::view_bytes() const -> std::size_t
{
    return detail::header_bytes() + max_name_bytes() +
           max_metrics() * (max_name_bytes() + sizeof(uint64_t));
}
auto view::to_json() const -> std::string
{
    return detail::to_json(m_data);
}

}
}