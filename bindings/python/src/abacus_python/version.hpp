// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>

namespace abacus_python
{
/// The STEINWURF_ABACUS_VERSION define should be updated on each
/// release
#define STEINWURF_ABACUS_PYTHON_VERSION v0_0_0

inline namespace STEINWURF_ABACUS_PYTHON_VERSION
{
/// @return The version of the library as a string
inline std::string version()
{
    return "0.0.0";
}
}
}
