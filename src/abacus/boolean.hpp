// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>

#include "kind.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// A boolean metric
struct boolean
{
    /// The primitive type of the metric
    using type = bool;

    /// The metric type
    struct metric
    {
        /// Default constructor
        metric() = default;

        /// Constructor
        /// @param memory The memory to use for the metric, note that the memory
        /// must be at least sizeof(type) + 1 bytes long.
        metric(uint8_t* memory) : m_memory(memory)
        {
            assert(m_memory != nullptr);
            memset(m_memory, 0, sizeof(type) + 1);
        }

        /// Constructor
        /// @param memory The memory to use for the metric, note that the memory
        /// must be at least sizeof(type) + 1 bytes long.
        /// @param value The initial value of the metric
        metric(uint8_t* memory, type value) : metric(memory)
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
        /// This will set the metric to an uninitialized state and cause
        /// has_value() to return false
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
            return m_memory[1];
        }

        /// Assign the metric a new value
        /// @param value The value to assign
        auto set_value(type value) -> void
        {
            assert(is_initialized());
            m_memory[0] = 1;
            m_memory[1] = value;
        }

        /// Assign the metric a new value
        /// @param value The value to assign
        /// @return a metric with the new value
        auto operator=(type value) -> metric&
        {
            assert(is_initialized());
            set_value(value);
            return *this;
        }

    private:
        /// The metric memory
        uint8_t* m_memory = nullptr;
    };

    /// The kind of the metric
    abacus::kind kind;

    /// The description of the metric
    std::string description;
};
}
}
