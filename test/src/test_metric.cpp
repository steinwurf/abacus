// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cmath>
#include <cstdint>
#include <cstring>
#include <gtest/gtest.h>

#include <abacus/type.hpp>

TEST(test_metric, constructor)
{
    uint8_t data[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    abacus::uint64::metric uint_metric(data);

    EXPECT_TRUE(uint_metric.is_initialized());
    EXPECT_FALSE(uint_metric.has_value());
    uint_metric = 42U;
    EXPECT_TRUE(uint_metric.has_value());
    EXPECT_EQ(uint_metric.value(), 42U);
}
/*
TEST(test_metric, float_assignment)
{
    uint8_t data[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    abacus::float64::metric double_metric(data);
    EXPECT_TRUE(double_metric.is_initialized());
    EXPECT_FALSE(double_metric.has_value());
    double_metric = 1123.12;
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
*/
