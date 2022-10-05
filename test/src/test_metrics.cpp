// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics.hpp>

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
        abacus::metric_info{name0, "A boolean metric",
                            abacus::metric_type::boolean},
        abacus::metric_info{name1, "An unsigned integer metric",
                            abacus::metric_type::uint64},
        abacus::metric_info{name2, "A signed integer metric",
                            abacus::metric_type::int64},
        abacus::metric_info{name3, "A floating point metric",
                            abacus::metric_type::float64},
        abacus::metric_info{name4, "A constant boolean metric",
                            abacus::metric_type::boolean,
                            abacus::metric_flags::constant},
        abacus::metric_info{name5, "A constant floating point metric",
                            abacus::metric_type::float64,
                            abacus::metric_flags::constant}};

    abacus::metrics metrics(infos);

    EXPECT_EQ(metrics.metric_count(), metric_count);

    EXPECT_FALSE(metrics.is_metric_constant(0));
    EXPECT_FALSE(metrics.is_metric_constant(1));
    EXPECT_FALSE(metrics.is_metric_constant(2));
    EXPECT_TRUE(metrics.is_metric_constant(3));
    EXPECT_FALSE(metrics.is_metric_constant(4));
    EXPECT_TRUE(metrics.is_metric_constant(5));

    EXPECT_FALSE(metrics.is_metric_initialized(0));
    auto metric0 =
        metrics.initialize_metric<abacus::metric_type::uint64>(name1);
    EXPECT_TRUE(metrics.is_metric_initialized(0));

    EXPECT_FALSE(metrics.is_metric_initialized(1));
    auto metric1 = metrics.initialize_metric<abacus::metric_type::int64>(name2);
    EXPECT_TRUE(metrics.is_metric_initialized(1));

    EXPECT_FALSE(metrics.is_metric_initialized(2));
    auto metric2 =
        metrics.initialize_metric<abacus::metric_type::float64>(name3);
    EXPECT_TRUE(metrics.is_metric_initialized(2));

    EXPECT_FALSE(metrics.is_metric_initialized(4));
    auto metric3 =
        metrics.initialize_metric<abacus::metric_type::boolean>(name0);
    EXPECT_TRUE(metrics.is_metric_initialized(4));

    EXPECT_FALSE(metrics.is_metric_initialized(5));
    metrics.initialize_constant(name4, true);
    EXPECT_TRUE(metrics.is_metric_initialized(5));

    EXPECT_FALSE(metrics.is_metric_initialized(3));
    metrics.initialize_constant(name5, 42.42);
    EXPECT_TRUE(metrics.is_metric_initialized(3));

    EXPECT_EQ(metrics.metric_name(0), name1);
    EXPECT_EQ(metrics.metric_name(1), name2);
    EXPECT_EQ(metrics.metric_name(2), name3);
    EXPECT_EQ(metrics.metric_name(4), name0);
    EXPECT_EQ(metrics.metric_name(5), name4);

    EXPECT_EQ(metrics.metric_description(0), "An unsigned integer metric");
    EXPECT_EQ(metrics.metric_description(1), "A signed integer metric");
    EXPECT_EQ(metrics.metric_description(2), "A floating point metric");
    EXPECT_EQ(metrics.metric_description(3),
              "A constant floating point metric");
    EXPECT_EQ(metrics.metric_description(4), "A boolean metric");
    EXPECT_EQ(metrics.metric_description(5), "A constant boolean metric");

    metric0 = 4U;
    metric1 = -4;
    metric2 = 3.14;
    metric3 = true;

    uint64_t uint_value = 0U;
    int64_t int_value = 0;
    double float_value = 0.0;
    bool bool_value = false;

    metrics.metric_value(0, uint_value);
    EXPECT_EQ(uint_value, 4U);

    metrics.metric_value(1, int_value);
    EXPECT_EQ(int_value, -4);

    metrics.metric_value(2, float_value);
    EXPECT_EQ(float_value, 3.14);

    metrics.metric_value(3, float_value);
    EXPECT_EQ(float_value, 42.42);

    metrics.metric_value(4, bool_value);
    EXPECT_EQ(bool_value, true);

    metrics.metric_value(5, bool_value);
    EXPECT_EQ(bool_value, true);

    EXPECT_EQ(metrics.metric_index(name0), 4U);
    EXPECT_EQ(metrics.metric_index(name1), 0U);
    EXPECT_EQ(metrics.metric_index(name2), 1U);
    EXPECT_EQ(metrics.metric_index(name3), 2U);
    EXPECT_EQ(metrics.metric_index(name4), 5U);
    EXPECT_EQ(metrics.metric_index(name5), 3U);
}

TEST(test_metrics, value_and_meta_bytes)
{
    const uint16_t metric_count = 2;

    std::string name0 = "metric0";
    std::string name1 = "metric1";

    abacus::metric_info infos[metric_count] = {
        abacus::metric_info{name0, "An unsigned integer metric",
                            abacus::metric_type::uint64},
        abacus::metric_info{name1, "A signed integer metric",
                            abacus::metric_type::int64}};

    abacus::metrics metrics(infos);

    metrics.initialize_metric<abacus::metric_type::uint64>(name0);
    metrics.initialize_metric<abacus::metric_type::int64>(name1);

    std::size_t meta_bytes = 0;
    // header size
    meta_bytes += 12;
    // name and description sizes
    meta_bytes += metric_count * 2 * 2;
    // names and descriptions
    for (std::size_t i = 0; i < metric_count; ++i)
    {
        meta_bytes += infos[i].name.size();
        meta_bytes += infos[i].description.size();
    }
    // types
    meta_bytes += metric_count;
    // is_contant bools
    meta_bytes += metric_count;

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
                            abacus::metric_type::uint64},
        abacus::metric_info{name1, "A signed integer metric",
                            abacus::metric_type::int64}};

    abacus::metrics metrics(infos);

    auto uint_metric =
        metrics.initialize_metric<abacus::metric_type::uint64>(name0);
    auto int_metric =
        metrics.initialize_metric<abacus::metric_type::int64>(name1);

    uint64_t uint_value = 0U;
    int64_t int_value = 0;

    uint_metric = 4U;
    int_metric = -4;

    metrics.metric_value(0, uint_value);
    metrics.metric_value(1, int_value);

    EXPECT_EQ(uint_value, 4U);
    EXPECT_EQ(int_value, -4);

    metrics.reset_metric(0);
    metrics.reset_metric(1);

    metrics.metric_value(0, uint_value);
    metrics.metric_value(1, int_value);

    EXPECT_EQ(uint_value, 0U);
    EXPECT_EQ(int_value, 0);

    uint_metric = 4U;
    int_metric = -4;

    metrics.metric_value(0, uint_value);
    metrics.metric_value(1, int_value);

    EXPECT_EQ(uint_value, 4U);
    EXPECT_EQ(int_value, -4);

    metrics.reset_metrics();

    metrics.metric_value(0, uint_value);
    metrics.metric_value(1, int_value);

    EXPECT_EQ(uint_value, 0U);
    EXPECT_EQ(int_value, 0);
}
