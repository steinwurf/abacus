// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "to_json.hpp"

#include <bourne/json.hpp>

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

    json["protocol_version"] = view.protocol_version();

    for (std::size_t i = 0; i < view.count(); ++i)
    {
        if ((!view.is_initialized(i)))
        {
            continue;
        }

        auto n = view.name(i);
        auto d = view.description(i);
        auto t = view.type(i);
        auto c = view.is_constant(i);
        bourne::json metric_data = {"description", d, "is_constant", c};
        switch (t)
        {
        case metric_type::uint64:
            view.value(i, uint_value);
            metric_data["value"] = uint_value;
            break;
        case metric_type::int64:
            view.value(i, int_value);
            metric_data["value"] = int_value;
            break;
        case metric_type::boolean:
            view.value(i, bool_value);
            metric_data["value"] = bool_value;
            break;
        case metric_type::float64:
            view.value(i, float_value);
            metric_data["value"] = float_value;
            break;
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
