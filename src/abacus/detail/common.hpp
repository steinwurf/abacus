// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
/// A optional_metric class for most metrics
template <typename Metric>
struct optional_metric
{

    using value_type = typename Metric::type;

    /// Default constructor
    optional_metric() = default;

    /// Constructor
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    optional_metric(uint8_t* memory) : m_memory(memory)
    {
        assert(m_memory != nullptr);
        memset(m_memory, 0, sizeof(value_type) + 1);
    }

    /// Constructor
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    /// @param value The initial value of the metric
    optional_metric(uint8_t* memory, value_type value) : optional_metric(memory)
    {
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
        return m_memory[0] == 1U;
    }

    /// Reset the metric
    /// This will set the metric to an uninitialized state causing
    /// has_value() to return false.
    auto reset() -> void
    {
        assert(is_initialized());
        m_memory[0] = 0;
    }

    /// Get the value of the metric
    /// @return The value of the metric
    auto value() const -> value_type
    {
        assert(is_initialized());
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
        std::memcpy(m_memory + 1, &value, sizeof(value));
    }

    /// Assign the metric a new value
    /// @param value The value to assign
    /// @return the metric with the new value
    auto operator=(value_type value) -> optional_metric&
    {
        assert(is_initialized());
        set_value(value);
        return *this;
    }

    /// Increment the metric
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(value_type value) -> optional_metric&
    {
        assert(has_value());

        if constexpr (std::is_floating_point_v<value_type>)
        {
            assert(!std::isnan(value) && "Cannot assign a NaN");
            assert(!std::isinf(value) && "Cannot assign an Inf/-Inf value");
        }

        auto new_value = this->value() + value;
        std::memcpy(m_memory + 1, &new_value, sizeof(value_type));
        return *this;
    }

    /// Decrement the metric
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(value_type value) -> optional_metric&
    {
        assert(has_value());

        if constexpr (std::is_floating_point_v<value_type>)
        {
            assert(!std::isnan(value) && "Cannot assign a NaN");
            assert(!std::isinf(value) && "Cannot assign an Inf/-Inf value");
        }

        auto new_value = this->value() - value;
        std::memcpy(m_memory + 1, &new_value, sizeof(value_type));
        return *this;
    }

    /// Increment the value of the metric
    /// @return The result of the arithmetic
    auto operator++() -> optional_metric&
    {
        assert(has_value());
        auto new_value = value() + 1;
        std::memcpy(m_memory + 1, &new_value, sizeof(value_type));
        return *this;
    }

    /// Decrement the value of the metric
    /// @return The result of the arithmetic
    auto operator--() -> optional_metric&
    {
        auto new_value = value() - 1;
        std::memcpy(m_memory + 1, &new_value, sizeof(value_type));
        return *this;
    }

protected:
    /// The metric memory
    uint8_t* m_memory = nullptr;
};

/// A required_metric class for most metrics
template <typename Metric>
struct required_metric
{

    using value_type = typename Metric::type;

    /// Default constructor
    required_metric() = default;

    /// Constructor
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    required_metric(uint8_t* memory) : m_memory(memory)
    {
        assert(m_memory != nullptr);
        memory[0] = 1;
        memset(m_memory + 1, 0, sizeof(value_type));
    }

    /// Constructor
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    /// @param value The initial value of the metric
    required_metric(uint8_t* memory, value_type value) : required_metric(memory)
    {
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

    /// Increment the metric
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(value_type value) -> required_metric&
    {

        if constexpr (std::is_floating_point_v<value_type>)
        {
            assert(!std::isnan(value) && "Cannot assign a NaN");
            assert(!std::isinf(value) && "Cannot assign an Inf/-Inf value");
        }

        auto new_value = this->value() + value;
        std::memcpy(m_memory + 1, &new_value, sizeof(value_type));
        return *this;
    }

    /// Decrement the metric
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(value_type value) -> required_metric&
    {
        if constexpr (std::is_floating_point_v<value_type>)
        {
            assert(!std::isnan(value) && "Cannot assign a NaN");
            assert(!std::isinf(value) && "Cannot assign an Inf/-Inf value");
        }

        auto new_value = this->value() - value;
        std::memcpy(m_memory + 1, &new_value, sizeof(value_type));
        return *this;
    }

    /// Increment the value of the metric
    /// @return The result of the arithmetic
    auto operator++() -> required_metric&
    {
        auto new_value = value() + 1;
        std::memcpy(m_memory + 1, &new_value, sizeof(value_type));
        return *this;
    }

    /// Decrement the value of the metric
    /// @return The result of the arithmetic
    auto operator--() -> required_metric&
    {
        auto new_value = value() - 1;
        std::memcpy(m_memory + 1, &new_value, sizeof(value_type));
        return *this;
    }

protected:
    /// The metric memory
    uint8_t* m_memory = nullptr;
};

}
}
}
