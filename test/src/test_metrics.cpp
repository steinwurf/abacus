// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <google/protobuf/util/message_differencer.h>

#include <abacus/metrics.hpp>
#include <abacus/parse_metadata.hpp>
#include <abacus/protocol_version.hpp>
#include <abacus/view.hpp>

TEST(test_metrics, empty)
{
    abacus::metrics metrics;

    EXPECT_EQ(0U, metrics.metadata().metrics().size());

    std::map<abacus::name, abacus::info> infos;
    abacus::metrics metrics2(infos);

    EXPECT_EQ(0U, metrics2.metadata().metrics().size());
}

enum class test_enum
{
    value0 = 0,
    value1 = 1,
    value2 = 2,
    value3 = 3
};

TEST(test_metrics, api)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";
    std::string name2 = "metric2";
    std::string name3 = "metric3";
    std::string name4 = "metric4";
    std::string name5 = "metric5";
    std::string name6 = "metric6";
    std::string name7 = "metric7";

    std::map<abacus::name, abacus::info> infos = {
        {abacus::name{name0},
         abacus::boolean{abacus::description{"A boolean metric"}}},
        {abacus::name{name1},
         abacus::uint64{abacus::kind::counter,
                        abacus::description{"An unsigned integer metric"},
                        abacus::unit{"bytes"}}},
        {abacus::name{name2},
         abacus::int64{abacus::kind::gauge,
                       abacus::description{"A signed integer metric"},
                       abacus::unit{"USD"}}},
        {abacus::name{name3},
         abacus::float64{abacus::kind::gauge,
                         abacus::description{"A floating point metric"},
                         abacus::unit{"ms"}}},
        {abacus::name{name4},
         abacus::constant{abacus::constant::boolean{true},
                          abacus::description{"A constant boolean metric"}}},
        {abacus::name{name5},
         abacus::constant{
             abacus::constant::float64{42.42},
             abacus::description{"A constant floating point metric"},
             abacus::unit{"ms"}}},
        {abacus::name{name6},
         abacus::enum8{abacus::description{"An enum metric"},
                       {{test_enum::value0, {"value0", "The value for 0"}},
                        {test_enum::value1, {"value1", "The value for 1"}},
                        {test_enum::value2, {"value2", "The value for 2"}},
                        {test_enum::value3, {"value3", "The value for 3"}}}}},
        {abacus::name{name7},
         abacus::constant{abacus::constant::str{"hello"},
                          abacus::description{"A string metric"}}}};

    abacus::metrics from_metrics(infos);
    abacus::metrics metrics(std::move(from_metrics));
    EXPECT_EQ(metrics.metadata().metrics().size(), 8U);
    EXPECT_EQ(metrics.metadata().protocol_version(),
              abacus::protocol_version());

    EXPECT_EQ(metrics.metadata().metrics().at(name0).boolean().description(),
              "A boolean metric");

    EXPECT_EQ(metrics.metadata().metrics().at(name0).boolean().unit(),
              ""); // empty unit

    EXPECT_EQ(metrics.metadata().metrics().at(name1).uint64().kind(),
              abacus::protobuf::Kind::COUNTER);
    EXPECT_EQ(metrics.metadata().metrics().at(name1).uint64().description(),
              "An unsigned integer metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name1).uint64().unit(), "bytes");

    EXPECT_EQ(metrics.metadata().metrics().at(name2).int64().kind(),
              abacus::protobuf::Kind::GAUGE);
    EXPECT_EQ(metrics.metadata().metrics().at(name2).int64().description(),
              "A signed integer metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name2).int64().unit(), "USD");

    EXPECT_EQ(metrics.metadata().metrics().at(name3).float64().kind(),
              abacus::protobuf::Kind::GAUGE);
    EXPECT_EQ(metrics.metadata().metrics().at(name3).float64().description(),
              "A floating point metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name3).float64().unit(), "ms");

    EXPECT_EQ(metrics.metadata().metrics().at(name4).constant().description(),
              "A constant boolean metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name4).constant().unit(),
              ""); // empty unit

    EXPECT_EQ(metrics.metadata().metrics().at(name5).constant().description(),
              "A constant floating point metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name5).constant().unit(), "ms");

    EXPECT_EQ(metrics.metadata().metrics().at(name6).enum8().description(),
              "An enum metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name6).enum8().values().size(),
              4U);
    EXPECT_EQ(
        metrics.metadata().metrics().at(name6).enum8().values().at(0).name(),
        "value0");
    EXPECT_EQ(metrics.metadata()
                  .metrics()
                  .at(name6)
                  .enum8()
                  .values()
                  .at(0)
                  .description(),
              "The value for 0");

    EXPECT_EQ(metrics.metadata().metrics().at(name7).constant().description(),
              "A string metric");

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name1));
    auto metric1 = metrics.initialize<abacus::uint64>(name1).set_value(9000U);
    EXPECT_TRUE(metrics.is_initialized(name1));

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name2));
    auto metric2 = metrics.initialize<abacus::int64>(name2);
    EXPECT_TRUE(metrics.is_initialized(name2));

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name3));
    auto metric3 = metrics.initialize<abacus::float64>(name3);
    EXPECT_TRUE(metrics.is_initialized(name3));

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name0));
    auto metric0 = metrics.initialize<abacus::boolean>(name0);
    EXPECT_TRUE(metrics.is_initialized(name0));

    EXPECT_TRUE(metrics.is_initialized(name4));
    EXPECT_TRUE(metrics.is_initialized(name5));

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name6));
    auto metric6 = metrics.initialize<abacus::enum8>(name6);
    EXPECT_TRUE(metrics.is_initialized(name6));

    EXPECT_TRUE(metrics.is_initialized(name7));

    EXPECT_TRUE(metrics.is_initialized());

    EXPECT_EQ(metric1.value(), 9000U);
    metric1 = 4U;
    EXPECT_EQ(metric1.value(), 4U);

    EXPECT_FALSE(metric2.has_value());
    metric2 = -4;
    EXPECT_TRUE(metric2.has_value());
    EXPECT_EQ(metric2.value(), -4);

    EXPECT_FALSE(metric3.has_value());
    metric3 = 3.14;
    EXPECT_TRUE(metric3.has_value());
    EXPECT_EQ(metric3.value(), 3.14);

    EXPECT_FALSE(metric0.has_value());
    metric0 = true;
    EXPECT_TRUE(metric0.has_value());
    EXPECT_EQ(metric0.value(), true);

    EXPECT_FALSE(metric6.has_value());
    metric6 = test_enum::value2;
    EXPECT_TRUE(metric6.has_value());
    EXPECT_EQ(metric6.value<test_enum>(), test_enum::value2);
}

