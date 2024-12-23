// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
/// Converts a string to a 32-bit FNV-1a hash used for identifying actions
constexpr auto hash_function(const uint8_t* data, std::size_t bytes) -> uint32_t
{
    uint32_t hash = 0x811c9dc5;            // FNV-1a 32-bit offset basis
    constexpr uint32_t prime = 0x01000193; // FNV-1a 32-bit prime

    for (std::size_t i = 0; i < bytes; ++i)
    {
        hash ^= static_cast<uint32_t>(data[i]);
        hash *= prime;
    }

    return hash;
}
}
}
}
