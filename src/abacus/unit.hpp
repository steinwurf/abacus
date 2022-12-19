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

struct unit
{
    explicit unit(const std::string& name) : value(name)
    {
    }

    std::string value;
};

}
}
