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
/// Wrapper for true to use as the is_constant argument in metric_info
const bool CONSTANT = true;

/// Wrapper for false
const bool NON_CONSTANT = false;

/// Object used to describe a metric. Used in the metrics() constructor.
struct metric_info
{
    /// Name of the metric
    std::string name;

    /// Description of the metric
    std::string description;

    /// Type of the metric. A value_type enum.
    value_type type;

    /// Bool describing if the metric is a constant or not. Use abacus::CONSTANT
    /// or abacus::NON_CONSTANT for code-clarity.
    bool is_constant;
};
}
}
