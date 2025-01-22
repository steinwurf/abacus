// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <variant>

#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// A tag type representing a required availability.
struct required_
{
};

/// A constant instance of the required_ tag type.
static const required_ required;

/// A tag type representing an optional availability.
struct optional_
{
};

/// A constant instance of the optional_ tag type.
static const optional_ optional;

/// A variant type that can hold either a required_ or an optional_.
using availability = std::variant<required_, optional_>;

/// Check if the given availability is optional.
/// @param a The availability to check.
/// @return true if the availability is optional, false otherwise.
static inline bool is_optional(const availability& a)
{
    return std::holds_alternative<optional_>(a);
}

/// Check if the given availability is required.
/// @param a The availability to check.
/// @return true if the availability is required, false otherwise.
static inline bool is_required(const availability& a)
{
    return std::holds_alternative<required_>(a);
}
}
}
