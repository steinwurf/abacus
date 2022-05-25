// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <vector>

#include "../metric_info.hpp"
#include "../value_types.hpp"
#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{

struct info_iterator
{
    std::vector<std::size_t> names_offsets;
    std::vector<std::size_t> descriptions_offsets;
    std::size_t type_bytes;
    std::size_t value_bytes;
    std::size_t eight_byte_values;
    std::size_t one_byte_values;
};

inline auto info_iterator_from_vector(std::vector<metric_info>& info)
    -> info_iterator
{
    info_iterator result = {{}, {}, 0, 0, 0, 0};

    auto iterations = 0;

    for (auto& i : info)
    {
        result.name_bytes += i.name.size() + 1;
        switch (i.value_type)
        {
        case value_type::boolean:
            result.bools_count++;
            result.value_bytes += 1;
            break;
        default:
            result.value_bytes += 8;
            break;
        }

        result.description_bytes += i.description.size() + 1;
        result.type_bytes += 1;
        iterations++;
    }

    return result;
}

inline auto info_descriptions_bytes(std::vector<metric_info>& info)
    -> std::size_t
{
    std::size_t descriptions_bytes = 0;
    for (auto& i : info)
    {
        descriptions_bytes += i.description.size() + 1;
    }
    return descriptions_bytes;
}

}
}
}
