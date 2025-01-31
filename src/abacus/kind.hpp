// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "availability.hpp"
#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// Tag type representing a gauge kind.
struct gauge
{
    abacus::availability availability;
};

/// Tag type representing a counter kind.
struct counter
{
    abacus::availability availability;
};

/// Tag type representing a constant kind.
template <class T>
struct constant
{
    T value;
};

// Enable class template argument deduction (CTAD)
template <class T>
constant(T) -> constant<T>;

// Special deduction guide for string literals to deduce std::string
constant(const char*) -> constant<std::string_view>;

}
}
