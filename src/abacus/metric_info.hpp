// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "value_types.hpp"
#include <string>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
const bool CONSTANT = true;
const bool NON_CONSTANT = false;
struct metric_info
{
    std::string name;
    std::string description;
    value_type type;
    bool is_constant;
};
}
}