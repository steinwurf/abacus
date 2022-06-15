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

inline auto raw_eight_byte_count(uint8_t* data) -> uint16_t*
{
    assert(data != nullptr);
    uint8_t* eight_byte_count_data = data + eight_byte_count_offset();
    return (uint16_t*)eight_byte_count_data;
}

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

inline auto raw_one_byte_count(uint8_t* data) -> uint16_t*
{
    assert(data != nullptr);
    uint8_t* one_byte_count_data = data + one_byte_count_offset();
    return (uint16_t*)one_byte_count_data;
}

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

inline auto name_sizes_offset() -> std::size_t
{
    return header_bytes();
}

inline auto raw_name_size(uint8_t* data, std::size_t index) -> uint16_t*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    uint8_t* name_sizes_data =
        data + name_sizes_offset() + index * sizeof(uint16_t);
    return (uint16_t*)name_sizes_data;
}

inline auto raw_name_size(const uint8_t* data, std::size_t index)
    -> const uint16_t*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    const uint8_t* name_sizes_data =
        data + name_sizes_offset() + index * sizeof(uint16_t);
    return (const uint16_t*)name_sizes_data;
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

    uint8_t* description_size_data =
        data + description_sizes_offset(data) + index * sizeof(uint16_t);
    return (uint16_t*)description_size_data;
}

inline auto raw_description_size(const uint8_t* data, std::size_t index)
    -> const uint16_t*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    const uint8_t* description_sizes_data =
        data + description_sizes_offset(data) + index * sizeof(uint16_t);
    return (const uint16_t*)description_sizes_data;
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
    // Skip header
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

    uint8_t* names_data = data + offset;
    return (char*)names_data;
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

    const uint8_t* names_data = data + offset;
    return (const char*)names_data;
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

    uint8_t* description_data = data + offset;
    return (char*)description_data;
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

    const uint8_t* description_data = data + offset;
    return (const char*)description_data;
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

inline auto is_constant_offset(const uint8_t* data) -> std::size_t
{
    return types_offset(data) + metric_count(data);
}

inline auto raw_is_constant(uint8_t* data, std::size_t index) -> bool*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    uint8_t* is_constant_data = data + is_constant_offset(data) + index;

    return (bool*)is_constant_data;
}

inline auto raw_is_constant(const uint8_t* data, std::size_t index) -> const
    bool*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    const uint8_t* is_constant_data = data + is_constant_offset(data) + index;

    return (const bool*)is_constant_data;
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
    return offset % 8 == 0 ? 0 : 8 - offset % 8;
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
template <typename T>
inline auto raw_value(uint8_t* data, std::size_t index) -> T*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    std::size_t offset = values_offset(data);
    uint16_t eight_byte_metrics = eight_byte_count(data);
    if (index < eight_byte_metrics)
    {
        assert(sizeof(T) == 8);
        return (T*)(data + offset + index * sizeof(T));
    }
    return (T*)(data + offset + eight_byte_metrics * 8 +
                (index - eight_byte_metrics) * sizeof(T));
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than metric_count().
/// @return a pointer to the index'th counter value
template <typename T>
inline auto raw_value(const uint8_t* data, std::size_t index) -> const T*
{
    assert(data != nullptr);
    assert(index < metric_count(data));

    std::size_t offset = values_offset(data);
    uint16_t eight_byte_metrics = eight_byte_count(data);
    if (index < eight_byte_metrics)
    {
        assert(sizeof(T) == 8);
        return (const T*)(data + offset + index * sizeof(T));
    }
    assert(sizeof(T) == 1);
    return (const T*)(data + offset + eight_byte_metrics * 8 +
                      (index - eight_byte_metrics) * sizeof(T));
}

inline auto metric_index(const uint8_t* data, const char* name) -> std::size_t
{
    assert(data != nullptr);
    assert(name != nullptr);

    for (std::size_t i = 0; i < metric_count(data); ++i)
    {
        if (strcmp(name, raw_name(data, i)) == 0)
        {
            return i;
        }
    }
    assert(false && "No metric with that name was found");
    return std::numeric_limits<std::size_t>::max();
}

inline auto is_metric_initialized(const uint8_t* data, std::size_t index)
    -> bool
{
    assert(index < metric_count(data));

    const char* name_data = raw_name(data, index);

    // If the name is non-zero it is initialized and valid. We just check the
    // first byte to see if it's zero.
    return name_data[0] != 0;
}

/// @param data The raw memory for the counters
/// @return The scope offset in copied memory.
inline auto scope_offset(const uint8_t* data) -> std::size_t
{
    return values_offset(data) + (eight_byte_count(data) * sizeof(uint64_t) +
                                  one_byte_count(data) * sizeof(bool));
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
    return remainder == 0 ? 0 : 8 - remainder;
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

    for (std::size_t i = 0; i < metric_count(data); ++i)
    {
        if ((!is_metric_initialized(data, i)))
        {
            continue;
        }

        auto n = raw_name(data, i);
        auto d = raw_description(data, i);
        auto t = static_cast<value_type>(*raw_type(data, i));
        auto c = is_constant(data, i);
        std::string type_string;
        std::string value_string;
        switch (t)
        {
        case value_type::unsigned_integral:
            type_string = "unsigned integer";
            value_string = std::to_string(*raw_value<uint64_t>(data, i));
            break;
        case value_type::signed_integral:
            type_string = "signed integer";
            value_string = std::to_string(*raw_value<int64_t>(data, i));
            break;
        case value_type::boolean:
            type_string = "boolean";
            value_string = *raw_value<bool>(data, i) ? "true" : "false";
            break;
        case value_type::floating_point:
            type_string = "floating point";
            value_string = std::to_string(*raw_value<double>(data, i));
            break;
        }

        // The key is the scope + the name
        json_stream << tab << "\"" << scope + "." + std::string(n) << "\": {";

        // Add a JSON object with description, value and is_constant bool.
        json_stream << newline << tab << tab << "\"description\": \"" << d
                    << "\",";
        json_stream << newline << tab << tab << "\"value\": " << value_string
                    << ",";
        json_stream << newline << tab << tab
                    << "\"is_constant\": " << (c ? "true" : "false") << ",";

        // Close the brackets.
        json_stream << newline << tab << "}";

        if (i != (metric_count(data) - 1U))
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
