// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bourne/json.hpp>

#include "detail/to_json.hpp"
#include "to_json.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
auto to_json(const view2& view, bool minimal) -> std::string
{
    bourne::json json = detail::to_json(view, minimal);
    return json.dump();
}

auto to_json(const uint8_t* metadata_data, std::size_t metadata_bytes,
             const uint8_t* value_data, std::size_t value_bytes,
             bool minimal) -> std::string
{
    view2 v;
    v.set_meta_data(metadata_data, metadata_bytes);
    v.set_value_data(value_data, value_bytes);
    return to_json(v, minimal);
}
}
}
