// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cmath>
#include <cstdint>
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

    std::optional<type> new_value = 10;
    m = new_value;
    EXPECT_TRUE(m.has_value());
    EXPECT_EQ(m.value(), type{10});

    new_value.reset();
    m = new_value;
    EXPECT_FALSE(m.has_value());
}

TEST(test_metric, integer)
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

    std::optional<type> new_value = 10.0;
    m = new_value;
    EXPECT_TRUE(m.has_value());
    EXPECT_EQ(m.value(), type{10.0});

    new_value.reset();
    m = new_value;
    EXPECT_FALSE(m.has_value());
}

TEST(test_metric, floating_point)
{

    floating_point_test<abacus::float64>();
    floating_point_test<abacus::float32>();
}

TEST(test_metric, boolean)
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

    std::optional<bool> new_value = true;
    m = new_value;
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(m.value());

    new_value.reset();
    m = new_value;
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

TEST(test_metric, enum8)
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

    m.reset();
    EXPECT_FALSE(m.has_value());

    std::optional<test_enum> new_value = test_enum::value0;

    m = new_value;
    EXPECT_TRUE(m.has_value());
    EXPECT_EQ(m.value<test_enum>(), test_enum::value0);

    new_value.reset();
    m = new_value;
    EXPECT_FALSE(m.has_value());
}

/// Disable these tests on windows as they produce the compile error C2124
/// "divide or mod by zero"
#ifndef _WIN32

TEST(test_metric, float_death1)
{
    uint8_t data[9];
    std::memset(data, 0, sizeof(data));
    abacus::metric<abacus::float64> double_metric(data);
    double_metric = 2;
    // Check that assignment to NaN is not allowed
    EXPECT_DEATH(double_metric = 0.0 / 0.0, "");
}

TEST(test_metric, float_death2)
{
    uint8_t data[9];
    std::memset(data, 0, sizeof(data));
    abacus::metric<abacus::float64> double_metric(data);
    double_metric = 2;
    // Check that that assignment to -NaN is not allowed
    EXPECT_DEATH(double_metric = -0.0 / 0.0, "");
}

TEST(test_metric, float_death3)
{
    uint8_t data[9];
    std::memset(data, 0, sizeof(data));
    abacus::metric<abacus::float64> double_metric(data);
    double_metric = 2;
    // Check that that assignment to +Inf is not allowed
    EXPECT_DEATH(double_metric = 1 / 0.0, "");
}

TEST(test_metric, float_death4)
{
    uint8_t data[9];
    std::memset(data, 0, sizeof(data));
    abacus::metric<abacus::float64> double_metric(data);
    double_metric = 2;
    // Check that that assignment to -Inf is not allowed
    EXPECT_DEATH(double_metric = 1 / -0.0, "");
}

TEST(test_metric, float_death5)
{
    uint8_t data[9];
    std::memset(data, 0, sizeof(data));
    abacus::metric<abacus::float64> double_metric(data);
    double_metric = 2;
    // Check that assignment to NaN is not allowed
    EXPECT_DEATH(double_metric += 0.0 / 0.0, "");
}

TEST(test_metric, float_death6)
{
    uint8_t data[9];
    std::memset(data, 0, sizeof(data));
    abacus::metric<abacus::float64> double_metric(data);
    double_metric = 2;
    // Check that that assignment to -NaN is not allowed
    EXPECT_DEATH(double_metric += -0.0 / 0.0, "");
}

// Check that that assignment to +Inf is not allowed
TEST(test_metric, float_death7)
{
    uint8_t data[9];
    std::memset(data, 0, sizeof(data));
    abacus::metric<abacus::float64> double_metric(data);
    double_metric = 2;
    EXPECT_DEATH(double_metric += 1 / 0.0, "");
}

TEST(test_metric, float_death8)
{
    uint8_t data[9];
    std::memset(data, 0, sizeof(data));
    abacus::metric<abacus::float64> double_metric(data);
    double_metric = 2;
    // Check that that assignment to -Inf is not allowed
    EXPECT_DEATH(double_metric += 1 / -0.0, "");
}

TEST(test_metric, float_death9)
{
    uint8_t data[9];
    std::memset(data, 0, sizeof(data));
    abacus::metric<abacus::float64> double_metric(data);
    double_metric = 2;
    // Check that assignment to NaN is not allowed
    EXPECT_DEATH(double_metric -= 0.0 / 0.0, "");
}

TEST(test_metric, float_death10)
{
    uint8_t data[9];
    std::memset(data, 0, sizeof(data));
    abacus::metric<abacus::float64> double_metric(data);
    double_metric = 2;
    // Check that that assignment to -NaN is not allowed
    EXPECT_DEATH(double_metric -= -0.0 / 0.0, "");
}

TEST(test_metric, float_death11)
{
    uint8_t data[9];
    std::memset(data, 0, sizeof(data));
    abacus::metric<abacus::float64> double_metric(data);
    double_metric = 2;
    // Check that that assignment to +Inf is not allowed
    EXPECT_DEATH(double_metric -= 1 / 0.0, "");
}

TEST(test_metric, float_death12)
{
    uint8_t data[9];
    std::memset(data, 0, sizeof(data));
    abacus::metric<abacus::float64> double_metric(data);
    double_metric = 2;
    // Check that that assignment to -Inf is not allowed
    EXPECT_DEATH(double_metric -= 1 / -0.0, "");
}

#endif
