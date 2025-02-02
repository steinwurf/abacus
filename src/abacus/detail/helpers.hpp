// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <type_traits>
#include <variant>

#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
// Helper to check if a type is in the variant
template <typename T, typename Variant>
struct is_in_variant;

template <typename T, typename... Types>
struct is_in_variant<T, std::variant<Types...>>
    : std::disjunction<std::is_same<T, Types>...>
{
};

template <typename T, typename Variant>
constexpr bool is_in_variant_v = is_in_variant<T, Variant>::value;

template <class Func>
static inline auto call_type(const protobuf::Metric& metric, const Func& func)
    -> decltype(func(metric.uint64()))
{
    switch (metric.type_case())
    {
    case protobuf::Metric::kUint64:
        return func(metric.uint64());
    case protobuf::Metric::kInt64:
        return func(metric.int64());
    case protobuf::Metric::kUint32:
        return func(metric.uint32());
    case protobuf::Metric::kInt32:
        return func(metric.int32());
    case protobuf::Metric::kFloat64:
        return func(metric.float64());
    case protobuf::Metric::kFloat32:
        return func(metric.float32());
    case protobuf::Metric::kBoolean:
        return func(metric.boolean());
    case protobuf::Metric::kEnum8:
        return func(metric.enum8());
    default:
        // This should never be reached
        assert(false);
        using ReturnType = decltype(func(metric.uint64()));
        return ReturnType();
    }
}

static inline std::size_t get_offset(const protobuf::Metric& m)
{
    return call_type(m, [](const auto& metric) { return metric.offset(); });
}

}
}
}
