// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/detail/raw.hpp>
#include <abacus/metrics.hpp>
#include <abacus/protocol_version.hpp>

TEST(test_metrics, default_constructor)
{
    const uint16_t metric_count = 6;

    std::string name0 = "metric0";
    std::string name1 = "metric1";
    std::string name2 = "metric2";
    std::string name3 = "metric3";
    std::string name4 = "metric4";
    std::string name5 = "metric5";

    abacus::metric_info infos[metric_count] = {
        abacus::metric_info{name0, "A boolean metric", abacus::type::boolean,
                            abacus::kind::counter},
        abacus::metric_info{name1, "An unsigned integer metric",
                            abacus::type::uint64, abacus::kind::counter,
                            abacus::unit{"bytes"}},
        abacus::metric_info{name2, "A signed integer metric",
                            abacus::type::int64, abacus::kind::gauge,
                            abacus::unit{"USD"}},
        abacus::metric_info{name3, "A floating point metric",
                            abacus::type::float64, abacus::kind::gauge,
                            abacus::unit{"ms"}},
        abacus::metric_info{name4, "A constant boolean metric",
                            abacus::type::boolean, abacus::kind::constant},
        abacus::metric_info{name5, "A constant floating point metric",
                            abacus::type::float64, abacus::kind::constant,
                            abacus::unit{"ms"}}};

    abacus::metrics metrics(infos);

    EXPECT_EQ(metrics.count(), metric_count);
    EXPECT_EQ(metrics.protocol_version(), abacus::protocol_version());

    EXPECT_EQ(metrics.kind(0), abacus::kind::counter);
    EXPECT_EQ(metrics.kind(1), abacus::kind::gauge);
    EXPECT_EQ(metrics.kind(2), abacus::kind::gauge);
    EXPECT_EQ(metrics.kind(3), abacus::kind::constant);
    EXPECT_EQ(metrics.kind(4), abacus::kind::counter);
    EXPECT_EQ(metrics.kind(5), abacus::kind::constant);

    EXPECT_FALSE(metrics.is_initialized(0));
    auto metric0 = metrics.initialize_metric<abacus::type::uint64>(name1);
    EXPECT_TRUE(metrics.is_initialized(0));

    EXPECT_FALSE(metrics.is_initialized(1));
    auto metric1 = metrics.initialize_metric<abacus::type::int64>(name2);
    EXPECT_TRUE(metrics.is_initialized(1));

    EXPECT_FALSE(metrics.is_initialized(2));
    auto metric2 = metrics.initialize_metric<abacus::type::float64>(name3);
    EXPECT_TRUE(metrics.is_initialized(2));

    EXPECT_FALSE(metrics.is_initialized(4));
    auto metric3 = metrics.initialize_metric<abacus::type::boolean>(name0);
    EXPECT_TRUE(metrics.is_initialized(4));

    EXPECT_FALSE(metrics.is_initialized(5));
    metrics.initialize_constant(name4, true);
    EXPECT_TRUE(metrics.is_initialized(5));

    EXPECT_FALSE(metrics.is_initialized(3));
    metrics.initialize_constant(name5, 42.42);
    EXPECT_TRUE(metrics.is_initialized(3));

    EXPECT_EQ(metrics.name(0), name1);
    EXPECT_EQ(metrics.name(1), name2);
    EXPECT_EQ(metrics.name(2), name3);
    EXPECT_EQ(metrics.name(4), name0);
    EXPECT_EQ(metrics.name(5), name4);

    EXPECT_EQ(metrics.description(0), "An unsigned integer metric");
    EXPECT_EQ(metrics.description(1), "A signed integer metric");
    EXPECT_EQ(metrics.description(2), "A floating point metric");
    EXPECT_EQ(metrics.description(3), "A constant floating point metric");
    EXPECT_EQ(metrics.description(4), "A boolean metric");
    EXPECT_EQ(metrics.description(5), "A constant boolean metric");

    EXPECT_EQ(metrics.unit(0), "bytes");
    EXPECT_EQ(metrics.unit(1), "USD");
    EXPECT_EQ(metrics.unit(2), "ms");
    EXPECT_EQ(metrics.unit(3), "ms");
    EXPECT_EQ(metrics.unit(4), "");
    EXPECT_EQ(metrics.unit(5), "");

    metric0 = 4U;
    metric1 = -4;
    metric2 = 3.14;
    metric3 = true;

    uint64_t uint_value = 0U;
    int64_t int_value = 0;
    double float_value = 0.0;
    bool bool_value = false;

    metrics.value(0, uint_value);
    EXPECT_EQ(uint_value, 4U);

    metrics.value(1, int_value);
    EXPECT_EQ(int_value, -4);

    metrics.value(2, float_value);
    EXPECT_EQ(float_value, 3.14);

    metrics.value(3, float_value);
    EXPECT_EQ(float_value, 42.42);

    metrics.value(4, bool_value);
    EXPECT_EQ(bool_value, true);

    metrics.value(5, bool_value);
    EXPECT_EQ(bool_value, true);

    EXPECT_EQ(metrics.index(name0), 4U);
    EXPECT_EQ(metrics.index(name1), 0U);
    EXPECT_EQ(metrics.index(name2), 1U);
    EXPECT_EQ(metrics.index(name3), 2U);
    EXPECT_EQ(metrics.index(name4), 5U);
    EXPECT_EQ(metrics.index(name5), 3U);
}

