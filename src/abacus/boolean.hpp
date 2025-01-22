// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>

#include "availability.hpp"
#include "description.hpp"
#include "detail/optional_metric.hpp"
#include "detail/required_metric.hpp"
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

    /// The optional metric type
    using optional = detail::optional_metric<boolean>;
    /// The required metric type
    using required = detail::required_metric<boolean>;

    /// Set the value of the metric
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    /// @param value The value to set
    static inline auto set_value(uint8_t* memory, type value) -> void
    {
        assert(memory != nullptr);
        memory[0] = 1;
        memory[1] = value;
    }

    /// Get the value of the metric
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    /// @return The value of the metric
    static inline auto value(const uint8_t* memory) -> type
    {
        assert(memory != nullptr);
        assert(memory[0] == 1);
        return memory[1];
    }

    /// The kind of the metric
    abacus::kind kind;

    /// The description of the metric
    abacus::description description;

    /// The availability of the metric
    abacus::availability availability;
};
}
}
