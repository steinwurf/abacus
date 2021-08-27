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

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{

inline auto header_size() -> std::size_t
{
    return 5;
}

inline auto max_name_bytes(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);

    const uint8_t* max_name_bytes_data = data;
    uint16_t max_name_bytes;
    std::memcpy(&max_name_bytes, max_name_bytes_data, sizeof(uint16_t));
    return max_name_bytes;
}

inline auto max_metrics(const uint8_t* data) -> uint16_t
{
    assert(data != nullptr);

    const uint8_t* max_metrics_data = data + 2;
    uint16_t max_metrics;
    std::memcpy(&max_metrics, max_metrics_data, sizeof(uint16_t));
    return max_metrics;
}

inline auto title_offset() -> std::size_t
{
    return header_size();
}

inline auto names_offset(const uint8_t* data) -> std::size_t
{
    // Skip header + title
    return header_size() + max_name_bytes(data);
}

inline auto values_offset(const uint8_t* data) -> std::size_t
{
    // Skip header + title + names
    return header_size() + max_name_bytes(data) +
           (max_metrics(data) * max_name_bytes(data));
}

inline auto raw_title(uint8_t* data) -> char*
{
    assert(data != nullptr);

    uint8_t* title_data = data + title_offset();
    char* title = (char*)title_data;

    return title;
}

inline auto raw_name(uint8_t* data, std::size_t index) -> char*
{
    assert(data != nullptr);

    const uint16_t max_metrics_var = max_metrics(data);
    assert(index < max_metrics_var);

    const uint16_t max_name_bytes_var = max_name_bytes(data);
    uint8_t* name_data =
        data + names_offset(data) + (index * max_name_bytes_var);

    char* name = (char*)name_data;

    return name;
}

inline auto raw_value(uint8_t* data, std::size_t index) -> uint64_t*
{
    assert(data != nullptr);

    const uint16_t max_metrics_var = max_metrics(data);
    assert(index < max_metrics_var);

    uint8_t* value_data =
        data + values_offset(data) + (index * sizeof(uint64_t));

    return reinterpret_cast<uint64_t*>(value_data);
}

inline auto is_metric_initialized(uint8_t* data, std::size_t index) -> bool
{
    assert(index < max_metrics(data));
    const char* name_data = raw_name(data, index);

    // If the name is non-zero it is initialized and valid. We just check the
    // first byte to see if it's zero.
    return name_data[0] != 0;
}

}
}
}