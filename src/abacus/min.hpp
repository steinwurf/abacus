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

/// Stronly typed min value for a metric
template <typename T>
struct min
{

    /// Default constructor
    min() = default;

    /// Constructor
    /// @param value The minimum value
    explicit min(T value) : value(value)
    {
    }

    /// The minimum value
    std::optional<T> value;
};
}
}
