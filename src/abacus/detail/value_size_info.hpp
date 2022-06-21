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
    std::vector<metric_info> eight_byte_metrics;
    std::vector<metric_info> one_byte_metrics;

    value_size_info(const std::vector<metric_info>& info)
    {
        assert(info.size() > 0);

        for (auto i : info)
        {
            switch (i.type)
            {
            case value_type::boolean:
                one_byte_metrics.push_back(i);
                break;
            default:
                eight_byte_metrics.push_back(i);
                break;
            }
        }
    }

    metric_info operator[](std::size_t index) const
    {
        assert(index < eight_byte_metrics.size() + one_byte_metrics.size());

        if (index < eight_byte_metrics.size())
        {
            return eight_byte_metrics[index];
        }
        else
        {
            return one_byte_metrics[index - eight_byte_metrics.size()];
        }
    }

    std::size_t size() const
    {
        return eight_byte_metrics.size() + one_byte_metrics.size();
    }
};
}
}
}
