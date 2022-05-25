// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <sstream>

#include "../value_types.hpp"
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
    std::memcpy(&metric_count, data, sizeof(uint16_t));

    assert(metric_count > 0);

    return metric_count;
}

inline auto scope_size_offset() -> std::size_t
{
    return 2;
}

/// @param data The raw memory for the counters
/// @return The raw memory of the scope_size.
inline auto raw_scope_size(uint8_t* data) -> uint16_t*
{
    assert(data != nullptr);
    uint8_t* scope_size_data = data + scope_size_offset();
    return (uint16_t*)scope_size_data;
}

/// @param data The raw memory for the counters
/// @return The raw memory of the scope_size.
inline auto raw_scope_size(const uint8_t* data) -> const uint16_t*
{
    assert(data != nullptr);
    const uint8_t* scope_size_data = data + scope_size_offset();
    return (const uint16_t*)scope_size_data;
}

/// @param data The raw memory for the counters
/// @return The current size of the scope the raw memory contains
inline auto scope_size(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);

    return *raw_scope_size(data);
}

inline auto name_bytes_offset() -> std::size_t
{
    return scope_size_offset() + sizeof(uint16_t);
}

/// @param data The raw memory for the counters
/// @return The raw memory of the name_bytes.
inline auto raw_name_bytes(uint8_t* data) -> uint16_t*
{
    assert(data != nullptr);
    uint8_t* name_bytes_data = data + name_bytes_offset();
    return (uint16_t*)name_bytes_data;
}

/// @param data The raw memory for the counters
/// @return The raw memory of the name_bytes.
inline auto raw_name_bytes(const uint8_t* data) -> const uint16_t*
{
    assert(data != nullptr);
    const uint8_t* name_bytes_data = data + name_bytes_offset();
    return (const uint16_t*)name_bytes_data;
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
    uint8_t* description_bytes_data = data + descriptions_bytes_offset();
    return (uint16_t*)description_bytes_data;
}

