// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "availability.hpp"
#include "description.hpp"
#include "kind.hpp"
#include "max.hpp"
#include "min.hpp"
#include "optional_metric.hpp"
#include "required_metric.hpp"
#include "unit.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// A 32-bit integer metric
struct string
{
    /// The primitive type of the metric
    using type = std::string_view;

    /// Set the value of the metric
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    /// @param value The value to set
    static inline auto set_value(uint8_t* memory, type value) -> void
    {
        assert(memory != nullptr);
        std::memcpy(memory + 1, value.data(), value.size());
    }

    /// Get the value of the metric
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    /// @return The value of the metric
    static inline auto value(const uint8_t* memory) -> type
    {
        assert(memory != nullptr);
        assert(memory[0] == 1);

        return std::string_view(reinterpret_cast<const char*>(memory + 1));
    }

    /// The metric kind
    std::variant<constant<std::string_view>> kind;

    /// The metric description
    abacus::description description;
};
}
}
