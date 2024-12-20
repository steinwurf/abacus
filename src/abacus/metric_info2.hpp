// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <variant>

#include "kind.hpp"
#include "unit.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
struct uint64
{
    using type = uint64_t;
    std::string description;
    abacus::kind kind;
    std::string unit;
    std::optional<uint64_t> min;
    std::optional<uint64_t> max;
};
struct int64
{
    using type = int64_t;
    std::string description;
    abacus::kind kind;
    std::string unit;
    std::optional<int64_t> min;
    std::optional<int64_t> max;
};
struct uint32
{
    using type = uint32_t;
    std::string description;
    abacus::kind kind;
    std::string unit;
    std::optional<uint32_t> min;
    std::optional<uint32_t> max;
};
struct int32
{
    using type = int32_t;
    std::string description;
    abacus::kind kind;
    std::string unit;
    std::optional<int32_t> min;
    std::optional<int32_t> max;
};
struct float64
{
    using type = double;
    std::string description;
    abacus::kind kind;
    std::string unit;
    std::optional<double> min;
    std::optional<double> max;
};
struct float32
{
    using type = float;
    std::string description;
    abacus::kind kind;
    std::string unit;
    std::optional<float> min;
    std::optional<float> max;
};
struct boolean
{
    using type = bool;
    std::string description;
    abacus::kind kind;
};

struct enum8
{
    using type = uint8_t;
    struct value
    {
        std::string name;
        std::string description;
    };
    std::string description;
    std::string unit;
    std::map<uint32_t, value> values;
};
using metric_info2 =
    std::map<std::string, std::variant<uint64, int64, uint32, int32, float64,
                                       float32, boolean, enum8>>;
}
}
