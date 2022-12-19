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
    std::string name2 = "metric3";

    abacus::metric_info infos[3] = {
        abacus::metric_info{name0, "An unsigned integer metric",
                            abacus::type::uint64, abacus::kind::counter,
                            abacus::unit{"bytes"}},
        abacus::metric_info{name1, "A signed integer metric",
                            abacus::type::int64, abacus::kind::gauge,
                            abacus::unit{"USD"}},
        abacus::metric_info{name2, "Constant floating point metric",
                            abacus::type::float64, abacus::kind::constant}};

    abacus::metrics metrics(infos);

    metrics.initialize_metric<abacus::type::uint64>(name0);

    metrics.initialize_metric<abacus::type::int64>(name1);

    metrics.initialize_constant(name2, 3.14);

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
    EXPECT_EQ(metrics.name(2), view.name(2));

    EXPECT_EQ(view.type(0), abacus::type::uint64);
    EXPECT_EQ(view.type(1), abacus::type::int64);
    EXPECT_EQ(view.type(2), abacus::type::float64);

    EXPECT_EQ(view.kind(0), abacus::kind::counter);
    EXPECT_EQ(view.kind(1), abacus::kind::gauge);
    EXPECT_EQ(view.kind(2), abacus::kind::constant);

    EXPECT_EQ(view.unit(0), "bytes");
    EXPECT_EQ(view.unit(1), "USD");
    EXPECT_EQ(view.unit(2), "");

    uint64_t metrics_value = 12;
    uint64_t view_value = 11;
    metrics.value(0, metrics_value);
    view.value(0, view_value);

    EXPECT_EQ(metrics_value, view_value);

    EXPECT_EQ(view.meta_data(), meta_data.data());
    EXPECT_EQ(view.value_data(), value_data.data());
}
