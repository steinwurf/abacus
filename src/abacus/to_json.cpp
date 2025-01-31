// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bourne/json.hpp>

#include "detail/to_json.hpp"
#include "parse.hpp"
#include "protobuf/metrics.pb.h"
#include "to_json.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

auto to_json(const uint8_t* metadata_data, std::size_t metadata_bytes,
             const uint8_t* value_data, std::size_t value_bytes,
             bool minimal) -> std::string
{
    view v;
    auto parsed = parse::metadata(metadata_data, metadata_bytes);
    if (!parsed.has_value())
    {
        return "";
    }

    if (v.set_metadata(parsed.value()))
    {
        if (v.set_value_data(value_data, value_bytes))
        {
            return to_json(v, minimal);
        }
    }
    return "";
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

auto to_json(const view& view, bool minimal) -> std::string
{
    bourne::json json = detail::to_json(view, minimal);
    return json.dump();
}

}
}
