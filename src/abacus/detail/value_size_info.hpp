// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <vector>

#include "../metric_info.hpp"
#include "../metric_type.hpp"
#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{

struct value_size_info
{
    std::vector<metric_info> m_eight_byte_metrics;
    std::vector<metric_info> m_one_byte_metrics;

    value_size_info(const metric_info* infos, std::size_t size)
    {
        assert(infos != nullptr);

        for (std::size_t i = 0; i < size; i++)
        {
            const auto& info = infos[i];
            switch (info.type)
            {
            case metric_type::boolean:
                m_one_byte_metrics.push_back(info);
                break;
            case metric_type::float64:
            case metric_type::int64:
            case metric_type::uint64:
                m_eight_byte_metrics.push_back(info);
                break;
            default:
                assert(false);
                break;
            }
        }
    }

    std::size_t eight_byte_metrics_count() const
    {
        return m_eight_byte_metrics.size();
    }

    std::size_t one_byte_metrics_count() const
    {
        return m_one_byte_metrics.size();
    }

    std::size_t size() const
    {
        return m_eight_byte_metrics.size() + m_one_byte_metrics.size();
    }

    metric_info operator[](std::size_t index) const
    {
        assert(index < size());

        if (index < m_eight_byte_metrics.size())
        {
            return m_eight_byte_metrics[index];
        }
        else
        {
            return m_one_byte_metrics[index - m_eight_byte_metrics.size()];
        }
    }
};
}
}
}
