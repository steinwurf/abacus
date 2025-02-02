// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "description.hpp"
#include "unit.hpp"
#include "version.hpp"

#include "boolean.hpp"
#include "enum8.hpp"
#include "float32.hpp"
#include "float64.hpp"
#include "int32.hpp"
#include "int64.hpp"
#include "uint32.hpp"
#include "uint64.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

/// Tag type representing a constant kind.
struct constant
{

    struct int64
    {
        int64_t value;
    };

    struct uint64
    {
        uint64_t value;
    };

    struct boolean
    {
        bool value;
    };

    struct float64
    {
        double value;
    };

    struct str
    {
        std::string value;
    };

    // The value of the constant
    std::variant<int64, uint64, boolean, float64, str> value;

    // The description of the constant
    abacus::description description;

    // The unit of the constant
    abacus::unit unit{};
};
}
}
