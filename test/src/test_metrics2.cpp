// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics2.hpp>

TEST(test_metrics2, empty)
{
    abacus::metrics2 metrics2;
    EXPECT_EQ(metrics2.count(), 0U);
    EXPECT_TRUE(metrics2.is_initialized());
}

TEST(test_metrics2, default_constructor)
{
    const uint16_t metric_count = 6;

    std::string name0 = "metric0";
    std::string name1 = "metric1";
    std::string name2 = "metric2";
    std::string name3 = "metric3";
    std::string name4 = "metric4";
    std::string name5 = "metric5";

    std::map<std::string, abacus::type2> infos = {
        {name0, abacus::boolean{abacus::kind::counter, "A boolean metric"}},
        {name1,
         abacus::uint32{abacus::kind::counter, "An unsigned integer metric"}},
        {name2, abacus::int32{abacus::kind::gauge, "A signed integer metric"}},
        {name3,
         abacus::float32{abacus::kind::gauge, "A floating point metric"}},
        {name4,
         abacus::boolean{abacus::kind::constant, "A constant boolean metric"}},
        {name5, abacus::float64{abacus::kind::constant,
                                "A constant floating point metric"}}};

    abacus::metrics2 metrics(infos);

    EXPECT_EQ(metrics.count(), metric_count);
    EXPECT_FALSE(metrics.is_initialized());

    auto m0 = metrics.initialize_metric<abacus::boolean>(name0);
    auto m1 = metrics.initialize_metric<abacus::uint32>(name1);
    auto m2 = metrics.initialize_metric<abacus::int32>(name2);
    auto m3 = metrics.initialize_metric<abacus::float32>(name3);
    metrics.initialize_constant<abacus::boolean>(name4, true);
    metrics.initialize_constant<abacus::float64>(name5, 42.42);

    EXPECT_TRUE(metrics.is_initialized());
}
