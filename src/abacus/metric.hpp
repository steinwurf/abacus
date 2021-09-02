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
    /// @param memory A pointer to a uint64_t
    metric(uint64_t* memory);

    /// Assign the counter a new value
    /// @param value The value to assign
    /// @return a counter with the new value
    auto operator=(uint64_t value) -> metric&;

    /// Increment the counter
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(uint64_t value) -> metric&;

    /// Increment the value of the counter
    /// @return The result of the arithmetic
    auto operator++() -> metric&;

    /// Decrement the counter
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(uint64_t value) -> metric&;

    /// Decrement the value of the counter
    /// @return The result of the arithmetic
    auto operator--() -> metric&;

    /// @return True if the metric has been assigned memory. False otherwise
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