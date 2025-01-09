// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <google/protobuf/message.h>

#include <abacus/metrics.hpp>
#include <abacus/protocol_version.hpp>

TEST(test_metrics, empty)
{
    abacus::metrics metrics;

    EXPECT_EQ(0U, metrics.metadata().metrics().size());

    std::map<std::string, abacus::type> infos;
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

    std::map<std::string, abacus::type> infos = {
        {name0, abacus::boolean{abacus::kind::COUNTER, "A boolean metric"}},
        {name1,
         abacus::uint64{abacus::kind::COUNTER, "An unsigned integer metric",
                        abacus::unit{"bytes"}}},
        {name2, abacus::int64{abacus::kind::GAUGE, "A signed integer metric",
                              abacus::unit{"USD"}}},
        {name3, abacus::float64{abacus::kind::GAUGE, "A floating point metric",
                                abacus::unit{"ms"}}},
        {name4,
         abacus::boolean{abacus::kind::CONSTANT, "A constant boolean metric"}},
        {name5, abacus::float64{abacus::kind::CONSTANT,
                                "A constant floating point metric",
                                abacus::unit{"ms"}}},
        {name6, abacus::enum8{"An enum metric",
                              {{0, {"value0", "The value for 0"}},
                               {1, {"value1", "The value for 1"}},
                               {2, {"value2", "The value for 2"}},
                               {3, {"value3", "The value for 3"}}}}}};

    abacus::metrics from_metrics(infos);
    abacus::metrics metrics(std::move(from_metrics));
    EXPECT_EQ(metrics.metadata().metrics().size(), 7U);
    EXPECT_EQ(metrics.metadata().protocol_version(),
              abacus::protocol_version());

    EXPECT_EQ(metrics.metadata().metrics().at(name0).boolean().kind(),
              abacus::kind::COUNTER);
    EXPECT_EQ(metrics.metadata().metrics().at(name0).boolean().description(),
              "A boolean metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name0).boolean().unit(),
              ""); // empty unit

    EXPECT_EQ(metrics.metadata().metrics().at(name1).uint64().kind(),
              abacus::kind::COUNTER);
    EXPECT_EQ(metrics.metadata().metrics().at(name1).uint64().description(),
              "An unsigned integer metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name1).uint64().unit(), "bytes");

    EXPECT_EQ(metrics.metadata().metrics().at(name2).int64().kind(),
              abacus::kind::GAUGE);
    EXPECT_EQ(metrics.metadata().metrics().at(name2).int64().description(),
              "A signed integer metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name2).int64().unit(), "USD");

    EXPECT_EQ(metrics.metadata().metrics().at(name3).float64().kind(),
              abacus::kind::GAUGE);
    EXPECT_EQ(metrics.metadata().metrics().at(name3).float64().description(),
              "A floating point metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name3).float64().unit(), "ms");

    EXPECT_EQ(metrics.metadata().metrics().at(name4).boolean().kind(),
              abacus::kind::CONSTANT);
    EXPECT_EQ(metrics.metadata().metrics().at(name4).boolean().description(),
              "A constant boolean metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name4).boolean().unit(),
              ""); // empty unit

    EXPECT_EQ(metrics.metadata().metrics().at(name5).float64().kind(),
              abacus::kind::CONSTANT);
    EXPECT_EQ(metrics.metadata().metrics().at(name5).float64().description(),
              "A constant floating point metric");
    EXPECT_EQ(metrics.metadata().metrics().at(name5).float64().unit(), "ms");

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
    auto metric0 = metrics.initialize_metric<abacus::uint64>(name1, 9000U);
    EXPECT_TRUE(metrics.is_initialized(name1));

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name2));
    auto metric1 = metrics.initialize_metric<abacus::int64>(name2);
    EXPECT_TRUE(metrics.is_initialized(name2));

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name3));
    auto metric2 = metrics.initialize_metric<abacus::float64>(name3);
    EXPECT_TRUE(metrics.is_initialized(name3));

    EXPECT_FALSE(metrics.is_initialized());
    EXPECT_FALSE(metrics.is_initialized(name0));
    auto metric3 = metrics.initialize_metric<abacus::boolean>(name0);
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
    auto metric6 = metrics.initialize_metric<abacus::enum8>(name6);
    EXPECT_TRUE(metrics.is_initialized(name6));

    EXPECT_TRUE(metrics.is_initialized());

    EXPECT_TRUE(metric0.has_value());
    EXPECT_EQ(metric0.value(), 9000U);
    metric0 = 4U;
    EXPECT_TRUE(metric0.has_value());
    EXPECT_EQ(metric0.value(), 4U);

    EXPECT_FALSE(metric1.has_value());
    metric1 = -4;
    EXPECT_TRUE(metric1.has_value());
    EXPECT_EQ(metric1.value(), -4);

    EXPECT_FALSE(metric2.has_value());
    metric2 = 3.14;
    EXPECT_TRUE(metric2.has_value());
    EXPECT_EQ(metric2.value(), 3.14);

    EXPECT_FALSE(metric3.has_value());
    metric3 = true;
    EXPECT_TRUE(metric3.has_value());
    EXPECT_EQ(metric3.value(), true);

    EXPECT_FALSE(metric6.has_value());
    metric6 = 2;
    EXPECT_TRUE(metric6.has_value());
    EXPECT_EQ(metric6.value(), 2);
}

