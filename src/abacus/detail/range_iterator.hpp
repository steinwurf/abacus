// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <ostream>

#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
template <class Value, class Increment>
struct range_iterator
{
    /// @param other Another index_range object
    /// @return True of the index_range objects match
    bool operator!=(const range_iterator& other) const
    {
        return m_i != other.m_i;
    }

    /// @todo return symbol_index
    /// @return The current value of the index range
    Value operator*() const
    {
        return m_i;
    }

    /// @return Increment the index range and return it.
    range_iterator& operator++()
    {
        if (Increment::value)
        {
            ++m_i;
        }
        else
        {
            --m_i;
        }

        return *this;
    }

    /// The current value of the index range
    Value m_i;
};

}
}
}