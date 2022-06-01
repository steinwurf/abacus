// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.
#pragma once

#include "../view_iterator.hpp"
#include "data_points.hpp"
#include "exporter.hpp"

#include <iostream>

namespace abacus
{
namespace time_series
{
/// @brief Outputs all data points as a printed JSON array in the ostream
class ostream_exporter final : public exporter
{
public:
    auto receive_data_points(data_points::iterator data_points_start,
                             data_points::iterator data_points_end)
        -> void override
    {
        std::stringstream string_stream;
        json_data_points(string_stream, data_points_start, data_points_end);
        std::cout << string_stream.rdbuf();
    }
};
}
}