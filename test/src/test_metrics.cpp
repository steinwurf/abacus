// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <google/protobuf/util/message_differencer.h>

#include <abacus/metrics.hpp>
#include <abacus/parse.hpp>
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

TEST(test_metrics, api)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";
    std::string name2 = "metric2";
    std::string name3 = "metric3";
    std::string name4 = "metric4";
    std::string name5 = "metric5";
    std::string name6 = "metric6";

    std::map<abacus::name, abacus::info> infos = {
        {abacus::name{name0},
         abacus::boolean{abacus::gauge{abacus::optional},
                         abacus::description{"A boolean metric"}}},
        {abacus::name{name1},
         abacus::uint64{abacus::counter{abacus::required},
                        abacus::description{"An unsigned integer metric"},
                        abacus::unit{"bytes"}}},
        {abacus::name{name2},
         abacus::int64{abacus::gauge{abacus::optional},
                       abacus::description{"A signed integer metric"},
                       abacus::unit{"USD"}}},
        {abacus::name{name3},
         abacus::float64{abacus::gauge{abacus::optional},
                         abacus::description{"A floating point metric"},
                         abacus::unit{"ms"}}},
        {abacus::name{name4},
         abacus::boolean{abacus::constant{},
                         abacus::description{"A constant boolean metric"}}},
        {abacus::name{name5},
         abacus::float64{
             abacus::constant{},
             abacus::description{"A constant floating point metric"},
             abacus::unit{"ms"}}},
        {abacus::name{name6},
         abacus::enum8{abacus::gauge{abacus::optional},
                       abacus::description{"An enum metric"},
                       {{0, {"value0", "The value for 0"}},
                        {1, {"value1", "The value for 1"}},
                        {2, {"value2", "The value for 2"}},
                        {3, {"value3", "The value for 3"}}}}}};

    abacus::metrics from_metrics(infos);
    abacus::metrics metrics(std::move(from_metrics));
    EXPECT_EQ(metrics.metadata().metrics().size(), 7U);
    EXPECT_EQ(metrics.metadata().protocol_version(),
              abacus::protocol_version());

    EXPECT_EQ(metrics.metadata().metrics().at(name0).boolean().kind_case(),
              abacus::protobuf::BoolMetric::KindCase::kGauge);
    EXPECT_EQ(metrics.metadata().metrics().at(name0).boolean().description(),
              "A boolean metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name0).boolean().unit(),
              ""); // empty unit

    EXPECT_EQ(metrics.metadata().metrics().at(name1).uint64().kind_case(),
              abacus::protobuf::UInt64Metric::KindCase::kCounter);
    EXPECT_EQ(metrics.metadata().metrics().at(name1).uint64().description(),
              "An unsigned integer metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name1).uint64().unit(), "bytes");

    EXPECT_EQ(metrics.metadata().metrics().at(name2).int64().kind_case(),
              abacus::protobuf::Int64Metric::KindCase::kGauge);
    EXPECT_EQ(metrics.metadata().metrics().at(name2).int64().description(),
              "A signed integer metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name2).int64().unit(), "USD");

    EXPECT_EQ(metrics.metadata().metrics().at(name3).float64().kind_case(),
              abacus::protobuf::Float64Metric::KindCase::kGauge);
    EXPECT_EQ(metrics.metadata().metrics().at(name3).float64().description(),
              "A floating point metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name3).float64().unit(), "ms");

    EXPECT_EQ(metrics.metadata().metrics().at(name4).boolean().kind_case(),
              abacus::protobuf::BoolMetric::KindCase::kConstant);
    EXPECT_EQ(metrics.metadata().metrics().at(name4).boolean().description(),
              "A constant boolean metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name4).boolean().unit(),
              ""); // empty unit
    EXPECT_EQ(metrics.metadata().metrics().at(name5).float64().kind_case(),
              abacus::protobuf::Float64Metric::KindCase::kConstant);

    EXPECT_EQ(metrics.metadata().metrics().at(name5).float64().description(),
              "A constant floating point metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name5).float64().unit(), "ms");

    EXPECT_EQ(metrics.metadata().metrics().at(name6).enum8().kind_case(),
              abacus::protobuf::Enum8Metric::KindCase::kGauge);
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

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name1));
    auto metric1 = metrics.initialize_required<abacus::uint64>(name1, 9000U);
    EXPECT_TRUE(metrics.is_initialized(name1));

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name2));
    auto metric2 = metrics.initialize_optional<abacus::int64>(name2);
    EXPECT_TRUE(metrics.is_initialized(name2));

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name3));
    auto metric3 = metrics.initialize_optional<abacus::float64>(name3);
    EXPECT_TRUE(metrics.is_initialized(name3));

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name0));
    auto metric0 = metrics.initialize_optional<abacus::boolean>(name0);
    EXPECT_TRUE(metrics.is_initialized(name0));

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name4));
    metrics.initialize_constant<abacus::boolean>(name4, true);
    EXPECT_TRUE(metrics.is_initialized(name4));

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name5));
    metrics.initialize_constant<abacus::float64>(name5, 42.42);
    EXPECT_TRUE(metrics.is_initialized(name5));

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name6));
    auto metric6 = metrics.initialize_optional<abacus::enum8>(name6);
    EXPECT_TRUE(metrics.is_initialized(name6));

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
    metric6 = 2;
    EXPECT_TRUE(metric6.has_value());
    EXPECT_EQ(metric6.value(), 2);
}

