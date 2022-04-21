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
    std::string scope = "scope";

    abacus::metrics metrics(max_metrics, max_name_bytes);
    auto metric = metrics.add_metric("metric");
    metrics.push_scope(scope);

    std::size_t storage_size =
        7 + (8 - (7 + max_metrics * max_name_bytes) % 8) +
        max_metrics * (max_name_bytes + sizeof(uint64_t)) + 8;

    EXPECT_EQ(metrics.storage_bytes(), storage_size);

    metric += 10;

    std::vector<uint8_t> data(metrics.storage_bytes());

    metrics.copy_storage(data.data());

    abacus::view view;

    view.set_data(data.data());

    EXPECT_EQ(max_metrics, view.max_metrics());
    EXPECT_EQ(scope.size(), view.scope_size());
    EXPECT_EQ(scope, view.scope());
    EXPECT_EQ(1U, view.count());
    EXPECT_EQ(max_name_bytes, view.max_name_bytes());
    EXPECT_EQ(metrics.metric_name(0), view.metric_name(0));
    EXPECT_EQ(metrics.metric_value(0), view.metric_value(0));
    EXPECT_EQ(view.data(), data.data());
}
