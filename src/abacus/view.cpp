// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <iostream>
#include <limits>

#include "detail/raw.hpp"
#include "view.hpp"

#include <endian/big_endian.hpp>
#include <endian/is_big_endian.hpp>
#include <endian/little_endian.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

template<class Value> 
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
    m_scope =
        std::string(detail::raw_scope(m_data), detail::scope_size(m_data));

    uint8_t endianness = detail::endianness(m_data);

    if (endianness == 0U)
    {
        m_is_little_endian = true;
    }
    else if (endianness == 1U)
    {
        m_is_big_endian = true;
    }
    else
    {
        assert(0 && "Endianness byte is not 0 or 1. Something went wrong.");
    }

    m_is_same_endianness = (bool)endianness == endian::is_big_endian();

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
    if (m_is_same_endianness)
    {
        return detail::metric_count(m_data);
    }
    else
    {
        if (m_is_little_endian)
        {
            endian::little_endian m_little_endian;
            return m_little_endian.get<uint16_t>(m_data);
        }
        else
        {
            endian::big_endian m_big_endian;
            return m_big_endian.get<uint16_t>(m_data);
        }
    }
}

auto view::scope_size() const -> uint16_t
{
    if (m_is_same_endianness)
    {
        return detail::scope_size(m_data);
    }
    else
    {
        if (m_is_little_endian)
        {
            endian::little_endian m_little_endian;
            return m_little_endian.get<uint16_t>(
                (uint8_t*)(detail::raw_scope_size(m_data)));
        }
        else
        {
            endian::big_endian m_big_endian;
            return m_big_endian.get<uint16_t>(
                (uint8_t*)detail::raw_scope_size(m_data));
        }
    }
}

auto view::endianness() const -> uint8_t
{
    return detail::endianness(m_data);
}

auto view::name_bytes() const -> uint16_t
{
    if (m_is_same_endianness)
    {
        return detail::name_bytes(m_data);
    }
    else
    {
        if (m_is_little_endian)
        {
            endian::little_endian m_little_endian;
            return m_little_endian.get<uint16_t>(
                (uint8_t*)(detail::raw_name_bytes(m_data)));
        }
        else
        {
            endian::big_endian m_big_endian;
            return m_big_endian.get<uint16_t>(
                (uint8_t*)detail::raw_name_bytes(m_data));
        }
    }
}

auto view::description_bytes() const -> uint16_t
{
    if (m_is_same_endianness)
    {
        return detail::descriptions_bytes(m_data);
    }
    else
    {
        if (m_is_little_endian)
        {
            endian::little_endian m_little_endian;
            return m_little_endian.get<uint16_t>(
                (uint8_t*)(detail::raw_descriptions_bytes(m_data)));
        }
        else
        {
            endian::big_endian m_big_endian;
            return m_big_endian.get<uint16_t>(
                (uint8_t*)detail::raw_descriptions_bytes(m_data));
        }
    }
}

auto view::eight_byte_count() const -> uint16_t
{
    if (m_is_same_endianness)
    {
        return detail::eight_byte_count(m_data);
    }
    else
    {
        if (m_is_little_endian)
        {
            endian::little_endian m_little_endian;
            return m_little_endian.get<uint16_t>(
                (uint8_t*)(detail::raw_eight_byte_count(m_data)));
        }
        else
        {
            endian::big_endian m_big_endian;
            return m_big_endian.get<uint16_t>(
                (uint8_t*)detail::raw_eight_byte_count(m_data));
        }
    }
}

auto view::one_byte_count() const -> uint16_t
{
    if (m_is_same_endianness)
    {
        return detail::one_byte_count(m_data);
    }
    else
    {
        if (m_is_little_endian)
        {
            endian::little_endian m_little_endian;
            return m_little_endian.get<uint16_t>(
                (uint8_t*)(detail::raw_one_byte_count(m_data)));
        }
        else
        {
            endian::big_endian m_big_endian;
            return m_big_endian.get<uint16_t>(
                (uint8_t*)detail::raw_one_byte_count(m_data));
        }
    }
}

auto view::is_metric_initialized(std::size_t index) const -> bool
{
    assert(index < metric_count());
    const char* name_data = detail::raw_name(m_data, index);
    return name_data[0] != 0;
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
    value = *(bool*)(detail::raw_value(m_data, index));
}

void view::metric_value(std::size_t index, uint64_t& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == value_type::uint64);
    if (m_is_same_endianness)
    {
        value = *(uint64_t*)(detail::raw_value(m_data, index));
    }
    else
    {
        if (m_is_little_endian)
        {
            endian::little_endian m_little_endian;
            value = m_little_endian.get<uint64_t>(
                (uint8_t*)(detail::raw_value(m_data, index)));
        }
        else
        {
            endian::big_endian m_big_endian;
            value = m_big_endian.get<uint64_t>(
                (uint8_t*)(detail::raw_value(m_data, index)));
        }
    }
}

void view::metric_value(std::size_t index, int64_t& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == value_type::int64);
    if (m_is_same_endianness)
    {
        value = *(int64_t*)(detail::raw_value(m_data, index));
    }
    else
    {
        if (m_is_little_endian)
        {
            endian::little_endian m_little_endian;
            value = m_little_endian.get<int64_t>(
                (uint8_t*)(detail::raw_value(m_data, index)));
        }
        else
        {
            endian::big_endian m_big_endian;
            value = m_big_endian.get<int64_t>(
                (uint8_t*)(detail::raw_value(m_data, index)));
        }
    }
}

void view::metric_value(std::size_t index, double& value) const
{
    assert(is_metric_initialized(index));
    assert(metric_type(index) == value_type::float64);
    if (m_is_same_endianness)
    {
        value = *(double*)(detail::raw_value(m_data, index));
    }
    else
    {
        if (m_is_little_endian)
        {
            endian::little_endian m_little_endian;
            value = m_little_endian.get<double>(
                (uint8_t*)(detail::raw_value(m_data, index)));
        }
        else
        {
            endian::big_endian m_big_endian;
            value = m_big_endian.get<double>(
                (uint8_t*)(detail::raw_value(m_data, index)));
        }
    }
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

    offset +=
        eight_byte_count() * sizeof(uint64_t) + one_byte_count() * sizeof(bool);

    offset += scope_size() + detail::scope_alignment_padding(m_data);

    return offset;
}

auto view::to_json() const -> std::string
{
    return detail::to_json(m_data, scope());
}
}
}
