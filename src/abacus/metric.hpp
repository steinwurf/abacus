// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <vector>

#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// Wrapper for the value of a counter.
class metric
{
public:
    /// Default constructor
    metric() = default;

    /// Create a new counter value from the pointer to an integer
    metric(uint64_t* memory);

    /// Assign the counter a new value
    auto operator=(uint64_t value) -> metric&;

    /// Increment the counter
    auto operator+=(uint64_t value) -> metric&;

    /// Increment the value of the counter
    auto operator++() -> metric&;

    /// Decrement the counter
    auto operator-=(uint64_t value) -> metric&;

    /// Decrement the value of the counter
    auto operator--() -> metric&;

    /// @return True of valid
    auto is_initialized() const -> bool;

private:
    /// Enable creation from the storage class
    friend class metrics;

private:
    /// The counter
    uint64_t* m_memory = nullptr;
};
}
}