// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics.hpp>
TEST(test_metric, default_constructor)
{
    uint64_t max_metrics = 10;
    uint64_t max_name_bytes = 32;
    uint8_t level = 0;

    abacus::metrics metrics(max_metrics, max_name_bytes, "test_metrics", level);

    auto count1 = metrics.initialize_metric(0, "count_1");

    EXPECT_EQ(metrics.metric_name(0), "count_1");
    EXPECT_EQ(metrics.metric_value(0), 0U);
    EXPECT_EQ(metrics.metrics_level(), 0U);

    ++count1;

    EXPECT_EQ(metrics.metric_value(0), 1U);

    abacus::metrics metrics1(max_metrics, max_name_bytes, "test_metrics1",
                             level);

    auto count2 = metrics1.initialize_metric(0, "count_2");

    EXPECT_EQ(metrics1.metric_name(0), "count_2");
    EXPECT_EQ(metrics1.metric_value(0), 0U);
    EXPECT_EQ(metrics1.metrics_level(), 0U);

    ++count2;

    EXPECT_EQ(metrics1.metric_value(0), 1U);

    metrics1.set_metrics_level(metrics.metrics_level() + 1);

    EXPECT_EQ(metrics1.metrics_level(), 1U);

    abacus::metrics metrics2(max_metrics, max_name_bytes, "test_metrics2",
                             level);

    auto count3 = metrics2.initialize_metric(0, "count_3");

    EXPECT_EQ(metrics2.metric_name(0), "count_3");
    EXPECT_EQ(metrics2.metric_value(0), 0U);
    EXPECT_EQ(metrics2.metrics_level(), 0U);

    count3 += 5;

    EXPECT_EQ(metrics2.metric_value(0), 5U);

    metrics2.set_metrics_level(metrics1.metrics_level() + 1);

    EXPECT_EQ(metrics2.metrics_level(), 2U);
}