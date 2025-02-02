// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>

#include "availability.hpp"
#include "description.hpp"
#include "kind.hpp"
#include "optional_metric.hpp"
#include "required_metric.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// A boolean metric
struct boolean
{
    /// The primitive type of the metric
    using type = bool;

    /// Ensure this is a 1 bit type
    static_assert(sizeof(bool) == 1, "bool must be 8 bits");

    /// Required boolean metric
    using required = required_metric<boolean>;

    /// Optional boolean metric
    using optional = optional_metric<boolean>;

    /// The availability of the metric
    abacus::availability availability;

    /// The description of the metric
    abacus::description description;
};
}
}
