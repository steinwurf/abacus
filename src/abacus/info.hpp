// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <variant>

#include "boolean.hpp"
#include "enum8.hpp"
#include "float32.hpp"
#include "float64.hpp"
#include "int32.hpp"
#include "int64.hpp"
#include "uint32.hpp"
#include "uint64.hpp"
namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// A variant for all the supported metrics
using info = std::variant<uint64, int64, uint32, int32, float64, float32,
                          boolean, enum8>;
}
}
