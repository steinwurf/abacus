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
auto to_json(const view& view) -> std::string
{
    bourne::json json = detail::to_json(view);
    return json.dump();
}

auto to_json(const uint8_t* metrics_data) -> std::string
{
    view v;
    v.set_data(metrics_data);
    return to_json(v);
}
}
}
