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

struct optional_value_type
{
};

inline constexpr auto optional = optional_value_type{};

struct required_value
{
};

/// A 64-bit integer metric
struct uint64
{
    /// The primitive type of the metric
    using type = uint64_t;

    /// Required uint64 metric
    using required = required_metric<uint64>;

    /// Optional uint64 metric
    using optional = optional_metric<uint64>;

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
