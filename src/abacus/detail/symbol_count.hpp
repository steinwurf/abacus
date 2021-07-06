// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <ostream>

#include <bourne/json.hpp>

#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
/// Strong integer representing a symbol count i.e. a number of symbols
struct symbol_count
{
    /// Default constructor
    symbol_count()
    {
    }

    /// Create a symbol index from a value
    explicit symbol_count(std::size_t symbol_count) : count(symbol_count)
    {
    }

    /// The value
    std::size_t count = 0;
};

inline auto operator+(const symbol_count& lhs, const symbol_count& rhs)
    -> symbol_count
{
    return symbol_count{lhs.count + rhs.count};
}

inline auto operator-(const symbol_count& lhs, const symbol_count& rhs)
    -> symbol_count
{
    assert(lhs.count >= rhs.count);
    return symbol_count{lhs.count - rhs.count};
}

inline auto operator<(const symbol_count& lhs, const symbol_count& rhs) -> bool
{
    return lhs.count < rhs.count;
}

inline auto operator<=(const symbol_count& lhs, const symbol_count& rhs) -> bool
{
    return lhs.count <= rhs.count;
}

inline auto operator>(const symbol_count& lhs, const symbol_count& rhs) -> bool
{
    return lhs.count > rhs.count;
}

inline auto operator>=(const symbol_count& lhs, const symbol_count& rhs) -> bool
{
    return lhs.count >= rhs.count;
}

inline auto operator!=(const symbol_count& lhs, const symbol_count& rhs) -> bool
{
    return lhs.count != rhs.count;
}

inline auto operator==(const symbol_count& lhs, const symbol_count& rhs) -> bool
{
    return lhs.count == rhs.count;
}

inline auto operator++(symbol_count& symbols) -> symbol_count&
{
    ++symbols.count;
    return symbols;
}

inline auto operator--(symbol_count& symbols) -> symbol_count&
{
    --symbols.count;
    return symbols;
}

inline std::ostream& operator<<(std::ostream& out, const symbol_count& symbols)
{
    return out << bourne::json({"symbol_count", symbols.count}).dump();
}
}
}
}
