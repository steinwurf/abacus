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

#include <google/protobuf/util/message_differencer.h>

TEST(test_view, api)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";
    std::string name2 = "metric2";
    std::string name3 = "metric3";

    std::map<abacus::name, abacus::type> infos = {
        {abacus::name{name0},
         abacus::uint64{abacus::counter,
                        abacus::description{"An unsigned integer metric"},
                        abacus::optional, abacus::unit{"bytes"}}},
        {abacus::name{name1},
         abacus::int64{abacus::gauge,
                       abacus::description{"A signed integer metric"},
                       abacus::optional, abacus::unit{"USD"}}},
        {abacus::name{name2},
         abacus::float64{
             abacus::constant,
             abacus::description{"A constant floating point metric"},
             abacus::required, abacus::unit{"ms"}}},
        {abacus::name{name3},
         abacus::enum8{abacus::description{"An enum metric"},
                       {{0, {"value0", "The value for 0"}},
                        {1, {"value1", "The value for 1"}},
                        {2, {"value2", "The value for 2"}},
                        {3, {"value3", "The value for 3"}}},
                       abacus::optional}}};

    abacus::metrics metrics(infos);

    auto metric0 = metrics.initialize_optional<abacus::uint64>(name0);

    auto metric1 = metrics.initialize_optional<abacus::int64>(name1);

    metrics.initialize_constant<abacus::float64>(name2, 3.14);

    auto metric3 = metrics.initialize_optional<abacus::enum8>(name3);

    std::vector<uint8_t> meta_data(metrics.metadata_bytes());
    std::vector<uint8_t> value_data(metrics.value_bytes());
    std::memcpy(meta_data.data(), metrics.metadata_data(),
                metrics.metadata_bytes());
    std::memcpy(value_data.data(), metrics.value_data(), metrics.value_bytes());

    abacus::view view;

    view.set_meta_data(meta_data.data(), meta_data.size());
    EXPECT_EQ(view.metadata().protocol_version(), abacus::protocol_version());

    EXPECT_EQ(metrics.metadata().metrics().size(),
              view.metadata().metrics().size());

    EXPECT_TRUE(google::protobuf::util::MessageDifferencer::Equals(
        metrics.metadata(), view.metadata()));

    auto success = view.set_value_data(value_data.data(), value_data.size());
    ASSERT_TRUE(success);

    std::optional<uint64_t> view_value0 = view.value<abacus::uint64>(name0);
    std::optional<int64_t> view_value1 = view.value<abacus::int64>(name1);
    std::optional<double> view_value2 = view.value<abacus::float64>(name2);
    std::optional<uint8_t> view_value3 = view.value<abacus::enum8>(name3);

    EXPECT_FALSE(view_value0.has_value());
    EXPECT_FALSE(view_value1.has_value());
    EXPECT_TRUE(view_value2.has_value());
    EXPECT_EQ(3.14, view_value2.value());
    EXPECT_FALSE(view_value3.has_value());

    // Update metrics
    metric0 = 9000U;
    metric1 = -1000;
    metric3 = 2;

    // and provide new value data to the view
    success = view.set_value_data(metrics.value_data(), metrics.value_bytes());
    ASSERT_TRUE(success);
    view_value0 = view.value<abacus::uint64>(name0);

    EXPECT_TRUE(view_value0.has_value());
    EXPECT_EQ(9000U, view_value0.value());

    view_value1 = view.value<abacus::int64>(name1);

    EXPECT_TRUE(view_value1.has_value());
    EXPECT_EQ(-1000, view_value1.value());

    view_value3 = view.value<abacus::enum8>(name3);

    EXPECT_TRUE(view_value3.has_value());
    EXPECT_EQ(2, view_value3.value());
}