TEST(test_metrics, value_and_metadata_bytes)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";

    std::map<abacus::name, abacus::info> infos = {
        {abacus::name{name0},
         abacus::uint64{abacus::kind::counter,
                        abacus::description{"An unsigned integer metric"},
                        abacus::unit{"bytes"}}},
        {abacus::name{name1},
         abacus::int64{abacus::kind::gauge,
                       abacus::description{"A signed integer metric"},
                       abacus::unit{"USD"}}}};

    abacus::metrics metrics{infos};

    auto m0 = metrics.initialize<abacus::uint64>(name0);
    auto m1 = metrics.initialize<abacus::int64>(name1);
    (void)m0;
    (void)m1;

    EXPECT_EQ(metrics.metadata().ByteSizeLong(), 108U);
    EXPECT_EQ(metrics.value_bytes(),
              sizeof(uint32_t) +         // hash
                  1 + sizeof(uint64_t) + // metric0 has_value + value
                  1 + sizeof(int64_t)    // metric1 has_value + value
    );
}

TEST(test_metrics, reset_counters)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";

    std::map<abacus::name, abacus::info> infos = {
        {abacus::name{name0},
         abacus::uint64{abacus::kind::counter,
                        abacus::description{"An unsigned integer metric"},
                        abacus::unit{"bytes"}}},
        {abacus::name{name1},
         abacus::int64{abacus::kind::gauge,
                       abacus::description{"A signed integer metric"},
                       abacus::unit{"USD"}}}};

    abacus::metrics metrics{infos};

    auto uint_metric = metrics.initialize<abacus::uint64>(name0);
    auto int_metric = metrics.initialize<abacus::int64>(name1);

    EXPECT_FALSE(uint_metric.has_value());
    EXPECT_FALSE(int_metric.has_value());
    uint_metric = 4U;
    int_metric = -4;

    EXPECT_TRUE(uint_metric.has_value());
    EXPECT_TRUE(int_metric.has_value());
    EXPECT_EQ(uint_metric.value(), 4U);
    EXPECT_EQ(int_metric.value(), -4);

    uint_metric.reset();
    int_metric.reset();

    EXPECT_FALSE(uint_metric.has_value());
    EXPECT_FALSE(int_metric.has_value());

    uint_metric = 4U;
    int_metric = -4;

    EXPECT_TRUE(uint_metric.has_value());
    EXPECT_TRUE(int_metric.has_value());
    EXPECT_EQ(uint_metric.value(), 4U);
    EXPECT_EQ(int_metric.value(), -4);
}

