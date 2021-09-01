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

inline auto header_bytes() -> std::size_t
{
    return 5;
}

inline auto max_name_bytes(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);

    uint16_t max_name_bytes;
    std::memcpy(&max_name_bytes, data, sizeof(uint16_t));
    return max_name_bytes;
}

inline auto max_metrics_offset() -> std::size_t
{
    return 2;
}

inline auto max_metrics(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);

    const uint8_t* max_metrics_data = data + max_metrics_offset();
    uint16_t max_metrics;
    std::memcpy(&max_metrics, max_metrics_data, sizeof(uint16_t));
    return max_metrics;
}

inline auto title_offset() -> std::size_t
{
    return header_bytes();
}

inline auto names_offset(const uint8_t* data) -> std::size_t
{
    // Skip header + title
    return header_bytes() + max_name_bytes(data);
}

inline auto values_offset(const uint8_t* data) -> std::size_t
{
    // Skip header + title + names
    return header_bytes() + max_name_bytes(data) +
           (max_metrics(data) * max_name_bytes(data));
}

inline auto raw_title(uint8_t* data) -> char*
{
    assert(data != nullptr);

    uint8_t* title_data = data + title_offset();

    return (char*)title_data;
}

inline auto raw_title(const uint8_t* data) -> const char*
{
    assert(data != nullptr);

    const uint8_t* title_data = data + title_offset();

    return (const char*)title_data;
}

inline auto raw_name(uint8_t* data, std::size_t index) -> char*
{
    assert(data != nullptr);
    assert(index < max_metrics(data));

    uint8_t* name_data =
        data + names_offset(data) + (index * max_name_bytes(data));

    return (char*)name_data;
}

inline auto raw_name(const uint8_t* data, std::size_t index) -> const char*
{
    assert(data != nullptr);
    assert(index < max_metrics(data));

    const uint8_t* name_data =
        data + names_offset(data) + (index * max_name_bytes(data));

    return (const char*)name_data;
}

inline auto raw_value(uint8_t* data, std::size_t index) -> uint64_t*
{
    assert(data != nullptr);
    assert(index < max_metrics(data));

    uint8_t* value_data =
        data + values_offset(data) + (index * sizeof(uint64_t));

    return reinterpret_cast<uint64_t*>(value_data);
}

inline auto raw_value(const uint8_t* data, std::size_t index) -> const uint64_t*
{
    assert(data != nullptr);
    assert(index < max_metrics(data));

    const uint8_t* value_data =
        data + values_offset(data) + (index * sizeof(uint64_t));

    return reinterpret_cast<const uint64_t*>(value_data);
}

inline auto is_metric_initialized(const uint8_t* data, std::size_t index)
    -> bool
{
    assert(index < max_metrics(data));
    const char* name_data = raw_name(data, index);

    // If the name is non-zero it is initialized and valid. We just check the
    // first byte to see if it's zero.
    return name_data[0] != 0;
}

inline auto to_json(const uint8_t* data) -> std::string
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

    return counters.dump();
}

}
}
}