// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
/// A common class for most metrics
template <typename T>
struct common
{
    /// The primitive type of the metric
    using type = typename T::type;

    /// The metric type
    using metric = typename T::metric;

    /// Default constructor
    common() = default;

    /// Constructor
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    common(uint8_t* memory) : m_memory(memory)
    {
        assert(m_memory != nullptr);
        memset(m_memory, 0, sizeof(type) + 1);
    }

    /// Constructor
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    /// @param value The initial value of the metric
    common(uint8_t* memory, type value) : common(memory)
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
    auto value() const -> type
    {
        assert(is_initialized());
        assert(has_value());
        type value;
        std::memcpy(&value, m_memory + 1, sizeof(value));
        return value;
    }

    /// Assign a new value to the metric
    /// @param value The value to assign
    auto set_value(type value) -> void
    {
        assert(is_initialized());
        assert(!std::isnan(value) && "Cannot assign a NaN");
        assert(!std::isinf(value) && "Cannot assign an Inf/-Inf value");
        m_memory[0] = 1;
        std::memcpy(m_memory + 1, &value, sizeof(value));
    }

    /// Assign the metric a new value
    /// @param value The value to assign
    /// @return the metric with the new value
    auto operator=(type value) -> metric&
    {
        assert(is_initialized());
        set_value(value);
        return (metric&)*this;
    }

    /// Increment the metric
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(type value) -> metric&
    {
        assert(has_value());
        auto new_value = this->value() + value;
        assert(!std::isnan(new_value) && "Cannot assign a NaN");
        assert(!std::isinf(new_value) && "Cannot assign an Inf/-Inf value");
        std::memcpy(m_memory + 1, &new_value, sizeof(new_value));
        return (metric&)*this;
    }

    /// Decrement the metric
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(type value) -> metric&
    {
        assert(has_value());
        auto new_value = this->value() - value;
        assert(!std::isnan(new_value) && "Cannot assign a NaN");
        assert(!std::isinf(new_value) && "Cannot assign an Inf/-Inf value");
        std::memcpy(m_memory + 1, &new_value, sizeof(new_value));
        return (metric&)*this;
    }

    /// Increment the value of the metric
    /// @return The result of the arithmetic
    auto operator++() -> metric&
    {
        assert(has_value());
        auto new_value = value() + 1;
        std::memcpy(m_memory + 1, &new_value, sizeof(new_value));
        return (metric&)*this;
    }

    /// Decrement the value of the metric
    /// @return The result of the arithmetic
    auto operator--() -> metric&
    {
        auto new_value = value() - 1;
        std::memcpy(m_memory + 1, &new_value, sizeof(new_value));
        return (metric&)*this;
    }

protected:
    /// The metric memory
    uint8_t* m_memory = nullptr;
};
}
}
}