TEST(test_metrics, value_and_meta_bytes)
{
    const uint16_t metric_count = 2;

    std::string name0 = "metric0";
    std::string name1 = "metric1";

    abacus::metric_info infos[metric_count] = {
        abacus::metric_info{name0, "An unsigned integer metric",
                            abacus::type::uint64, abacus::kind::counter,
                            abacus::unit{"bytes"}},
        abacus::metric_info{name1, "A signed integer metric",
                            abacus::type::int64, abacus::kind::gauge,
                            abacus::unit{"USD"}}};

    abacus::metrics metrics(infos);

    metrics.initialize_metric<abacus::type::uint64>(name0);
    metrics.initialize_metric<abacus::type::int64>(name1);

    std::size_t meta_bytes = 0;
    // header size
    meta_bytes += abacus::detail::header_bytes();
    // name and description sizes
    meta_bytes += metric_count * 3 * 2;
    // names and descriptions
    for (std::size_t i = 0; i < metric_count; ++i)
    {
        meta_bytes += infos[i].name.size();
        meta_bytes += infos[i].description.size();
        meta_bytes += infos[i].unit.value.size();
    }
    // types
    meta_bytes += metric_count;
    // is_contant bools
    meta_bytes += metric_count;
    // min and max
    meta_bytes += metric_count * sizeof(uint64_t) * 2;

    ASSERT_EQ(meta_bytes, metrics.meta_bytes());

    std::size_t value_bytes = 0;
    value_bytes += metric_count * 8;
    value_bytes += (metric_count + 7) / 8;
    ASSERT_EQ(value_bytes, metrics.value_bytes());
}

TEST(test_metrics, reset_counters)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";

    abacus::metric_info infos[2] = {
        abacus::metric_info{name0, "An unsigned integer metric",
                            abacus::type::uint64, abacus::kind::counter,
                            abacus::unit{"bytes"}},
        abacus::metric_info{name1, "A signed integer metric",
                            abacus::type::int64, abacus::kind::gauge,
                            abacus::unit{"USD"}}};

    abacus::metrics metrics(infos);

    auto uint_metric = metrics.initialize_metric<abacus::type::uint64>(name0);
    auto int_metric = metrics.initialize_metric<abacus::type::int64>(name1);

    uint64_t uint_value = 0U;
    int64_t int_value = 0;

    uint_metric = 4U;
    int_metric = -4;

    metrics.value(0, uint_value);
    metrics.value(1, int_value);

    EXPECT_EQ(uint_value, 4U);
    EXPECT_EQ(int_value, -4);

    metrics.reset_metric(0);
    metrics.reset_metric(1);

    metrics.value(0, uint_value);
    metrics.value(1, int_value);

    EXPECT_EQ(uint_value, 0U);
    EXPECT_EQ(int_value, 0);

    uint_metric = 4U;
    int_metric = -4;

    metrics.value(0, uint_value);
    metrics.value(1, int_value);

    EXPECT_EQ(uint_value, 4U);
    EXPECT_EQ(int_value, -4);

    metrics.reset_metrics();

    metrics.value(0, uint_value);
    metrics.value(1, int_value);

    EXPECT_EQ(uint_value, 0U);
    EXPECT_EQ(int_value, 0);
}