static const std::vector<uint8_t> expected_metadata = {
    0x08, 0x02, 0x1d, 0xb0, 0xd7, 0xc6, 0x24, 0x22, 0x21, 0x0a, 0x07, 0x6d,
    0x65, 0x74, 0x72, 0x69, 0x63, 0x33, 0x12, 0x16, 0x42, 0x14, 0x08, 0x1f,
    0x12, 0x10, 0x41, 0x20, 0x62, 0x6f, 0x6f, 0x6c, 0x65, 0x61, 0x6e, 0x20,
    0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x22, 0x2c, 0x0a, 0x07, 0x6d, 0x65,
    0x74, 0x72, 0x69, 0x63, 0x32, 0x12, 0x21, 0x32, 0x1f, 0x08, 0x16, 0x12,
    0x17, 0x41, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x69, 0x6e, 0x67, 0x20,
    0x70, 0x6f, 0x69, 0x6e, 0x74, 0x20, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63,
    0x22, 0x02, 0x6d, 0x73, 0x22, 0x2d, 0x0a, 0x07, 0x6d, 0x65, 0x74, 0x72,
    0x69, 0x63, 0x31, 0x12, 0x22, 0x1a, 0x20, 0x08, 0x0d, 0x12, 0x17, 0x41,
    0x20, 0x73, 0x69, 0x67, 0x6e, 0x65, 0x64, 0x20, 0x69, 0x6e, 0x74, 0x65,
    0x67, 0x65, 0x72, 0x20, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x22, 0x03,
    0x55, 0x53, 0x44, 0x22, 0x34, 0x0a, 0x07, 0x6d, 0x65, 0x74, 0x72, 0x69,
    0x63, 0x30, 0x12, 0x29, 0x12, 0x27, 0x08, 0x04, 0x12, 0x1a, 0x41, 0x6e,
    0x20, 0x75, 0x6e, 0x73, 0x69, 0x67, 0x6e, 0x65, 0x64, 0x20, 0x69, 0x6e,
    0x74, 0x65, 0x67, 0x65, 0x72, 0x20, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63,
    0x18, 0x01, 0x22, 0x05, 0x62, 0x79, 0x74, 0x65, 0x73,
};

static const std::vector<uint8_t> expected_value_data = {
    0x01, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0xd6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xc0, 0x61, 0x40, 0x01, 0x01,
};

TEST(test_metrics, protocol_version)
{
    SCOPED_TRACE(::testing::Message()
                 << "protocol version: " << abacus::protocol_version());
    SCOPED_TRACE(
        ::testing::Message()
        << "If this test fails, you need to update the protocol version");
    std::map<abacus::name, abacus::info> infos = {
        {abacus::name{"metric0"},
         abacus::uint64{abacus::kind::counter,
                        abacus::description{"An unsigned integer metric"},
                        abacus::unit{"bytes"}}},
        {abacus::name{"metric1"},
         abacus::int64{abacus::kind::gauge,
                       abacus::description{"A signed integer metric"},
                       abacus::unit{"USD"}}},
        {abacus::name{"metric2"},
         abacus::float64{abacus::kind::gauge,
                         abacus::description{"A floating point metric"},
                         abacus::unit{"ms"}}},
        {abacus::name{"metric3"},
         abacus::boolean{abacus::description{"A boolean metric"}}}};

    abacus::metrics metrics(infos);

    auto uint_metric =
        metrics.initialize<abacus::uint64>("metric0").set_value(42);
    auto int_metric =
        metrics.initialize<abacus::int64>("metric1").set_value(-42);
    auto float_metric =
        metrics.initialize<abacus::float64>("metric2").set_value(142.0);
    auto bool_metric =
        metrics.initialize<abacus::boolean>("metric3").set_value(true);

    (void)uint_metric;
    (void)int_metric;
    (void)float_metric;
    (void)bool_metric;

    EXPECT_EQ(metrics.metadata().ByteSizeLong(), expected_metadata.size());
    {
        // Print the metadata as hex
        std::stringstream meta_stream;
        meta_stream << std::hex;

        auto metadata = metrics.metadata();
        std::vector<uint8_t> metadata_data(metadata.ByteSizeLong());
        metadata.SerializeToArray(metadata_data.data(), metadata_data.size());

        for (auto byte : metadata_data)
        {
            meta_stream << "0x" << std::setw(2) << std::setfill('0')
                        << static_cast<int>(byte) << ", ";
        }
        SCOPED_TRACE(::testing::Message() << "Meta data:\n"
                                          << meta_stream.str());

        auto expected = abacus::parse_metadata(expected_metadata.data(),
                                               expected_metadata.size());
        ASSERT_TRUE(expected.has_value());
        auto actual = metrics.metadata();

        expected.value().set_sync_value(1);
        actual.set_sync_value(1);

        ASSERT_TRUE(expected.value().IsInitialized());
        ASSERT_TRUE(actual.IsInitialized());
        EXPECT_TRUE(google::protobuf::util::MessageDifferencer::Equals(
            expected.value(), actual));
    }

    EXPECT_EQ(metrics.value_bytes() - 4U, expected_value_data.size());

    {
        std::stringstream value_stream;
        value_stream << std::hex;
        for (std::size_t i = 4; i < metrics.value_bytes(); ++i)
        {
            value_stream << "0x" << std::setw(2) << std::setfill('0')
                         << static_cast<int>(metrics.value_data()[i]) << ", ";
        }
        SCOPED_TRACE(::testing::Message() << "Value data:\n"
                                          << value_stream.str());
        EXPECT_EQ(
            expected_value_data,
            std::vector<uint8_t>(metrics.value_data() + 4,
                                 metrics.value_data() + metrics.value_bytes()));
    }
}

