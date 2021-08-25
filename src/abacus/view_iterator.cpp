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

view_iterator::view_iterator(uint8_t* data, std::size_t size) :
    m_data(data), m_data_size(size)
{
    assert(data != nullptr);
    assert(size > 0);

    std::vector<uint16_t> max_name_bytes_data = {};
    std::vector<uint16_t> max_metrics_data = {};

    std::vector<std::size_t> view_sizes = {};
    std::size_t current_view_start = 0;

    std::vector<view> views = {};

    std::size_t i = 0;

    while (current_view_start < m_data_size)
    {
        assert(m_data_size - current_view_start > header_size);

        max_name_bytes_data.push_back(*(m_data + 3 + current_view_start));
        max_metrics_data.push_back(*(m_data + 5 + current_view_start));

        view_sizes.push_back(header_size + max_name_bytes_data[i] +
                             max_metrics_data[i] *
                                 (max_name_bytes_data[i] + sizeof(uint64_t)));

        std::vector<uint8_t> view_data(view_sizes[i]);

        std::memcpy(view_data.data(), m_data + current_view_start,
                    view_sizes[i]);

        view this_view;
        this_view.set_data(view_data.data());

        views.push_back(this_view);
    }

    m_view_count = views.size();
    m_views = views;
}

view_iterator::~view_iterator()
{
    ::operator delete(m_data);
}

auto view_iterator::get_view(std::size_t index) const -> view
{
    return m_views[index];
}

}
}