inline auto raw_descriptions_bytes(const uint8_t* data) -> const uint16_t*
{
    assert(data != nullptr);
    const uint8_t* description_bytes_data = data + descriptions_bytes_offset();
    return (const uint16_t*)description_bytes_data;
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

/// @param data The raw memory for the counters
/// @return The raw memory of the count of the eight byte values.
inline auto raw_eight_byte_count(uint8_t* data) -> uint16_t*
{
    assert(data != nullptr);
    uint8_t* eight_byte_count_data = data + eight_byte_count_offset();
    return (uint16_t*)eight_byte_count_data;
}

/// @param data The raw memory for the counters
/// @return The raw memory of the count of the eight byte values.
inline auto raw_eight_byte_count(const uint8_t* data) -> const uint16_t*
{
    assert(data != nullptr);
    const uint8_t* eight_byte_count_data = data + eight_byte_count_offset();
    return (const uint16_t*)eight_byte_count_data;
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

/// @param data The raw memory for the counters
/// @return The raw memory of the count of the one byte values.
inline auto raw_one_byte_count(uint8_t* data) -> uint16_t*
{
    assert(data != nullptr);
    uint8_t* one_byte_count_data = data + one_byte_count_offset();
    return (uint16_t*)one_byte_count_data;
}

/// @param data The raw memory for the counters
/// @return The raw memory of the count of the one byte values.
inline auto raw_one_byte_count(const uint8_t* data) -> const uint16_t*
{
    assert(data != nullptr);
    const uint8_t* one_byte_count_data = data + one_byte_count_offset();
    return (const uint16_t*)one_byte_count_data;
}

inline auto one_byte_count(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);

    return *raw_one_byte_count(data);
}

/// @param data The raw memory for the counters
/// @return The maximum metrics the raw memory can contain
inline auto names_offset() -> std::size_t
{
    // Skip header
    return header_bytes();
}

inline auto descriptions_offset(const uint8_t* data) -> std::size_t
{
    return names_offset() + name_bytes(data);
}

inline auto types_offset(const uint8_t* data) -> std::size_t
{
    return descriptions_offset(data) + descriptions_bytes(data);
}

/// @param offset The offset in the raw memory
/// @return The extra padding to add to the offset for alignment
inline auto alignment_padding(std::size_t offset, std::size_t alignment)
    -> std::size_t
{
    return alignment - offset % alignment;
}

/// @param data The raw memory for the counters
/// @return The values offset in the raw memory
inline auto values_offset(const uint8_t* data) -> std::size_t
{
    std::size_t offset =
        types_offset(data) + sizeof(uint8_t) * metric_count(data);

    return offset + alignment_padding(offset, sizeof(uint64_t));
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than metric_count().
/// @return The raw name of a counter in memory
inline auto raw_name(uint8_t* data, std::size_t bytes_offset) -> char*
{
    assert(data != nullptr);

    uint8_t* name_data = data + names_offset() + bytes_offset;

    return (char*)name_data;
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than metric_count().
/// @return The raw name of a counter in memory
inline auto raw_name(const uint8_t* data, std::size_t bytes_offset) -> const
    char*
{
    assert(data != nullptr);

    const uint8_t* name_data = data + names_offset() + bytes_offset;

    return (const char*)name_data;
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than metric_count().
/// @return The raw type of a counter in memory
inline auto raw_type(uint8_t* data, std::size_t index) -> uint8_t*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    uint8_t* type_data = data + types_offset(data) + index;

    return type_data;
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than metric_count().
/// @return The raw type of a counter in memory
inline auto raw_type(const uint8_t* data, std::size_t index) -> const uint8_t*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    const uint8_t* type_data = data + types_offset(data) + index;

    return type_data;
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than metric_count().
/// @return a pointer to the index'th counter value
template <typename T>
inline auto raw_value(uint8_t* data, std::size_t index) -> T*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    uint8_t* value_data;

    switch (sizeof(T))
    {
    case 8:
        value_data = data + values_offset(data) + index * 8;
        break;
    case 1:
        value_data = data + values_offset(data) + eight_byte_count(data) * 8 +
                     (index - eight_byte_count(data));
    }

    return reinterpret_cast<T*>(value_data);
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than metric_count().
/// @return a pointer to the index'th counter value
template <typename T>
inline auto raw_value(const uint8_t* data, std::size_t index) -> const T*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    const uint8_t* value_data;

    switch (sizeof(T))
    {
    case 8:
        value_data = data + values_offset(data) + index * 8;
        break;
    case 1:
        value_data = data + values_offset(data) + eight_byte_count(data) * 8 +
                     (index - eight_byte_count(data));
    }

    return reinterpret_cast<const T*>(value_data);
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than metric_count().
/// @return True if the metric is found in memory. False otherwise
inline auto has_metric(const uint8_t* data, std::size_t index) -> bool
{
    assert(index < metric_count(data));
    const char* name_data = raw_name(data, index);

    // If the name is non-zero it is initialized and valid. We just check the
    // first byte to see if it's zero.
    return name_data[0] != 0;
}

/// @param data The raw memory for the counters
/// @return The number of initialized metrics in memory
inline auto count(const uint8_t* data) -> std::size_t
{
    assert(data != nullptr);

    std::size_t count = 0U;

    for (std::size_t i = 0; i < metric_count(data); ++i)
    {
        if (has_metric(data, i))
        {
            ++count;
        }
    }

    return count;
}

/// @param data The raw memory for the counters
/// @return The scope offset in copied memory.
inline auto scope_offset(const uint8_t* data) -> std::size_t
{
    return values_offset(data) + (metric_count(data) * sizeof(uint64_t));
}

/// @param data The raw memory for the counters
/// @return The raw scope in memory
inline auto raw_scope(uint8_t* data) -> char*
{
    assert(data != nullptr);

    uint8_t* scope_data = data + scope_offset(data);

    return (char*)scope_data;
}

/// @param data The raw memory for the counters
/// @return The raw scope in memory
inline auto raw_scope(const uint8_t* data) -> const char*
{
    assert(data != nullptr);

    const uint8_t* scope_data = data + scope_offset(data);

    return (const char*)scope_data;
}

inline auto scope_alignment_padding(uint8_t* data) -> std::size_t
{
    std::size_t remainder = ((scope_offset(data) + scope_size(data)) % 8);
    if (remainder == 0)
    {
        return 0;
    }
    else
    {
        return 8 - remainder;
    }
}

/// @param data The raw memory for the counters
/// @param scope string to append to the front of the metric names in the json.
/// @param closed If true, the json produced will be closed by brackets.
/// Intented to be used with the view_iterator class to gather all metrics
/// in a JSON object.
/// @return The counters in json-format
inline auto to_json(const uint8_t* data, std::string scope = "",
                    bool closed = true) -> std::string
{
    std::string space = " ";
    std::string newline = "\n";
    std::string tab = "\t";
    assert(data != nullptr);
    std::stringstream json_stream;
    if (closed)
    {
        json_stream << "{" << newline;
    }

    for (std::size_t i = 0; i < count(data); ++i)
    {
        if ((!has_metric(data, i)))
        {
            continue;
        }

        auto n = raw_name(data, i);
        auto t = static_cast<value_type>(*raw_type(data, i));
        std::string value_string;
        switch (t)
        {
        case value_type::unsigned_integral:
            value_string = std::to_string(*raw_value<uint64_t>(data, i));
            break;
        case value_type::signed_integral:
            value_string = std::to_string(*raw_value<int64_t>(data, i));
            break;
        case value_type::boolean:
            value_string = std::to_string(*raw_value<bool>(data, i));
            break;
        case value_type::floating_point:
            value_string = std::to_string(*raw_value<double>(data, i));
            break;
        }

        json_stream << tab << "\"" << scope + "." + std::string(n)
                    << "\":" << space << value_string;
        if (i != (count(data) - 1U))
        {
            json_stream << "," << newline;
        }
    }
    if (closed)
    {
        json_stream << newline << "}";
    }

    return json_stream.str();
}

}
}
}
