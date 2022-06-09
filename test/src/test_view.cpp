// // Copyright (c) Steinwurf ApS 2020.
// // All Rights Reserved
// //
// // Distributed under the "BSD License". See the accompanying LICENSE.rst
// // file.

// #include <cstring>
// #include <gtest/gtest.h>

// #include <abacus/metrics.hpp>
// #include <abacus/view.hpp>

// TEST(test_view, api)
// {
//     uint16_t max_metrics = 10;
//     uint16_t max_name_bytes = 32;
//     std::string scope = "scope";

//     abacus::metrics metrics(max_metrics, max_name_bytes);
//     auto metric = metrics.add_metric<uint64_t>("metric");
//     metrics.push_scope(scope);

//     std::size_t storage_size =
//         8 + (8 - (max_metrics * max_name_bytes + max_metrics) % 8) +
//         max_metrics * (max_name_bytes + sizeof(uint64_t) + 1) + 8;

//     EXPECT_EQ(metrics.storage_bytes(), storage_size);

//     metric += 10;

//     std::vector<uint8_t> data(metrics.storage_bytes());

//     metrics.copy_storage(data.data());

//     abacus::view view;

//     view.set_data(data.data());

//     EXPECT_EQ(max_metrics, view.max_metrics());
//     EXPECT_EQ(scope.size(), view.scope_size());
//     EXPECT_EQ(scope, view.scope());
//     EXPECT_EQ(1U, view.count());
//     EXPECT_EQ(max_name_bytes, view.max_name_bytes());
//     EXPECT_EQ(metrics.metric_name(0), view.metric_name(0));
//     EXPECT_EQ(metrics.metric_type(0), abacus::value_type::unsigned_integral);
//     EXPECT_EQ(view.metric_type(0), abacus::value_type::unsigned_integral);
//     uint64_t metric_value = 12;
//     uint64_t view_value = 11;
//     metrics.metric_value(metric_value, 0);
//     view.metric_value(view_value, 0);
//     EXPECT_EQ(metric_value, view_value);
//     EXPECT_EQ(view.data(), data.data());
// }
