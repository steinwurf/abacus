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

/// Wrapper for a unit string for more explicit code
struct unit
{
    /// Default constructor
    unit() = default;

    /// Explicit constructor
    explicit unit(const std::string& name) : value(name)
    {
    }

    /// @brief  Check if the unit is empty
    /// @return True if the unit is empty otherwise false
    bool empty() const
    {
        return value.empty();
    }

    /// The unit string
    std::string value;
};

}
}
