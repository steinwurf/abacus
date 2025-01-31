// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "description.hpp"
#include "unit.hpp"
#include "version.hpp"

#include "boolean.hpp"
#include "enum8.hpp"
#include "float32.hpp"
#include "float64.hpp"
#include "int32.hpp"
#include "int64.hpp"
#include "uint32.hpp"
#include "uint64.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

template <class T>
struct value
{
    // The value of the constant
    T value;
};

// Enable class template argument deduction (CTAD)
template <class T>
value(T) -> value<T>;

// Special deduction guide for string literals to deduce std::string
value(const char*) -> value<std::string_view>;

/// Tag type representing a constant kind.
struct constant
{
    // The value of the constant
    std::variant<abacus::value<uint64::type>, abacus::value<uint32::type>,
                 abacus::value<int64::type>, abacus::value<int32::type>,
                 abacus::value<float64::type>, abacus::value<float32::type>,
                 abacus::value<boolean::type>, abacus::value<enum8::type>,
                 abacus::value<std::string_view>>
        value;
    // The description of the constant
    abacus::description description;
    // The unit of the constant
    abacus::unit unit{};
};
}
}
