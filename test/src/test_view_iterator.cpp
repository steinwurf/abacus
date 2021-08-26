// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics.hpp>
#include <abacus/view_iterator.hpp>

TEST(test_view_iterator, default_constructor)
{
    uint16_t max_metrics = 10;
    uint16_t max_name_bytes = 32;
    std::string title1 = "metrics1";
    std::string title2 = "metrics2";

    abacus::metrics metrics1(max_metrics, max_name_bytes, title1);
    abacus::metrics metrics2(max_metrics, max_name_bytes, title2);

    std::vector<uint8_t> combined_data(metrics1.storage_bytes() +
                                       metrics2.storage_bytes());
    metrics1.copy_storage(combined_data.data());
    metrics2.copy_storage(combined_data.data() + metrics1.storage_bytes());

    abacus::view_iterator iterator(combined_data.data(), combined_data.size());

    EXPECT_EQ(iterator.view_count(), 2U);

    abacus::view iterator_view1 = iterator.get_view(0);
    abacus::view iterator_view2 = iterator.get_view(1);

    EXPECT_EQ(max_metrics, iterator_view1.max_metrics());
    EXPECT_EQ(max_name_bytes, iterator_view1.max_name_bytes());
    EXPECT_EQ(title1, iterator_view1.raw_title());

    EXPECT_EQ(max_metrics, iterator_view2.max_metrics());
    EXPECT_EQ(max_name_bytes, iterator_view2.max_name_bytes());
    EXPECT_EQ(title2, iterator_view2.raw_title());
}