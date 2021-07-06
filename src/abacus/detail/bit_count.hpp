// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <type_traits>

#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
/// Strong value representing a bit count
struct bit_count
{
    /// Default constructor
    bit_count()
    {
    }

    /// Create a bit count from a value
    explicit bit_count(std::size_t bit_count) : count(bit_count)
    {
    }

    /// The value
    std::size_t count = 0;
};

/// bit_count operators
inline auto operator<(const bit_count& lhs, const bit_count& rhs) -> bool
{
    return lhs.count < rhs.count;
}

inline auto operator>(const bit_count& lhs, const bit_count& rhs) -> bool
{
    return lhs.count > rhs.count;
}

inline auto operator++(bit_count& bits) -> bit_count&
{
    ++bits.count;
    return bits;
}

inline auto operator--(bit_count& bits) -> bit_count&
{
    --bits.count;
    return bits;
}

inline auto operator==(const bit_count& lhs, const bit_count& rhs) -> bool
{
    return lhs.count == rhs.count;
}

inline auto operator!=(const bit_count& lhs, const bit_count& rhs) -> bool
{
    return lhs.count != rhs.count;
}

}
}
}
