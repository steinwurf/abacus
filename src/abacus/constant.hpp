// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "description.hpp"
#include "unit.hpp"
#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

/// A constant value stored in the metadata
struct constant
{
    /// A 64-bit unsigned integer
    struct uint64
    {
        /// The type used to store the value
        using type = uint64_t;
        /// The value of the constant
        type value;
    };

    /// A 64-bit signed integer
    struct int64
    {
        /// The type used to store the value
        using type = int64_t;
        /// The value of the constant
        type value;
    };

    /// A 64-bit floating point number
    struct float64
    {
        /// The type used to store the value
        using type = double;
        /// The value of the constant
        type value;
    };

    /// A boolean value
    struct boolean
    {
        /// The type used to store the value
        using type = bool;
        /// The value of the constant
        type value;
    };

    /// A string value
    struct str
    {
        /// The type used to store the value
        using type = std::string;
        /// The value of the constant
        type value;
    };

    /// The value of the constant
    std::variant<uint64, int64, float64, boolean, str> value;

    /// The description of the constant
    abacus::description description;

    /// The unit of the constant
    abacus::unit unit{};
};
}
}
