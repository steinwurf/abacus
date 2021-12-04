// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics.hpp>
#include <abacus/time_series/collector.hpp>
#include <abacus/time_series/time_series_exporter.hpp>

struct test_time_series_time_series_exporter : ::testing::Test
{
    test_time_series_time_series_exporter() : metrics(10, 32, "test_metrics")
    {
    }

    void SetUp() override
    {
        exporter =
            std::make_shared<abacus::time_series::time_series_exporter>();
        exporters.emplace_back(exporter);
        test_aggregate = collector.new_aggregate("my|aggregate");

        count1 = metrics.initialize_metric(0, "count_1");
    }

    abacus::metrics metrics;
    abacus::time_series::collector collector;
    std::shared_ptr<abacus::time_series::time_series_exporter> exporter;
    abacus::time_series::exporters exporters;
    abacus::time_series::aggregate_ptr test_aggregate;
    abacus::metric count1;
};

TEST_F(test_time_series_time_series_exporter, time_index)
{
    abacus::time_series::time_series_exporter::time_index index;

    EXPECT_EQ(index.find_near(0),
              abacus::time_series::time_series_exporter::time_index::NOT_FOUND);

    for (size_t i = 1; i <= 99; i++)
    {
        index.emplace_back(i * 10, i);
    }

    EXPECT_EQ(index.front().m_time, 10);
    EXPECT_EQ(index.front().m_data_point_index, 1);
    EXPECT_EQ(index.back().m_time, 990);
    EXPECT_EQ(index.back().m_data_point_index, 99);

    auto end = index.size() - 1;
    for (size_t i = 1; i <= 99; i++)
    {
        EXPECT_EQ(index.find_near(i * 10 + 5), std::min(end, i));
        EXPECT_EQ(index.find_near(i * 10 + 4), i - 1);
        EXPECT_EQ(index.find_near(i * 10 + 3), i - 1);
        EXPECT_EQ(index.find_near(i * 10 + 2), i - 1);
        EXPECT_EQ(index.find_near(i * 10 + 1), i - 1);
        EXPECT_EQ(index.find_near(i * 10), i - 1);
        EXPECT_EQ(index.find_near(i * 10 - 1), i - 1);
        EXPECT_EQ(index.find_near(i * 10 - 2), i - 1);
        EXPECT_EQ(index.find_near(i * 10 - 3), i - 1);
        EXPECT_EQ(index.find_near(i * 10 - 4), i - 1);
        EXPECT_EQ(index.find_near(i * 10 - 5), i - 1);
    }
}

TEST_F(test_time_series_time_series_exporter, general_usage)
{
    for (std::size_t j = 0; j < 10; j++)
    {
        for (std::size_t i = 0; i < 50; i++)
        {
            ++count1;
            EXPECT_EQ(metrics.metric_value(0), i + 1 + j * 50);

            // Add new data point plus a duplicate
            metrics.copy_storage(collector.new_data_point(
                test_aggregate, i + j * 50, metrics.storage_bytes()));
            metrics.copy_storage(collector.new_data_point(
                test_aggregate, i + j * 50, metrics.storage_bytes()));
        }

        collector.flush_to_exporters(exporters);

        auto filtered = exporter->get(25 + j * 50, 50 + j * 50);
        EXPECT_EQ(filtered.size(), 50);
        EXPECT_EQ(filtered.front().metrics.time(), 25 + j * 50);
        EXPECT_EQ(filtered.back().metrics.time(), 49 + j * 50);

        filtered = exporter->get(10 + j * 50, 15 + j * 50);
        EXPECT_EQ(filtered.size(), 12);
        EXPECT_EQ(filtered.front().metrics.time(), 10 + j * 50);
        EXPECT_EQ(filtered.back().metrics.time(), 15 + j * 50);

        exporter->clear();
    }

    auto filtered = exporter->get(5, 10);
    EXPECT_EQ(filtered.size(), 0);
}

