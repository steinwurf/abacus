// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <map>
#include <string>

#include "availability.hpp"
#include "description.hpp"
#include "optional_metric.hpp"
#include "required_metric.hpp"
#include "unit.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// A enumeration metric
struct enum8
{
    /// The primitive type of the metric
    using type = uint8_t;

    /// Required enum8 metric
    using required = required_metric<enum8>;

    /// Optional enum8 metric
    using optional = optional_metric<enum8>;

    /// Set the value of the metric
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    /// @param value The value to set
    template <typename T>
    static inline auto set_value(uint8_t* memory, T value) -> void
    {
        static_assert(std::is_enum_v<T>, "T must be an enum");
        static_assert(sizeof(std::underlying_type_t<T>) == sizeof(type),
                      "The underlying type of the enum must match the type");
        assert(memory != nullptr);
        memory[1] = (std::underlying_type_t<T>)value;
    }

    /// Set the value of the metric
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    /// @param value The value to set
    static inline auto set_value(uint8_t* memory, type value) -> void
    {
        assert(memory != nullptr);
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

    /// Get the enumeration value of the metric
    /// @param memory The memory to use for the metric, note that the memory
    ///        must be at least sizeof(type) + 1 bytes long.
    /// @return The enumeration value of the metric
    template <typename T>
    static inline auto value(const uint8_t* memory) -> T
    {
        static_assert(std::is_enum_v<T>, "T must be an enum");
        static_assert(sizeof(std::underlying_type_t<T>) == sizeof(type),
                      "The underlying type of the enum must match the type");
        assert(memory != nullptr);
        assert(memory[0] == 1);
        return (T)memory[1];
    }

    /// The enumeration value type
    struct value_info
    {
        /// The name of the value
        std::string name;

        /// The description of the value
        std::string description;
    };

    /// The metric kind
    std::variant<gauge, constant> kind;

    /// The metric description
    abacus::description description;

    /// The enumeration value information
    std::map<uint32_t, value_info> values;

    /// The unit of the metric
    abacus::unit unit{};
};
}
}
