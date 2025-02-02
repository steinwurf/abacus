// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/info.hpp>
#include <abacus/optional_metric.hpp>
#include <abacus/required_metric.hpp>

template <typename T>
void integer_test()
{
    using type = typename T::type;

    {
        using optional = typename T::optional;
        uint8_t data[sizeof(type) + 1];
        std::memset(data, 0, sizeof(data));
        optional o;
        EXPECT_FALSE(o.is_initialized());
        o = optional(data);
        EXPECT_TRUE(o.is_initialized());
        EXPECT_FALSE(o.has_value());
        o = 10U;
        EXPECT_TRUE(o.has_value());
        EXPECT_EQ(o.value(), type{10});
        o += 12;
        o -= 2;
        ++o;
        --o;

        EXPECT_EQ(o.value(), type{20});
        o.reset();
        EXPECT_FALSE(o.has_value());
    }
    {
        using required = typename T::required;
        uint8_t data[sizeof(type) + 1];
        std::memset(data, 0, sizeof(data));
        required r;
        EXPECT_FALSE(r.is_initialized());
        r = required(data);
        r.set_value(10U);
        EXPECT_TRUE(r.is_initialized());
        EXPECT_EQ(r.value(), type{10});
        r += 12;
        r -= 2;
        ++r;
        --r;

        EXPECT_EQ(r.value(), type{20});
    }
}

TEST(test_info, integer)
{
    integer_test<abacus::uint64>();
    integer_test<abacus::int64>();
    integer_test<abacus::uint32>();
    integer_test<abacus::int32>();
}

template <typename T>
void floating_point_test()
{
    using type = typename T::type;

    {
        using optional = typename T::optional;
        uint8_t data[sizeof(type) + 1];
        std::memset(data, 0, sizeof(data));
        optional o;
        EXPECT_FALSE(o.is_initialized());
        o = optional(data);
        EXPECT_TRUE(o.is_initialized());
        EXPECT_FALSE(o.has_value());
        o = 10.0;
        EXPECT_TRUE(o.has_value());
        EXPECT_EQ(o.value(), 10.0);
        o += 12.0;
        o -= 2.0;
        ++o;
        --o;

        EXPECT_EQ(o.value(), 20.0);
        o.reset();
        EXPECT_FALSE(o.has_value());
    }
    {
        using required = typename T::required;
        uint8_t data[sizeof(type) + 1];
        std::memset(data, 0, sizeof(data));
        required r;
        EXPECT_FALSE(r.is_initialized());
        r = required(data);
        r.set_value(10.0);
        EXPECT_TRUE(r.is_initialized());
        EXPECT_EQ(r.value(), type{10.0});
        r += 12.0;
        r -= 2.0;
        ++r;
        --r;

        EXPECT_EQ(r.value(), type{20.0});
    }
}

TEST(test_info, floating_point)
{

    floating_point_test<abacus::float64>();
    floating_point_test<abacus::float32>();
}

TEST(test_info, boolean)
{
    {
        uint8_t data[sizeof(bool) + 1];
        std::memset(data, 0, sizeof(data));
        abacus::boolean::optional o;
        EXPECT_FALSE(o.is_initialized());
        o = abacus::boolean::optional(data);
        EXPECT_TRUE(o.is_initialized());
        EXPECT_FALSE(o.has_value());
        o = true;
        EXPECT_TRUE(o.has_value());
        EXPECT_EQ(o.value(), true);
        o = false;
        EXPECT_EQ(o.value(), false);
        o.reset();
        EXPECT_FALSE(o.has_value());
    }
    {
        uint8_t data[sizeof(bool) + 1];
        std::memset(data, 0, sizeof(data));
        abacus::boolean::required r;
        EXPECT_FALSE(r.is_initialized());
        r = abacus::boolean::required(data);
        r.set_value(true);
        EXPECT_TRUE(r.is_initialized());
        EXPECT_EQ(r.value(), true);
        r = false;
        EXPECT_EQ(r.value(), false);
    }
}
namespace
{
enum class test_enum
{
    value0 = 0,
    value1 = 1,
    value2 = 2,
    value3 = 3
};
}

TEST(test_info, enum8)
{
    {
        uint8_t data[sizeof(uint8_t) + 1];
        std::memset(data, 0, sizeof(data));
        abacus::enum8::optional o;
        EXPECT_FALSE(o.is_initialized());
        o = abacus::enum8::optional(data);
        EXPECT_TRUE(o.is_initialized());
        EXPECT_FALSE(o.has_value());

        o.set_value(test_enum::value0);
        EXPECT_EQ(o.value<test_enum>(), test_enum::value0);
        o.set_value(test_enum::value1);
        EXPECT_EQ(o.value<test_enum>(), test_enum::value1);
        o.set_value(test_enum::value2);
        EXPECT_EQ(o.value<test_enum>(), test_enum::value2);
        o.set_value(test_enum::value3);
        EXPECT_EQ(o.value<test_enum>(), test_enum::value3);

        o = test_enum::value0;
        EXPECT_EQ(o.value<test_enum>(), test_enum::value0);
        o = test_enum::value1;
        EXPECT_EQ(o.value<test_enum>(), test_enum::value1);
        o = test_enum::value2;
        EXPECT_EQ(o.value<test_enum>(), test_enum::value2);
        o = test_enum::value3;
        EXPECT_EQ(o.value<test_enum>(), test_enum::value3);
    }

    {
        uint8_t data[sizeof(uint8_t) + 1];
        std::memset(data, 0, sizeof(data));
        abacus::enum8::required r;
        EXPECT_FALSE(r.is_initialized());
        r = abacus::enum8::required(data);

        EXPECT_TRUE(r.is_initialized());

        r.set_value(test_enum::value0);
        EXPECT_EQ(r.value<test_enum>(), test_enum::value0);
        r.set_value(test_enum::value1);
        EXPECT_EQ(r.value<test_enum>(), test_enum::value1);
        r.set_value(test_enum::value2);
        EXPECT_EQ(r.value<test_enum>(), test_enum::value2);
        r.set_value(test_enum::value3);
        EXPECT_EQ(r.value<test_enum>(), test_enum::value3);

        r = test_enum::value0;
        EXPECT_EQ(r.value<test_enum>(), test_enum::value0);
        r = test_enum::value1;
        EXPECT_EQ(r.value<test_enum>(), test_enum::value1);
        r = test_enum::value2;
        EXPECT_EQ(r.value<test_enum>(), test_enum::value2);
        r = test_enum::value3;
        EXPECT_EQ(r.value<test_enum>(), test_enum::value3);
    }
}
