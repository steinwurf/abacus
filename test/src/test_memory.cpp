// // Copyright (c) Steinwurf ApS 2020.
// // All Rights Reserved
// //
// // Distributed under the "BSD License". See the accompanying LICENSE.rst
// // file.

// #include <cstring>
// #include <gtest/gtest.h>
// #include <string>

// #include <abacus/metrics.hpp>
// #include <cstdint>

// TEST(test_memory, default_test)
// {
//     uint64_t count = 0xFFFFFFFFFFFFFFFFU;
//     uint16_t max_name_bytes = 32;
//     uint16_t max_metrics = 32;
//     abacus::metrics metrics(max_metrics, max_name_bytes);

//     for (std::size_t i = 0; i < max_metrics; i++)
//     {
//         std::string name = "metric" + std::to_string(i);
//         std::size_t offset = i % 4;
//         switch (offset)
//         {
//         case 0:
//         {
//             auto bool_metric = metrics.add_metric<bool>(name);
//             bool_metric = true;
//             break;
//         }
//         case 1:
//         {
//             auto uint_metric = metrics.add_metric<uint64_t>(name);
//             uint_metric += count;
//             break;
//         }
//         case 2:
//         {
//             auto double_metric = metrics.add_metric<double>(name);
//             double_metric += 1.23 * i;
//             break;
//         }
//         case 3:
//         {
//             auto int_metric = metrics.add_metric<int64_t>(name);
//             int_metric -= count + 1;
//             break;
//         }
//         }
//     }
// }
