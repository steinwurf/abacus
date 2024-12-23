// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <functional>
#include <map>
#include <string>
#include <variant>

#include "detail/hash_function.hpp"
#include "metric.hpp"
#include "metric_info2.hpp"
#include "protocol_version.hpp"
#include "type.hpp"
#include "version.hpp"

#include "protobuf/metrics.pb.h"

#include <endian/big_endian.hpp>
#include <endian/is_big_endian.hpp>
#include <endian/little_endian.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

template <typename T>
static inline auto get_value(const uint8_t* data, const std::string& name)
    -> std::optional<typename T::type>
{
}

template <typename T>
static inline auto
get_value(const protobuf::MetricsMetadata& meta, const uint8_t* value_data,
          const std::string& name) -> std::optional<typename T::type>
{
    auto endian = meta.endianness();
    uint32_t sync_bytes;
    if (endian == protobuf::Endianness::BIG)
    {
        sync_bytes = endian::big_endian::get<uint32_t>(value_data);
    }
    else
    {
        sync_bytes = endian::little_endian::get<uint32_t>(value_data);
    }

    // Check the sync bytes
    if (meta.sync() != sync_bytes)
    {
        return std::nullopt;
    }

    const protobuf::Metric& proto_metric = meta.metrics().at(name);
    auto offset = proto_metric.offset();

    typename T::metric m(value_data + offset);
    if (!m.has_value())
    {
        return std::nullopt;
    }
    return m.value();
}
}
}
