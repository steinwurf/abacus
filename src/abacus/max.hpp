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
struct max2
{
    max2() = default;

    explicit max2(T value) : value(value)
    {
    }

    std::optional<T> value;
};
union max
{

    max(uint64_t value) : m_uint(value)
    {
    }

    max(double value) : m_double(value)
    {
    }

    max(int64_t value) : m_int(value)
    {
    }

    max() : m_uint(0)
    {
    }

    uint64_t m_uint;
    double m_double;
    int64_t m_int;
};
}
}
