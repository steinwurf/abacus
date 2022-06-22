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

/// Enum used to declare and identify the types of the metric objects.
enum class value_type : uint8_t
{
    /// uint64_t type
    uint64 = 0U,

    /// int64_t type
    int64,

    /// double type
    float64,

    /// bool type
    boolean
};
}
}
