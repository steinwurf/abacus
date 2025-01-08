// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "detail/common.hpp"

#include "kind.hpp"
#include "max.hpp"
#include "min.hpp"
#include "unit.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// A 32-bit signed integer metric
struct int32
{
    /// The primitive type of the metric
    using type = int32_t;

    /// The metric type
    struct metric : public detail::common<int32>
    {
        /// Inherit the constructors and various operators from the common class
        using detail::common<int32>::common;

        /// Inherit the assignment operator from the common class
        using detail::common<int32>::operator=;
    };

    /// The metric kind
    abacus::kind kind;

    /// The metric description
    std::string description;

    /// The unit of the metric
    abacus::unit unit{};

    /// The minimum value of the metric
    abacus::min<type> min{};

    /// The maximum value of the metric
    abacus::max<type> max{};
};
}
}
