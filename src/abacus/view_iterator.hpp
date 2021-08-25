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
public:
    static constexpr std::size_t header_size = 9;

public:
    /// Default constructor
    view_iterator(uint8_t* data, std::size_t size);

    ~view_iterator();

    auto get_view(std::size_t index) const -> view;

private:
    uint8_t* m_data = nullptr;

    std::size_t m_data_size = 0;

    std::size_t m_view_count = 0;

    std::vector<view> m_views = {};
};
}
}