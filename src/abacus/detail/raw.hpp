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

#include "../metric_flags.hpp"
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

inline auto is_big_endian_byte(const uint8_t* data) -> uint8_t;

/// @param data The raw memory for the counters
/// @return The maximum bytes a metric name can contain
inline auto metric_count(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);

    if (is_big_endian_byte(data))
    {
        return endian::big_endian::get<uint16_t>(data);
    }
    else
    {
        return endian::little_endian::get<uint16_t>(data);
    }
}

inline auto is_big_endian_byte_offset() -> std::size_t
{
    return sizeof(uint16_t);
}

inline auto raw_is_big_endian_byte(uint8_t* data) -> uint8_t*
{
    assert(data != nullptr);
    return data + is_big_endian_byte_offset();
}

inline auto raw_is_big_endian_byte(const uint8_t* data) -> const uint8_t*
{
    assert(data != nullptr);
    return data + is_big_endian_byte_offset();
}

inline auto is_big_endian_byte(const uint8_t* data) -> uint8_t
{
    return *raw_is_big_endian_byte(data);
}

inline auto name_bytes_offset() -> std::size_t
{
    return is_big_endian_byte_offset() + sizeof(uint16_t);
}

/// @param data The raw memory for the counters
/// @return The raw memory of the name_bytes.
inline auto raw_name_bytes(uint8_t* data) -> uint16_t*
{
    assert(data != nullptr);
    return (uint16_t*)(data + name_bytes_offset());
}

/// @param data The raw memory for the counters
/// @return The raw memory of the name_bytes.
inline auto raw_name_bytes(const uint8_t* data) -> const uint16_t*
{
    assert(data != nullptr);
    return (const uint16_t*)(data + name_bytes_offset());
}

/// @param data The raw memory for the counters
/// @return The raw memory of the name_bytes.
inline auto name_bytes(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);
    return *raw_name_bytes(data);
}

inline auto descriptions_bytes_offset() -> std::size_t
{
    return name_bytes_offset() + sizeof(uint16_t);
}

/// @param data The raw memory for the counters
/// @return The raw memory of the name_bytes.
inline auto raw_descriptions_bytes(uint8_t* data) -> uint16_t*
{
    assert(data != nullptr);
    return (uint16_t*)(data + descriptions_bytes_offset());
}

inline auto raw_descriptions_bytes(const uint8_t* data) -> const uint16_t*
{
    assert(data != nullptr);
    return (const uint16_t*)(data + descriptions_bytes_offset());
}

inline auto descriptions_bytes(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);
    return *raw_descriptions_bytes(data);
}

inline auto eight_byte_count_offset() -> std::size_t
{
    return descriptions_bytes_offset() + sizeof(uint16_t);
}

inline auto raw_eight_byte_count(uint8_t* data) -> uint16_t*
{
    assert(data != nullptr);
    return (uint16_t*)(data + eight_byte_count_offset());
}

inline auto raw_eight_byte_count(const uint8_t* data) -> const uint16_t*
{
    assert(data != nullptr);
    return (const uint16_t*)(data + eight_byte_count_offset());
}

inline auto eight_byte_count(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);
    return *raw_eight_byte_count(data);
}

inline auto one_byte_count_offset() -> std::size_t
{
    return eight_byte_count_offset() + sizeof(uint16_t);
}

inline auto raw_one_byte_count(uint8_t* data) -> uint16_t*
{
    assert(data != nullptr);
    return (uint16_t*)(data + one_byte_count_offset());
}

inline auto raw_one_byte_count(const uint8_t* data) -> const uint16_t*
{
    assert(data != nullptr);
    return (const uint16_t*)(data + one_byte_count_offset());
}

inline auto one_byte_count(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);
    return *raw_one_byte_count(data);
}

inline auto name_sizes_offset() -> std::size_t
{
    return header_bytes();
}

inline auto raw_name_size(uint8_t* data, std::size_t index) -> uint16_t*
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return (uint16_t*)(data + name_sizes_offset() + index * sizeof(uint16_t));
}

inline auto raw_name_size(const uint8_t* data, std::size_t index)
    -> const uint16_t*
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return (uint16_t*)(data + name_sizes_offset() + index * sizeof(uint16_t));
}

inline auto name_size(const uint8_t* data, std::size_t index) -> uint16_t
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return *raw_name_size(data, index);
}

