// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <limits>
#include <sstream>

#include <endian/big_endian.hpp>
#include <endian/little_endian.hpp>

#include "../metric_kind.hpp"
#include "../metric_type.hpp"
#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{

/// @return The size of the header in bytes
inline auto header_bytes() -> std::size_t
{
    return 12;
}

inline auto is_big_endian_byte_offset() -> std::size_t
{
    return 0;
}

inline auto is_big_endian(const uint8_t* meta_data) -> bool
{
    return meta_data[is_big_endian_byte_offset()] != 0;
}

template <class T>
T read(const uint8_t* meta_data, const uint8_t* data)
{
    if (is_big_endian(meta_data))
    {
        return endian::big_endian::get<T>(data);
    }
    else
    {
        return endian::little_endian::get<T>(data);
    }
}

inline auto protocol_version_offset() -> std::size_t
{
    return is_big_endian_byte_offset() + sizeof(bool);
}

inline auto protocol_version(const uint8_t* meta_data) -> uint8_t
{
    return meta_data[protocol_version_offset()];
}

inline auto name_bytes_offset() -> std::size_t
{
    auto offset = protocol_version_offset() + sizeof(uint8_t);
    // Assert that the offset is a multiple of 2 as name_bytes is a uint16_t and
    // therefore must be aligned on a 2 byte boundary
    assert(offset % 2 == 0);
    return offset;
}

inline auto name_bytes(const uint8_t* meta_data) -> uint16_t
{
    return read<uint16_t>(meta_data, meta_data + name_bytes_offset());
}

inline auto descriptions_bytes_offset() -> std::size_t
{
    return name_bytes_offset() + sizeof(uint16_t);
}

inline auto descriptions_bytes(const uint8_t* meta_data) -> uint16_t
{
    return read<uint16_t>(meta_data, meta_data + descriptions_bytes_offset());
}

inline auto description_bytes(const uint8_t* meta_data) -> uint16_t
{
    return read<uint16_t>(meta_data, meta_data + descriptions_bytes_offset());
}

inline auto unit_bytes_offset() -> std::size_t
{
    return descriptions_bytes_offset() + sizeof(uint16_t);
}

inline auto unit_bytes(const uint8_t* meta_data) -> uint16_t
{
    return read<uint16_t>(meta_data, meta_data + unit_bytes_offset());
}

inline auto eight_byte_count_offset() -> std::size_t
{
    return unit_bytes_offset() + sizeof(uint16_t);
}

inline auto eight_byte_count(const uint8_t* meta_data) -> uint16_t
{
    return read<uint16_t>(meta_data, meta_data + eight_byte_count_offset());
}

inline auto one_byte_count_offset() -> std::size_t
{
    return eight_byte_count_offset() + sizeof(uint16_t);
}

inline auto one_byte_count(const uint8_t* meta_data) -> uint16_t
{
    return read<uint16_t>(meta_data, meta_data + one_byte_count_offset());
}

inline auto metric_count(const uint8_t* meta_data) -> uint16_t
{
    assert(meta_data != nullptr);
    return eight_byte_count(meta_data) + one_byte_count(meta_data);
}

inline auto name_sizes_offset() -> std::size_t
{
    return one_byte_count_offset() + sizeof(uint16_t);
}

inline auto name_size(const uint8_t* meta_data, std::size_t index) -> uint16_t
{
    assert(meta_data != nullptr);
    assert(index < metric_count(meta_data));

    return read<uint16_t>(meta_data, meta_data + name_sizes_offset() +
                                         index * sizeof(uint16_t));
}

inline auto description_sizes_offset(const uint8_t* meta_data) -> std::size_t
{
    return name_sizes_offset() + metric_count(meta_data) * sizeof(uint16_t);
}

inline auto description_size(const uint8_t* meta_data, std::size_t index)
    -> uint16_t
{
    assert(meta_data != nullptr);
    assert(index < metric_count(meta_data));

    return read<uint16_t>(meta_data, meta_data +
                                         description_sizes_offset(meta_data) +
                                         index * sizeof(uint16_t));
}

inline auto unit_sizes_offset(const uint8_t* meta_data) -> std::size_t
{
    return description_sizes_offset(meta_data) +
           metric_count(meta_data) * sizeof(uint16_t);
}

inline auto unit_size(const uint8_t* meta_data, std::size_t index) -> uint16_t
{
    assert(meta_data != nullptr);
    assert(index < metric_count(meta_data));

    return read<uint16_t>(meta_data, meta_data + unit_sizes_offset(meta_data) +
                                         index * sizeof(uint16_t));
}

inline auto names_offset(const uint8_t* meta_data) -> std::size_t
{
    return unit_sizes_offset(meta_data) +
           metric_count(meta_data) * sizeof(uint16_t);
}

inline auto name(const uint8_t* meta_data, std::size_t index) -> const char*
{
    assert(meta_data != nullptr);
    assert(index < metric_count(meta_data));

    std::size_t offset = names_offset(meta_data);
    for (std::size_t i = 0; i < index; ++i)
    {
        offset += name_size(meta_data, i);
    }
    return (const char*)(meta_data + offset);
}

