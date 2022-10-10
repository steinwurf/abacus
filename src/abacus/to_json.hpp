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
/// @param meta_data The meta data of the metrics view.
/// @param value_data The value data of the metrics view.
auto to_json(const uint8_t* meta_data, const uint8_t* value_data)
    -> std::string;

/// @return a JSON-formatted string of a single views data.
/// @param view A view with access to metrics-data.
auto to_json(const view& view) -> std::string;
}
}
