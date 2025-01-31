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

// Helper to detect the presence of mutable_gauge function
template <typename T, typename = void>
struct has_mutable_gauge : std::false_type
{
};

template <typename T>
struct has_mutable_gauge<
    T, std::void_t<decltype(std::declval<T>().mutable_gauge())>>
    : std::true_type
{
};

template <typename T>
inline constexpr bool has_mutable_gauge_v = has_mutable_gauge<T>::value;

// Helper to detect the presence of counter function
template <typename T, typename = void>
struct has_mutable_counter : std::false_type
{
};

template <typename T>
struct has_mutable_counter<
    T, std::void_t<decltype(std::declval<T>().mutable_counter())>>
    : std::true_type
{
};

template <typename T>
inline constexpr bool has_mutable_counter_v = has_mutable_counter<T>::value;

// Helper to detect the presence of mutable_constant function
template <typename T, typename = void>
struct has_mutable_constant : std::false_type
{
};

template <typename T>
struct has_mutable_constant<
    T, std::void_t<decltype(std::declval<T>().mutable_constant())>>
    : std::true_type
{
};

template <typename T>
inline constexpr bool has_mutable_constant_v = has_mutable_constant<T>::value;

// Helper to detect the presence of offset function
template <typename T, typename = void>
struct has_offset : std::false_type
{
};

template <typename T>
struct has_offset<T, std::void_t<decltype(std::declval<T>().offset())>>
    : std::true_type
{
};

template <typename T>
inline constexpr bool has_offset_v = has_offset<T>::value;

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

// Helper to detect the presence of kGauge in an enum
template <typename T, typename = void>
struct has_kGauge : std::false_type
{
};

template <typename T>
struct has_kGauge<T, std::void_t<decltype(T::kGauge)>> : std::true_type
{
};

template <typename T>
inline constexpr bool has_kGauge_v = has_kGauge<T>::value;

// Helper to detect the presence of kCounter in an enum
template <typename T, typename = void>
struct has_kCounter : std::false_type
{
};

template <typename T>
struct has_kCounter<T, std::void_t<decltype(T::kCounter)>> : std::true_type
{
};

template <typename T>
inline constexpr bool has_kCounter_v = has_kCounter<T>::value;

// Helper to detect the presence of kConstant in an enum
template <typename T, typename = void>
struct has_kConstant : std::false_type
{
};

template <typename T>
struct has_kConstant<T, std::void_t<decltype(T::kConstant)>> : std::true_type
{
};

template <typename T>
inline constexpr bool has_kConstant_v = has_kConstant<T>::value;

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
    case protobuf::Metric::kString:
        return func(metric.string());
    default:
        // This should never be reached
        assert(false);
        using ReturnType = decltype(func(metric.uint64()));
        return ReturnType();
    }
}

static inline std::size_t get_offset(const protobuf::Metric& m)
{
    return call_type(m,
                     [](const auto& metric)
                     {
                         using Metric = std::decay_t<decltype(metric)>;
                         if constexpr (has_offset_v<Metric>)
                         {
                             return metric.offset();
                         }
                         return 0U;
                     });
}

}
}
}
