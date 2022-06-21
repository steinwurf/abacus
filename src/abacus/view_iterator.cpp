// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "view_iterator.hpp"

#include "detail/raw.hpp"

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
        m_views.emplace_back();
        m_views.back().set_data(data + offset);
        offset += m_views.back().view_bytes();
    }

    assert(size == offset);
}

auto view_iterator::get_view(std::size_t index) const -> const view&
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
