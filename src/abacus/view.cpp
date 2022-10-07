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

void view::set_meta_data(const uint8_t* meta_data)
{
    m_meta_data = meta_data;
    // Make sure that the data is 8-byte-aligned
    assert(reinterpret_cast<uint64_t>(m_meta_data) % 8U == 0U);
    m_value_data = nullptr;

    for (std::size_t i = 0; i < count(); ++i)
    {
        m_name_to_index.emplace(name(i), i);
    }
}

void view::set_value_data(const uint8_t* value_data)
{
    assert(m_meta_data != nullptr);
    m_value_data = value_data;

    // Make sure that the data is 8-byte-aligned
    assert(reinterpret_cast<uint64_t>(m_value_data) % 8U == 0U);
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

auto view::count() const -> uint16_t
{
    return detail::metric_count(m_meta_data);
}

auto view::is_initialized(std::size_t index) const -> bool
{
    assert(index < count());
    return detail::is_metric_initialized(m_meta_data, m_value_data, index);
}

auto view::name(std::size_t index) const -> std::string
{
    return {detail::name(m_meta_data, index),
            detail::name_size(m_meta_data, index)};
}

auto view::description(std::size_t index) const -> std::string
{
    return {detail::description(m_meta_data, index),
            detail::description_size(m_meta_data, index)};
}

auto view::type(std::size_t index) const -> abacus::metric_type
{
    return detail::type(m_meta_data, index);
}

auto view::is_constant(std::size_t index) const -> bool
{
    return static_cast<bool>(detail::flags(m_meta_data, index) &
                             metric_flags::constant);
}

void view::value(std::size_t index, bool& value) const
{
    assert(is_initialized(index));
    assert(type(index) == metric_type::boolean);
    value = detail::value<bool>(m_meta_data, m_value_data, index);
}

void view::value(std::size_t index, uint64_t& value) const
{
    assert(is_initialized(index));
    assert(type(index) == metric_type::uint64);
    value = detail::value<uint64_t>(m_meta_data, m_value_data, index);
}

void view::value(std::size_t index, int64_t& value) const
{
    assert(is_initialized(index));
    assert(type(index) == metric_type::int64);
    value = detail::value<int64_t>(m_meta_data, m_value_data, index);
}

void view::value(std::size_t index, double& value) const
{
    assert(is_initialized(index));
    assert(type(index) == metric_type::float64);
    value = detail::value<double>(m_meta_data, m_value_data, index);
}

auto view::index(const std::string& name) const -> std::size_t
{
    return m_name_to_index.at(name);
}

}
}
