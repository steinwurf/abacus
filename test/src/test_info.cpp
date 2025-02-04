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
    uint8_t data[sizeof(type) + 1];
    std::memset(data, 0, sizeof(data));
    abacus::metric<T> m;
    EXPECT_FALSE(m.is_initialized());
    m = abacus::metric<T>(data);
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
    m.reset();
    EXPECT_FALSE(m.has_value());
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
    uint8_t data[sizeof(type) + 1];
    std::memset(data, 0, sizeof(data));
    abacus::metric<T> m;
    EXPECT_FALSE(m.is_initialized());
    m = abacus::metric<T>(data);
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
    m.reset();
    EXPECT_FALSE(m.has_value());
}

TEST(test_info, floating_point)
{

    floating_point_test<abacus::float64>();
    floating_point_test<abacus::float32>();
}

TEST(test_info, boolean)
{

    uint8_t data[sizeof(bool) + 1];
    std::memset(data, 0, sizeof(data));
    abacus::metric<abacus::boolean> m;
    EXPECT_FALSE(m.is_initialized());
    m = abacus::metric<abacus::boolean>(data);
    EXPECT_TRUE(m.is_initialized());
    EXPECT_FALSE(m.has_value());
    m = true;
    EXPECT_TRUE(m.has_value());
    EXPECT_EQ(m.value(), true);
    m = false;
    EXPECT_EQ(m.value(), false);
    m.reset();
    EXPECT_FALSE(m.has_value());
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
        abacus::metric<abacus::enum8> m;
        EXPECT_FALSE(m.is_initialized());
        m = abacus::metric<abacus::enum8>(data);
        EXPECT_TRUE(m.is_initialized());
        EXPECT_FALSE(m.has_value());

        m.set_value(test_enum::value0);
        EXPECT_EQ(m.value<test_enum>(), test_enum::value0);
        m.set_value(test_enum::value1);
        EXPECT_EQ(m.value<test_enum>(), test_enum::value1);
        m.set_value(test_enum::value2);
        EXPECT_EQ(m.value<test_enum>(), test_enum::value2);
        m.set_value(test_enum::value3);
        EXPECT_EQ(m.value<test_enum>(), test_enum::value3);

        m = test_enum::value0;
        EXPECT_EQ(m.value<test_enum>(), test_enum::value0);
        m = test_enum::value1;
        EXPECT_EQ(m.value<test_enum>(), test_enum::value1);
        m = test_enum::value2;
        EXPECT_EQ(m.value<test_enum>(), test_enum::value2);
        m = test_enum::value3;
        EXPECT_EQ(m.value<test_enum>(), test_enum::value3);
    }
}
