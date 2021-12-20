// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

#include "version.hpp"

namespace abacus
{

inline namespace STEINWURF_ABACUS_VERSION
{

/// The units supported in the abacus library
enum class unit : uint8_t
{
    /// The unit used for packet numbers
    packets = 0U,
    /// The unit used for bytes
    bytes,
    /// The unit used for symbols
    symbols,
    /// The none-unit
    none

};

inline auto units_to_string(abacus::unit unit) -> const char*
{
    static const char* string_definitions[] = {"packets", "bytes", "symbols",
                                               "none"};

    return string_definitions[static_cast<uint8_t>(unit)];
}

}
}
