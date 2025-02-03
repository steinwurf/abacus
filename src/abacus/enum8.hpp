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

#include "description.hpp"
#include "unit.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// A enumeration metric
struct enum8
{
    struct enum_value
    {
        template <typename T>
        enum_value(T v) : value(static_cast<uint8_t>(v))
        {
        }

        auto operator<(const enum_value& other) const -> bool
        {
            return value < other.value;
        }

        uint8_t value;
    };

    /// The primitive type of the metric
    using type = uint8_t;

    /// The enumeration value type
    struct value_info
    {
        /// The name of the value
        std::string name;

        /// The description of the value
        std::string description;
    };

    /// The metric description
    abacus::description description;

    /// The enumeration value information
    std::map<enum_value, value_info> values;

    /// The unit of the metric
    abacus::unit unit{};
};
}
}
