// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "parse_metadata.hpp"

#include <cassert>
#include <optional>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
auto parse_metadata(const uint8_t* metadata_data, std::size_t metadata_bytes)
    -> std::optional<protobuf::MetricsMetadata>
{
    assert(metadata_data != nullptr);
    assert(metadata_bytes > 0);
    protobuf::MetricsMetadata metadata;
    auto result = metadata.ParseFromArray(metadata_data, metadata_bytes);
    if (!result)
    {
        return std::nullopt;
    }
    return metadata;
}
}
}
