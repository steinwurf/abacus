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

    /// The optional metric type
    using optional = optional_metric<enum8>;

    /// The required metric type
    using required = required_metric<enum8>;

    /// The enumeration value type
    struct value
    {
        /// The name of the value
        std::string name;

        /// The description of the value
        std::string description;
    };

    /// The metric description
    std::string description;

    /// The enumeration values
    std::map<uint32_t, value> values;

    /// The availability of the metric
    abacus::availability availability;

    /// The unit of the metric
    abacus::unit unit{};
};
}
}
