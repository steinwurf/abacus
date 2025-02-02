// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <map>
#include <string>

#include "availability.hpp"
#include "description.hpp"
#include "optional_metric.hpp"
#include "required_metric.hpp"
#include "unit.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// A enumeration metric
struct enum8
{
    /// The primitive type of the metric
    using type = uint8_t;

    /// Required enum8 metric
    using required = required_metric<enum8>;

    /// Optional enum8 metric
    using optional = optional_metric<enum8>;

    /// The enumeration value type
    struct value_info
    {
        /// The name of the value
        std::string name;

        /// The description of the value
        std::string description;
    };

    /// The availability of the metric
    abacus::availability availability;

    /// The metric description
    abacus::description description;

    /// The enumeration value information
    std::map<uint32_t, value_info> values;

    /// The unit of the metric
    abacus::unit unit{};
};
}
}
