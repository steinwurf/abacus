// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics.hpp>
TEST(test_metrics, default_constructor)
{
    uint16_t max_metrics = 10;
    uint16_t max_name_bytes = 32;
    std::string title = "test_metrics";

    abacus::metrics metrics(max_metrics, max_name_bytes, title);

    auto count1 = metrics.initialize_metric(0, "count_1");

    auto storage_size =
        5 + max_name_bytes +
        (8 - (5 + max_name_bytes + max_metrics * max_name_bytes + max_metrics) %
                 8) +
        max_metrics * (max_name_bytes + sizeof(uint8_t) + sizeof(uint64_t));

    EXPECT_TRUE(metrics.is_metric_initialized(0));
    EXPECT_EQ(metrics.metric_name(0), "count_1");
    EXPECT_EQ(metrics.metric_value(0), 0U);
    EXPECT_STREQ(metrics.metric_unit(0), "none");
    EXPECT_EQ(metrics.storage_bytes(), storage_size);

    ++count1;

    EXPECT_EQ(metrics.metric_value(0), 1U);

    std::string title1 = "test_metrics1";

    abacus::metrics metrics1(max_metrics, max_name_bytes, title1);

    auto count2 = metrics1.initialize_metric(0, "count_2",
                                             abacus::unit::gigabits_per_second);

    EXPECT_TRUE(metrics1.is_metric_initialized(0));
    EXPECT_EQ(metrics1.metric_name(0), "count_2");
    EXPECT_STREQ(metrics1.metric_unit(0), "gigabits/second");
    EXPECT_EQ(metrics1.metric_value(0), 0U);

    ++count2;

    EXPECT_EQ(metrics1.metric_value(0), 1U);

    std::string title2 = "test_metrics2";
    abacus::metrics metrics2(max_metrics, max_name_bytes, title2);

    auto count3 =
        metrics2.initialize_metric(0, "count_3", abacus::unit::milliseconds);

    EXPECT_TRUE(metrics2.is_metric_initialized(0));
    EXPECT_EQ(metrics2.metric_name(0), "count_3");
    EXPECT_STREQ(metrics2.metric_unit(0), "milliseconds");
    EXPECT_EQ(metrics2.metric_value(0), 0U);

    count3 = 5U;

    EXPECT_EQ(metrics2.metric_value(0), 5U);
}

TEST(test_metrics, copy_storage)
{
    uint16_t max_metrics = 10;
    uint16_t max_name_bytes = 32;

    abacus::metrics metrics(max_metrics, max_name_bytes, "test_metrics");

    metrics.initialize_metric(0, "count_1");

    std::size_t size = metrics.storage_bytes();
    std::vector<uint8_t> data1(size);
    std::vector<uint8_t> data2(size);

    metrics.copy_storage(data1.data());
    metrics.copy_storage(data2.data());

    EXPECT_EQ(metrics.storage_bytes(), data1.size());
    EXPECT_EQ(metrics.storage_bytes(), data2.size());
    EXPECT_EQ(data2, data1);
}

TEST(test_metrics, reset_counters)
{
    uint64_t count = 1;
    uint16_t max_name_bytes = 32;
    uint16_t max_metrics = 10;

    abacus::metrics metrics(max_metrics, max_name_bytes, "metrics");

    for (std::size_t i = 0; i < max_metrics; i++)
    {
        std::string name = "metric" + std::to_string(i);
        auto metric = metrics.initialize_metric(i, name);
        metric = count;
    }

    metrics.reset_metric(9);
    EXPECT_EQ(metrics.metric_value(9), (uint64_t)0);

    metrics.reset_metrics();

    for (std::size_t i = 0; i < max_metrics; i++)
    {
        EXPECT_EQ(metrics.metric_value(i), (uint64_t)0);
    }
}
