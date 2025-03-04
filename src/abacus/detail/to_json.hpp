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
/// @param view A view with access to metrics-data.
/// @param minimal If true, the JSON will a simple map between metric names and
/// values.
/// @return a JSON-formatted string of a metrics views data.
auto to_json(const view& view, bool minimal) -> bourne::json;
}
}
}