TEST(test_metrics, value_and_metadata_bytes)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";

    std::map<std::string, abacus::type> infos = {
        {name0,
         abacus::uint64{abacus::kind::COUNTER, "An unsigned integer metric",
                        abacus::unit{"bytes"}}},
        {name1, abacus::int64{abacus::kind::GAUGE, "A signed integer metric",
                              abacus::unit{"USD"}}}};

    abacus::metrics metrics{infos};

    auto m0 = metrics.initialize_metric<abacus::uint64>(name0);
    auto m1 = metrics.initialize_metric<abacus::int64>(name1);
    (void)m0;
    (void)m1;

    EXPECT_EQ(metrics.metadata_bytes(), 108U);
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

    std::map<std::string, abacus::type> infos = {
        {name0,
         abacus::uint64{abacus::kind::COUNTER, "An unsigned integer metric",
                        abacus::unit{"bytes"}}},
        {name1, abacus::int64{abacus::kind::GAUGE, "A signed integer metric",
                              abacus::unit{"USD"}}}};

    abacus::metrics metrics{infos};

    auto uint_metric = metrics.initialize_metric<abacus::uint64>(name0);
    auto int_metric = metrics.initialize_metric<abacus::int64>(name1);

    EXPECT_FALSE(uint_metric.has_value());
    EXPECT_FALSE(int_metric.has_value());
    uint_metric = 4U;
    int_metric = -4;

    EXPECT_TRUE(uint_metric.has_value());
    EXPECT_TRUE(int_metric.has_value());
    EXPECT_EQ(uint_metric.value(), 4U);
    EXPECT_EQ(int_metric.value(), -4);

    metrics.reset_metrics();

    EXPECT_FALSE(uint_metric.has_value());
    EXPECT_FALSE(int_metric.has_value());

    uint_metric = 4U;
    int_metric = -4;

    EXPECT_TRUE(uint_metric.has_value());
    EXPECT_TRUE(int_metric.has_value());
    EXPECT_EQ(uint_metric.value(), 4U);
    EXPECT_EQ(int_metric.value(), -4);
}

static const std::vector<uint8_t> expected_meta_data = {
    0x08, 0x02, 0x1d, 0x76, 0x78, 0x4e, 0x60, 0x22, 0x34, 0x0a, 0x07, 0x6d,
    0x65, 0x74, 0x72, 0x69, 0x63, 0x30, 0x12, 0x29, 0x08, 0x04, 0x12, 0x25,
    0x0a, 0x1a, 0x41, 0x6e, 0x20, 0x75, 0x6e, 0x73, 0x69, 0x67, 0x6e, 0x65,
    0x64, 0x20, 0x69, 0x6e, 0x74, 0x65, 0x67, 0x65, 0x72, 0x20, 0x6d, 0x65,
    0x74, 0x72, 0x69, 0x63, 0x10, 0x01, 0x1a, 0x05, 0x62, 0x79, 0x74, 0x65,
    0x73, 0x22, 0x2d, 0x0a, 0x07, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x31,
    0x12, 0x22, 0x08, 0x0d, 0x1a, 0x1e, 0x0a, 0x17, 0x41, 0x20, 0x73, 0x69,
    0x67, 0x6e, 0x65, 0x64, 0x20, 0x69, 0x6e, 0x74, 0x65, 0x67, 0x65, 0x72,
    0x20, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x1a, 0x03, 0x55, 0x53, 0x44,
    0x22, 0x2c, 0x0a, 0x07, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x32, 0x12,
    0x21, 0x08, 0x16, 0x32, 0x1d, 0x0a, 0x17, 0x41, 0x20, 0x66, 0x6c, 0x6f,
    0x61, 0x74, 0x69, 0x6e, 0x67, 0x20, 0x70, 0x6f, 0x69, 0x6e, 0x74, 0x20,
    0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x1a, 0x02, 0x6d, 0x73, 0x22, 0x21,
    0x0a, 0x07, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x33, 0x12, 0x16, 0x08,
    0x1f, 0x42, 0x12, 0x0a, 0x10, 0x41, 0x20, 0x62, 0x6f, 0x6f, 0x6c, 0x65,
    0x61, 0x6e, 0x20, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63};

