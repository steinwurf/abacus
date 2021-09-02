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
/// This class takes a pointer containing information from one or more
/// metrics objects and separates them into views.
///
/// See view.hpp for more about views.
class view_iterator
{
    /// view_count().
    /// @return A view of the index metrics object in memory
    auto get_view(std::size_t index) const -> view;

    /// @return The number of views/metrics in memory
    auto view_count() const -> std::size_t;

private:
    /// Vector containing the different views
    std::vector<view> m_views = {};
};
}
}