// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bourne/json.hpp>

#include "to_json.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

void add_view_data_to_json(const view& single_view, bourne::json& json_data)
{
    uint64_t uint_value = 0U;
    int64_t int_value = 0;
    bool bool_value = false;
    double float_value = 0.0;

    auto metric_count = single_view.metric_count();
    assert(metric_count > 0U);

    std::string scope = single_view.scope();

    for (std::size_t i = 0; i < metric_count; ++i)
    {
        if ((!single_view.is_metric_initialized(i)))
        {
            continue;
        }

        auto n = single_view.metric_name(i);
        auto d = single_view.metric_description(i);
        auto t = single_view.get_metric_type(i);
        auto c = single_view.metric_is_constant(i);
        bourne::json metric_data = {"description", d, "is_constant", c};
        switch (t)
        {
        case metric_type::uint64:
            single_view.metric_value(i, uint_value);
            metric_data["value"] = uint_value;
            break;
        case metric_type::int64:
            single_view.metric_value(i, int_value);
            metric_data["value"] = int_value;
            break;
        case metric_type::boolean:
            single_view.metric_value(i, bool_value);
            metric_data["value"] = bool_value;
            break;
        case metric_type::float64:
            single_view.metric_value(i, float_value);
            metric_data["value"] = float_value;
            break;
        }
        std::string scoped_name = scope + "." + n;
        json_data[scoped_name] = metric_data;
    }
}

auto to_json(const view& single_view) -> std::string
{
    bourne::json json_data = bourne::json::object();

    add_view_data_to_json(single_view, json_data);

    return json_data.dump();
}

auto to_json(const std::vector<view>& views) -> std::string
{
    bourne::json json_data = bourne::json::object();

    for (auto v : views)
    {
        add_view_data_to_json(v, json_data);
    }

    return json_data.dump();
}
}
}
