// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "metric_type.hpp"
#include "qualifier.hpp"
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
    metric_type type;

    /// enum describing if the metric is a constant or not.
    qualifier is_constant;
};
}
}