// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics.hpp>
TEST(test_memory, default_test)
{
    uint64_t count = 0xFFFFFFFFFFFFFFFFU;
    uint16_t max_name_bytes = 32;
    uint16_t max_metrics = 32;
    abacus::metrics metrics(max_metrics, max_name_bytes, "metrics");

    for (std::size_t i = 0; i < max_metrics; i++)
    {
        std::string name = "metric" + std::to_string(i);
        auto metric = metrics.initialize_metric(i, name);
        metric += count;
    }
}