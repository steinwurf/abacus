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
    std::size_t m_metric_count = 0;
    std::size_t m_one_byte_count = 0;
    std::size_t m_eight_byte_count = 0;
    std::vector<metric_info> m_eight_byte_metrics;
    std::vector<metric_info> m_one_byte_metrics;

    value_size_info(metric_info* info, std::size_t size) : m_metric_count(size)
    {
        assert(info != nullptr);
        assert(m_metric_count > 0);

        for (std::size_t i = 0; i < m_metric_count; i++)
        {
            switch (info[i].type)
            {
            case metric_type::boolean:
                m_one_byte_metrics.push_back(info[i]);
                m_one_byte_count++;
                break;
            default:
                m_eight_byte_metrics.push_back(info[i]);
                m_eight_byte_count++;
                break;
            }
        }
    }

    std::size_t size() const
    {
        return m_metric_count;
    }

    metric_info operator[](std::size_t index) const
    {
        assert(index < size());

        if (index < m_eight_byte_count)
        {
            return m_eight_byte_metrics[index];
        }
        else
        {
            return m_one_byte_metrics[index - m_eight_byte_count];
        }
    }
};
}
}
}
