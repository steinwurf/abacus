
// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "../protobuf/abacus_metric.pb.h"
#include "../view.hpp"

#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
using metrics_message = abacus::protobuf::Metrics;
namespace detail
{
auto to_protobuf(const view& view) -> metrics_message;
}
}
}
