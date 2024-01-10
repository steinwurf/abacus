// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <cmath>

#include "type.hpp"
#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// Wrapper for the value of a counter.
///
/// See metric<abacus::type::uint64>,
/// metric<abacus::type::int64>, metric<abacus::type::float64> and
/// metric<abacus::type::boolean> for template specializations.
template <abacus::type MetricType>
class metric;

/// Metric wrapping uint64_t value.
template <>
class metric<abacus::type::uint64>
{
public:
    /// The underlying data type
    using value_type = uint64_t;

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
    auto operator=(uint64_t value) -> metric<abacus::type::uint64>&
    {
        assert(is_initialized());
        *m_memory = value;
        return *this;
    }

    /// Increment the counter
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(uint64_t value) -> metric<abacus::type::uint64>&
    {
        assert(is_initialized());
        *m_memory += value;
        return *this;
    }

    /// Decrement the counter
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(uint64_t value) -> metric<abacus::type::uint64>&
    {
        assert(is_initialized());
        *m_memory -= value;
        return *this;
    }

    /// Increment the value of the counter
    /// @return The result of the arithmetic
    auto operator++() -> metric<abacus::type::uint64>&
    {
        assert(is_initialized());
        *m_memory += 1;
        return *this;
    }

    /// Decrement the value of the counter
    /// @return The result of the arithmetic
    auto operator--() -> metric<abacus::type::uint64>&
    {
        assert(is_initialized());
        *m_memory -= 1;
        return *this;
    }

    /// @return True if the metric has been assigned memory. False otherwise
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

    /// @return The value of the counter
    auto value() const -> uint64_t
    {
        assert(is_initialized());
        return *m_memory;
    }

private:
    /// The metric memory
    uint64_t* m_memory = nullptr;
};

/// Metric wrapping int64_t value.
template <>
class metric<abacus::type::int64>
{
public:
    /// The underlying data type
    using value_type = int64_t;

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
    auto operator=(int64_t value) -> metric<abacus::type::int64>&
    {
        assert(is_initialized());
        *m_memory = value;
        return *this;
    }

    /// Increment the counter
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(int64_t value) -> metric<abacus::type::int64>&
    {
        assert(is_initialized());
        *m_memory += value;
        return *this;
    }

    /// Decrement the counter
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(int64_t value) -> metric<abacus::type::int64>&
    {
        assert(is_initialized());
        *m_memory -= value;
        return *this;
    }

    /// Increment the value of the counter
    /// @return The result of the arithmetic
    auto operator++() -> metric<abacus::type::int64>&
    {
        assert(is_initialized());
        *m_memory += 1;
        return *this;
    }

    /// Decrement the value of the counter
    /// @return The result of the arithmetic
    auto operator--() -> metric<abacus::type::int64>&
    {
        assert(is_initialized());
        *m_memory -= 1;
        return *this;
    }

    /// @return True if the metric has been assigned memory. False otherwise
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

    /// @return The value of the counter
    auto value() const -> int64_t
    {
        assert(is_initialized());
        return *m_memory;
    }

private:
    /// The metric memory
    value_type* m_memory = nullptr;
};

/// Metric wrapping double value.
template <>
class metric<abacus::type::float64>
{
public:
    /// The underlying data type
    using value_type = double;

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
    auto operator=(double value) -> metric<abacus::type::float64>&
    {
        assert(is_initialized());
        // We don't allow assignment to NaN or Inf/-Inf
        assert(!std::isnan(value) && !std::isinf(value) && "Cannot assign a "
                                                             "NaN or Inf/-Inf "
                                                             "value to a "
                                                             "float metric");
        *m_memory = value;
        return *this;
    }

    /// Increment the counter
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(double value) -> metric<abacus::type::float64>&
    {
        assert(is_initialized());
        // We don't allow assignment to NaN or Inf/-Inf
        assert(!std::isnan(value) && !std::isinf(value) && "Cannot assign a "
                                                           "NaN or Inf/-Inf "
                                                           "value to a "
                                                           "float metric");
        *m_memory += value;
        return *this;
    }

    /// Decrement the counter
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(double value) -> metric<abacus::type::float64>&
    {
        assert(is_initialized());
        // We don't allow assignment to NaN or Inf/-Inf
        assert(!std::isnan(value) && !std::isinf(value) && "Cannot assign a "
                                                           "NaN or Inf/-Inf "
                                                           "value to a "
                                                           "float metric");

        *m_memory -= value;
        return *this;
    }

    /// Increment the value of the counter
    /// @return The result of the arithmetic
    auto operator++() -> metric<abacus::type::float64>&
    {
        assert(is_initialized());
        *m_memory += 1;
        return *this;
    }

    /// Decrement the value of the counter
    /// @return The result of the arithmetic
    auto operator--() -> metric<abacus::type::float64>&
    {
        assert(is_initialized());
        *m_memory -= 1;
        return *this;
    }

    /// @return True if the metric has been assigned memory. False otherwise
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

    /// @return The value of the counter
    auto value() const -> double
    {
        assert(is_initialized());
        return *m_memory;
    }

private:
    /// The metric memory
    double* m_memory = nullptr;
};

/// Metric wrapping bool value.
template <>
class metric<abacus::type::boolean>
{
public:
    /// The underlying data type
    using value_type = bool;

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
    auto operator=(bool value) -> metric<abacus::type::boolean>&
    {
        assert(is_initialized());
        *m_memory = value;
        return *this;
    }

    /// @return True if the metric has been assigned memory. False otherwise
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

    /// @return The value of the counter
    auto value() const -> bool
    {
        assert(is_initialized());
        return *m_memory;
    }

private:
    /// The metric memory
    bool* m_memory = nullptr;
};

}
}
