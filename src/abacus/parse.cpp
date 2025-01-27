// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "parse.hpp"

#include <cassert>
#include <map>
#include <vector>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
auto parse::metadata(const uint8_t* metadata_data,
                     std::size_t metadata_bytes) -> protobuf::MetricsMetadata
{
    assert(metadata_data != nullptr);
    assert(metadata_bytes > 0);
    protobuf::MetricsMetadata metadata;
    metadata.ParseFromArray(metadata_data, metadata_bytes);
    return metadata;
}

auto parse::metrics(const uint8_t* metrics_data,
                    std::size_t metrics_bytes) -> protobuf::Metrics
{
    assert(metrics_data != nullptr);
    assert(metrics_bytes > 0);
    protobuf::Metrics metrics;
    metrics.ParseFromArray(metrics_data, metrics_bytes);
    return metrics;
}
}
}
