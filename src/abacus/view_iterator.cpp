// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "view_iterator.hpp"

#include <algorithm>
#include <cstring>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

view_iterator::view_iterator(const uint8_t* data, std::size_t size)
{
    assert(data != nullptr);
    assert(size > 0);

    std::size_t offset = 0;

    while (offset < size)
    {
        view v;
        v.set_data(data + offset);

        m_views.push_back(v);

        offset += v.view_bytes();
    }

    assert(size == offset);
}

auto view_iterator::get_view(std::size_t index) const -> view
{
    assert(index < m_views.size());
    return m_views[index];
}

auto view_iterator::view_count() const -> std::size_t
{
    return m_views.size();
}

}
}