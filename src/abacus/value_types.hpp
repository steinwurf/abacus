// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
enum class value_type : uint8_t
{
    unsigned_integral,
    signed_integral,
    floating_point,
    boolean
};
}
}
