// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <limits>

#include "detail/raw.hpp"
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
    // assert(reinterpret_cast<uint64_t>(m_meta_data) % 8U == 0U);
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
    // assert(reinterpret_cast<uint64_t>(m_value_data) % 8U == 0U);
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

auto view::protocol_version() const -> uint8_t
{
    return detail::protocol_version(m_meta_data);
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

auto view::unit(std::size_t index) const -> std::string
{
    return {detail::unit(m_meta_data, index),
            detail::unit_size(m_meta_data, index)};
}

auto view::type(std::size_t index) const -> abacus::type
{
    return detail::type(m_meta_data, index);
}

auto view::kind(std::size_t index) const -> abacus::kind
{
    return detail::kind(m_meta_data, index);
}

auto view::min(std::size_t index) const -> abacus::min
{
    switch (type(index))
    {
    case abacus::type::boolean:
        return detail::min_value<uint64_t>(m_meta_data, index);
    case abacus::type::uint64:
        return detail::min_value<uint64_t>(m_meta_data, index);
    case abacus::type::int64:
        return detail::min_value<int64_t>(m_meta_data, index);
    case abacus::type::float64:
        return detail::min_value<double>(m_meta_data, index);
    default:
        assert(false && "Unknown type");
        return abacus::min{};
    }
}

auto view::max(std::size_t index) const -> abacus::max
{
    switch (type(index))
    {
    case abacus::type::boolean:
        return detail::max_value<uint64_t>(m_meta_data, index);
    case abacus::type::uint64:
        return detail::max_value<uint64_t>(m_meta_data, index);
    case abacus::type::int64:
        return detail::max_value<int64_t>(m_meta_data, index);
    case abacus::type::float64:
        return detail::max_value<double>(m_meta_data, index);
    default:
        assert(false && "Unknown type");
        return abacus::max{};
    }
}

void view::value(std::size_t index, bool& value) const
{
    assert(is_initialized(index));
    assert(type(index) == abacus::type::boolean);
    value = detail::value<bool>(m_meta_data, m_value_data, index);
}

void view::value(std::size_t index, uint64_t& value) const
{
    assert(is_initialized(index));
    assert(type(index) == abacus::type::uint64);
    value = detail::value<uint64_t>(m_meta_data, m_value_data, index);
}

void view::value(std::size_t index, int64_t& value) const
{
    assert(is_initialized(index));
    assert(type(index) == abacus::type::int64);
    value = detail::value<int64_t>(m_meta_data, m_value_data, index);
}

void view::value(std::size_t index, double& value) const
{
    assert(is_initialized(index));
    assert(type(index) == abacus::type::float64);
    value = detail::value<double>(m_meta_data, m_value_data, index);
}

auto view::index(const std::string& name) const -> std::size_t
{
    return m_name_to_index.at(name);
}

}
}
