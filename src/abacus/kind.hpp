// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "availability.hpp"
#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// Tag type representing a gauge kind.
struct gauge
{
    abacus::availability availability;
};

/// Tag type representing a counter kind.
struct counter
{
    abacus::availability availability;
};

/// Tag type representing a constant kind.
struct constant
{
};
}
}
