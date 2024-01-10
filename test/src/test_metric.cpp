// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

// Pragma needed to be able to compile code, that divides by literal zero with
// MSVC
#pragma fenv_access(on)

#include <cmath>
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

TEST(test_metric, float_assignment)
{
    double double_count = 1123.12;
    abacus::metric<abacus::type::float64> double_metric(&double_count);
    EXPECT_TRUE(double_metric.is_initialized());
    EXPECT_DOUBLE_EQ(double_metric.value(), 1123.12);

    // Assignment
    // Check that assignment to NaN is not allowed
    EXPECT_DEATH(double_metric = 0.0 / 0.0, "");
    // Check that that assignment to -NaN is not allowed
    EXPECT_DEATH(double_metric = -0.0 / 0.0, "");
    // Check that that assignment to +Inf is not allowed
    EXPECT_DEATH(double_metric = 1 / 0.0, "");
    // Check that that assignment to -Inf is not allowed
    EXPECT_DEATH(double_metric = 1 / -0.0, "");

    // Add and Assign
    // Check that assignment to NaN is not allowed
    EXPECT_DEATH(double_metric += 0.0 / 0.0, "");
    // Check that that assignment to -NaN is not allowed
    EXPECT_DEATH(double_metric += -0.0 / 0.0, "");
    // Check that that assignment to +Inf is not allowed
    EXPECT_DEATH(double_metric += 1 / 0.0, "");
    // Check that that assignment to -Inf is not allowed
    EXPECT_DEATH(double_metric += 1 / -0.0, "");

    // Subtract and Assign
    // Check that assignment to NaN is not allowed
    EXPECT_DEATH(double_metric -= 0.0 / 0.0, "");
    // Check that that assignment to -NaN is not allowed
    EXPECT_DEATH(double_metric -= -0.0 / 0.0, "");
    // Check that that assignment to +Inf is not allowed
    EXPECT_DEATH(double_metric -= 1 / 0.0, "");
    // Check that that assignment to -Inf is not allowed
    EXPECT_DEATH(double_metric -= 1 / -0.0, "");
}
