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
/// Enum used to declare and identify whether a metric object is a constant or
/// not.
enum class qualifier : bool
{
    /// wrapper for false, not a constant
    non_constant = false,
    /// wrapper for true, a constant
    constant = true
};
}
}
