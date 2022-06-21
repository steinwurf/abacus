// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics.hpp>
#include <abacus/view.hpp>

TEST(test_view, api)
{
    std::string scope = "scope";

    uint16_t metric_count = 2;

    std::string name0 = "metric0";
    std::string name1 = "metric1";

    std::vector<abacus::metric_info> infos;

    infos.reserve(metric_count);

    infos.push_back(abacus::metric_info{name0, "An unsigned integer metric",
                                        abacus::value_type::unsigned_integral,
                                        abacus::NON_CONSTANT});
    infos.push_back(abacus::metric_info{name1, "A signed integer metric",
                                        abacus::value_type::signed_integral,
                                        abacus::NON_CONSTANT});

    abacus::metrics metrics(infos);

    metrics.initialize_metric<abacus::value_type::unsigned_integral>(0, name0);

    metrics.initialize_metric<abacus::value_type::signed_integral>(1, name1);

    metrics.push_scope(scope);

    std::vector<uint8_t> data(metrics.storage_bytes());

    metrics.copy_storage(data.data(), metrics.storage_bytes());

    abacus::view view;

    view.set_data(data.data());

    EXPECT_EQ(metrics.metric_count(), view.metric_count());
    EXPECT_EQ(metrics.scope_size(), view.scope_size());
    EXPECT_EQ(metrics.scope(), view.scope());
    EXPECT_EQ(metrics.name_bytes(), view.name_bytes());
    EXPECT_EQ(metrics.description_bytes(), view.description_bytes());

    EXPECT_EQ(metrics.metric_name(0), view.metric_name(0));
    EXPECT_EQ(metrics.metric_name(1), view.metric_name(1));

    EXPECT_EQ(metrics.metric_type(0), abacus::value_type::unsigned_integral);
    EXPECT_EQ(metrics.metric_type(1), abacus::value_type::signed_integral);
    EXPECT_EQ(view.metric_type(0), abacus::value_type::unsigned_integral);
    EXPECT_EQ(view.metric_type(1), abacus::value_type::signed_integral);

    uint64_t metric_value = 12;
    uint64_t view_value = 11;
    metrics.metric_value(0, metric_value);
    view.metric_value(0, view_value);

    EXPECT_EQ(metric_value, view_value);

    EXPECT_EQ(view.data(), data.data());
}
