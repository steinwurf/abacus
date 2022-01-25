// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

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

auto view::data() const -> const uint8_t*
{
    assert(m_data != nullptr);
    return m_data;
}

auto view::max_name_bytes() const -> uint16_t
{
    return detail::max_name_bytes(m_data);
}

auto view::max_prefix_bytes() const -> uint16_t
{
    return detail::max_prefix_bytes(m_data);
}

auto view::max_metrics() const -> uint16_t
{
    return detail::max_metrics(m_data);
}

auto view::get_prefix() const -> std::string
{
    std::string prefix = detail::raw_prefix(m_data);
    return prefix;
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

auto view::metric_index(const std::string& name) const -> std::size_t
{

    for (std::size_t index = 0; index < max_metrics(); ++index)
    {
        if (!is_metric_initialized(index))
        {
            continue;
        }

        if (metric_name(index) == name)
        {
            return index;
        }
    }

    assert(false && "Metric index was not found");
}

auto view::metric_prefix(std::size_t index) const -> std::string
{
    assert(is_metric_initialized(index));
    std::string prefix = detail::raw_prefix(m_data);
    return prefix;
}

auto view::metrics_count() const -> std::size_t
{
    std::size_t count = 0U;
    for (std::size_t i = 0U; i < max_metrics(); ++i)
    {
        if (is_metric_initialized(i))
        {
            ++count;
        }
    }
    return count;
}

auto view::is_metric_initialized(std::size_t index) const -> bool
{
    return detail::is_metric_initialized(m_data, index);
}

auto view::view_bytes() const -> std::size_t
{
    assert(reinterpret_cast<uint64_t>(m_data) % 8U == 0U);
    std::size_t bytes_before_values = detail::header_bytes() +
                                      max_name_bytes() +
                                      max_name_bytes() * max_metrics();

    return bytes_before_values +
           detail::values_alignment_padding(bytes_before_values) +
           max_metrics() * sizeof(uint64_t);
}

auto view::to_json() const -> std::string
{
    bourne::json counters = bourne::json::parse(detail::to_json(m_data));

    bourne::json full_json = bourne::json::object();

    std::string prefix = detail::raw_prefix(m_data);

    full_json[prefix] = counters;

    return full_json.dump();
}

}
}
