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
struct optional_metric
{
    using value_type = typename Metric::type;

    /// Default constructor
    optional_metric() = default;

    /// Constructor
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(value_type) + 1 bytes long.
    /// @param value The initial value of the metric
    optional_metric(uint8_t* memory)
    {
        assert(memory != nullptr);
        m_memory = memory;
    }

    /// Constructor
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(value_type) + 1 bytes long.
    /// @param value The initial value of the metric
    optional_metric(uint8_t* memory, value_type value) : optional_metric(memory)
    {
        assert(m_memory != nullptr);
        set_value(value);
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
    auto set_value(value_type value) -> void
    {
        assert(is_initialized());

        if constexpr (std::is_floating_point_v<value_type>)
        {
            assert(!std::isnan(value) && "Cannot assign a NaN");
            assert(!std::isinf(value) && "Cannot assign an Inf/-Inf value");
        }
        m_memory[0] = 1;
        std::memcpy(m_memory + 1, &value, sizeof(value_type));
    }

    /// Assign the metric a new value
    /// @param value The value to assign
    /// @return the metric with the new value
    auto operator=(value_type value) -> optional_metric&
    {
        set_value(value);
        return *this;
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
    /// @param value The value to add
    /// @return The result of the arithmetic
    template <
        typename U = Metric,
        typename = std::enable_if_t<detail::has_arithmetic_operators<U>::value>>
    auto operator+=(value_type value) -> optional_metric&
    {
        assert(has_value());
        value_type new_value = this->value() + value;

        if constexpr (std::is_floating_point_v<value_type>)
        {
            assert(!std::isnan(new_value) && "Cannot assign a NaN");
            assert(!std::isinf(new_value) && "Cannot assign an Inf/-Inf value");
        }

        std::memcpy(m_memory + 1, &new_value, sizeof(value_type));
        return *this;
    }

    /// Decrement the metric
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    template <
        typename U = Metric,
        typename = std::enable_if_t<detail::has_arithmetic_operators<U>::value>>
    auto operator-=(value_type value) -> optional_metric&
    {
        assert(has_value());
        value_type new_value = this->value() - value;

        if constexpr (std::is_floating_point_v<value_type>)
        {
            assert(!std::isnan(new_value) && "Cannot assign a NaN");
            assert(!std::isinf(new_value) && "Cannot assign an Inf/-Inf value");
        }

        std::memcpy(m_memory + 1, &new_value, sizeof(value_type));
        return *this;
    }

    /// Increment the value of the metric
    /// @return The result of the arithmetic
    template <
        typename U = Metric,
        typename = std::enable_if_t<detail::has_arithmetic_operators<U>::value>>
    auto operator++() -> optional_metric&
    {
        assert(has_value());
        value_type new_value = this->value() + 1;
        std::memcpy(m_memory + 1, &new_value, sizeof(value_type));
        return *this;
    }

    /// Decrement the value of the metric
    /// @return The result of the arithmetic
    template <
        typename U = Metric,
        typename = std::enable_if_t<detail::has_arithmetic_operators<U>::value>>
    auto operator--() -> optional_metric&
    {
        assert(has_value());
        value_type new_value = this->value() - 1;
        std::memcpy(m_memory + 1, &new_value, sizeof(value_type));
        return *this;
    }

public:
    /// Enum specializations

    /// Constructor for enum8
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    /// @param value The initial enum value of the metric
    template <typename T,
              typename = std::enable_if_t<std::is_enum_v<T> &&
                                          std::is_same_v<Metric, enum8>>>
    optional_metric(uint8_t* memory, T value) :
        optional_metric(memory, static_cast<value_type>(value))
    {
        static_assert(
            sizeof(std::underlying_type_t<T>) == sizeof(value_type),
            "The underlying type of the enum must match the value_type");
    }

    template <typename T,
              typename = std::enable_if_t<std::is_enum_v<T> &&
                                          std::is_same_v<Metric, enum8>>>
    auto operator=(T value) -> optional_metric&
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
        assert((std::underlying_type_t<T>)value <=
               std::numeric_limits<value_type>::max());

        set_value(static_cast<value_type>(value));
    }

protected:
    /// The metric memory
    uint8_t* m_memory = nullptr;
};
}
}
