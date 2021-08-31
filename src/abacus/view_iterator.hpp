// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <vector>

#include "version.hpp"
#include "view.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
class view_iterator
{
    /// This class takes a data buffer containing information from one or more
    /// metrics objects and separates them into views.

    /// See view.hpp for more about views.
public:
    static constexpr std::size_t header_size = 5;

public:
    /// Default constructor
    view_iterator(const uint8_t* data, std::size_t size);

    /// @return A view of the index metrics object in memory
    auto get_view(std::size_t index) const -> view;

    /// @return The number of views/metrics in memory
    auto view_count() const -> std::size_t;

private:
    std::vector<view> m_views = {};
};
}
}