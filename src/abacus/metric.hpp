// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

#include "boolean.hpp"
#include "enum8.hpp"
#include "float32.hpp"
#include "float64.hpp"
#include "int32.hpp"
#include "int64.hpp"
#include "uint32.hpp"
#include "uint64.hpp"
#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

/// An optional metric class for most metrics
template <typename Metric>
struct metric
{
    using value_type = typename Metric::type;

    /// Default constructor
    metric() = default;

    /// Constructor
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(value_type) + 1 bytes long.
    /// @param value The initial value of the metric
    metric(uint8_t* memory)
    {
        assert(memory != nullptr);
        m_memory = memory;
    }

    /// Check if the metric is initialized
    /// @return true if the metric is initialized
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

    /// Check if the metric has a value
    /// @return true if the metric has a value
    auto has_value() const -> bool
    {
        assert(is_initialized());
        return m_memory[0] == 1;
    }

    /// Get the value of the metric
    /// @return The value of the metric
    auto value() const -> value_type
    {
        assert(has_value());
        value_type value;
        std::memcpy(&value, m_memory + 1, sizeof(value_type));
        return value;
    }

    /// Assign a new value to the metric
    /// @param value The value to assign
    auto set_value(value_type value) -> metric&
    {
        assert(is_initialized());

        if constexpr (std::is_floating_point_v<value_type>)
        {
            assert(!std::isnan(value) && "Cannot assign a NaN");
            assert(!std::isinf(value) && "Cannot assign an Inf/-Inf value");
        }

        m_memory[0] = 1;
        std::memcpy(&value, m_memory + 1, sizeof(value_type));

        return *this;
    }

    /// Assign the metric a new value
    /// @param value The value to assign
    /// @return the metric with the new value
    auto operator=(value_type value) -> metric&
    {
        return set_value(value);
    }

    /// Reset the metric. This will cause the metric to not have a value
    auto reset() -> void
    {
        assert(is_initialized());
        m_memory[0] = 0;
    }

public:
    /// Arithmetic operators

    /// Increment the metric
    /// @param increment The value to add
    /// @return The result of the arithmetic
    auto operator+=(value_type increment) -> metric&
    {
        set_value(value() + increment);
        return *this;
    }

    /// Decrement the metric
    /// @param decrement The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(value_type decrement) -> metric&
    {
        set_value(value() - decrement);
        return *this;
    }

    /// Increment the value of the metric
    /// @return The result of the arithmetic
    auto operator++() -> metric&
    {
        set_value(value() + 1);
        return *this;
    }

    /// Decrement the value of the metric
    /// @return The result of the arithmetic
    auto operator--() -> metric&
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
struct metric<enum8>
{
    /// Default constructor
    metric() = default;

    /// Constructor
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(value_type) + 1 bytes long.
    /// @param value The initial value of the metric
    metric(uint8_t* memory)
    {
        assert(memory != nullptr);
        m_memory = memory;
    }

    /// Check if the metric is initialized
    /// @return true if the metric is initialized
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

    /// Check if the metric has a value
    /// @return true if the metric has a value
    auto has_value() const -> bool
    {
        assert(is_initialized());
        return m_memory[0] == 1;
    }

    /// The the value as a specific enum type
    /// @return The value of the metric as the enum type
    template <typename T>
    auto value() const -> T
    {
        static_assert(std::is_enum_v<T>);

        assert(has_value());

        return static_cast<T>(m_memory[1]);
    }

    /// Assign a new value to the metric
    /// @param value The value to assign
    template <typename T>
    auto set_value(T value) -> metric&
    {
        // @todo enable again
        // static_assert(std::is_enum_v<T>);

        assert(static_cast<int64_t>(value) <=
                   std::numeric_limits<uint8_t>::max() &&
               "The value is too large to fit in the enum");
        assert(static_cast<int64_t>(value) >=
                   std::numeric_limits<uint8_t>::min() &&
               "The value is too small to fit in the enum");

        assert(is_initialized());

        m_memory[0] = 1;
        m_memory[1] = static_cast<uint8_t>(value);

        return *this;
    }

    /// Assign the metric a new value
    template <class T>
    auto operator=(T value) -> metric&
    {
        return set_value(value);
    }

    /// Reset the metric. This will cause the metric to not have a value
    auto reset() -> void
    {
        assert(is_initialized());
        m_memory[0] = 0;
    }

protected:
    /// The metric memory
    uint8_t* m_memory = nullptr;
};

/// Boolean specializations
template <>
struct metric<boolean>
{
    /// Default constructor
    metric() = default;

    /// Constructor
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(value_type) + 1 bytes long.
    /// @param value The initial value of the metric
    metric(uint8_t* memory)
    {
        assert(memory != nullptr);
        m_memory = memory;
    }

    /// Check if the metric is initialized
    /// @return true if the metric is initialized
    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

    /// Check if the metric has a value
    /// @return true if the metric has a value
    auto has_value() const -> bool
    {
        assert(is_initialized());
        return m_memory[0] == 1;
    }

    /// Get the value of the metric
    /// @return The value of the metric
    auto value() const -> bool
    {
        assert(has_value());
        return static_cast<bool>(m_memory[1]);
    }

    /// Assign a new value to the metric
    /// @param value The value to assign
    auto set_value(bool value) -> metric&
    {
        assert(is_initialized());
        m_memory[0] = 1;
        m_memory[1] = static_cast<uint8_t>(value);

        return *this;
    }

    /// Assign the metric a new value
    /// @param value The value to assign
    /// @return the metric with the new value
    auto operator=(bool value) -> metric&
    {
        return set_value(value);
    }

    /// Reset the metric. This will cause the metric to not have a value
    auto reset() -> void
    {
        assert(is_initialized());
        m_memory[0] = 0;
    }

private:
    /// The metric memory
    uint8_t* m_memory = nullptr;
};
}
}
