// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <optional>

#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// Stronly typed max value for a metric
template <typename T>
struct max
{

    /// Default constructor
    max() = default;

    /// Constructor
    /// @param value The maximum value
    explicit max(T value) : value(value)
    {
    }

    /// The maximum value
    std::optional<T> value;
};
}
}
