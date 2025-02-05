// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <type_traits>
#include <variant>

#include "../constant.hpp"
#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
// Helper to check if a metric is a constant

template <typename Metric, typename Variant>
struct is_constant_;

template <typename T, typename... Types>
struct is_constant_<T, std::variant<Types...>>
    : std::disjunction<std::is_same<T, Types>...>
{
};

template <typename T>
constexpr bool is_constant_v =
    is_constant_<T, decltype(constant::value)>::value;

}
}
}
