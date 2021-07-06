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

    abacus::metrics metrics(max_metrics, max_name_bytes, "test_metrics");

    auto count1 = metrics.initialize_metric(0, "count_1");

    EXPECT_EQ(metrics.metric_name(0), "count_1");
    EXPECT_EQ(metrics.metric_value(0), 0U);

    ++count1;

    EXPECT_EQ(metrics.metric_value(0), 1U);
}