
// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <vector>

#include "protobuf/abacus_metric.pb.h"
#include "version.hpp"
#include "view.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

using metrics_message = protobuf::Metrics;
/// @return a protobuf of a metrics views data.
/// @param meta_data The meta data of the metrics view.
/// @param value_data The value data of the metrics view.
auto to_protobuf(const uint8_t* meta_data, const uint8_t* value_data)
    -> metrics_message;

/// @return a protobuf of single views data.
/// @param view A view with access to metrics-data.
auto to_protobuf(const view& view) -> metrics_message;
}
}
