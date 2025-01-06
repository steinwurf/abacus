// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <optional>

#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

template <typename T>
struct min2
{
    min2() = default;

    explicit min2(T value) : value(value)
    {
    }

    std::optional<T> value;
};

union min
{
    min(uint64_t value) : m_uint(value)
    {
    }

    min(double value) : m_double(value)
    {
    }

    min(int64_t value) : m_int(value)
    {
    }

    min() : m_uint(0)
    {
    }

    uint64_t m_uint;
    double m_double;
    int64_t m_int;
};
}
}
