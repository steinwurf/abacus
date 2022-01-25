// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/detail/raw.hpp>
TEST(detail_test_raw, api)
{
    uint16_t max_name_bytes = 10;
    uint16_t max_prefix_bytes = 32;
    uint16_t max_metrics = 2;
    std::size_t alignment_padding =
        8 - (abacus::detail::header_bytes() + max_prefix_bytes +
             max_name_bytes * (max_metrics)) %
                8;
    std::size_t size = abacus::detail::header_bytes() + max_prefix_bytes +
                       alignment_padding +
                       max_metrics * (max_name_bytes + sizeof(uint64_t));
    std::vector<uint8_t> data(size);

    // Write the header
    std::memcpy(data.data(), &max_name_bytes, sizeof(uint16_t));
    std::memcpy(data.data() + 2, &max_prefix_bytes, sizeof(uint16_t));
    std::memcpy(data.data() + 4, &max_metrics, sizeof(uint16_t));
    data[6] = 8U;

    std::string prefix = "test";

    std::memcpy(abacus::detail::raw_prefix(data.data()), prefix.data(),
                prefix.size());

    std::string metric_name_1 = "metric_1";
    char* name_data_1 = abacus::detail::raw_name(data.data(), 0);
    uint64_t* value_data_1 = abacus::detail::raw_value(data.data(), 0);

    std::memcpy(name_data_1, metric_name_1.data(), metric_name_1.size());
    uint64_t value_1 = 1U;
    std::memcpy(value_data_1, &value_1, sizeof(uint64_t));

    std::string metric_name_2 = "metric";
    char* name_data_2 = abacus::detail::raw_name(data.data(), 1);
    uint64_t* value_data_2 = abacus::detail::raw_value(data.data(), 1);

    std::memcpy(name_data_2, metric_name_2.data(), metric_name_2.size());
    uint64_t value_2 = 2U;
    std::memcpy(value_data_2, &value_2, sizeof(uint64_t));

    EXPECT_EQ(abacus::detail::max_name_bytes(data.data()), max_name_bytes);
    EXPECT_EQ(abacus::detail::max_metrics(data.data()), max_metrics);
    EXPECT_EQ(abacus::detail::max_prefix_bytes(data.data()), max_prefix_bytes);
    EXPECT_EQ(abacus::detail::raw_prefix(data.data()), prefix);

    EXPECT_EQ(abacus::detail::raw_name(data.data(), 0), metric_name_1);
    EXPECT_EQ(*abacus::detail::raw_value(data.data(), 0), 1U);
    EXPECT_EQ(abacus::detail::is_metric_initialized(data.data(), 0), true);

    EXPECT_EQ(abacus::detail::raw_name(data.data(), 1), metric_name_2);
    EXPECT_EQ(*abacus::detail::raw_value(data.data(), 1), 2U);
    EXPECT_EQ(abacus::detail::is_metric_initialized(data.data(), 1), true);
}
