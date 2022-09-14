// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <limits>

#include "detail/raw.hpp"
#include "metric_flags.hpp"
#include "view.hpp"

#include <endian/big_endian.hpp>
#include <endian/little_endian.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

template <class Value>
constexpr auto read(bool is_big, const uint8_t* data) -> Value
{
    if (is_big)
    {
        return endian::big_endian::get<Value>(data);
    }
    else
    {
        return endian::little_endian::get<Value>(data);
    }
}

void view::set_data(const uint8_t* data)
{
    assert(data != nullptr);
    m_data = data;
    m_is_big_endian = detail::is_big_endian_byte(m_data);
    for (std::size_t i = 0; i < metric_count(); ++i)
    {
        m_name_to_index.emplace(metric_name(i), i);
    }
}

auto view::data() const -> const uint8_t*
{
    assert(m_data != nullptr);
    return m_data;
}

auto view::metric_count() const -> uint16_t
{
    return read<uint16_t>(m_is_big_endian, m_data);
}

auto view::is_metric_initialized(std::size_t index) const -> bool
{
    assert(index < metric_count());
    return detail::is_metric_initialized(m_data, index);
}

auto view::metric_name(std::size_t index) const -> std::string
{
    return {detail::raw_name(m_data, index), detail::name_size(m_data, index)};
}

auto view::metric_description(std::size_t index) const -> std::string
{
    return {detail::raw_description(m_data, index),
            detail::description_size(m_data, index)};
}

auto view::metric_type(std::size_t index) const -> abacus::metric_type
{
    assert(is_metric_initialized(index));
    return static_cast<abacus::metric_type>(*detail::raw_type(m_data, index));
}

auto view::is_metric_constant(std::size_t index) const -> bool
{
    assert(is_metric_initialized(index));
    return (bool)(detail::flags(m_data, index) & metric_flags::constant);
}

void view::metric_value(std::size_t index, bool& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == metric_type::boolean);
    value = read<uint8_t>(m_is_big_endian,
                          (uint8_t*)detail::raw_value(m_data, index));
}

void view::metric_value(std::size_t index, uint64_t& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == metric_type::uint64);
    value = read<uint64_t>(m_is_big_endian,
                           (uint8_t*)detail::raw_value(m_data, index));
}

void view::metric_value(std::size_t index, int64_t& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == metric_type::int64);
    value = read<int64_t>(m_is_big_endian,
                          (uint8_t*)detail::raw_value(m_data, index));
}

void view::metric_value(std::size_t index, double& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == metric_type::float64);
    value = read<double>(m_is_big_endian,
                         (uint8_t*)detail::raw_value(m_data, index));
}

auto view::metric_index(const std::string& name) const -> std::size_t
{
    return m_name_to_index.at(name);
}

auto view::view_bytes() const -> std::size_t
{
    return detail::storage_bytes(m_data);
}
}
}
