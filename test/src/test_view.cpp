// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics2.hpp>
#include <abacus/protocol_version.hpp>
#include <abacus/view2.hpp>

#include <google/protobuf/util/message_differencer.h>

TEST(test_view, api)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";
    std::string name2 = "metric3";

    std::map<std::string, abacus::type2> infos = {
        {name0,
         abacus::uint64{abacus::kind::COUNTER, "An unsigned integer metric",
                        abacus::unit{"bytes"}}},
        {name1, abacus::int64{abacus::kind::GAUGE, "A signed integer metric",
                              abacus::unit{"USD"}}},
        {name2, abacus::float64{abacus::kind::CONSTANT,
                                "A constant floating point metric",
                                abacus::unit{"ms"}}}};

    abacus::metrics2 metrics(infos);

    auto metric0 = metrics.initialize_metric<abacus::uint64>(name0);

    auto metric1 = metrics.initialize_metric<abacus::int64>(name1);

    metrics.initialize_constant<abacus::float64>(name2, 3.14);

    std::vector<uint8_t> meta_data(metrics.metadata_bytes());
    std::vector<uint8_t> value_data(metrics.value_bytes());
    std::memcpy(meta_data.data(), metrics.metadata_data(),
                metrics.metadata_bytes());
    std::memcpy(value_data.data(), metrics.value_data(), metrics.value_bytes());

    abacus::view2 view;

    view.set_meta_data(meta_data.data(), meta_data.size());
    EXPECT_EQ(view.metadata().protocol_version(), abacus::protocol_version());

    EXPECT_EQ(metrics.metadata().metrics().size(),
              view.metadata().metrics().size());

    EXPECT_TRUE(google::protobuf::util::MessageDifferencer::Equals(
        metrics.metadata(), view.metadata()));

    auto success = view.set_value_data(value_data.data(), value_data.size());
    ASSERT_TRUE(success);

    std::optional<uint64_t> view_value = view.value<abacus::uint64>(name0);

    EXPECT_FALSE(view_value.has_value());

    // Update metric
    metric0 = 9000U;
    // and provide new value data to the view
    view.set_value_data(metrics.value_data(), metrics.value_bytes());
    view_value = view.value<abacus::uint64>(name0);

    EXPECT_TRUE(view_value.has_value());
    EXPECT_EQ(9000U, view_value.value());
}
