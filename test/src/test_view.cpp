// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics.hpp>
#include <abacus/view.hpp>

TEST(test_view, api)
{
    uint16_t max_metrics = 10;
    uint16_t max_name_bytes = 32;
    uint16_t max_prefix_bytes = 32;
    std::string prefix = "metrics";

    abacus::metrics metrics(max_metrics, max_name_bytes, max_prefix_bytes,
                            prefix);
    auto metric = metrics.initialize_metric("metric");

    metric += 10;

    std::vector<uint8_t> data(metrics.storage_bytes());

    metrics.copy_storage(data.data());

    abacus::view view;

    view.set_data(data.data());

    EXPECT_EQ(max_metrics, view.max_metrics());
    EXPECT_EQ(1U, view.metrics_count());
    EXPECT_EQ(max_name_bytes, view.max_name_bytes());
    EXPECT_EQ(prefix, view.get_prefix());
    EXPECT_EQ(prefix, view.metric_prefix(0));
    EXPECT_EQ(metrics.metric_name(0), view.metric_name(0));
    EXPECT_EQ(metrics.metric_value(0), view.metric_value(0));
    EXPECT_EQ(view.data(), data.data());
}
