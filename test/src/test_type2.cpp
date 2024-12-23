// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/type2.hpp>

template <typename T>
void integer_test()
{
    using metric = typename T::metric;
    using type = typename T::type;

    uint8_t data[sizeof(type) + 1];
    metric m;
    EXPECT_FALSE(m.is_initialized());
    m = metric(data);
    EXPECT_TRUE(m.is_initialized());
    EXPECT_FALSE(m.has_value());
    m = 10U;
    EXPECT_TRUE(m.has_value());
    EXPECT_EQ(m.value(), 10U);
    m += 12;
    m -= 2;
    ++m;
    --m;

    EXPECT_EQ(m.value(), 20U);
}

template <typename T>
void float_test()
{
    using metric = typename T::metric;
    using type = typename T::type;

    uint8_t data[sizeof(type) + 1];
    metric m;
    EXPECT_FALSE(m.is_initialized());
    m = metric(data);
    EXPECT_TRUE(m.is_initialized());
    EXPECT_FALSE(m.has_value());
    m = 10.0;
    EXPECT_TRUE(m.has_value());
    EXPECT_EQ(m.value(), 10.0);
    m += 12.0;
    m -= 2.0;
    ++m;
    --m;

    EXPECT_EQ(m.value(), 20.0);
}

void boolean_test()
{
    uint8_t data[sizeof(bool) + 1];
    abacus::boolean::metric m;
    EXPECT_FALSE(m.is_initialized());
    m = abacus::boolean::metric(data);
    EXPECT_TRUE(m.is_initialized());
    EXPECT_FALSE(m.has_value());
    m = true;
    EXPECT_TRUE(m.has_value());
    EXPECT_EQ(m.value(), true);
    m = false;
    EXPECT_EQ(m.value(), false);
}

void enum8_test()
{
    uint8_t data[sizeof(uint8_t) + 1];
    abacus::enum8::metric m;
    EXPECT_FALSE(m.is_initialized());
    m = abacus::enum8::metric(data);
    EXPECT_TRUE(m.is_initialized());
    EXPECT_FALSE(m.has_value());
    m = 10U;
    EXPECT_TRUE(m.has_value());
    EXPECT_EQ(m.value(), 10U);
    m = 20U;
    EXPECT_EQ(m.value(), 20U);
}

TEST(test_metrics2, api)
{
    integer_test<abacus::uint64>();
    integer_test<abacus::int64>();
    integer_test<abacus::uint32>();
    integer_test<abacus::int32>();

    float_test<abacus::float64>();
    float_test<abacus::float32>();

    boolean_test();

    enum8_test();
}
