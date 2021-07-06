// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <type_traits>

#include "../version.hpp"

#include "bit_index.hpp"
#include "range.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
using bit_range = range<bit_index, struct _bit_range>;
}
}
}
