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

/// Tag type representing a constant kind.
struct constant
{

    struct uint64
    {
        using type = uint64_t;
        type value;
    };

    struct int64
    {
        using type = int64_t;
        type value;
    };

    struct float64
    {
        using type = double;
        type value;
    };

    struct boolean
    {
        using type = bool;
        type value;
    };

    struct str
    {
        using type = std::string;
        type value;
    };

    // The value of the constant
    std::variant<uint64, int64, float64, boolean, str> value;

    // The description of the constant
    abacus::description description;

    // The unit of the constant
    abacus::unit unit{};
};
}
}
