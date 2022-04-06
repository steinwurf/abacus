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
        if (std::is_unsigned<T>::value)
        {
            assert(value <= *m_memory);
        }
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

    /// Logical OR between the counter and a value
    /// @param value The value to AND with
    /// @return The result of the arithmetic
    auto operator||(T value) -> metric<T>&
    {
        assert(m_memory != nullptr);
        *m_memory = *m_memory || value;
        return *this;
    }

    /// Logical AND between the counter and a value
    /// @param value The value to OR with
    /// @return The result of the arithmetic
    auto operator&&(T value) -> metric<T>&
    {
        assert(m_memory != nullptr);
        *m_memory = *m_memory && value;
        return *this;
    }

    /// Logical NOT of the counter
    /// @return The result of the arithmetic
    auto operator!() -> metric<T>&
    {
        assert(m_memory != nullptr);
        *m_memory = !*m_memory;
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

// Bool special cases. Increment and Decrement makes no sense with bools (is it
// the NOT operator?). Assign and add/subtract has no real purpose either.

/// The postfix increment operator is not allowed for bool.
template <>
inline auto metric<bool>::operator++() -> metric<bool>&
{
    assert(false && "metric::operator++: Not allowed for bool.");
    return *this;
}

/// The postfix decrement operator is not allowed for bool.
template <>
inline auto metric<bool>::operator--() -> metric<bool>&
{
    assert(false && "metric::operator--: Not allowed for bool.");
    return *this;
}

/// The assign and add operator is not allowed for bool.
template <>
inline auto metric<bool>::operator+=(bool value) -> metric<bool>&
{
    assert(false && "metric::operator+=: Not allowed for bool.");
    (void)value;
    return *this;
}

/// The assign and subtract operator is not allowed for bool.
template <>
inline auto metric<bool>::operator-=(bool value) -> metric<bool>&
{
    assert(false && "metric::operator-=: Not allowed for bool.");
    (void)value;
    return *this;
}

}
}
