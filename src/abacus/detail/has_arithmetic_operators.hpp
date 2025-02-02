// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
struct boolean;
struct enum8;
namespace detail
{
/// Check if a metric supports arithmetic operators
template <typename Metric>
struct has_arithmetic_operators : std::true_type
{
};

template <>
struct has_arithmetic_operators<boolean> : std::false_type
{
};

template <>
struct has_arithmetic_operators<enum8> : std::false_type
{
};

}
}
}
