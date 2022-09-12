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

/// @param data The raw memory for the counters
/// @return The maximum bytes a metric name can contain
inline auto metric_count(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);

    uint16_t metric_count;
    metric_count = *(uint16_t*)data;

    assert(metric_count > 0);

    return metric_count;
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

inline auto raw_name(uint8_t* data, std::size_t index) -> char*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    std::size_t offset = names_offset(data);
    for (std::size_t i = 0; i < index; ++i)
    {
        offset += name_size(data, i);
    }
    return (char*)(data + offset);
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

inline auto descriptions_offset(const uint8_t* data) -> std::size_t
{
    return names_offset(data) + name_bytes(data);
}

inline auto raw_description(uint8_t* data, std::size_t index) -> char*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    std::size_t offset = descriptions_offset(data);
    for (std::size_t i = 0; i < index; ++i)
    {
        offset += description_size(data, i);
    }
    return (char*)(data + offset);
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

inline auto is_constant_offset(const uint8_t* data) -> std::size_t
{
    return types_offset(data) + metric_count(data);
}

inline auto raw_is_constant(uint8_t* data, std::size_t index) -> bool*
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return (bool*)(data + is_constant_offset(data) + index);
}

inline auto raw_is_constant(const uint8_t* data, std::size_t index) -> const
    bool*
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return (const bool*)(data + is_constant_offset(data) + index);
}

inline auto is_constant(const uint8_t* data, std::size_t index) -> bool
{
    assert(data != nullptr);
    assert(index < metric_count(data));
    return *raw_is_constant(data, index);
}

/// @param offset The offset in the raw memory
/// @return The extra padding to add to the offset for alignment
inline auto alignment_padding(std::size_t offset) -> std::size_t
{
    auto remainder = offset % 8;
    return remainder == 0 ? 0 : 8 - remainder;
}

/// @param data The raw memory for the counters
/// @return The values offset in the raw memory
inline auto values_offset(const uint8_t* data) -> std::size_t
{
    std::size_t offset =
        is_constant_offset(data) + sizeof(bool) * metric_count(data);
    return offset + alignment_padding(offset);
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than metric_count().
/// @return a pointer to the index'th counter value
inline auto raw_value(uint8_t* data, std::size_t index) -> void*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    std::size_t offset = values_offset(data);
    uint16_t eight_byte_metrics = eight_byte_count(data);
    if (index < eight_byte_metrics)
    {
        return (void*)(data + offset + index * 8U);
    }
    return (void*)(data + offset + eight_byte_metrics * 8 +
                   (index - eight_byte_metrics));
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than metric_count().
/// @return a pointer to the index'th counter value
inline auto raw_value(const uint8_t* data, std::size_t index) -> const void*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    std::size_t offset = values_offset(data);
    uint16_t eight_byte_metrics = eight_byte_count(data);
    if (index < eight_byte_metrics)
    {
        return (const void*)(data + offset + index * 8U);
    }
    return (const void*)(data + offset + eight_byte_metrics * 8 +
                         (index - eight_byte_metrics));
}

inline auto is_metric_initialized(const uint8_t* data, std::size_t index)
    -> bool
{
    assert(index < metric_count(data));
    // If the name is non-zero it is initialized and valid.
    // We just check the first byte to see if it's zero.
    return type(data, index) != metric_type::uninitialized;
}

/// @param data The raw memory for the counters
/// @return The total number of bytes used.
inline auto storage_bytes(const uint8_t* data) -> std::size_t
{
    return values_offset(data) + eight_byte_count(data) * 8U +
           one_byte_count(data);
}
}
}
}
