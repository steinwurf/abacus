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
struct parse
{
private:
    /// Private constructor to prevent instantiation of this class
    parse() = delete;

public:
    /// @param metadata_data The meta data pointer
    /// @param metadata_bytes The meta data size in bytes
    static auto
    metadata(const uint8_t* metadata_data,
             std::size_t metadata_bytes) -> protobuf::MetricsMetadata;

    /// @param metrics_data The metrics pointer
    /// @param metrics_bytes The metrics size in bytes
    static auto metrics(const uint8_t* metrics_data,
                        std::size_t metrics_bytes) -> protobuf::Metrics;
};
}
}
