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
/// Enum used for identifying various metric flags.
enum class kind : uint8_t
{
    /// A counter is a cumulative metric that represents a single monotonically
    /// increasing counter whose value can only increase or be reset to zero on
    /// restart. For example, you can use a counter to represent the number of
    /// requests served, tasks completed, or errors.
    counter = 0U,

    /// A constant is a metric that represents a single numerical value that
    /// never changes.
    constant,

    /// A gauge is a metric that represents a single numerical value that can
    /// arbitrarily go up and down.
    /// Gauges are typically used for measured values like temperatures or
    /// current memory usage, but also "counts" that can go up and down, like
    /// the number of concurrent requests.
    gauge,
};
}
}
