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

/// Strongly typed unit for a metric
struct name
{
    /// Default constructor
    name() = default;

    /// Explicit constructor
    explicit name(const std::string& v) : value(v)
    {
    }

    /// @return True if the name is empty otherwise false
    bool empty() const
    {
        return value.empty();
    }

    /// The name string
    std::string value;
};

inline bool operator==(const name& lhs, const name& rhs)
{
    return lhs.value == rhs.value;
}

inline bool operator<(const name& lhs, const name& rhs)
{
    return lhs.value < rhs.value;
}

}
}
