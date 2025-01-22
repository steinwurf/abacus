// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "availability.hpp"
#include "description.hpp"
#include "detail/optional_metric.hpp"
#include "detail/required_metric.hpp"
#include "kind.hpp"
#include "max.hpp"
#include "min.hpp"
#include "unit.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// A 32-bit integer metric
struct uint32
{
    /// The primitive type of the metric
    using type = uint32_t;

    /// The optional metric type
    using optional = detail::optional_metric<uint32>;

    /// The required metric type
    using required = detail::required_metric<uint32>;

    /// Set the value of the metric
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    /// @param value The value to set
    static inline auto set_value(uint8_t* memory, type value) -> void
    {
        assert(memory != nullptr);
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
    abacus::kind kind;

    /// The metric description
    abacus::description description;

    /// The availability of the metric
    abacus::availability availability;

    /// The unit of the metric
    abacus::unit unit{};

    /// The minimum value of the metric
    abacus::min<type> min{};

    /// The maximum value of the metric
    abacus::max<type> max{};
};
}
}
