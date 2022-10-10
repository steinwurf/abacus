// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics.hpp>
#include <abacus/protocol_version.hpp>
#include <abacus/view.hpp>

TEST(test_view, api)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";

    abacus::metric_info infos[2] = {
        abacus::metric_info{name0, "An unsigned integer metric",
                            abacus::metric_type::uint64},
        abacus::metric_info{name1, "A signed integer metric",
                            abacus::metric_type::int64}};

    abacus::metrics metrics(infos);

    metrics.initialize_metric<abacus::metric_type::uint64>(name0);

    metrics.initialize_metric<abacus::metric_type::int64>(name1);

    std::vector<uint8_t> meta_data(metrics.meta_bytes());
    std::vector<uint8_t> value_data(metrics.value_bytes());
    std::memcpy(meta_data.data(), metrics.meta_data(), metrics.meta_bytes());
    std::memcpy(value_data.data(), metrics.value_data(), metrics.value_bytes());

    abacus::view view;

    view.set_meta_data(meta_data.data());
    view.set_meta_data(meta_data.data());
    EXPECT_EQ(view.protocol_version(), abacus::protocol_version());

    view.set_value_data(value_data.data());

    EXPECT_EQ(metrics.count(), view.count());

    EXPECT_EQ(metrics.name(0), view.name(0));
    EXPECT_EQ(metrics.name(1), view.name(1));

    EXPECT_EQ(view.type(0), abacus::metric_type::uint64);
    EXPECT_EQ(view.type(1), abacus::metric_type::int64);

    uint64_t metrics_value = 12;
    uint64_t view_value = 11;
    metrics.value(0, metrics_value);
    view.value(0, view_value);

    EXPECT_EQ(metrics_value, view_value);

    EXPECT_EQ(view.meta_data(), meta_data.data());
    EXPECT_EQ(view.value_data(), value_data.data());
}
