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
    return 5;
}

/// @param data The raw memory for the counters
/// @return The maximum bytes a metric name can contain
inline auto max_name_bytes(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);

    uint16_t max_name_bytes;
    std::memcpy(&max_name_bytes, data, sizeof(uint16_t));

    assert(max_name_bytes > 0);

    return max_name_bytes;
}

/// @return The max_metrics offset in the raw memory
inline auto max_metrics_offset() -> std::size_t
{
    return 2;
}

/// @param data The raw memory for the counters
/// @return The maximum metrics the raw memory can contain
inline auto max_metrics(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);

    const uint8_t* max_metrics_data = data + max_metrics_offset();
    uint16_t max_metrics;
    std::memcpy(&max_metrics, max_metrics_data, sizeof(uint16_t));

    assert(max_metrics > 0);

    return max_metrics;
}

/// @param data The raw memory for the counters
/// @return The maximum metrics the raw memory can contain
inline auto names_offset() -> std::size_t
{
    // Skip header
    return header_bytes();
}

/// @param offset The offset in the raw memory
/// @return The extra padding to add to the offset for alignment
inline auto values_alignment_padding(std::size_t offset) -> std::size_t
{
    return 8 - offset % 8;
}

/// @param data The raw memory for the counters
/// @return The values offset in the raw memory
inline auto values_offset(const uint8_t* data) -> std::size_t
{
    // Skip header + scope + names
    std::size_t offset =
        header_bytes() + (max_metrics(data) * max_name_bytes(data));

    // align to 8 bytes
    offset += values_alignment_padding(offset);

    return offset;
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than max_metrics().
/// @return The raw name of a counter in memory
inline auto raw_name(uint8_t* data, std::size_t index) -> char*
{
    assert(data != nullptr);
    assert(index < max_metrics(data));

    uint8_t* name_data = data + names_offset() + (index * max_name_bytes(data));

    return (char*)name_data;
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than max_metrics().
/// @return The raw name of a counter in memory
inline auto raw_name(const uint8_t* data, std::size_t index) -> const char*
{
    assert(data != nullptr);
    assert(index < max_metrics(data));

    const uint8_t* name_data =
        data + names_offset() + (index * max_name_bytes(data));

    return (const char*)name_data;
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than max_metrics().
/// @return a pointer to the index'th counter value
inline auto raw_value(uint8_t* data, std::size_t index) -> uint64_t*
{
    assert(data != nullptr);
    assert(index < max_metrics(data));

    uint8_t* value_data =
        data + values_offset(data) + (index * sizeof(uint64_t));

    return reinterpret_cast<uint64_t*>(value_data);
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than max_metrics().
/// @return a pointer to the index'th counter value
inline auto raw_value(const uint8_t* data, std::size_t index) -> const uint64_t*
{
    assert(data != nullptr);
    assert(index < max_metrics(data));

    const uint8_t* value_data =
        data + values_offset(data) + (index * sizeof(uint64_t));

    return reinterpret_cast<const uint64_t*>(value_data);
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than max_metrics().
/// @return True if the metric is found in memory. False otherwise
inline auto is_metric_initialized(const uint8_t* data, std::size_t index)
    -> bool
{
    assert(index < max_metrics(data));
    const char* name_data = raw_name(data, index);

    // If the name is non-zero it is initialized and valid. We just check the
    // first byte to see if it's zero.
    return name_data[0] != 0;
}

/// @param data The raw memory for the counters
/// @return The number of initialized metrics in memory
inline auto metrics_count(const uint8_t* data) -> std::size_t
{
    assert(data != nullptr);

    std::size_t count = 0U;

    for (std::size_t i = 0; i < max_metrics(data); ++i)
    {
        if (is_metric_initialized(data, i))
        {
            ++count;
        }
    }

    return count;
}

/// @param data The raw memory for the counters
/// @param prettier If true, the output will be more human-readable format.
/// Otherwise, it will be compact JSON.
/// @param top_level If true, the json produced will be closed by brackets.
/// Intented to be used with the view_iterator class to gather all metrics
/// in a JSON object.
/// @return The counters in json-format
inline auto to_json(const uint8_t* data, bool prettier = true,
                    bool top_level = true) -> std::string
{
    std::string space = prettier ? " " : "";
    std::string newline = prettier ? "\n" : "";
    std::string tab = prettier ? "\t" : "";
    assert(data != nullptr);
    std::stringstream json_stream;
    if (top_level)
    {
        json_stream << "{" << newline;
    }

    for (std::size_t i = 0; i < metrics_count(data); ++i)
    {
        if ((!is_metric_initialized(data, i)))
        {
            continue;
        }

        auto n = raw_name(data, i);
        auto v = *raw_value(data, i);

        json_stream << tab << "\"" << std::string(n) << "\":" << space
                    << std::to_string(v);
        if (i != (metrics_count(data) - 1U))
        {
            json_stream << "," << newline;
        }
    }
    if (top_level)
    {
        json_stream << newline << "}";
    }

    return json_stream.str();
}

}
}
}
