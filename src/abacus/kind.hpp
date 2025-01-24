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
/// Tag type representing a gauge kind.
struct gauge_
{
    /// The protobuf kind value for the gauge kind.
    static constexpr protobuf::Kind value = protobuf::Kind::GAUGE;
};

/// Constant instance of the gauge_ tag type.
static const gauge_ gauge;

/// Tag type representing a counter kind.
struct counter_
{
    /// The protobuf kind value for the counter kind.
    static constexpr protobuf::Kind value = protobuf::Kind::COUNTER;
};

/// Constant instance of the counter_ tag type.
static const counter_ counter;

/// Tag type representing a constant kind.
struct constant_
{
    /// The protobuf kind value for the constant kind.
    static constexpr protobuf::Kind value = protobuf::Kind::CONSTANT;
};

/// Constant instance of the constant_ tag type.
static const constant_ constant;

/// Variant type that can hold either a gauge_, counter_, or constant_.
using kind = std::variant<gauge_, counter_, constant_>;

/// Converts the given kind to its corresponding protobuf::Kind value.
/// @param k The kind to convert.
/// @return The corresponding protobuf::Kind value.
/// @throws std::runtime_error if the kind is unknown.
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
