// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>
#include <vector>

namespace abacus
{
namespace telemetry
{
enum class format
{
    BINARY,
    JSON
};

using time_type = uint64_t;
using aggregate = std::vector<std::string>;
using aggregate_ptr = std::shared_ptr<aggregate>;
}
}