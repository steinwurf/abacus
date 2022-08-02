// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <vector>

#include "version.hpp"
#include "view.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// @return a JSON-formatted string of a metrics views data.
/// @param metrics_data the data of the metrics views to be serialized to JSON.
auto to_json(const uint8_t* metrics_data) -> std::string;

/// @return a JSON-formatted string of a single views data.
/// @param view A view with access to metrics-data.
auto to_json(const view& view) -> std::string;
}
}
