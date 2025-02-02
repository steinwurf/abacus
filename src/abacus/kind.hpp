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
/// A tag type representing a gauge kind.
struct gauge_
{
    static constexpr abacus::protobuf::Kind kind = abacus::protobuf::GAUGE;
};

/// A constant instance of the gauge_ tag type.
static const gauge_ gauge;

/// A tag type representing an counter kind.
struct counter_
{
    static constexpr abacus::protobuf::Kind kind = abacus::protobuf::COUNTER;
};

/// A constant instance of the counter_ tag type.
static const counter_ counter;

/// A variant type that can hold either a gauge_ or an counter_.
using kind = std::variant<gauge_, counter_>;

/// Check if the given kind is counter.
/// @param k The kind to check.
/// @return true if the kind is counter, false otherwise.
static inline bool is_counter(const kind& k)
{
    return std::holds_alternative<counter_>(k);
}

/// Check if the given kind is gauge.
/// @param a The kind to check.
/// @return true if the kind is gauge, false otherwise.
static inline bool is_gauge(const kind& k)
{
    return std::holds_alternative<gauge_>(k);
}

}
}
