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

    abacus::metrics metrics(max_metrics, max_name_bytes);

    auto count1 = metrics.add_metric("count_1");

    auto storage_size = 6 + (8 - (6 + max_metrics * max_name_bytes) % 8) +
                        max_metrics * (max_name_bytes + sizeof(uint64_t));
    EXPECT_EQ(metrics.count(), 1U);
    EXPECT_EQ(metrics.metric_index("count_1"), 0U);
    EXPECT_EQ(metrics.metric_name(0), "count_1");
    EXPECT_EQ(metrics.metric_value(0), 0U);
    EXPECT_EQ(metrics.storage_bytes(), storage_size);

    ++count1;

    EXPECT_EQ(metrics.metric_value(0), 1U);

    abacus::metrics metrics1(max_metrics, max_name_bytes);

    auto count2 = metrics1.add_metric("count_2");

    EXPECT_EQ(metrics1.count(), 1U);
    EXPECT_EQ(metrics1.metric_index("count_2"), 0U);
    EXPECT_EQ(metrics1.metric_name(0), "count_2");
    EXPECT_EQ(metrics1.metric_value(0), 0U);

    ++count2;

    EXPECT_EQ(metrics1.metric_value(0), 1U);

    abacus::metrics metrics2(max_metrics, max_name_bytes);

    auto count3 = metrics2.add_metric("count_3");

    EXPECT_EQ(metrics2.count(), 1U);
    EXPECT_EQ(metrics2.metric_index("count_3"), 0U);
    EXPECT_EQ(metrics2.metric_name(0), "count_3");
    EXPECT_EQ(metrics2.metric_value(0), 0U);

    count3 = 5U;

    EXPECT_EQ(metrics2.metric_value(0), 5U);
}

TEST(test_metrics, copy_storage)
{
    uint16_t max_metrics = 10;
    uint16_t max_name_bytes = 32;

    abacus::metrics metrics(max_metrics, max_name_bytes);

    metrics.add_metric("count_1");
    metrics.push_scope("scope");

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

    abacus::metrics metrics(max_metrics, max_name_bytes);

    for (std::size_t i = 0; i < max_metrics; i++)
    {
        std::string name = "metric" + std::to_string(i);
        auto metric = metrics.add_metric(name);
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

TEST(test_metrics, add_scope)
{
    uint16_t max_name_bytes = 32;
    uint16_t max_metrics = 10;

    std::string scope = "metrics";

    abacus::metrics metrics(max_metrics, max_name_bytes);

    for (std::size_t i = 0; i < max_metrics; i++)
    {
        std::string name = "metric" + std::to_string(i);
        auto metric = metrics.add_metric(name);
        metric = i;
    }

    metrics.push_scope(scope);
    metrics.push_scope("test");

    EXPECT_EQ(metrics.count(), max_metrics);

    for (std::size_t i = 0; i < metrics.count(); i++)
    {
        EXPECT_EQ(metrics.scope(), "test.metrics");
        EXPECT_EQ(metrics.scope_size(), std::string("test.metrics").size());
        EXPECT_EQ(metrics.metric_name(i), "metric" + std::to_string(i));
    }
}
