// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <bourne/json.hpp>

#include "../view.hpp"

#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
auto to_json(const view& view) -> bourne::json
{
    auto json = bourne::json::object();
    uint64_t uint_value = 0U;
    int64_t int_value = 0;
    bool bool_value = false;
    double float_value = 0.0;

    for (std::size_t i = 0; i < view.metric_count(); ++i)
    {
        if ((!view.is_metric_initialized(i)))
        {
            continue;
        }

        auto n = view.metric_name(i);
        auto d = view.metric_description(i);
        auto t = view.metric_type(i);
        auto c = view.is_metric_constant(i);
        bourne::json metric_data = {"description", d, "is_constant", c};
        switch (t)
        {
        case metric_type::uint64:
            view.metric_value(i, uint_value);
            metric_data["value"] = uint_value;
            break;
        case metric_type::int64:
            view.metric_value(i, int_value);
            metric_data["value"] = int_value;
            break;
        case metric_type::boolean:
            view.metric_value(i, bool_value);
            metric_data["value"] = bool_value;
            break;
        case metric_type::float64:
            view.metric_value(i, float_value);
            metric_data["value"] = float_value;
            break;
        case metric_type::uninitialized:
        default:
            assert(false);
            break;
        }

        json[n] = metric_data;
    }
    return json;
}

}
}
}
