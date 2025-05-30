// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>

#include "protobuf/metrics.pb.h"
#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

/// @param metadata_data The meta data pointer
/// @param metadata_bytes The meta data size in bytes
auto parse_metadata(const uint8_t* metadata_data, std::size_t metadata_bytes)
    -> std::optional<protobuf::MetricsMetadata>;

}
}
