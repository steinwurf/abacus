// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <type_traits>

#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
/// Get the size of the primitive backing type of a metric
/// @tparam Metric The metric to get the size of
/// @return The size of the primitive backing type of the metric
template <typename Metric>
constexpr static auto size_of_type() -> std::size_t
{
    return sizeof(typename std::remove_pointer<Metric>::type::type);
}
}
}
}
