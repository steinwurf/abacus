// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>

#include "../version.hpp"

#include "bit_count.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
/// Strong value representing a bit index
struct bit_index
{

    /// Default constructor
    bit_index()
    {
    }

    /// Create a bit count from a value
    explicit bit_index(std::size_t bit_index) : index(bit_index)
    {
    }

    /// The value
    std::size_t index = 0;
};

/// bit_index operators
inline auto operator-(const bit_index& lhs, const bit_index& rhs) -> bit_count
{
    assert(lhs.index >= rhs.index);
    return bit_count{lhs.index - rhs.index};
}

inline auto operator+(const bit_index& lhs, const bit_count& rhs) -> bit_index
{
    return bit_index{lhs.index + rhs.count};
}

inline auto operator--(bit_index& lhs) -> bit_index&
{
    --lhs.index;
    return lhs;
}

inline auto operator++(bit_index& lhs) -> bit_index&
{
    ++lhs.index;
    return lhs;
}

inline auto operator==(const bit_index& lhs, const bit_index& rhs) -> bool
{
    return lhs.index == rhs.index;
}

inline auto operator!=(const bit_index& lhs, const bit_index& rhs) -> bool
{
    return lhs.index != rhs.index;
}

inline auto operator<(const bit_index& lhs, const bit_index& rhs) -> bool
{
    return lhs.index < rhs.index;
}

}
}
}
