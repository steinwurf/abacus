// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

#include "detail/has_arithmetic_operators.hpp"

#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// An optional metric class for most metrics
template <typename Metric>
struct required_metric
{
    using value_type = typename Metric::type;

    /// Default constructor
    required_metric() = default;

    /// Constructor
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(value_type) + 1 bytes long.
    /// @param value The initial value of the metric
    required_metric(uint8_t* memory)
    {
        assert(memory != nullptr);
        m_memory = memory;

        // Set the initialized flag to true as this should always be the case
        // for required metrics.
        m_memory[0] = 1;
    }

    /// Check if the metric is initialized
    /// @return true if the metric is initialized
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

    /// Get the value of the metric
    /// @return The value of the metric
    auto value() const -> value_type
    {
        assert(is_initialized());

        value_type value;
        std::memcpy(&value, m_memory + 1, sizeof(value_type));
        return value;
    }

    /// Assign a new value to the metric
    /// @param value The value to assign
    auto set_value(value_type value) -> void
    {
        assert(is_initialized());

        if constexpr (std::is_floating_point_v<value_type>)
        {
            assert(!std::isnan(value) && "Cannot assign a NaN");
            assert(!std::isinf(value) && "Cannot assign an Inf/-Inf value");
        }

        std::memcpy(m_memory + 1, &value, sizeof(value_type));
    }

    /// Assign the metric a new value
    /// @param value The value to assign
    /// @return the metric with the new value
    auto operator=(value_type value) -> required_metric&
    {
        set_value(value);
        return *this;
    }

public:
    /// Arithmetic operators

    /// Increment the metric
    /// @param increment The value to add
    /// @return The result of the arithmetic
    auto operator+=(value_type increment) -> required_metric&
    {
        set_value(increment + value());
        return *this;
    }

    /// Decrement the metric
    /// @param decrement The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(value_type decrement) -> required_metric&
    {
        set_value(value() - decrement);
        return *this;
    }

    /// Increment the value of the metric
    /// @return The result of the arithmetic
    auto operator++() -> required_metric&
    {
        set_value(value() + 1);
        return *this;
    }

    /// Decrement the value of the metric
    /// @return The result of the arithmetic
    auto operator--() -> required_metric&
    {
        set_value(value() - 1);
        return *this;
    }

protected:
    /// The metric memory
    uint8_t* m_memory = nullptr;
};

/// Enum specializations
template <>
struct required_metric<enum8>
{
    /// Default constructor
    required_metric() = default;

    /// Constructor
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(value_type) + 1 bytes long.
    /// @param value The initial value of the metric
    required_metric(uint8_t* memory)
    {
        assert(memory != nullptr);
        m_memory = memory;

        // Set the initialized flag to true as this should always be the case
        // for required metrics.
        m_memory[0] = 1;
    }

    /// Check if the metric is initialized
    /// @return true if the metric is initialized
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

    /// The the value as a specific enum type
    /// @return The value of the metric as the enum type
    template <typename T>
    auto value() const -> T
    {
        static_assert(std::is_enum_v<T>);
        assert(is_initialized());

        return static_cast<T>(m_memory[1]);
    }

    /// Assign a new value to the metric
    /// @param value The value to assign
    template <typename T>
    auto set_value(T value) -> void
    {
        // @todo reenable this check
        // static_assert(std::is_enum_v<T>);

        assert(static_cast<int64_t>(value) <=
                   std::numeric_limits<uint8_t>::max() &&
               "The value is too large to fit in the enum");
        assert(static_cast<int64_t>(value) >=
                   std::numeric_limits<uint8_t>::min() &&
               "The value is too small to fit in the enum");

        assert(is_initialized());

        m_memory[1] = static_cast<uint8_t>(value);
    }

    /// Assign the metric a new value
    template <class T>
    auto operator=(T value) -> required_metric&
    {
        set_value(value);
        return *this;
    }

protected:
    /// The metric memory
    uint8_t* m_memory = nullptr;
};

/// Boolean specializations
template <>
struct required_metric<boolean>
{
    /// Default constructor
    required_metric() = default;

    /// Constructor
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(value_type) + 1 bytes long.
    /// @param value The initial value of the metric
    required_metric(uint8_t* memory)
    {
        assert(memory != nullptr);
        m_memory = memory;

        // Set the initialized flag to true as this should always be the case
        // for required metrics.
        m_memory[0] = 1;
    }

    /// Check if the metric is initialized
    /// @return true if the metric is initialized
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

    /// Get the value of the metric
    /// @return The value of the metric
    auto value() const -> bool
    {
        assert(is_initialized());
        return static_cast<bool>(m_memory[1]);
    }

    /// Assign a new value to the metric
    /// @param value The value to assign
    auto set_value(bool value) -> void
    {
        assert(is_initialized());
        m_memory[1] = static_cast<uint8_t>(value);
    }

    /// Assign the metric a new value
    /// @param value The value to assign
    /// @return the metric with the new value
    auto operator=(bool value) -> required_metric&
    {
        set_value(value);
        return *this;
    }

private:
    /// The metric memory
    uint8_t* m_memory = nullptr;
};
}
}
