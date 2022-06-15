// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics.hpp>
TEST(test_metrics, default_constructor)
{
    uint16_t max_metrics = 10;
    uint16_t max_name_bytes = 32;

    abacus::metrics metrics(max_metrics, max_name_bytes);

    auto count1 = metrics.add_metric<uint64_t>("uint_1");
    EXPECT_EQ(metrics.count(), 1U);
    EXPECT_EQ(metrics.metric_index("uint_1"), 0U);
    EXPECT_EQ(metrics.metric_name(0), "uint_1");
    EXPECT_EQ(metrics.metric_type(0), abacus::value_type::unsigned_integral);

    uint64_t value_0 = 3;
    metrics.metric_value(value_0, 0);
    EXPECT_EQ(value_0, 0U);

    ++count1;

    metrics.metric_value(value_0, 0);
    EXPECT_EQ(value_0, 1U);

    abacus::metrics metrics1(max_metrics, max_name_bytes);

    auto double2 = metrics1.add_metric<double>("count_2");

    EXPECT_EQ(metrics1.count(), 1U);
    EXPECT_EQ(metrics1.metric_index("count_2"), 0U);
    EXPECT_EQ(metrics1.metric_name(0), "count_2");
    EXPECT_EQ(metrics1.metric_type(0), abacus::value_type::floating_point);

    double value_1 = 4.0;
    metrics1.metric_value(value_1, 0);
    EXPECT_EQ(value_1, 0.0);

    ++double2;

    metrics1.metric_value(value_1, 0);
    EXPECT_EQ(value_1, 1.0);

    abacus::metrics metrics2(max_metrics, max_name_bytes);

    auto count3 = metrics2.add_metric<bool>("count_3");

    EXPECT_EQ(metrics2.count(), 1U);
    EXPECT_EQ(metrics2.metric_index("count_3"), 0U);
    EXPECT_EQ(metrics2.metric_name(0), "count_3");
    EXPECT_EQ(metrics2.metric_type(0), abacus::value_type::boolean);
    bool value_2 = true;
    metrics2.metric_value(value_2, 0);
    EXPECT_FALSE(value_2);

    count3 = true;
    metrics2.metric_value(value_2, 0);
    EXPECT_TRUE(value_2);
}

TEST(test_metrics, copy_storage)
{
    uint16_t max_metrics = 10;
    uint16_t max_name_bytes = 32;

    abacus::metrics metrics(max_metrics, max_name_bytes);

    metrics.add_metric<double>("count_1");
    metrics.push_scope("scope");

    std::size_t size = 0;
    // header size
    size += 6;
    // names
    size += max_metrics * max_name_bytes;
    // types
    size += max_metrics;
    // alignment padding
    size += 8 - (size % 8);
    // values
    size += max_metrics * 8;
    // scope
    size += metrics.scope_size();
    // scope padding
    size += 8 - (size % 8);

    EXPECT_EQ(size, metrics.storage_bytes());
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
        auto metric = metrics.add_metric<uint64_t>(name);
        metric = count;
    }

    uint64_t value = count;
    metrics.reset_metric(9);
    metrics.metric_value(value, 9);
    EXPECT_EQ(value, 0U);

    metrics.reset_metrics();

    for (std::size_t i = 0; i < max_metrics; i++)
    {
        value = count;
        metrics.metric_value(value, i);
        EXPECT_EQ(value, (uint64_t)0);
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
        auto metric = metrics.add_metric<int64_t>(name);
        metric = i;
    }

    metrics.push_scope(scope);
    metrics.push_scope("test");

    EXPECT_EQ(metrics.count(), max_metrics);

    for (std::size_t i = 0; i < metrics.count(); i++)
    {
        EXPECT_EQ(metrics.scope(), "test.metrics");
        EXPECT_EQ(metrics.scope_size(), std::string("test.metrics").size() + 1);
        EXPECT_EQ(metrics.metric_name(i), "metric" + std::to_string(i));
    }
}
