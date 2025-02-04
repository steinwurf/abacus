// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "description.hpp"
#include "kind.hpp"
#include "max.hpp"
#include "min.hpp"
#include "unit.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

/// A 64-bit integer metric
struct uint64
{
    /// The primitive type of the metric
    using type = uint64_t;

    /// The metric kind
    abacus::kind kind;

    /// The metric description
    abacus::description description;

    /// The unit of the metric
    abacus::unit unit{};

    /// The minimum value of the metric
    abacus::min<type> min{};

    /// The maximum value of the metric
    abacus::max<type> max{};
};
}
}
