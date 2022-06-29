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

/// Enum used to declare the endianness of the machine copying metrics data.
enum class endianness : uint8_t
{
    /// little-endian machine
    little_endian = 0U,
    /// big-endian machine
    big_endian
};
}
}
