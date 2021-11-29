// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/detail/raw.hpp>
#include <abacus/metrics.hpp>
#include <abacus/telemetry/collector.hpp>
#include <abacus/telemetry/exporter.hpp>

struct test_exporter : abacus::telemetry::exporter
{
    auto receive_data_points(
        std::vector<abacus::telemetry::data_point>::iterator data_points_start,
        std::vector<abacus::telemetry::data_point>::iterator data_points_end)
        -> void override
    {
        std::copy(data_points_start, data_points_end,
                  std::back_inserter(data_points));
    }

    abacus::telemetry::data_points data_points;
};

TEST(test_telemetry, collector)
{
    // Collector and exporter
    abacus::telemetry::collector collector;
    auto exporter = std::make_shared<test_exporter>();
    collector.add_exporter(exporter);

    // Aggregate
    auto test_aggregate = collector.new_aggregate("my|aggregate");

    // Metrics
    uint16_t max_metrics = 10;
    uint16_t max_name_bytes = 32;
    std::string title = "test_metrics";
    abacus::metrics metrics(max_metrics, max_name_bytes, title);
    auto count1 = metrics.initialize_metric(0, "count_1");

    for (std::size_t i = 0; i < 50; i++)
    {
        ++count1;
        EXPECT_EQ(metrics.metric_value(0), i + 1);

        metrics.copy_storage(collector.new_data_point(test_aggregate, 123456789,
                                                      metrics.storage_bytes()));
        collector.flush();

        EXPECT_EQ(exporter->data_points.size(), i + 1);
        EXPECT_TRUE(exporter->data_points[i].aggregate->operator[](0) == "my");
        EXPECT_TRUE(exporter->data_points[i].aggregate->operator[](1) ==
                    "aggregate");
        EXPECT_EQ(exporter->data_points[i].metrics.size(),
                  metrics.storage_bytes() +
                      sizeof(abacus::telemetry::time_type));
        auto view_iterator = exporter->data_points[i].metrics.view_iterator();
        EXPECT_EQ(view_iterator.get_view(0).metric_value(0), i + 1);
    }
}