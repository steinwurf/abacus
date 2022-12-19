// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "to_string.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

std::string to_string(abacus::kind kind)
{
    switch (kind)
    {
    case abacus::kind::counter:
        return "counter";
    case abacus::kind::gauge:
        return "gauge";
    case abacus::kind::constant:
        return "constant";
    default:
        return "unknown";
    }
}

}
}
