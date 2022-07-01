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
    uint16_t metric_count = 5;

    std::string name0 = "metric0";
    std::string name1 = "metric1";
    std::string name2 = "metric2";
    std::string name3 = "metric3";
    std::string name4 = "metric4";

    abacus::metric_info infos[5] = {
        abacus::metric_info{name0, "A boolean metric",
                            abacus::metric_type::boolean,
                            abacus::qualifier::non_constant},
        abacus::metric_info{name1, "An unsigned integer metric",
                            abacus::metric_type::uint64,
                            abacus::qualifier::non_constant},
        abacus::metric_info{name2, "A signed integer metric",
                            abacus::metric_type::int64,
                            abacus::qualifier::non_constant},
        abacus::metric_info{name3, "A floating point metric",
                            abacus::metric_type::float64,
                            abacus::qualifier::non_constant},
        abacus::metric_info{name4, "A constant boolean metric",
                            abacus::metric_type::boolean,
                            abacus::qualifier::constant}};

    abacus::metrics metrics(infos);

    EXPECT_EQ(metrics.metric_count(), metric_count);

    EXPECT_FALSE(metrics.metric_is_constant(0));
    EXPECT_FALSE(metrics.metric_is_constant(1));
    EXPECT_FALSE(metrics.metric_is_constant(2));
    EXPECT_FALSE(metrics.metric_is_constant(3));
    EXPECT_TRUE(metrics.metric_is_constant(4));

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

    EXPECT_FALSE(metrics.is_metric_initialized(3));
    auto metric3 =
        metrics.initialize_metric<abacus::metric_type::boolean>(name0);
    EXPECT_TRUE(metrics.is_metric_initialized(3));

    EXPECT_FALSE(metrics.is_metric_initialized(4));
    metrics.initialize_constant(name4, true);
    EXPECT_TRUE(metrics.is_metric_initialized(4));

    EXPECT_EQ(metrics.metric_name(0), name1);
    EXPECT_EQ(metrics.metric_name(1), name2);
    EXPECT_EQ(metrics.metric_name(2), name3);
    EXPECT_EQ(metrics.metric_name(3), name0);
    EXPECT_EQ(metrics.metric_name(4), name4);

    EXPECT_EQ(metrics.metric_description(0), "An unsigned integer metric");
    EXPECT_EQ(metrics.metric_description(1), "A signed integer metric");
    EXPECT_EQ(metrics.metric_description(2), "A floating point metric");
    EXPECT_EQ(metrics.metric_description(3), "A boolean metric");
    EXPECT_EQ(metrics.metric_description(4), "A constant boolean metric");

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

    metrics.metric_value(3, bool_value);
    EXPECT_EQ(bool_value, true);

    metrics.metric_value(4, bool_value);
    EXPECT_EQ(bool_value, true);

    EXPECT_EQ(metrics.metric_index(name0), 3U);
    EXPECT_EQ(metrics.metric_index(name1), 0U);
    EXPECT_EQ(metrics.metric_index(name2), 1U);
    EXPECT_EQ(metrics.metric_index(name3), 2U);
    EXPECT_EQ(metrics.metric_index(name4), 4U);
}

TEST(test_metrics, copy_storage)
{
    uint16_t metric_count = 2;

    std::string name0 = "metric0";
    std::string name1 = "metric1";

    abacus::metric_info infos[2] = {
        abacus::metric_info{name0, "An unsigned integer metric",
                            abacus::metric_type::uint64,
                            abacus::qualifier::non_constant},
        abacus::metric_info{name1, "A signed integer metric",
                            abacus::metric_type::int64,
                            abacus::qualifier::non_constant}};

    abacus::metrics metrics(infos);

    metrics.initialize_metric<abacus::metric_type::uint64>(name0);
    metrics.initialize_metric<abacus::metric_type::int64>(name1);

    metrics.push_scope("scope");

    std::size_t size = 0;
    // header size
    size += 12;
    // name and description sizes
    size += metric_count * 2 * 2;
    // names and descriptions
    for (std::size_t i = 0; i < metric_count; ++i)
    {
        size += infos[i].name.size() + 1;
        size += infos[i].description.size() + 1;
    }
    // types
    size += metric_count;
    // is_contant bools
    size += metric_count;
    // alignment padding
    size += size % 8 == 0 ? 0 : 8 - (size % 8);
    // values
    size += metric_count * 8;
    // scope
    size += metrics.scope_size();
    // scope padding
    size += size % 8 == 0 ? 0 : 8 - (size % 8);

    EXPECT_EQ(size, metrics.storage_bytes());
    std::vector<uint8_t> data1(size);
    std::vector<uint8_t> data2(size);

    metrics.copy_storage(data1.data(), data1.size());
    metrics.copy_storage(data2.data(), data2.size());

    EXPECT_EQ(metrics.storage_bytes(), data1.size());
    EXPECT_EQ(metrics.storage_bytes(), data2.size());
    EXPECT_EQ(data2, data1);
}

TEST(test_metrics, reset_counters)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";

    abacus::metric_info infos[2] = {
        abacus::metric_info{name0, "An unsigned integer metric",
                            abacus::metric_type::uint64,
                            abacus::qualifier::non_constant},
        abacus::metric_info{name1, "A signed integer metric",
                            abacus::metric_type::int64,
                            abacus::qualifier::non_constant}};

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

TEST(test_metrics, add_scope)
{
    uint16_t metric_count = 5;

    std::string name0 = "metric0";
    std::string name1 = "metric1";
    std::string name2 = "metric2";
    std::string name3 = "metric3";
    std::string name4 = "metric4";

    abacus::metric_info infos[5] = {
        abacus::metric_info{name0, "A boolean metric",
                            abacus::metric_type::boolean,
                            abacus::qualifier::non_constant},
        abacus::metric_info{name1, "An unsigned integer metric",
                            abacus::metric_type::uint64,
                            abacus::qualifier::non_constant},
        abacus::metric_info{name2, "A signed integer metric",
                            abacus::metric_type::int64,
                            abacus::qualifier::non_constant},
        abacus::metric_info{name3, "A floating point metric",
                            abacus::metric_type::float64,
                            abacus::qualifier::non_constant},
        abacus::metric_info{name4, "A constant boolean metric",
                            abacus::metric_type::boolean,
                            abacus::qualifier::constant}};

    abacus::metrics metrics(infos);

    std::string scope = "metrics";

    metrics.push_scope(scope);
    metrics.push_scope("test");

    EXPECT_EQ(metrics.metric_count(), metric_count);

    for (std::size_t i = 0; i < metrics.metric_count(); i++)
    {
        EXPECT_EQ(metrics.scope(), "test.metrics");
        EXPECT_EQ(metrics.scope_size(), std::string("test.metrics").size() + 1);
    }
}
