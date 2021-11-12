// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "types.hpp"

#include <abacus/view_iterator.hpp>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

namespace abacus
{
namespace telemetry
{
struct data_point
{
    data_point() = default;

    data_point(aggregate_ptr agg, std::size_t metrics_size) :
        aggregate(std::move(agg)), metrics(metrics_size)
    {
    }

    aggregate_ptr aggregate;
    struct metrics_type : std::vector<uint8_t>
    {
        metrics_type() = default;

        explicit metrics_type(const std::size_t& s) : std::vector<uint8_t>(s)
        {
        }

        auto view_iterator() -> view_iterator
        {
            return {data() + sizeof(time_type), size() - sizeof(time_type)};
        }

        auto time() -> const time_type&
        {
            return *(time_type*)data();
        }
    } metrics;
};

using data_points = std::vector<data_point>;

/// @return The maximum size of the names of all available metrics in given view
/// iterator
auto get_max_metric_name_width(const view_iterator& iterator) -> std::size_t
{
    std::size_t max_metric_name_width = 0;

    for (std::size_t i = 0; i < iterator.view_count(); i++)
    {
        auto v = iterator.get_view(i);

        for (std::size_t j = 0; j < v.max_metrics(); j++)
        {
            max_metric_name_width =
                std::max(max_metric_name_width, v.metric_name(j).length());
        }
    }

    return max_metric_name_width;
}

auto json_aggregate_array(std::ostream& os, const aggregate_ptr& agg,
                          const std::string& view_title) -> void
{
    os << "[";
    for (auto& part : *agg)
    {
        os << "\"" << part << "\", ";
    }
    os << "\"" << view_title << "\"]";
}

auto json_data_points(std::ostream& os, data_points::iterator data_points_start,
                      data_points::iterator data_points_end,
                      bool skip_array = false) -> void
{
    if (!skip_array)
    {
        os << "[" << std::endl;
    }

    for (auto itr = data_points_start; itr != data_points_end; itr++)
    {
        auto time = itr->metrics.time();
        auto iterator = itr->metrics.view_iterator();

        auto max_metric_name_width = get_max_metric_name_width(iterator);
        for (std::size_t i = 0; i < iterator.view_count(); i++)
        {
            auto v = iterator.get_view(i);

            os << "  {" << std::endl;
            os << "    \"time\": " << time << "," << std::endl;
            os << "    \"aggregate\": ";
            json_aggregate_array(os, itr->aggregate, v.get_title());
            os << "," << std::endl;

            os << "    \"metrics\": {" << std::endl;
            for (std::size_t j = 0; j < v.max_metrics(); j++)
            {
                os << "      \"" << v.metric_name(j) << std::left
                   << std::setw(max_metric_name_width + 3 -
                                v.metric_name(j).length())
                   << "\": " << v.metric_value(j);

                if (j + 1 == v.max_metrics())
                {
                    os << std::endl;
                }
                else
                {
                    os << "," << std::endl;
                }
            }
            os << "    }" << std::endl;

            if (itr + 1 == data_points_end && i + 1 == iterator.view_count())
            {
                os << "  }" << std::endl;
            }
            else
            {
                os << "  }," << std::endl;
            }
        }
    }

    if (!skip_array)
    {
        os << "]" << std::endl;
    }
}
}
}