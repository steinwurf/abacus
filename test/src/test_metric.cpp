// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metric.hpp>
TEST(test_metric, constructor)
{
    uint64_t count = 10000;
    abacus::metric metric(&count);
    EXPECT_TRUE(metric.is_initialized());
}