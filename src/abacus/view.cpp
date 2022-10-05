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

void view::set_meta_data(const uint8_t* meta_data)
{
    m_meta_data = meta_data;
    m_value_data = nullptr;

    m_is_big_endian = detail::is_big_endian_byte(m_meta_data);
    for (std::size_t i = 0; i < metric_count(); ++i)
    {
        m_name_to_index.emplace(metric_name(i), i);
    }
}

void view::set_value_data(const uint8_t* value_data)
{
    assert(m_meta_data != nullptr);
    m_value_data = value_data;
}

const uint8_t* view::meta_data() const
{
    return m_meta_data;
}

const uint8_t* view::value_data() const
{
    return m_value_data;
}

std::size_t view::meta_bytes() const
{
    return detail::meta_bytes(m_meta_data);
}

std::size_t view::value_bytes() const
{
    return detail::value_bytes(m_meta_data);
}

auto view::metric_count() const -> uint16_t
{
    return detail::metric_count(m_meta_data);
}

auto view::is_metric_initialized(std::size_t index) const -> bool
{
    assert(index < metric_count());
    return detail::is_metric_initialized(m_meta_data, m_value_data, index);
}

auto view::metric_name(std::size_t index) const -> std::string
{
    return {detail::raw_name(m_meta_data, index),
            detail::name_size(m_meta_data, index)};
}

auto view::metric_description(std::size_t index) const -> std::string
{
    return {detail::raw_description(m_meta_data, index),
            detail::description_size(m_meta_data, index)};
}

auto view::metric_type(std::size_t index) const -> abacus::metric_type
{
    assert(is_metric_initialized(index));
    return static_cast<abacus::metric_type>(
        *detail::raw_type(m_meta_data, index));
}

auto view::is_metric_constant(std::size_t index) const -> bool
{
    assert(is_metric_initialized(index));
    return (bool)(detail::flags(m_meta_data, index) & metric_flags::constant);
}

void view::metric_value(std::size_t index, bool& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == metric_type::boolean);
    value = read<uint8_t>(
        m_is_big_endian,
        (uint8_t*)detail::raw_value(m_meta_data, m_value_data, index));
}

void view::metric_value(std::size_t index, uint64_t& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == metric_type::uint64);
    value = read<uint64_t>(
        m_is_big_endian,
        (uint8_t*)detail::raw_value(m_meta_data, m_value_data, index));
}

void view::metric_value(std::size_t index, int64_t& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == metric_type::int64);
    value = read<int64_t>(
        m_is_big_endian,
        (uint8_t*)detail::raw_value(m_meta_data, m_value_data, index));
}

void view::metric_value(std::size_t index, double& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == metric_type::float64);
    value = read<double>(
        m_is_big_endian,
        (uint8_t*)detail::raw_value(m_meta_data, m_value_data, index));
}

auto view::metric_index(const std::string& name) const -> std::size_t
{
    return m_name_to_index.at(name);
}

}
}
