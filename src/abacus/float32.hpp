// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>

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
/// A 32-bit floating point metric
struct float32
{
    /// The primitive type of the metric
    using type = float;

    /// Required float32 metric
    using required = required_metric<float32>;

    /// Optional float32 metric
    using optional = optional_metric<float32>;

    /// Set the value of the metric
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    /// @param value The value to set
    static inline auto set_value(uint8_t* memory, type value) -> void
    {
        assert(memory != nullptr);

        assert(!std::isnan(value) && "Cannot assign a NaN");
        assert(!std::isinf(value) && "Cannot assign an Inf/-Inf value");

        std::memcpy(memory + 1, &value, sizeof(type));
    }

    /// Get the value of the metric
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    /// @return The value of the metric
    static inline auto value(const uint8_t* memory) -> type
    {
        assert(memory != nullptr);
        assert(memory[0] == 1);
        type value;
        std::memcpy(&value, memory + 1, sizeof(type));
        return value;
    }

    /// The metric kind
    std::variant<gauge, counter, constant<type>> kind;

    /// The metric description
    abacus::description description;

    /// The unit of the metric
    abacus::unit unit{};

    /// The minimum value of the metric
    abacus::min<type> min{};

    /// The maximum value of the metric
    abacus::max<type> max{};
};
}
}
