// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bourne/json.hpp>

#include "detail/to_json.hpp"
#include "protobuf/metrics.pb.h"
#include "to_json.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

auto to_json(const protobuf::Metrics& metrics, bool minimal) -> std::string
{
    const auto& values = metrics.values();
    return to_json(metrics.metadata(), (const uint8_t*)values.data(),
                   values.size(), minimal);
}

auto to_json(const view& view, bool minimal) -> std::string
{
    bourne::json json = detail::to_json(view, minimal);
    return json.dump();
}

auto to_json(const protobuf::MetricsMetadata& metadata,
             const uint8_t* value_data, std::size_t value_bytes,
             bool minimal) -> std::string
{
    view v;
    if (v.set_metadata(metadata))
    {
        if (v.set_value_data(value_data, value_bytes))
        {
            return to_json(v, minimal);
        }
    }
    return "";
}
}
}
