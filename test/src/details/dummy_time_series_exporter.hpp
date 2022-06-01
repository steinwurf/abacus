#pragma once
#include <abacus/time_series/exporter.hpp>

namespace details
{
struct dummy_exporter : abacus::time_series::exporter
{
    auto receive_data_points(
        std::vector<abacus::time_series::data_point>::iterator
            data_points_start,
        std::vector<abacus::time_series::data_point>::iterator data_points_end)
        -> void override
    {
        std::copy(data_points_start, data_points_end,
                  std::back_inserter(data_points));
    }

    abacus::time_series::data_points data_points;
};
}