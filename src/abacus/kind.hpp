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
enum class kind
{
    gauge = protobuf::Kind::GAUGE,
    counter = protobuf::Kind::COUNTER
};
}
}
