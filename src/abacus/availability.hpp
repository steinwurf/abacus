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

struct required_
{
};
static const required_ required;

struct optional_
{
};
static const optional_ optional;

using availability = std::variant<required_, optional_>;

static inline bool is_optional(const availability& a)
{
    return std::holds_alternative<optional_>(a);
}

static inline bool is_required(const availability& a)
{
    return std::holds_alternative<required_>(a);
}
}
}