TEST(test_metrics, value_and_metadata_bytes)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";

    std::map<abacus::name, abacus::info> infos = {
        {abacus::name{name0},
         abacus::uint64{abacus::counter{abacus::optional},
                        abacus::description{"An unsigned integer metric"},
                        abacus::unit{"bytes"}}},
        {abacus::name{name1},
         abacus::int64{abacus::gauge{abacus::optional},
                       abacus::description{"A signed integer metric"},
                       abacus::unit{"USD"}}}};

    abacus::metrics metrics{infos};

    auto m0 = metrics.initialize_optional<abacus::uint64>(name0);
    auto m1 = metrics.initialize_optional<abacus::int64>(name1);
    (void)m0;
    (void)m1;

    EXPECT_EQ(metrics.metadata().ByteSizeLong(), 114U);
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
         abacus::uint64{abacus::counter{abacus::optional},
                        abacus::description{"An unsigned integer metric"},
                        abacus::unit{"bytes"}}},
        {abacus::name{name1},
         abacus::int64{abacus::gauge{abacus::optional},
                       abacus::description{"A signed integer metric"},
                       abacus::unit{"USD"}}}};

    abacus::metrics metrics{infos};

    auto uint_metric = metrics.initialize_optional<abacus::uint64>(name0);
    auto int_metric = metrics.initialize_optional<abacus::int64>(name1);

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
    0x08, 0x02, 0x1d, 0x57, 0x84, 0xe9, 0x44, 0x22, 0x2f, 0x0a, 0x07, 0x6d,
    0x65, 0x74, 0x72, 0x69, 0x63, 0x31, 0x12, 0x24, 0x08, 0x0d, 0x22, 0x20,
    0x0a, 0x17, 0x41, 0x20, 0x73, 0x69, 0x67, 0x6e, 0x65, 0x64, 0x20, 0x69,
    0x6e, 0x74, 0x65, 0x67, 0x65, 0x72, 0x20, 0x6d, 0x65, 0x74, 0x72, 0x69,
    0x63, 0x12, 0x00, 0x2a, 0x03, 0x55, 0x53, 0x44, 0x22, 0x23, 0x0a, 0x07,
    0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x33, 0x12, 0x18, 0x08, 0x1f, 0x4a,
    0x14, 0x0a, 0x10, 0x41, 0x20, 0x62, 0x6f, 0x6f, 0x6c, 0x65, 0x61, 0x6e,
    0x20, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x12, 0x00, 0x22, 0x34, 0x0a,
    0x07, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x30, 0x12, 0x29, 0x08, 0x04,
    0x1a, 0x25, 0x0a, 0x1a, 0x41, 0x6e, 0x20, 0x75, 0x6e, 0x73, 0x69, 0x67,
    0x6e, 0x65, 0x64, 0x20, 0x69, 0x6e, 0x74, 0x65, 0x67, 0x65, 0x72, 0x20,
    0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x1a, 0x00, 0x2a, 0x05, 0x62, 0x79,
    0x74, 0x65, 0x73, 0x22, 0x2e, 0x0a, 0x07, 0x6d, 0x65, 0x74, 0x72, 0x69,
    0x63, 0x32, 0x12, 0x23, 0x08, 0x16, 0x3a, 0x1f, 0x0a, 0x17, 0x41, 0x20,
    0x66, 0x6c, 0x6f, 0x61, 0x74, 0x69, 0x6e, 0x67, 0x20, 0x70, 0x6f, 0x69,
    0x6e, 0x74, 0x20, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x12, 0x00, 0x2a,
    0x02, 0x6d, 0x73};

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
         abacus::uint64{abacus::counter{abacus::required},
                        abacus::description{"An unsigned integer metric"},
                        abacus::unit{"bytes"}}},
        {abacus::name{"metric1"},
         abacus::int64{abacus::gauge{abacus::required},
                       abacus::description{"A signed integer metric"},
                       abacus::unit{"USD"}}},
        {abacus::name{"metric2"},
         abacus::float64{abacus::gauge{abacus::required},
                         abacus::description{"A floating point metric"},
                         abacus::unit{"ms"}}},
        {abacus::name{"metric3"},
         abacus::boolean{abacus::gauge{abacus::required},
                         abacus::description{"A boolean metric"}}}};

    abacus::metrics metrics(infos);

    auto uint_metric =
        metrics.initialize_required<abacus::uint64>("metric0", 42U);
    auto int_metric =
        metrics.initialize_required<abacus::int64>("metric1", -42);
    auto float_metric =
        metrics.initialize_required<abacus::float64>("metric2", 142.0);
    auto bool_metric =
        metrics.initialize_required<abacus::boolean>("metric3", true);

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

        auto expected = abacus::parse::metadata(expected_metadata.data(),
                                                expected_metadata.size());
        auto actual = metrics.metadata();

        expected.set_sync_value(1);
        actual.set_sync_value(1);

        EXPECT_TRUE(google::protobuf::util::MessageDifferencer::Equals(expected,
                                                                       actual));
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
    // Create one of each metric both required and optional
    std::map<abacus::name, abacus::info> infos = {
        {abacus::name{"uint64_required"},
         abacus::uint64{abacus::counter{abacus::required},
                        abacus::description{""}}},
        {abacus::name{"uint64_optional"},
         abacus::uint64{abacus::counter{abacus::optional},
                        abacus::description{""}}},
        {abacus::name{"uint32_required"},
         abacus::uint32{abacus::counter{abacus::required},
                        abacus::description{""}}},
        {abacus::name{"uint32_optional"},
         abacus::uint32{abacus::counter{abacus::optional},
                        abacus::description{""}}},
        {abacus::name{"int64_required"},
         abacus::int64{abacus::gauge{abacus::required},
                       abacus::description{""}}},
        {abacus::name{"int64_optional"},
         abacus::int64{abacus::gauge{abacus::optional},
                       abacus::description{""}}},
        {abacus::name{"int32_required"},
         abacus::int32{abacus::gauge{abacus::required},
                       abacus::description{""}}},
        {abacus::name{"int32_optional"},
         abacus::int32{abacus::gauge{abacus::optional},
                       abacus::description{""}}},
        {abacus::name{"float64_required"},
         abacus::float64{abacus::gauge{abacus::required},
                         abacus::description{""}}},
        {abacus::name{"float64_optional"},
         abacus::float64{abacus::gauge{abacus::optional},
                         abacus::description{""}}},
        {abacus::name{"float32_required"},
         abacus::float32{abacus::gauge{abacus::required},
                         abacus::description{""}}},
        {abacus::name{"float32_optional"},
         abacus::float32{abacus::gauge{abacus::optional},
                         abacus::description{""}}},
        {abacus::name{"boolean_required"},
         abacus::boolean{abacus::gauge{abacus::required},
                         abacus::description{""}}},
        {abacus::name{"boolean_optional"},
         abacus::boolean{abacus::gauge{abacus::optional},
                         abacus::description{""}}},
        {abacus::name{"enum8_required"},
         abacus::enum8{abacus::gauge{abacus::required},
                       abacus::description{""},
                       {{0, {"", ""}}}}},
        {abacus::name{"enum8_optional"},
         abacus::enum8{abacus::gauge{abacus::optional},
                       abacus::description{""},
                       {{0, {"", ""}}}}},
        {abacus::name{"uint64_constant"},
         abacus::uint64{abacus::constant{}, abacus::description{""}}},
        {abacus::name{"uint32_constant"},
         abacus::uint32{abacus::constant{}, abacus::description{""}}},
        {abacus::name{"int64_constant"},
         abacus::int64{abacus::constant{}, abacus::description{""}}},
        {abacus::name{"int32_constant"},
         abacus::int32{abacus::constant{}, abacus::description{""}}},
        {abacus::name{"float64_constant"},
         abacus::float64{abacus::constant{}, abacus::description{""}}},
        {abacus::name{"float32_constant"},
         abacus::float32{abacus::constant{}, abacus::description{""}}},
        {abacus::name{"boolean_constant"},
         abacus::boolean{abacus::constant{}, abacus::description{""}}},
        {abacus::name{"enum8_constant"}, abacus::enum8{abacus::constant{},
                                                       abacus::description{""},
                                                       {{0, {"", ""}}}}},

        // Finally a metric that we do not initialize
        {abacus::name{"not_initialized_required"},
         abacus::uint64{abacus::counter{abacus::required},
                        abacus::description{""}}},
        {abacus::name{"not_initialized_optional"},
         abacus::uint64{abacus::counter{abacus::optional},
                        abacus::description{""}}}};

    abacus::metrics metrics(infos);

    // Inirialize metrics
    auto uint64_required =
        metrics.initialize_required<abacus::uint64>("uint64_required", 1U);
    auto uint64_optional =
        metrics.initialize_optional<abacus::uint64>("uint64_optional");
    auto uint32_required =
        metrics.initialize_required<abacus::uint32>("uint32_required", 2U);
    auto uint32_optional =
        metrics.initialize_optional<abacus::uint32>("uint32_optional");
    auto int64_required =
        metrics.initialize_required<abacus::int64>("int64_required", 3);
    auto int64_optional =
        metrics.initialize_optional<abacus::int64>("int64_optional");
    auto int32_required =
        metrics.initialize_required<abacus::int32>("int32_required", 4);
    auto int32_optional =
        metrics.initialize_optional<abacus::int32>("int32_optional");
    auto float64_required =
        metrics.initialize_required<abacus::float64>("float64_required", 5.0);
    auto float64_optional =
        metrics.initialize_optional<abacus::float64>("float64_optional");
    auto float32_required =
        metrics.initialize_required<abacus::float32>("float32_required", 6.0);
    auto float32_optional =
        metrics.initialize_optional<abacus::float32>("float32_optional");
    auto boolean_required =
        metrics.initialize_required<abacus::boolean>("boolean_required", true);
    auto boolean_optional =
        metrics.initialize_optional<abacus::boolean>("boolean_optional");
    auto enum8_required =
        metrics.initialize_required<abacus::enum8>("enum8_required", 0U);
    auto enum8_optional =
        metrics.initialize_optional<abacus::enum8>("enum8_optional");

    // Initialize constant metrics
    metrics.initialize_constant<abacus::uint64>("uint64_constant", 1111U);
    metrics.initialize_constant<abacus::uint32>("uint32_constant", 2222U);
    metrics.initialize_constant<abacus::int64>("int64_constant", 3333);
    metrics.initialize_constant<abacus::int32>("int32_constant", 4444);
    metrics.initialize_constant<abacus::float64>("float64_constant", 5555.0);
    metrics.initialize_constant<abacus::float32>("float32_constant", 6666.0);
    metrics.initialize_constant<abacus::boolean>("boolean_constant", true);
    metrics.initialize_constant<abacus::enum8>("enum8_constant", 77U);

    // Check all required values
    EXPECT_EQ(uint64_required.value(), 1U);
    EXPECT_EQ(uint32_required.value(), 2U);
    EXPECT_EQ(int64_required.value(), 3);
    EXPECT_EQ(int32_required.value(), 4);
    EXPECT_EQ(float64_required.value(), 5.0);
    EXPECT_EQ(float32_required.value(), 6.0);
    EXPECT_EQ(boolean_required.value(), true);
    EXPECT_EQ(enum8_required.value(), 0U);

    // Check all optional values returns false for has_value
    EXPECT_FALSE(uint64_optional.has_value());
    EXPECT_FALSE(uint32_optional.has_value());
    EXPECT_FALSE(int64_optional.has_value());
    EXPECT_FALSE(int32_optional.has_value());
    EXPECT_FALSE(float64_optional.has_value());
    EXPECT_FALSE(float32_optional.has_value());
    EXPECT_FALSE(boolean_optional.has_value());
    EXPECT_FALSE(enum8_optional.has_value());

    // Set all optional values to some value
    uint64_optional = 11U;
    uint32_optional = 22U;
    int64_optional = 33;
    int32_optional = 44;
    float64_optional = 55.0;
    float32_optional = 66.0;
    boolean_optional = false;
    enum8_optional = 1U;

    // Check all optional values
    EXPECT_EQ(uint64_optional.value(), 11U);
    EXPECT_EQ(uint32_optional.value(), 22U);
    EXPECT_EQ(int64_optional.value(), 33);
    EXPECT_EQ(int32_optional.value(), 44);
    EXPECT_EQ(float64_optional.value(), 55.0);
    EXPECT_EQ(float32_optional.value(), 66.0);
    EXPECT_EQ(boolean_optional.value(), false);
    EXPECT_EQ(enum8_optional.value(), 1U);

    // Change and check all required values
    uint64_required = 111U;
    uint32_required = 222U;
    int64_required = 333;
    int32_required = 444;
    float64_required = 555.0;
    float32_required = 666.0;
    boolean_required = false;
    enum8_required = 2;

    EXPECT_EQ(uint64_required.value(), 111U);
    EXPECT_EQ(uint32_required.value(), 222U);
    EXPECT_EQ(int64_required.value(), 333);
    EXPECT_EQ(int32_required.value(), 444);
    EXPECT_EQ(float64_required.value(), 555.0);
    EXPECT_EQ(float32_required.value(), 666.0);
    EXPECT_EQ(boolean_required.value(), false);
    EXPECT_EQ(enum8_required.value(), 2U);

    // Create a view to see the constants
    abacus::view view;
    ASSERT_TRUE(view.set_metadata(metrics.metadata()));
    ASSERT_TRUE(
        view.set_value_data(metrics.value_data(), metrics.value_bytes()));

    // Check all constant values
    EXPECT_EQ(view.value<abacus::uint64>("uint64_constant").value(), 1111U);
    EXPECT_EQ(view.value<abacus::uint32>("uint32_constant").value(), 2222U);
    EXPECT_EQ(view.value<abacus::int64>("int64_constant").value(), 3333);
    EXPECT_EQ(view.value<abacus::int32>("int32_constant").value(), 4444);
    EXPECT_EQ(view.value<abacus::float64>("float64_constant").value(), 5555.0);
    EXPECT_EQ(view.value<abacus::float32>("float32_constant").value(), 6666.0);
    EXPECT_EQ(view.value<abacus::boolean>("boolean_constant").value(), true);
    EXPECT_EQ(view.value<abacus::enum8>("enum8_constant").value(), 77U);

    // While we are at it, let's check the other values as well
    EXPECT_EQ(view.value<abacus::uint64>("uint64_required").value(), 111U);
    EXPECT_EQ(view.value<abacus::uint32>("uint32_required").value(), 222U);
    EXPECT_EQ(view.value<abacus::int64>("int64_required").value(), 333);
    EXPECT_EQ(view.value<abacus::int32>("int32_required").value(), 444);
    EXPECT_EQ(view.value<abacus::float64>("float64_required").value(), 555.0);
    EXPECT_EQ(view.value<abacus::float32>("float32_required").value(), 666.0);
    EXPECT_EQ(view.value<abacus::boolean>("boolean_required").value(), false);
    EXPECT_EQ(view.value<abacus::enum8>("enum8_required").value(), 2U);

    EXPECT_EQ(view.value<abacus::uint64>("uint64_optional").value(), 11U);
    EXPECT_EQ(view.value<abacus::uint32>("uint32_optional").value(), 22U);
    EXPECT_EQ(view.value<abacus::int64>("int64_optional").value(), 33);
    EXPECT_EQ(view.value<abacus::int32>("int32_optional").value(), 44);
    EXPECT_EQ(view.value<abacus::float64>("float64_optional").value(), 55.0);
    EXPECT_EQ(view.value<abacus::float32>("float32_optional").value(), 66.0);
    EXPECT_EQ(view.value<abacus::boolean>("boolean_optional").value(), false);
    EXPECT_EQ(view.value<abacus::enum8>("enum8_optional").value(), 1U);

    // And finally the not initialized value
    EXPECT_FALSE(
        view.value<abacus::uint64>("not_initialized_required").has_value());
    EXPECT_FALSE(
        view.value<abacus::uint64>("not_initialized_optional").has_value());

    // Reset all values
    metrics.reset();

    // Check all required values
    EXPECT_EQ(uint64_required.value(), 1U);
    EXPECT_EQ(uint32_required.value(), 2U);
    EXPECT_EQ(int64_required.value(), 3);
    EXPECT_EQ(int32_required.value(), 4);
    EXPECT_EQ(float64_required.value(), 5.0);
    EXPECT_EQ(float32_required.value(), 6.0);
    EXPECT_EQ(boolean_required.value(), true);
    EXPECT_EQ(enum8_required.value(), 0U);

    // Check all optional values returns false for has_value
    EXPECT_FALSE(uint64_optional.has_value());
    EXPECT_FALSE(uint32_optional.has_value());
    EXPECT_FALSE(int64_optional.has_value());
    EXPECT_FALSE(int32_optional.has_value());
    EXPECT_FALSE(float64_optional.has_value());
    EXPECT_FALSE(float32_optional.has_value());
    EXPECT_FALSE(boolean_optional.has_value());
    EXPECT_FALSE(enum8_optional.has_value());

    // Update view and check all constant values
    ASSERT_TRUE(
        view.set_value_data(metrics.value_data(), metrics.value_bytes()));

    // Check all constant values
    EXPECT_EQ(view.value<abacus::uint64>("uint64_constant"), 1111U);
    EXPECT_EQ(view.value<abacus::uint32>("uint32_constant"), 2222U);
    EXPECT_EQ(view.value<abacus::int64>("int64_constant"), 3333);
    EXPECT_EQ(view.value<abacus::int32>("int32_constant"), 4444);
    EXPECT_EQ(view.value<abacus::float64>("float64_constant"), 5555.0);
    EXPECT_EQ(view.value<abacus::float32>("float32_constant"), 6666.0);
    EXPECT_EQ(view.value<abacus::boolean>("boolean_constant"), true);

    EXPECT_EQ(view.value<abacus::uint64>("uint64_required").value(), 1U);
    EXPECT_EQ(view.value<abacus::uint32>("uint32_required").value(), 2U);
    EXPECT_EQ(view.value<abacus::int64>("int64_required").value(), 3);
    EXPECT_EQ(view.value<abacus::int32>("int32_required").value(), 4);
    EXPECT_EQ(view.value<abacus::float64>("float64_required").value(), 5.0);
    EXPECT_EQ(view.value<abacus::float32>("float32_required").value(), 6.0);
    EXPECT_EQ(view.value<abacus::boolean>("boolean_required").value(), true);
    EXPECT_EQ(view.value<abacus::enum8>("enum8_required").value(), 0U);

    EXPECT_FALSE(view.value<abacus::uint64>("uint64_optional").has_value());
    EXPECT_FALSE(view.value<abacus::uint32>("uint32_optional").has_value());
    EXPECT_FALSE(view.value<abacus::int64>("int64_optional").has_value());
    EXPECT_FALSE(view.value<abacus::int32>("int32_optional").has_value());
    EXPECT_FALSE(view.value<abacus::float64>("float64_optional").has_value());
    EXPECT_FALSE(view.value<abacus::float32>("float32_optional").has_value());
    EXPECT_FALSE(view.value<abacus::boolean>("boolean_optional").has_value());
    EXPECT_FALSE(view.value<abacus::enum8>("enum8_optional").has_value());

    EXPECT_FALSE(
        view.value<abacus::uint64>("not_initialized_required").has_value());
    EXPECT_FALSE(
        view.value<abacus::uint64>("not_initialized_optional").has_value());
}