static const std::vector<uint8_t> expected_value_data = {
    0x76, 0x78, 0x4e, 0x60, 0x01, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0xd6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x61, 0x40, 0x01, 0x01,
};

namespace google::protobuf::internal
{
// Note: This method must be named exactly MapTestForceDeterministic to be able
// to call the SetDefaultSerializationDeterministic method as a friend function.
void MapTestForceDeterministic()
{
    // This function is used to force deterministic serialization for the
    // map. This is used to make the test more stable and we can compare
    // the serialized metadata.
    io::CodedOutputStream::SetDefaultSerializationDeterministic();
}
}

TEST(test_metrics, protocol_version)
{
    // Force deterministic serialization
    google::protobuf::internal::MapTestForceDeterministic();
    ASSERT_TRUE(google::protobuf::io::CodedOutputStream::
                    IsDefaultSerializationDeterministic());

    SCOPED_TRACE(::testing::Message()
                 << "protocol version: " << abacus::protocol_version());
    SCOPED_TRACE(
        ::testing::Message()
        << "If this test fails, you need to update the protocol version");
    std::map<std::string, abacus::type> infos = {
        {"metric0",
         abacus::uint64{abacus::kind::COUNTER, "An unsigned integer metric",
                        abacus::unit{"bytes"}}},
        {"metric1",
         abacus::int64{abacus::kind::GAUGE, "A signed integer metric",
                       abacus::unit{"USD"}}},
        {"metric2",
         abacus::float64{abacus::kind::GAUGE, "A floating point metric",
                         abacus::unit{"ms"}}},
        {"metric3", abacus::boolean{abacus::kind::GAUGE, "A boolean metric"}}};

    abacus::metrics metrics(infos);

    auto uint_metric =
        metrics.initialize_metric<abacus::uint64>("metric0", 42U);
    auto int_metric = metrics.initialize_metric<abacus::int64>("metric1", -42);
    auto float_metric =
        metrics.initialize_metric<abacus::float64>("metric2", 142.0);
    auto bool_metric =
        metrics.initialize_metric<abacus::boolean>("metric3", true);

    (void)uint_metric;
    (void)int_metric;
    (void)float_metric;
    (void)bool_metric;

    EXPECT_EQ(metrics.metadata_bytes(), expected_meta_data.size());
    {
        // Print the metadata as hex
        std::stringstream meta_stream;
        meta_stream << std::hex;
        for (std::size_t i = 0; i < metrics.metadata_bytes(); ++i)
        {
            meta_stream << "0x" << std::setw(2) << std::setfill('0')
                        << static_cast<int>(metrics.metadata_data()[i]) << ", ";
        }
        SCOPED_TRACE(::testing::Message() << "Meta data:\n"
                                          << meta_stream.str());
        EXPECT_EQ(expected_meta_data,
                  std::vector<uint8_t>(metrics.metadata_data(),
                                       metrics.metadata_data() +
                                           metrics.metadata_bytes()));
    }

    EXPECT_EQ(metrics.value_bytes(), expected_value_data.size());

    {
        std::stringstream value_stream;
        value_stream << std::hex;
        for (std::size_t i = 0; i < metrics.value_bytes(); ++i)
        {
            value_stream << "0x" << std::setw(2) << std::setfill('0')
                         << static_cast<int>(metrics.value_data()[i]) << ", ";
        }
        SCOPED_TRACE(::testing::Message() << "Value data:\n"
                                          << value_stream.str());
        EXPECT_EQ(
            expected_value_data,
            std::vector<uint8_t>(metrics.value_data(),
                                 metrics.value_data() + metrics.value_bytes()));
    }
}
