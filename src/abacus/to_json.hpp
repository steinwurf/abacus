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
/// @param metadata_data The metadata data of the metrics.
/// @param metadata_bytes The size of the metadata data.
/// @param value_data The value data of the metrics.
/// @param value_bytes The size of the value data.
/// @param minimal If true, the JSON will be slimmed down to only contain the
///        the value data.
auto to_json(const uint8_t* metadata_data, std::size_t metadata_bytes,
             const uint8_t* value_data, std::size_t value_bytes,
             bool minimal = false) -> std::string;

/// @return a JSON-formatted string of a metrics views data.
/// @param metadata The metadata of the metrics.
/// @param value_data The value data of the metrics.
/// @param value_bytes The size of the value data.
/// @param minimal If true, the JSON will be slimmed down to only contain the
///        the value data.
auto to_json(const protobuf::MetricsMetadata& metadata,
             const uint8_t* value_data, std::size_t value_bytes,
             bool minimal = false) -> std::string;

/// @return a JSON-formatted string of a single views data.
/// @param view A view with access to metrics-data.
/// @param minimal If true, the JSON will be slimmed down to only contain the
///        the value data.
auto to_json(const view& view, bool minimal = false) -> std::string;
}
}
