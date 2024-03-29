

// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "detail/to_protobuf.hpp"
#include "protobuf/metrics.pb.h"
#include "to_protobuf.hpp"
#include "view.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

auto to_protobuf(const view& view) -> metrics_message
{
    return detail::to_protobuf(view);
}

auto to_protobuf(const uint8_t* meta_data, const uint8_t* value_data)
    -> metrics_message
{
    view v;
    v.set_meta_data(meta_data);
    v.set_value_data(value_data);
    return to_protobuf(v);
}
}
}
