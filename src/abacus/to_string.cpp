// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "to_string.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

std::string to_string(metric_kind kind)
{
    switch (kind)
    {
    case metric_kind::counter:
        return "counter";
    case metric_kind::gauge:
        return "gauge";
    case metric_kind::constant:
        return "constant";
    default:
        return "unknown";
    }
}

}
}
