// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

/// Strongly typed description for a metric
struct description
{
    /// Default constructor
    description() = default;

    /// Explicit constructor
    explicit description(const std::string& v) : value(v)
    {
    }

    /// The description string
    std::string value;
};
}
}
