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
/// A required_metric class for most metrics
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
    required_metric(uint8_t* memory, value_type value)
    {
        assert(memory != nullptr);
        m_memory = memory;
        // Set the initialized flag to true as this should always be the case
        // for required metrics.
        m_memory[0] = 1;
        set_value(value);
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
    /// @param value The value to add
    /// @return The result of the arithmetic
    template <
        typename U = Metric,
        typename = std::enable_if_t<detail::has_arithmetic_operators<U>::value>>
    auto operator+=(value_type value) -> required_metric&
    {
        set_value(this->value() + value);
        return *this;
    }

    /// Decrement the metric
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    template <
        typename U = Metric,
        typename = std::enable_if_t<detail::has_arithmetic_operators<U>::value>>
    auto operator-=(value_type value) -> required_metric&
    {
        set_value(this->value() - value);
        return *this;
    }

    /// Increment the value of the metric
    /// @return The result of the arithmetic
    template <
        typename U = Metric,
        typename = std::enable_if_t<detail::has_arithmetic_operators<U>::value>>
    auto operator++() -> required_metric&
    {
        set_value(value() + 1);
        return *this;
    }

    /// Decrement the value of the metric
    /// @return The result of the arithmetic
    template <
        typename U = Metric,
        typename = std::enable_if_t<detail::has_arithmetic_operators<U>::value>>
    auto operator--() -> required_metric&
    {
        set_value(value() - 1);
        return *this;
    }

public:
    /// Enum specializations

    /// Constructor for enum8
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(value_type) + 1 bytes long.
    /// @param value The initial enum value of the metric
    template <typename T,
              typename = std::enable_if_t<std::is_enum_v<T> &&
                                          std::is_same_v<Metric, enum8>>>
    required_metric(uint8_t* memory, T value) :
        required_metric(memory, static_cast<value_type>(value))
    {
        static_assert(
            sizeof(std::underlying_type_t<T>) == sizeof(value_type),
            "The underlying type of the enum must match the value_type");
    }

    template <typename T,
              typename = std::enable_if_t<std::is_enum_v<T> &&
                                          std::is_same_v<Metric, enum8>>>
    auto operator=(T value) -> required_metric&
    {
        set_value(value);
        return *this;
    }

    /// Assign a new value to the metric
    /// @param value The value to assign
    template <typename T,
              typename = std::enable_if_t<std::is_enum_v<T> &&
                                          std::is_same_v<Metric, enum8>>>
    auto set_value(T value) -> void
    {
        assert(is_initialized());
        assert((typename std::underlying_type<T>::type)value <=
               std::numeric_limits<value_type>::max());

        set_value(static_cast<value_type>(value));
    }

protected:
    /// The metric memory
    uint8_t* m_memory = nullptr;
};

}
}
