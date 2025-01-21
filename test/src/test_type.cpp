// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/type.hpp>

template <typename T>
void integer_test()
{
    using metric = typename T::optional;
    using type = typename T::type;

    uint8_t data[sizeof(type) + 1];
    std::memset(data, 0, sizeof(data));
    metric m;
    EXPECT_FALSE(m.is_initialized());
    m = metric(data);
    EXPECT_TRUE(m.is_initialized());
    EXPECT_FALSE(m.has_value());
    m = 10U;
    EXPECT_TRUE(m.has_value());
    EXPECT_EQ(m.value(), type{10});
    m += 12;
    m -= 2;
    ++m;
    --m;

    EXPECT_EQ(m.value(), type{20});
}

TEST(test_metrics, integer)
{
    integer_test<abacus::uint64>();
    integer_test<abacus::int64>();
    integer_test<abacus::uint32>();
    integer_test<abacus::int32>();
}

template <typename T>
void floating_point_test()
{
    using metric = typename T::optional;
    using type = typename T::type;

    uint8_t data[sizeof(type) + 1];
    std::memset(data, 0, sizeof(data));
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

TEST(test_metrics, floating_point)
{

    floating_point_test<abacus::float64>();
    floating_point_test<abacus::float32>();
}

TEST(test_metrics, boolean)
{
    uint8_t data[sizeof(bool) + 1];
    std::memset(data, 0, sizeof(data));
    abacus::boolean::optional m;
    EXPECT_FALSE(m.is_initialized());
    m = abacus::boolean::optional(data);
    EXPECT_TRUE(m.is_initialized());
    EXPECT_FALSE(m.has_value());
    m = true;
    EXPECT_TRUE(m.has_value());
    EXPECT_EQ(m.value(), true);
    m = false;
    EXPECT_EQ(m.value(), false);
}

TEST(test_metrics, enum8)
{
    uint8_t data[sizeof(uint8_t) + 1];
    std::memset(data, 0, sizeof(data));
    abacus::enum8::optional m;
    EXPECT_FALSE(m.is_initialized());
    m = abacus::enum8::optional(data);
    EXPECT_TRUE(m.is_initialized());
    EXPECT_FALSE(m.has_value());
    m = 10U;
    EXPECT_TRUE(m.has_value());
    EXPECT_EQ(m.value(), 10U);
    m = 20U;
    EXPECT_EQ(m.value(), 20U);
}
