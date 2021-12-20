// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "metric_unit.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

auto units_to_string(abacus::unit unit) -> const char*
{
    static const char* string_definitions[] = {
        "packets",          "bytes",
        "symbols",          "percent",
        "seconds",          "milliseconds",
        "microseconds",     "bytes/second",
        "kilobytes/second", "megabytes/second",
        "gigabytes/second", "bits/second",
        "kilobits/second",  "megabits/second",
        "gigabits/second",  "none"};

    return string_definitions[static_cast<uint8_t>(unit)];
}
}
}
