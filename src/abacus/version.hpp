// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>

namespace abacus
{
/// Here we define the STEINWURF_ABACUS_VERSION this should be updated on each
/// release
#define STEINWURF_ABACUS_VERSION v7_0_0

inline namespace STEINWURF_ABACUS_VERSION
{
/// @return The version of the library as string
std::string version();
}
}
