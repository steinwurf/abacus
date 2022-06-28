// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <type_traits>
#include <vector>

#include "value_type.hpp"
#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// Wrapper for the value of a counter.
///
/// See metric<value_type::uint64>,
/// metric<value_type::int64>, metric<value_type::float64> and
/// metric<value_type::boolean> for template specializations.
template <value_type ValueType>
class metric;

/// Metric wrapping uint64_t value.
template <>
class metric<value_type::uint64>
{

public:
    /// Default constructor
    metric() = default;

    /// Create a new counter value from the pointer
    /// @param memory A pointer to a value

    metric(uint64_t* memory) : m_memory(memory)
    {
        assert(m_memory != nullptr);
    }

    /// Assign the counter a new value
    /// @param value The value to assign
    /// @return a counter with the new value
    auto operator=(uint64_t value) -> metric<value_type::uint64>&
    {
        *m_memory = value;
        return *this;
    }

    /// Increment the counter
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(uint64_t value) -> metric<value_type::uint64>&
    {
        *m_memory += value;
        return *this;
    }

    /// Decrement the counter
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(uint64_t value) -> metric<value_type::uint64>&
    {
        *m_memory -= value;
        return *this;
    }

    /// Increment the value of the counter
    /// @return The result of the arithmetic
    auto operator++() -> metric<value_type::uint64>&
    {
        *m_memory += 1;
        return *this;
    }

    /// Decrement the value of the counter
    /// @return The result of the arithmetic
    auto operator--() -> metric<value_type::uint64>&
    {
        *m_memory -= 1;
        return *this;
    }

    /// @return True if the metric has been assigned memory. False otherwise
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

private:
    /// The metric memory
    uint64_t* m_memory = nullptr;
};

/// Metric wrapping int64_t value.
template <>
class metric<value_type::int64>
{

public:
    /// Default constructor
    metric() = default;

    /// Create a new counter value from the pointer
    /// @param memory A pointer to a value

    metric(int64_t* memory) : m_memory(memory)
    {
        assert(m_memory != nullptr);
    }

    /// Assign the counter a new value
    /// @param value The value to assign
    /// @return a counter with the new value
    auto operator=(int64_t value) -> metric<value_type::int64>&
    {
        *m_memory = value;
        return *this;
    }

    /// Increment the counter
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(int64_t value) -> metric<value_type::int64>&
    {
        *m_memory += value;
        return *this;
    }

    /// Decrement the counter
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(int64_t value) -> metric<value_type::int64>&
    {
        *m_memory -= value;
        return *this;
    }

    /// Increment the value of the counter
    /// @return The result of the arithmetic
    auto operator++() -> metric<value_type::int64>&
    {
        *m_memory += 1;
        return *this;
    }

    /// Decrement the value of the counter
    /// @return The result of the arithmetic
    auto operator--() -> metric<value_type::int64>&
    {
        *m_memory -= 1;
        return *this;
    }

    /// @return True if the metric has been assigned memory. False otherwise
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

private:
    /// The metric memory
    int64_t* m_memory = nullptr;
};

/// Metric wrapping double value.
template <>
class metric<value_type::float64>
{

public:
    /// Default constructor
    metric() = default;

    /// Create a new counter value from the pointer
    /// @param memory A pointer to a value

    metric(double* memory) : m_memory(memory)
    {
        assert(m_memory != nullptr);
    }

    /// Assign the counter a new value
    /// @param value The value to assign
    /// @return a counter with the new value
    auto operator=(double value) -> metric<value_type::float64>&
    {
        *m_memory = value;
        return *this;
    }

    /// Increment the counter
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(double value) -> metric<value_type::float64>&
    {
        *m_memory += value;
        return *this;
    }

    /// Decrement the counter
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(double value) -> metric<value_type::float64>&
    {
        *m_memory -= value;
        return *this;
    }

    /// Increment the value of the counter
    /// @return The result of the arithmetic
    auto operator++() -> metric<value_type::float64>&
    {
        *m_memory += 1;
        return *this;
    }

    /// Decrement the value of the counter
    /// @return The result of the arithmetic
    auto operator--() -> metric<value_type::float64>&
    {
        *m_memory -= 1;
        return *this;
    }

    /// @return True if the metric has been assigned memory. False otherwise
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

private:
    /// The metric memory
    double* m_memory = nullptr;
};

/// Metric wrapping bool value.
template <>
class metric<value_type::boolean>
{

public:
    /// Default constructor
    metric() = default;

    /// Create a new counter value from the pointer
    /// @param memory A pointer to a value

    metric(bool* memory) : m_memory(memory)
    {
        assert(m_memory != nullptr);
    }

    /// Assign the counter a new value
    /// @param value The value to assign
    /// @return a counter with the new value
    auto operator=(bool value) -> metric<value_type::boolean>&
    {
        *m_memory = value;
        return *this;
    }

    /// @return True if the metric has been assigned memory. False otherwise
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

private:
    /// The metric memory
    bool* m_memory = nullptr;
};

}
}
