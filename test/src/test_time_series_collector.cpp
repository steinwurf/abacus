// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <gtest/gtest.h>

#include "details/dummy_time_series_exporter.hpp"

#include <abacus/metrics.hpp>
#include <abacus/time_series/collector.hpp>

TEST(test_time_series_collector, general_usage)
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

    for (std::size_t i = 0; i < 50; i++)
    {
        ++count1;
        EXPECT_EQ(metrics.metric_value(0), i + 1);

        metrics.copy_storage(collector.new_data_point(test_aggregate, 123456789,
                                                      metrics.storage_bytes()));
        collector.flush_to_exporters(exporters);

        EXPECT_EQ(exporter->data_points.size(), i + 1);
        EXPECT_TRUE(exporter->data_points[i].aggregate->operator[](0) == "my");
        EXPECT_TRUE(exporter->data_points[i].aggregate->operator[](1) ==
                    "aggregate");
        EXPECT_EQ(exporter->data_points[i].metrics.size(),
                  metrics.storage_bytes() +
                      sizeof(abacus::time_series::time_type));
        auto view_iterator = exporter->data_points[i].metrics.view_iterator();
        EXPECT_EQ(view_iterator.get_view(0).metric_value(0), i + 1);
    }
}
