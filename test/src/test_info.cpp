// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/info.hpp>
#include <abacus/metric.hpp>

template <typename T>
void integer_test()
{
    using type = typename T::type;

    {
        using optional = abacus::metric<T>;
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
        using optional = abacus::metric<T>;
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
        abacus::metric<abacus::boolean> o;
        EXPECT_FALSE(o.is_initialized());
        o = abacus::metric<abacus::boolean>(data);
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
        abacus::metric<abacus::enum8> o;
        EXPECT_FALSE(o.is_initialized());
        o = abacus::metric<abacus::enum8>(data);
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
}
