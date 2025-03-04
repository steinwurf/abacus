// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "protobuf/metrics.pb.h"
#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// The kind of metric
enum class kind
{
    /// A gauge metric. This is a metric can can be both incremented and
    /// decremented.
    gauge = abacus::protobuf::GAUGE,
    /// A counter metric. This is a metric that can only be incremented.
    counter = abacus::protobuf::COUNTER
};

}
}
