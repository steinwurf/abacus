// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <string>
#include <vector>

namespace abacus
{
namespace time_series
{

/// Used to tell the format of a collection of data points.
enum class format
{
    BINARY,
    JSON
};

/// Used as type for working with time in arbitrary numerical unit.
using time_type = uint64_t;

/// An aggregate is a vector where each part, or element, is used to represent
/// the position of a data point in metrics hierarchy.
using aggregate = std::vector<std::string>;
using aggregate_ptr = std::shared_ptr<aggregate>;
}
}