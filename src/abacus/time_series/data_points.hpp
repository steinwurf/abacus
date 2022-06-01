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
namespace time_series
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

namespace details
{
/// @return The maximum size of the names of all available metrics in given view
/// iterator
inline auto get_max_metric_name_width(const view_iterator& iterator)
    -> std::size_t
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

/// Appends given aggregate as a JSON array to given ostream
template <bool compact = false>
inline auto json_aggregate_array(std::ostream& os, const aggregate_ptr& agg,
                                 const std::string& view_title) -> void
{
    constexpr auto json_spac = compact ? "" : " ";

    os << "[";
    for (auto& part : *agg)
    {
        os << "\"" << part << "\"," << json_spac;
    }
    os << "\"" << view_title << "\"]";
}
}

/// Appends data points as JSON to given ostream
template <bool compact = false>
inline auto json_data_points(std::ostream& os,
                             data_points::iterator data_points_start,
                             data_points::iterator data_points_end,
                             bool skip_array = false) -> void
{
    constexpr auto json_tab1 = compact ? "" : "   ";
    constexpr auto json_tab2 = compact ? "" : "     ";
    constexpr auto json_tab3 = compact ? "" : "       ";
    constexpr auto json_newl = compact ? "" : "\n";
    constexpr auto json_spac = compact ? "" : " ";

    using namespace details;

    if (!skip_array)
    {
        os << "[" << json_newl;
    }

    for (auto itr = data_points_start; itr != data_points_end; itr++)
    {
        auto time = itr->metrics.time();
        auto iterator = itr->metrics.view_iterator();

        auto max_metric_name_width =
            compact ? 0 : get_max_metric_name_width(iterator);
        for (std::size_t i = 0; i < iterator.view_count(); i++)
        {
            auto v = iterator.get_view(i);
            os << json_tab1 << "{" << json_newl;
            os << json_tab2 << "\"time\":" << json_spac << time << ","
               << json_newl;
            os << json_tab2 << "\"aggregate\":" << json_spac;
            json_aggregate_array<compact>(os, itr->aggregate, v.get_title());
            os << "," << json_newl;

            os << json_tab2 << "\"metrics\":" << json_spac << "{" << json_newl;
            for (std::size_t j = 0; j < v.max_metrics(); j++)
            {
                os << json_tab3 << "\"" << v.metric_name(j);
                if (!compact)
                {
                    os << std::left
                       << std::setw(max_metric_name_width + 3 -
                                    v.metric_name(j).length());
                }

                os << "\":" << json_spac << v.metric_value(j);

                if (j + 1 == v.max_metrics())
                {
                    os << json_newl;
                }
                else
                {
                    os << "," << json_newl;
                }
            }
            os << json_tab2 << "}" << json_newl;

            if (itr + 1 == data_points_end && i + 1 == iterator.view_count())
            {
                os << json_tab1 << "}" << json_newl;
            }
            else
            {
                os << json_tab1 << "}," << json_newl;
            }
        }
    }

    if (!skip_array)
    {
        os << "]" << json_newl;
    }
}
}
}