static const std::vector<uint8_t> expected_meta_data = {
    0x00, 0x01, 0x1c, 0x00, 0x58, 0x00, 0x0a, 0x00, 0x03, 0x00, 0x01, 0x00,
    0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x1a, 0x00, 0x17, 0x00,
    0x17, 0x00, 0x10, 0x00, 0x05, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x30, 0x6d, 0x65, 0x74, 0x72, 0x69,
    0x63, 0x31, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x32, 0x6d, 0x65, 0x74,
    0x72, 0x69, 0x63, 0x33, 0x41, 0x6e, 0x20, 0x75, 0x6e, 0x73, 0x69, 0x67,
    0x6e, 0x65, 0x64, 0x20, 0x69, 0x6e, 0x74, 0x65, 0x67, 0x65, 0x72, 0x20,
    0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x41, 0x20, 0x73, 0x69, 0x67, 0x6e,
    0x65, 0x64, 0x20, 0x69, 0x6e, 0x74, 0x65, 0x67, 0x65, 0x72, 0x20, 0x6d,
    0x65, 0x74, 0x72, 0x69, 0x63, 0x41, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74,
    0x69, 0x6e, 0x67, 0x20, 0x70, 0x6f, 0x69, 0x6e, 0x74, 0x20, 0x6d, 0x65,
    0x74, 0x72, 0x69, 0x63, 0x41, 0x20, 0x62, 0x6f, 0x6f, 0x6c, 0x65, 0x61,
    0x6e, 0x20, 0x6d, 0x65, 0x74, 0x72, 0x69, 0x63, 0x62, 0x79, 0x74, 0x65,
    0x73, 0x55, 0x53, 0x44, 0x6d, 0x73, 0x00, 0x01, 0x02, 0x03, 0x00, 0x02,
    0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const std::vector<uint8_t> expected_value_data = {
    0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd6,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xc0, 0x61, 0x40, 0x01, 0x0f};

TEST(test_metrics, protocol_version)
{
    SCOPED_TRACE(::testing::Message()
                 << "protocol version: " << abacus::protocol_version());
    SCOPED_TRACE(
        ::testing::Message()
        << "If this test fails, you need to update the protocol version");
    abacus::metric_info infos[4] = {
        abacus::metric_info{"metric0", "An unsigned integer metric",
                            abacus::type::uint64, abacus::kind::counter,
                            abacus::unit{"bytes"}},
        abacus::metric_info{"metric1", "A signed integer metric",
                            abacus::type::int64, abacus::kind::gauge,
                            abacus::unit{"USD"}},
        abacus::metric_info{"metric2", "A floating point metric",
                            abacus::type::float64, abacus::kind::gauge,
                            abacus::unit{"ms"}},
        abacus::metric_info{"metric3", "A boolean metric",
                            abacus::type::boolean, abacus::kind::gauge}};

    abacus::metrics metrics(infos);

    auto uint_metric =
        metrics.initialize_metric<abacus::type::uint64>("metric0");
    auto int_metric = metrics.initialize_metric<abacus::type::int64>("metric1");
    auto float_metric =
        metrics.initialize_metric<abacus::type::float64>("metric2");
    auto bool_metric =
        metrics.initialize_metric<abacus::type::boolean>("metric3");
    uint_metric = 42U;
    int_metric = -42;
    float_metric = 142.0;
    bool_metric = true;

    EXPECT_EQ(metrics.meta_bytes(), expected_meta_data.size());
    EXPECT_EQ(metrics.value_bytes(), expected_value_data.size());
    {
        // Print the meta data as hex
        std::stringstream meta_stream;
        meta_stream << std::hex;
        for (std::size_t i = 0; i < metrics.meta_bytes(); ++i)
        {
            meta_stream << "0x" << std::setw(2) << std::setfill('0')
                        << static_cast<int>(metrics.meta_data()[i]) << ", ";
        }
        SCOPED_TRACE(::testing::Message() << "Meta data:\n"
                                          << meta_stream.str());
        EXPECT_EQ(
            expected_meta_data,
            std::vector<uint8_t>(metrics.meta_data(),
                                 metrics.meta_data() + metrics.meta_bytes()));
    }
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

TEST(test_metrics, observe)
{
    abacus::metric_info infos[4] = {
        abacus::metric_info{"metric0", "An unsigned integer metric",
                            abacus::type::uint64, abacus::kind::counter,
                            abacus::unit{"bytes"}},
        abacus::metric_info{"metric1", "A signed integer metric",
                            abacus::type::int64, abacus::kind::gauge,
                            abacus::unit{"USD"}},
        abacus::metric_info{"metric2", "A floating point metric",
                            abacus::type::float64, abacus::kind::gauge,
                            abacus::unit{"ms"}},
        abacus::metric_info{"metric3", "A boolean metric",
                            abacus::type::boolean, abacus::kind::gauge}};

    abacus::metrics metrics(infos);

    uint64_t observed_uint = 0;
    int64_t observed_int = 0;
    double observed_float = 0.0;
    bool observed_bool = false;

    uint64_t value_uint = 0;
    int64_t value_int = 0;
    double value_float = 0.0;
    bool value_bool = false;

    metrics.observe_metric("metric0",
                           abacus::delegate<uint64_t()>(
                               [&observed_uint]() { return observed_uint; }));

    metrics.observe_metric("metric1",
                           abacus::delegate<int64_t()>(
                               [&observed_int]() { return observed_int; }));
    metrics.observe_metric("metric2",
                           abacus::delegate<double()>(
                               [&observed_float]() { return observed_float; }));
    metrics.observe_metric(
        "metric3",
        abacus::delegate<bool()>([&observed_bool]() { return observed_bool; }));

    observed_uint = 42U;
    observed_int = -42;
    observed_float = 142.0;
    observed_bool = true;

    metrics.value(0, value_uint);
    metrics.value(1, value_int);
    metrics.value(2, value_float);
    metrics.value(3, value_bool);

    EXPECT_EQ(value_uint, observed_uint);
    EXPECT_EQ(value_int, observed_int);
    EXPECT_EQ(value_float, observed_float);
    EXPECT_EQ(value_bool, observed_bool);
}