TEST_F(test_time_series_time_series_exporter, query_methods)
{
    for (std::size_t i = 10; i < 20; i++)
    {
        ++count1;
        metrics.copy_storage(collector.new_data_point(test_aggregate, i,
                                                      metrics.storage_bytes()));
    }

    collector.flush_to_exporters(exporters);

    // Get time range
    auto filtered = exporter->get(0, 9);
    EXPECT_EQ(filtered.size(), 0);

    filtered = exporter->get(0, 10);
    EXPECT_EQ(filtered.size(), 1);

    filtered = exporter->get(10, 10);
    EXPECT_EQ(filtered.size(), 1);

    filtered = exporter->get(19, 19);
    EXPECT_EQ(filtered.size(), 1);

    filtered = exporter->get(19, 20);
    EXPECT_EQ(filtered.size(), 1);

    filtered = exporter->get(20, 20);
    EXPECT_EQ(filtered.size(), 0);

    filtered = exporter->get(10, 19);
    EXPECT_EQ(filtered.size(), 10);

    // Get latest
    filtered = exporter->get(0);
    EXPECT_EQ(filtered.size(), 0);

    filtered = exporter->get(1);
    EXPECT_EQ(filtered.size(), 1);

    filtered = exporter->get(10);
    EXPECT_EQ(filtered.size(), 10);

    filtered = exporter->get(11);
    EXPECT_EQ(filtered.size(), 10);
}

TEST_F(test_time_series_time_series_exporter, time_capacity)
{
    exporter->set_capacity(10);

    for (std::size_t j = 0; j < 10; j++)
    {
        for (std::size_t i = 0; i < 50; i++)
        {
            ++count1;
            EXPECT_EQ(metrics.metric_value(0), i + 1 + j * 50);

            // Add new data point plus a duplicate
            metrics.copy_storage(collector.new_data_point(
                test_aggregate, i, metrics.storage_bytes()));
            metrics.copy_storage(collector.new_data_point(
                test_aggregate, i, metrics.storage_bytes()));

            collector.flush_to_exporters(exporters);
        }

        // We expect 10 unique time points but 2 data point for each, therefore
        // 20 data points in total
        auto filtered = exporter->get(50);
        EXPECT_EQ(filtered.size(), 20);
        for (std::size_t i = 0; i < filtered.size(); i++)
        {
            EXPECT_EQ(
                filtered[i].metrics.view_iterator().get_view(0).metric_value(0),
                (i / 2) + 41 + j * 50);
        }
    }
}

TEST_F(test_time_series_time_series_exporter, flush_equal_to_time_capacity)
{
    exporter->set_capacity(10);

    for (std::size_t j = 0; j < 10; j++)
    {
        for (std::size_t i = 0; i < 10; i++)
        {
            ++count1;
            EXPECT_EQ(metrics.metric_value(0), i + 1 + j * 10);

            // Add new data point plus a duplicate
            metrics.copy_storage(collector.new_data_point(
                test_aggregate, i, metrics.storage_bytes()));
            metrics.copy_storage(collector.new_data_point(
                test_aggregate, i, metrics.storage_bytes()));
        }

        collector.flush_to_exporters(exporters);

        // We expect 10 unique time points but 2 data point for each, therefore
        // 20 data points in total
        auto filtered = exporter->get(50);
        EXPECT_EQ(filtered.size(), 20);
        for (std::size_t i = 0; i < filtered.size(); i++)
        {
            EXPECT_EQ(
                filtered[i].metrics.view_iterator().get_view(0).metric_value(0),
                (i / 2) + 1 + j * 10);
        }
    }
}

TEST_F(test_time_series_time_series_exporter, flush_more_than_time_capacity)
{
    exporter->set_capacity(10);

    for (std::size_t j = 0; j < 10; j++)
    {
        for (std::size_t i = 0; i < 50; i++)
        {
            ++count1;
            EXPECT_EQ(metrics.metric_value(0), i + 1 + j * 50);

            // Add new data point plus a duplicate
            metrics.copy_storage(collector.new_data_point(
                test_aggregate, i, metrics.storage_bytes()));
            metrics.copy_storage(collector.new_data_point(
                test_aggregate, i, metrics.storage_bytes()));
        }

        collector.flush_to_exporters(exporters);

        // We expect 10 unique time points but 2 data point for each, therefore
        // 20 data points in total
        auto filtered = exporter->get(50);
        EXPECT_EQ(filtered.size(), 20);
        for (std::size_t i = 0; i < filtered.size(); i++)
        {
            EXPECT_EQ(
                filtered[i].metrics.view_iterator().get_view(0).metric_value(0),
                (i / 2) + 41 + j * 50);
        }
    }
}