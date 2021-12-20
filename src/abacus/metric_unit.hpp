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
enum class metric_unit : uint8_t
{
    /// The unit used for packet numbers
    packets = 0U,
    /// The unit used for bytes
    bytes,
    /// The unit used for symbols
    symbols,
    /// The unit used for percentages
    percent,
    /// The units used for time
    seconds,
    milliseconds,
    microseconds,
    /// The units used for bandwidths
    /// (Giga) (Mega) (Kilo) Bytes
    bytes_per_second,
    kilobytes_per_second,
    megabytes_per_second,
    gigabytes_per_second,
    /// (Giga) (Mega) (Kilo) Bits
    bits_per_second,
    kilobits_per_second,
    megabits_per_second,
    gigabits_per_second,
    /// The none-unit
    none

};

static const char* units_to_string[] = {"packets",          "bytes",
                                        "symbols",          "percent",
                                        "seconds",          "milliseconds",
                                        "microseconds",     "bytes/second",
                                        "kilobytes/second", "megabytes/second",
                                        "gigabytes/second", "bits/second",
                                        "kilobits/second",  "megabits/second",
                                        "gigabits/second",  "none"};
}
}
