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
// Variant overload visit helper
template <typename... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};
template <class... Ts>
overload(Ts...) -> overload<Ts...>;
}
}
}
