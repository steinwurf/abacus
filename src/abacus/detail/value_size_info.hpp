// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <vector>

#include "../metric_info.hpp"
#include "../value_type.hpp"
#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{

struct value_size_info
{
    metric_info* m_eight_byte_metrics = nullptr;
    metric_info* m_one_byte_metrics = nullptr;
    std::size_t m_metric_count = 0;
    std::size_t m_one_byte_count = 0;
    std::size_t m_eight_byte_count = 0;

    value_size_info(metric_info* info, std::size_t size) : m_metric_count(size)
    {
        assert(info != nullptr);
        assert(m_metric_count > 0);

        // Get the indices of bool metrics and 8-byte type metrics.
        std::size_t one_byte_indices[m_metric_count];
        std::size_t eight_byte_indices[m_metric_count];

        for (std::size_t i = 0; i < m_metric_count; i++)
        {
            switch (info[i].type)
            {
            case value_type::boolean:
                one_byte_indices[m_one_byte_count] = i;
                m_one_byte_count++;
                break;
            default:
                eight_byte_indices[m_eight_byte_count] = i;
                m_eight_byte_count++;
                break;
            }
        }

        // Create arrays with appropriate sizes and fill them with elements from
        // given pointer.
        m_one_byte_metrics = new metric_info[m_one_byte_count];
        m_eight_byte_metrics = new metric_info[m_eight_byte_count];

        for (std::size_t i = 0; i < m_metric_count; i++)
        {
            if (i < m_one_byte_count)
            {
                m_one_byte_metrics[i] = info[one_byte_indices[i]];
            }
            else
            {
                m_eight_byte_metrics[i - m_one_byte_count] =
                    info[eight_byte_indices[i - m_one_byte_count]];
            }
        }
    }

    ~value_size_info()
    {
        delete[] m_eight_byte_metrics;
        delete[] m_one_byte_metrics;
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
