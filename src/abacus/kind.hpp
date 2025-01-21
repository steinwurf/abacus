// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "protobuf/kind.pb.h"
#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
struct gauge_
{
    static constexpr protobuf::Kind value = protobuf::Kind::GAUGE;
};
static const gauge_ gauge;

struct counter_
{
    static constexpr protobuf::Kind value = protobuf::Kind::COUNTER;
};
static const counter_ counter;

struct constant_
{
    static constexpr protobuf::Kind value = protobuf::Kind::CONSTANT;
};
static const constant_ constant;

using kind = std::variant<gauge_, counter_, constant_>;

static inline auto to_protobuf(const kind& k) -> protobuf::Kind
{
    if (std::holds_alternative<gauge_>(k))
    {
        return protobuf::Kind::GAUGE;
    }
    else if (std::holds_alternative<counter_>(k))
    {
        return protobuf::Kind::COUNTER;
    }
    else if (std::holds_alternative<constant_>(k))
    {
        return protobuf::Kind::CONSTANT;
    }
    else
    {
        throw std::runtime_error("Unknown kind");
    }
}
}
}
