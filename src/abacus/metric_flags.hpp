// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// Enum used for identifying various metric flags.
enum class metric_flags : uint8_t
{
    constant = 1 << 0, // 0b00000001
};

inline metric_flags operator|(metric_flags lhs, metric_flags rhs)
{
    return static_cast<metric_flags>(
        static_cast<std::underlying_type_t<metric_flags>>(lhs) |
        static_cast<std::underlying_type_t<metric_flags>>(rhs));
}

inline metric_flags operator&(metric_flags lhs, metric_flags rhs)
{
    return static_cast<metric_flags>(
        static_cast<std::underlying_type_t<metric_flags>>(lhs) &
        static_cast<std::underlying_type_t<metric_flags>>(rhs));
}
}
}