TEST(test_metrics, reset)
{
    // Create one of each metric
    std::map<abacus::name, abacus::info> infos = {
        {abacus::name{"uint64"},
         abacus::uint64{abacus::kind::counter, abacus::description{""}}},
        {abacus::name{"uint32"},
         abacus::uint32{abacus::kind::counter, abacus::description{""}}},
        {abacus::name{"int64"},
         abacus::int64{abacus::kind::gauge, abacus::description{""}}},
        {abacus::name{"int32"},
         abacus::int32{abacus::kind::gauge, abacus::description{""}}},
        {abacus::name{"float64"},
         abacus::float64{abacus::kind::gauge, abacus::description{""}}},
        {abacus::name{"float32"},
         abacus::float32{abacus::kind::gauge, abacus::description{""}}},
        {abacus::name{"boolean"}, abacus::boolean{abacus::description{""}}},
        {abacus::name{"enum8"},
         abacus::enum8{abacus::description{""}, {{0, {"", ""}}}}},
        {abacus::name{"uint64_constant"},
         abacus::constant{abacus::constant::uint64{1111},
                          abacus::description{""}}},
        {abacus::name{"int64_constant"},
         abacus::constant{abacus::constant::int64{2222},
                          abacus::description{""}}},
        {abacus::name{"float64_constant"},
         abacus::constant{abacus::constant::float64{3333.0},
                          abacus::description{""}}},
        {abacus::name{"boolean_constant"},
         abacus::constant{abacus::constant::boolean{true},
                          abacus::description{""}}},
        {abacus::name{"string_constant"},
         abacus::constant{abacus::constant::str{"fivefivefivefive"},
                          abacus::description{""}}},
        // Finally a metric that we do not initialize
        {abacus::name{"not_initialized"},
         abacus::uint64{abacus::kind::counter, abacus::description{""}}}};

    abacus::metrics metrics(infos);

    // Initialize metrics
    auto uint64 = metrics.initialize<abacus::uint64>("uint64");
    auto uint32 = metrics.initialize<abacus::uint32>("uint32");
    auto int64 = metrics.initialize<abacus::int64>("int64");
    auto int32 = metrics.initialize<abacus::int32>("int32");
    auto float64 = metrics.initialize<abacus::float64>("float64");
    auto float32 = metrics.initialize<abacus::float32>("float32");
    auto boolean = metrics.initialize<abacus::boolean>("boolean");
    auto enum8 = metrics.initialize<abacus::enum8>("enum8");

    // Check that all metric returns false for has_value
    EXPECT_FALSE(uint64.has_value());
    EXPECT_FALSE(uint32.has_value());
    EXPECT_FALSE(int64.has_value());
    EXPECT_FALSE(int32.has_value());
    EXPECT_FALSE(float64.has_value());
    EXPECT_FALSE(float32.has_value());
    EXPECT_FALSE(boolean.has_value());
    EXPECT_FALSE(enum8.has_value());

    // Set all values to some value
    uint64 = 11U;
    uint32 = 22U;
    int64 = 33;
    int32 = 44;
    float64 = 55.0;
    float32 = 66.0;
    boolean = false;
    enum8 = test_enum::value1;

    // Check all values
    EXPECT_EQ(uint64.value(), 11U);
    EXPECT_EQ(uint32.value(), 22U);
    EXPECT_EQ(int64.value(), 33);
    EXPECT_EQ(int32.value(), 44);
    EXPECT_EQ(float64.value(), 55.0);
    EXPECT_EQ(float32.value(), 66.0);
    EXPECT_EQ(boolean.value(), false);
    EXPECT_EQ(enum8.value<test_enum>(), test_enum::value1);

    // Change and check all values (now using set_value function)
    uint64.set_value(111U);
    uint32.set_value(222U);
    int64.set_value(333);
    int32.set_value(444);
    float64.set_value(555.0);
    float32.set_value(666.0);
    boolean.set_value(true);
    enum8.set_value(test_enum::value2);

    EXPECT_EQ(uint64.value(), 111U);
    EXPECT_EQ(uint32.value(), 222U);
    EXPECT_EQ(int64.value(), 333);
    EXPECT_EQ(int32.value(), 444);
    EXPECT_EQ(float64.value(), 555.0);
    EXPECT_EQ(float32.value(), 666.0);
    EXPECT_EQ(boolean.value(), true);
    EXPECT_EQ(enum8.value<test_enum>(), test_enum::value2);

    // Create a view to see the constants
    abacus::view view;
    ASSERT_TRUE(view.set_metadata(metrics.metadata()));
    ASSERT_TRUE(
        view.set_value_data(metrics.value_data(), metrics.value_bytes()));

    // Check all constant values
    EXPECT_EQ(view.value<abacus::constant::uint64>("uint64_constant"), 1111U);
    EXPECT_EQ(view.value<abacus::constant::int64>("int64_constant"), 2222);
    EXPECT_EQ(view.value<abacus::constant::float64>("float64_constant"),
              3333.0);
    EXPECT_EQ(view.value<abacus::constant::boolean>("boolean_constant"), true);
    EXPECT_EQ(view.value<abacus::constant::str>("string_constant"),
              "fivefivefivefive");

    // While we are at it, let's check the other values as well
    EXPECT_EQ(view.value<abacus::uint64>("uint64").value(), 111U);
    EXPECT_EQ(view.value<abacus::uint32>("uint32").value(), 222U);
    EXPECT_EQ(view.value<abacus::int64>("int64").value(), 333);
    EXPECT_EQ(view.value<abacus::int32>("int32").value(), 444);
    EXPECT_EQ(view.value<abacus::float64>("float64").value(), 555.0);
    EXPECT_EQ(view.value<abacus::float32>("float32").value(), 666.0);
    EXPECT_EQ(view.value<abacus::boolean>("boolean").value(), true);
    EXPECT_EQ(view.value<abacus::enum8>("enum8").value(), 2U);

    // And finally the not initialized value
    EXPECT_FALSE(view.value<abacus::uint64>("not_initialized").has_value());

    // Reset all values
    metrics.reset();

    // Check all values returns false for has_value
    EXPECT_FALSE(uint64.has_value());
    EXPECT_FALSE(uint32.has_value());
    EXPECT_FALSE(int64.has_value());
    EXPECT_FALSE(int32.has_value());
    EXPECT_FALSE(float64.has_value());
    EXPECT_FALSE(float32.has_value());
    EXPECT_FALSE(boolean.has_value());
    EXPECT_FALSE(enum8.has_value());

    // Update view and check all constant values
    ASSERT_TRUE(
        view.set_value_data(metrics.value_data(), metrics.value_bytes()));

    // Check all constant values
    EXPECT_EQ(view.value<abacus::constant::uint64>("uint64_constant"), 1111U);
    EXPECT_EQ(view.value<abacus::constant::int64>("int64_constant"), 2222);
    EXPECT_EQ(view.value<abacus::constant::float64>("float64_constant"),
              3333.0);
    EXPECT_EQ(view.value<abacus::constant::boolean>("boolean_constant"), true);
    EXPECT_EQ(view.value<abacus::constant::str>("string_constant"),
              "fivefivefivefive");

    EXPECT_FALSE(view.value<abacus::uint64>("uint64").has_value());
    EXPECT_FALSE(view.value<abacus::uint32>("uint32").has_value());
    EXPECT_FALSE(view.value<abacus::int64>("int64").has_value());
    EXPECT_FALSE(view.value<abacus::int32>("int32").has_value());
    EXPECT_FALSE(view.value<abacus::float64>("float64").has_value());
    EXPECT_FALSE(view.value<abacus::float32>("float32").has_value());
    EXPECT_FALSE(view.value<abacus::boolean>("boolean").has_value());
    EXPECT_FALSE(view.value<abacus::enum8>("enum8").has_value());

    EXPECT_FALSE(view.value<abacus::uint64>("not_initialized").has_value());
}
