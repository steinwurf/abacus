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
/// A 32-bit integer metric
struct uint32
{
    /// The primitive type of the metric
    using type = uint32_t;

    /// The optional metric type
    using optional = optional_metric<uint32>;

    /// The required metric type
    using required = required_metric<uint32>;

    /// The metric kind
    abacus::kind kind;

    /// The metric description
    abacus::description description;

    /// The availability of the metric
    abacus::availability availability;

    /// The unit of the metric
    abacus::unit unit{};

    /// The minimum value of the metric
    abacus::min<type> min{};

    /// The maximum value of the metric
    abacus::max<type> max{};
};
}
}
