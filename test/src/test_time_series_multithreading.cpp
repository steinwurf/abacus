// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <gtest/gtest.h>
#include <thread>

#include "details/dummy_time_series_exporter.hpp"

#include <abacus/metrics.hpp>
#include <abacus/time_series/collector.hpp>
#include <abacus/time_series/data_points_queue.hpp>

TEST(test_time_series_export_lock_free, general_usage)
{
    // Collector and exporter
    abacus::time_series::collector collector;
    auto exporter = std::make_shared<details::dummy_exporter>();
    abacus::time_series::exporters exporters({exporter});

    // Aggregate
    auto test_aggregate = collector.new_aggregate("my|aggregate");

    // Metrics
    uint16_t max_metrics = 10;
    uint16_t max_name_bytes = 32;
    std::string title = "test_metrics";
    abacus::metrics metrics(max_metrics, max_name_bytes, title);
    auto count1 = metrics.initialize_metric(0, "count_1");

    // Lock free queue
    abacus::time_series::data_points_queue queue;

    // Export in another thread
    std::thread export_thread([&]() {
        while (exporter->data_points.size() < 50)
        {
            queue.flush_to_exporters(exporters);
        }
    });

    for (std::size_t i = 0; i < 50; i++)
    {
        ++count1;
        EXPECT_EQ(metrics.metric_value(0), i + 1);

        metrics.copy_storage(collector.new_data_point(test_aggregate, i,
                                                      metrics.storage_bytes()));

        // Flush collected data points to the lock free queue
        collector.flush([&](auto begin, auto end) { queue.push(begin, end); });
    }

    // Wait for the exporter to finish its job
    export_thread.join();

    EXPECT_EQ(exporter->data_points.size(), 50);
}