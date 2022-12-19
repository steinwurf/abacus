// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstdint>
#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metric.hpp>
TEST(test_metric, constructor)
{
    uint64_t uint_count = 10000U;

    abacus::metric<abacus::type::uint64> uint_metric(&uint_count);

    EXPECT_TRUE(uint_metric.is_initialized());

    int64_t int_count = 10000;
    abacus::metric<abacus::type::int64> int_metric(&int_count);
    EXPECT_TRUE(int_metric.is_initialized());

    double double_count = 1123.12;
    abacus::metric<abacus::type::float64> double_metric(&double_count);
    EXPECT_TRUE(double_metric.is_initialized());

    bool bool_count = true;
    abacus::metric<abacus::type::boolean> bool_metric(&bool_count);
    EXPECT_TRUE(bool_metric.is_initialized());
}