inline auto description_sizes_offset(const uint8_t* data) -> std::size_t
{
    return name_sizes_offset() + metric_count(data) * sizeof(uint16_t);
}

inline auto raw_description_size(uint8_t* data, std::size_t index) -> uint16_t*
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return (uint16_t*)(data + description_sizes_offset(data) +
                       index * sizeof(uint16_t));
}

inline auto raw_description_size(const uint8_t* data, std::size_t index)
    -> const uint16_t*
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return (uint16_t*)(data + description_sizes_offset(data) +
                       index * sizeof(uint16_t));
}

inline auto description_size(const uint8_t* data, std::size_t index) -> uint16_t
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return *raw_description_size(data, index);
}

/// @param data The raw memory for the counters
/// @return The maximum metrics the raw memory can contain
inline auto names_offset(const uint8_t* data) -> std::size_t
{
    return description_sizes_offset(data) +
           metric_count(data) * sizeof(uint16_t);
}

inline auto raw_name(const uint8_t* data, std::size_t index) -> const char*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    std::size_t offset = names_offset(data);
    for (std::size_t i = 0; i < index; ++i)
    {
        offset += name_size(data, i);
    }
    return (const char*)(data + offset);
}

inline auto raw_name(uint8_t* data, std::size_t index) -> char*
{
    return const_cast<char*>(raw_name(const_cast<const uint8_t*>(data), index));
}

inline auto descriptions_offset(const uint8_t* data) -> std::size_t
{
    return names_offset(data) + name_bytes(data);
}

inline auto raw_description(const uint8_t* data, std::size_t index) -> const
    char*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    std::size_t offset = descriptions_offset(data);
    for (std::size_t i = 0; i < index; ++i)
    {
        offset += description_size(data, i);
    }
    return (const char*)(data + offset);
}

inline auto raw_description(uint8_t* data, std::size_t index) -> char*
{
    return const_cast<char*>(
        raw_description(const_cast<const uint8_t*>(data), index));
}

inline auto types_offset(const uint8_t* data) -> std::size_t
{
    return descriptions_offset(data) + descriptions_bytes(data);
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than metric_count().
/// @return The raw type of a counter in memory
inline auto raw_type(uint8_t* data, std::size_t index) -> uint8_t*
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return (data + types_offset(data) + index);
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than metric_count().
/// @return The raw type of a counter in memory
inline auto raw_type(const uint8_t* data, std::size_t index) -> const uint8_t*
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return (data + types_offset(data) + index);
}

inline auto type(const uint8_t* data, std::size_t index) -> metric_type
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return static_cast<metric_type>(*raw_type(data, index));
}

inline auto flags_offset(const uint8_t* data) -> std::size_t
{
    return types_offset(data) + metric_count(data);
}

inline auto raw_flags(uint8_t* data, std::size_t index) -> metric_flags*
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return (metric_flags*)(data + flags_offset(data) + index);
}

inline auto raw_flags(const uint8_t* data, std::size_t index)
    -> const metric_flags*
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return (const metric_flags*)(data + flags_offset(data) + index);
}

inline auto flags(const uint8_t* data, std::size_t index) -> metric_flags
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return static_cast<metric_flags>(*raw_flags(data, index));
}

/// @param offset The offset in the raw memory
/// @return The extra padding to add to the offset for alignment
inline auto alignment_padding(std::size_t offset) -> std::size_t
{
    auto remainder = offset % 8;
    return remainder == 0 ? 0 : 8 - remainder;
}

inline auto meta_bytes(const uint8_t* data) -> std::size_t
{
    return flags_offset(data) + sizeof(bool) * metric_count(data);
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than metric_count().
/// @return a pointer to the index'th counter value
inline auto raw_value(const uint8_t* meta_data, const uint8_t* value_data,
                      std::size_t index) -> const void*
{
    assert(meta_data != nullptr);
    assert(value_data != nullptr);
    assert(index < metric_count(meta_data));

    uint16_t eight_byte_metrics = eight_byte_count(meta_data);
    if (index < eight_byte_metrics)
    {
        return (void*)(value_data + index * 8U);
    }
    return (void*)(value_data + eight_byte_metrics * 8 +
                   (index - eight_byte_metrics));
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than metric_count().
/// @return a pointer to the index'th counter value
inline auto raw_value(const uint8_t* meta_data, uint8_t* value_data,
                      std::size_t index) -> void*
{
    return const_cast<void*>(
        raw_value(meta_data, const_cast<const uint8_t*>(value_data), index));
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