inline auto name(uint8_t* meta_data, std::size_t index) -> char*
{
    return const_cast<char*>(
        name(const_cast<const uint8_t*>(meta_data), index));
}

inline auto descriptions_offset(const uint8_t* meta_data) -> std::size_t
{
    return names_offset(meta_data) + name_bytes(meta_data);
}

inline auto description(const uint8_t* meta_data, std::size_t index) -> const
    char*
{
    assert(meta_data != nullptr);
    assert(index < metric_count(meta_data));

    std::size_t offset = descriptions_offset(meta_data);
    for (std::size_t i = 0; i < index; ++i)
    {
        offset += description_size(meta_data, i);
    }
    return (const char*)(meta_data + offset);
}

inline auto description(uint8_t* meta_data, std::size_t index) -> char*
{
    return const_cast<char*>(
        description(const_cast<const uint8_t*>(meta_data), index));
}

inline auto units_offset(const uint8_t* meta_data) -> std::size_t
{
    return descriptions_offset(meta_data) + descriptions_bytes(meta_data);
}

inline auto unit(const uint8_t* meta_data, std::size_t index) -> const char*
{
    assert(meta_data != nullptr);
    assert(index < metric_count(meta_data));

    std::size_t offset = units_offset(meta_data);
    for (std::size_t i = 0; i < index; ++i)
    {
        offset += unit_size(meta_data, i);
    }
    return (const char*)(meta_data + offset);
}

inline auto unit(uint8_t* meta_data, std::size_t index) -> char*
{
    return const_cast<char*>(
        unit(const_cast<const uint8_t*>(meta_data), index));
}

inline auto types_offset(const uint8_t* meta_data) -> std::size_t
{
    return units_offset(meta_data) + unit_bytes(meta_data);
}

inline auto type(const uint8_t* meta_data, std::size_t index) -> metric_type
{
    assert(meta_data != nullptr);
    assert(index < metric_count(meta_data));

    std::size_t offset = types_offset(meta_data) + index;
    return static_cast<metric_type>(
        read<uint8_t>(meta_data, meta_data + offset));
}

inline auto kind_offset(const uint8_t* meta_data) -> std::size_t
{
    return types_offset(meta_data) + metric_count(meta_data);
}

inline auto kind(const uint8_t* meta_data, std::size_t index) -> metric_kind
{
    assert(meta_data != nullptr);
    assert(index < metric_count(meta_data));

    std::size_t offset = kind_offset(meta_data) + index;
    return static_cast<metric_kind>(
        read<uint8_t>(meta_data, meta_data + offset));
}

inline auto meta_bytes(const uint8_t* meta_data) -> std::size_t
{
    return kind_offset(meta_data) + sizeof(uint8_t) * metric_count(meta_data);
}

/// @param offset The offset in the raw memory
/// @return The extra padding to add to the offset for alignment
inline auto alignment_padding(std::size_t offset) -> std::size_t
{
    auto remainder = offset % 8;
    return remainder == 0 ? 0 : 8 - remainder;
}

inline auto value_ptr(const uint8_t* meta_data, uint8_t* counter_data,
                      std::size_t index) -> void*
{
    assert(meta_data != nullptr);
    assert(counter_data != nullptr);
    assert(index < metric_count(meta_data));

    uint16_t eight_byte_metrics = eight_byte_count(meta_data);
    if (index < eight_byte_metrics)
    {
        return static_cast<void*>(counter_data + index * 8U);
    }

    return static_cast<void*>(counter_data + eight_byte_metrics * 8 +
                              (index - eight_byte_metrics));
}

template <class T>
inline T value(const uint8_t* meta_data, const uint8_t* counter_data,
               std::size_t index)
{
    assert(meta_data != nullptr);
    assert(counter_data != nullptr);
    assert(index < metric_count(meta_data));

    return read<T>(meta_data,
                   static_cast<uint8_t*>(value_ptr(
                       meta_data, const_cast<uint8_t*>(counter_data), index)));
}

inline auto metrics_bytes(const uint8_t* meta_data) -> std::size_t
{
    return eight_byte_count(meta_data) * 8U + one_byte_count(meta_data);
}

inline auto is_metric_initialized(const uint8_t* meta_data,
                                  const uint8_t* value_data, std::size_t index)
    -> bool
{
    assert(index < metric_count(meta_data));
    // The last bytes after the value data is a bitset of initialized metrics
    auto initialized = value_data + metrics_bytes(meta_data);
    return (initialized[index / 8] & (1 << (index % 8))) != 0;
}

inline void set_metric_initialized(const uint8_t* meta_data,
                                   uint8_t* value_data, std::size_t index)
{
    assert(index < metric_count(meta_data));
    // The last bytes after the value data is a bitset of initialized metrics
    auto initialized = value_data + metrics_bytes(meta_data);
    initialized[index / 8] |= (1 << (index % 8));
}

inline auto value_bytes(const uint8_t* meta_data) -> std::size_t
{
    return metrics_bytes(meta_data) + (metric_count(meta_data) + 7) / 8;
}
}
}
}
