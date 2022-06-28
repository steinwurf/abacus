// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "value_type.hpp"
#include <string>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

enum class qualifier : bool
{
    non_constant = false,
    constant = true
};

/// Object used to describe a metric. Used in the metrics() constructor.
struct metric_info
{
    /// Name of the metric
    std::string name;

    /// Description of the metric
    std::string description;

    /// Type of the metric. A value_type enum.
    value_type type;

    /// enum describing if the metric is a constant or not.
    qualifier is_constant;
};
}
}
