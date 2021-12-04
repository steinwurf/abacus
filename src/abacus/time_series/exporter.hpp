// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <memory>
#include <vector>

#include "data_points.hpp"

namespace abacus
{
namespace time_series
{
/// @brief Interface for exporters
class exporter
{
public:
    /// The entry point for flushing data points from one or more collectors
    virtual auto receive_data_points(data_points::iterator data_points_start,
                                     data_points::iterator data_points_end)
        -> void = 0;
};

using exporters = std::vector<std::shared_ptr<exporter>>;
}
}