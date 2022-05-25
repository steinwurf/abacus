// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <type_traits>
#include <vector>

#include "value_types.hpp"
#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// Wrapper for the value of a counter.
template <value_type v>
class metric;

template <class T>
class metric_integer
{
    static_assert(std::is_integral<T>::value,
                  "metric_integer can only be used with integral types");
};

template <class T>
class metric_float
{
    static_assert(std::is_floating_point<T>::value,
                  "metric_float can only be used with floating point types");
};

template <>
class metric<value_type::unsigned_integral> : public metric_integer<uint64_t>
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
    auto operator=(uint64_t value) -> metric<value_type::unsigned_integral>&
    {
        *m_memory = value;
        return *this;
    }

    /// Increment the counter
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(uint64_t value) -> metric<value_type::unsigned_integral>&
    {
        *m_memory += value;
        return *this;
    }

    /// Decrement the counter
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(uint64_t value) -> metric<value_type::unsigned_integral>&
    {
        *m_memory -= value;
        return *this;
    }

    /// Increment the value of the counter
    /// @return The result of the arithmetic
    auto operator++() -> metric<value_type::unsigned_integral>&
    {
        *m_memory += 1;
        return *this;
    }

    /// Decrement the value of the counter
    /// @return The result of the arithmetic
    auto operator--() -> metric<value_type::unsigned_integral>&
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
    uint64_t* m_memory;
};

template <>
class metric<value_type::signed_integral> : public metric_integer<int64_t>
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
    auto operator=(int64_t value) -> metric<value_type::signed_integral>&
    {
        *m_memory = value;
        return *this;
    }

    /// Increment the counter
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(int64_t value) -> metric<value_type::signed_integral>&
    {
        *m_memory += value;
        return *this;
    }

    /// Decrement the counter
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(int64_t value) -> metric<value_type::signed_integral>&
    {
        *m_memory -= value;
        return *this;
    }

    /// Increment the value of the counter
    /// @return The result of the arithmetic
    auto operator++() -> metric<value_type::signed_integral>&
    {
        *m_memory += 1;
        return *this;
    }

    /// Decrement the value of the counter
    /// @return The result of the arithmetic
    auto operator--() -> metric<value_type::signed_integral>&
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
    int64_t* m_memory;
};

template <>
class metric<value_type::floating_point> : public metric_float<double>
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
    auto operator=(double value) -> metric<value_type::floating_point>&
    {
        *m_memory = value;
        return *this;
    }

    /// Increment the counter
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(double value) -> metric<value_type::floating_point>&
    {
        *m_memory += value;
        return *this;
    }

    /// Decrement the counter
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(double value) -> metric<value_type::floating_point>&
    {
        *m_memory -= value;
        return *this;
    }

    /// Increment the value of the counter
    /// @return The result of the arithmetic
    auto operator++() -> metric<value_type::floating_point>&
    {
        *m_memory += 1;
        return *this;
    }

    /// Decrement the value of the counter
    /// @return The result of the arithmetic
    auto operator--() -> metric<value_type::floating_point>&
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
    double* m_memory;
};

template <>
class metric<value_type::boolean> : public metric_integer<bool>
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

    /// Increment the counter
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(bool value) -> metric<value_type::boolean>&
    {
        *m_memory += value;
        return *this;
    }

    /// Decrement the counter
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(bool value) -> metric<value_type::boolean>&
    {
        *m_memory -= value;
        return *this;
    }

    /// Increment the value of the counter
    /// @return The result of the arithmetic
    auto operator++() -> metric<value_type::boolean>&
    {
        *m_memory += 1;
        return *this;
    }

    /// Decrement the value of the counter
    /// @return The result of the arithmetic
    auto operator--() -> metric<value_type::boolean>&
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
    bool* m_memory;
};

}
}
