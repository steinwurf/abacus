// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "kind.hpp"
#include "max.hpp"
#include "min.hpp"
#include "type.hpp"
#include "unit.hpp"

#include <string>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

/// Object used to describe a metric. Used in the metrics() constructor.
struct metric_info
{
    /// Name of the metric
    std::string name;

    /// Description of the metric
    std::string description;

    /// Type of the metric. A metric_type enum.
    abacus::type type;

    /// enum describing the flags of a metric.
    abacus::kind kind;

    /// The unit of the metric
    abacus::unit unit = abacus::unit{""};

    /// The minimum value of the metric
    abacus::min min = abacus::min{uint64_t{0U}};

    /// The maximum value of the metric
    abacus::max max = abacus::max{uint64_t{0U}};
};
}
}
