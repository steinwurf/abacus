// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics.hpp>
#include <abacus/view_iterator.hpp>

TEST(test_view_iterator, default_constructor)
{
    std::string name1 = "metric0";
    std::string name2 = "metric1";

    std::string description1 = "An unsigned integer metric";
    std::string description2 = "A signed integer metric";

    std::vector<abacus::metric_info> infos1;
    std::vector<abacus::metric_info> infos2;

    infos1.reserve(1);
    infos2.reserve(1);

    infos1.push_back(abacus::metric_info{name1, description1,
                                         abacus::value_type::unsigned_integral,
                                         abacus::NON_CONSTANT});
    infos2.push_back(abacus::metric_info{name2, description2,
                                         abacus::value_type::signed_integral,
                                         abacus::NON_CONSTANT});

    abacus::metrics metrics1(infos1);
    abacus::metrics metrics2(infos2);

    metrics1.push_scope("test1");
    metrics2.push_scope("test2");

    metrics1.initialize_metric<abacus::value_type::unsigned_integral>(0, name1);

    metrics2.initialize_metric<abacus::value_type::signed_integral>(0, name2);

    auto size1 = metrics1.storage_bytes();
    auto size2 = metrics2.storage_bytes();

    std::size_t combined_size = size1 + size2;
    std::vector<uint8_t> combined_data(combined_size);

    metrics1.copy_storage(combined_data.data(), metrics1.storage_bytes());
    metrics2.copy_storage(combined_data.data() + metrics1.storage_bytes(),
                          metrics2.storage_bytes());

    abacus::view_iterator iterator(combined_data.data(), combined_data.size());

    EXPECT_EQ(iterator.view_count(), 2U);

    abacus::view view1 = iterator.get_view(0);
    abacus::view view2 = iterator.get_view(1);

    EXPECT_EQ(1U, view1.metric_count());
    EXPECT_EQ(name1.size() + 1, view1.name_bytes());
    EXPECT_EQ(description1.size() + 1, view1.description_bytes());
    EXPECT_EQ(metrics1.scope(), view1.scope());
    EXPECT_EQ(name1, view1.metric_name(0));
    EXPECT_EQ(description1, view1.metric_description(0));

    uint64_t value1 = 12;
    metrics1.metric_value(0, value1);
    EXPECT_EQ(value1, 0U);

    EXPECT_EQ(1U, view2.metric_count());
    EXPECT_EQ(name2.size() + 1, view2.name_bytes());
    EXPECT_EQ(description2.size() + 1, view2.description_bytes());
    EXPECT_EQ(name2, view2.metric_name(0));
    EXPECT_EQ(metrics2.scope(), view2.scope());
    EXPECT_EQ(description2, view2.metric_description(0));

    int64_t value2 = 12;
    metrics2.metric_value(0, value2);
    EXPECT_EQ(value2, 0);
}
