// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>

#include "../version.hpp"
#include <bourne/json.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{

/// @return The size of the header in bytes
inline auto header_bytes() -> std::size_t
{
    return 7;
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

/// @return The offset of the max_scope_bytes data in the header
inline auto max_scope_bytes_offset() -> std::size_t
{
    return 2;
}

/// @param data The raw memory for the counters
/// @return The maximum bytes a scope can contain
inline auto max_scope_bytes(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);

    const uint8_t* max_scope_bytes_data = data + max_scope_bytes_offset();
    uint16_t max_scope_bytes;
    std::memcpy(&max_scope_bytes, max_scope_bytes_data, sizeof(uint16_t));

    assert(max_scope_bytes > 0);

    return max_scope_bytes;
}

/// @return The max_metrics offset in the raw memory
inline auto max_metrics_offset() -> std::size_t
{
    return 4;
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

/// @return The scope offset in the raw memory
inline auto scope_offset() -> std::size_t
{
    return header_bytes();
}

/// @param data The raw memory for the counters
/// @return The maximum metrics the raw memory can contain
inline auto names_offset(const uint8_t* data) -> std::size_t
{
    // Skip header + scope
    return header_bytes() + max_scope_bytes(data);
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
    std::size_t offset = header_bytes() + max_scope_bytes(data) +
                         (max_metrics(data) * max_name_bytes(data));

    // align to 8 bytes
    offset += values_alignment_padding(offset);

    return offset;
}

/// @param data The raw memory for the counters
/// @return The raw scope in memory
inline auto raw_scope(uint8_t* data) -> char*
{
    assert(data != nullptr);

    uint8_t* scope_data = data + scope_offset();

    return (char*)scope_data;
}

/// @param data The raw memory for the counters
/// @return The raw scope in memory
inline auto raw_scope(const uint8_t* data) -> const char*
{
    assert(data != nullptr);

    const uint8_t* scope_data = data + scope_offset();

    return (const char*)scope_data;
}

/// @param data The raw memory for the counters
/// @param index The index of a counter. Must be less than max_metrics().
/// @return The raw name of a counter in memory
inline auto raw_name(uint8_t* data, std::size_t index) -> char*
{
    assert(data != nullptr);
    assert(index < max_metrics(data));

    uint8_t* name_data =
        data + names_offset(data) + (index * max_name_bytes(data));

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
        data + names_offset(data) + (index * max_name_bytes(data));

    return (const char*)name_data;
}

inline void prepend_name(const std::string& text, std::size_t index,
                         uint8_t* data)
{
    char* name_data = raw_name(data, index);
    std::size_t text_size = text.size();
    std::size_t name_size = std::strlen(name_data);
    assert((text_size + 1 + name_size <= max_name_bytes(data)));

    std::memcpy(name_data + text_size + 1, name_data, name_size);
    std::memcpy(name_data, text.data(), text_size);
    name_data[text_size] = '_';
    // name_data[text_size + name_size + 1] = '\0';
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
/// @return The counters in json-format
inline auto to_json(const uint8_t* data) -> bourne::json
{
    assert(data != nullptr);
    bourne::json counters = bourne::json::object();

    for (std::size_t i = 0; i < max_metrics(data); ++i)
    {
        if ((!is_metric_initialized(data, i)))
        {
            continue;
        }

        auto n = raw_name(data, i);
        auto v = *raw_value(data, i);

        counters[n] = v;
    }

    return counters;
}

}
}
}
