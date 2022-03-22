// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <type_traits>
#include <vector>

#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// Wrapper for the value of a counter.
template <class T>
class metric
{
private:
    // using check_is_bool = std::enable_if<std::is_same<T, bool>::value, R>;

public:
    /// Default constructor
    metric() = default;

    /// Create a new counter value from the pointer
    /// @param memory A pointer to a value

    metric(T* memory) : m_memory(memory)
    {
        assert(m_memory != nullptr);
    }

    /// Assign the counter a new value
    /// @param value The value to assign
    /// @return a counter with the new value
    auto operator=(T value) -> metric<T>&
    {
        assert(m_memory != nullptr);

        *m_memory = value;
        return *this;
    }

    /// Increment the counter
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(T value) -> metric<T>&
    {
        assert(m_memory != nullptr);

        *m_memory += value;
        return *this;
    }

    /// Decrement the counter
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(T value) -> metric<T>&
    {
        assert(m_memory != nullptr);

        *m_memory -= value;
        return *this;
    }

    /// Increment the value of the counter
    /// @return The result of the arithmetic
    auto operator++() -> metric<T>&
    {
        assert(m_memory != nullptr);
        *m_memory += 1;
        return *this;
    }

    /// Decrement the value of the counter
    /// @return The result of the arithmetic
    auto operator--() -> metric<T>&
    {
        assert(m_memory != nullptr);
        *m_memory -= 1;
        return *this;
    }

    /// @return True if the metric has been assigned memory. False otherwise
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

private:
    /// Enable creation from the storage class
    // friend class metrics;

private:
    /// The metric memory
    T* m_memory;
};
}
}
