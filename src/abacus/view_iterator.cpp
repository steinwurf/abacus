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

auto view_iterator::to_json(bool prettier) const -> std::string
{
    std::string space = prettier ? " " : "";
    std::string newline = prettier ? "\n" : "";
    std::string tab = prettier ? "\t" : "";

    std::string counters_json = "{" + newline;

    for (std::size_t i = 0; i < m_views.size(); ++i)
    {
        view view = m_views[i];
        for (std::size_t i = 0; i < view.metrics_count(); ++i)
        {
            auto n = view.metric_name(i);
            auto v = view.metric_value(i);

            counters_json +=
                tab + "\"" + std::string(n) + "\":" + space + std::to_string(v);
            if (i != (view.metrics_count() - 1U))
            {
                counters_json += "," + newline;
            }
        }
        counters_json += ",";
    }
    counters_json += newline + "}";

    return counters_json;
}

}
}
