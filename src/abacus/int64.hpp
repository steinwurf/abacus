// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "availability.hpp"
#include "description.hpp"
#include "kind.hpp"
#include "max.hpp"
#include "min.hpp"
#include "optional_metric.hpp"
#include "required_metric.hpp"
#include "unit.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// A 64-bit signed integer metric
struct int64
{
    /// The primitive type of the metric
    using type = int64_t;

    /// Required int64 metric
    using required = required_metric<int64>;

    /// Optional int64 metric
    using optional = optional_metric<int64>;

    /// The metric kind
    abacus::kind kind;

    /// The availability of the metric
    abacus::availability availability;

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
