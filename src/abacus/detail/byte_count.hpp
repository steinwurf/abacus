// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <limits>
#include <ostream>

#include <bourne/json.hpp>

#include "../version.hpp"

#include "symbol_count.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
/// Strong value representing a byte count
struct byte_count
{
    /// Default constructor
    byte_count()
    {
    }

    /// Create a byte count from a value
    explicit byte_count(std::size_t byte_count) : count(byte_count)
    {
    }

    /// The value
    std::size_t count = 0;
};

/// byte_count operators
inline auto operator+(const byte_count& lhs, const byte_count& rhs)
    -> byte_count
{
    return byte_count{lhs.count + rhs.count};
}

inline auto operator*(const byte_count& lhs, const symbol_count& rhs)
    -> byte_count
{
    return byte_count{lhs.count * rhs.count};
}

inline auto operator*(const symbol_count& lhs, const byte_count& rhs)
    -> byte_count
{
    return byte_count{lhs.count * rhs.count};
}

inline auto operator-(const byte_count& lhs, const byte_count& rhs)
    -> byte_count
{
    assert(lhs.count >= rhs.count);
    return byte_count{lhs.count - rhs.count};
}

inline auto operator+=(byte_count& lhs, const byte_count& rhs) -> byte_count&
{
    lhs.count += rhs.count;
    return lhs;
}

inline auto operator*=(byte_count& lhs, const symbol_count& rhs) -> byte_count&
{
    lhs.count *= rhs.count;
    return lhs;
}

inline auto operator-=(byte_count& lhs, const byte_count& rhs) -> byte_count&
{
    assert(lhs.count >= rhs.count);
    lhs.count -= rhs.count;
    return lhs;
}

inline auto operator>(const byte_count& lhs, const byte_count& rhs) -> bool
{
    return lhs.count > rhs.count;
}

inline auto operator>=(const byte_count& lhs, const byte_count& rhs) -> bool
{
    return lhs.count >= rhs.count;
}

inline auto operator==(const byte_count& lhs, const byte_count& rhs) -> bool
{
    return lhs.count == rhs.count;
}

inline auto operator<=(const byte_count& lhs, const byte_count& rhs) -> bool
{
    return lhs.count <= rhs.count;
}

inline auto operator<(const byte_count& lhs, const byte_count& rhs) -> bool
{
    return lhs.count < rhs.count;
}

inline auto operator<<(std::ostream& out, const byte_count& bytes)
    -> std::ostream&
{
    return out << bourne::json({"byte_count", bytes.count}).dump();
}

}
}
}
