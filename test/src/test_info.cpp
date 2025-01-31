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
        r = required(data, 10U);
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
        r = required(data, 10.0);
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
        r = abacus::boolean::required(data, true);
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
        o = 10U;
        EXPECT_TRUE(o.has_value());
        EXPECT_EQ(o.value(), 10U);
        o = 20U;
        EXPECT_EQ(o.value(), 20U);
        o.reset();
        EXPECT_FALSE(o.has_value());

        o.set_value(test_enum::value0);
        EXPECT_EQ(o.value(), 0U);
        o.set_value(test_enum::value1);
        EXPECT_EQ(o.value(), 1U);
        o.set_value(test_enum::value2);
        EXPECT_EQ(o.value(), 2U);
        o.set_value(test_enum::value3);
        EXPECT_EQ(o.value(), 3U);

        o = test_enum::value0;
        EXPECT_EQ(o.value(), 0U);
        o = test_enum::value1;
        EXPECT_EQ(o.value(), 1U);
        o = test_enum::value2;
        EXPECT_EQ(o.value(), 2U);
        o = test_enum::value3;
        EXPECT_EQ(o.value(), 3U);
    }

    {
        uint8_t data[sizeof(uint8_t) + 1];
        std::memset(data, 0, sizeof(data));
        abacus::enum8::required r;
        EXPECT_FALSE(r.is_initialized());
        r = abacus::enum8::required(data, 10U);
        EXPECT_TRUE(r.is_initialized());
        EXPECT_EQ(r.value(), 10U);
        r = 20U;
        EXPECT_EQ(r.value(), 20U);

        r.set_value(test_enum::value0);
        EXPECT_EQ(r.value(), 0U);
        r.set_value(test_enum::value1);
        EXPECT_EQ(r.value(), 1U);
        r.set_value(test_enum::value2);
        EXPECT_EQ(r.value(), 2U);
        r.set_value(test_enum::value3);
        EXPECT_EQ(r.value(), 3U);

        r = test_enum::value0;
        EXPECT_EQ(r.value(), 0U);
        r = test_enum::value1;
        EXPECT_EQ(r.value(), 1U);
        r = test_enum::value2;
        EXPECT_EQ(r.value(), 2U);
        r = test_enum::value3;
        EXPECT_EQ(r.value(), 3U);
    }
}

TEST(test_info, string)
{
    {
        uint8_t data[6];
        std::memset(data, 0, sizeof(data));
        data[0] = 1;
        abacus::string::set_value(data, "hello");
        EXPECT_EQ(abacus::string::value(data), "hello");
    }
}
