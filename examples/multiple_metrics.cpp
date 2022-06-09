// // Copyright (c) Steinwurf ApS 2020.
// // All Rights Reserved
// //
// // Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

// #include <iostream>

// #include <abacus/metrics.hpp>
// #include <abacus/view_iterator.hpp>

// // Example of metrics on two cars. A Volkswagen and a BMW.

// int main()
// {
//     /// Choose the constructor values for the metrics class
//     uint64_t max_metrics = 10;
//     uint64_t max_name_bytes = 50;
//     abacus::metrics vw(max_metrics, max_name_bytes);
//     abacus::metrics bmw(max_metrics, max_name_bytes);

//     vw.push_scope("volkswagen");
//     bmw.push_scope("bmw");
//     /// A car has headlights. Two of them usually
//     auto headlights1 = vw.add_metric<bool>("headlights");
//     auto headlights2 = bmw.add_metric<bool>("headlights");

//     headlights1 = true;
//     headlights2 = false;

//     /// What about the gas mileage?
//     auto fuel_consumption1 = vw.add_metric<double>("fuel_consumption");
//     auto fuel_consumption2 = bmw.add_metric<double>("fuel_consumption");

//     fuel_consumption1 = 20.8;
//     fuel_consumption2 = 15.9;

//     /// Most cars are 4-wheelers as well
//     auto wheels1 = vw.add_metric<uint64_t>("wheels");
//     auto wheels2 = bmw.add_metric<uint64_t>("wheels");

//     wheels1 = 4;
//     wheels2 = 4;

//     /// We can print out the counters neatly.
//     std::cout << vw.to_json() << std::endl;
//     std::cout << bmw.to_json() << std::endl;

//     /// We want to export the metrics memory, so we need a new storage.
//     /// We copy both metrics into one data storage.
//     std::vector<uint8_t> data(vw.storage_bytes() + bmw.storage_bytes());

//     /// Copy the memory into the new storage
//     vw.copy_storage(data.data());
//     bmw.copy_storage(data.data() + vw.storage_bytes());

//     /// We can use the view-iterator class to separate the two metrics
//     objects
//     /// in the exported data storage.
//     abacus::view_iterator car_iterator(data.data(), data.size());

//     /// Now we can view both the VW and the BMW metrics using the
//     /// view_iterator::get_view(index) function. Lets see what it contains:

//     for (std::size_t i = 0; i < car_iterator.view_count(); i++)
//     {
//         auto view = car_iterator.get_view(i);
//         std::cout << "View has the following metrics:" << std::endl;

//         for (std::size_t i = 0; i < view.count(); i++)
//         {
//             /// If a counter in memory has no name, it's not yet addd.
//             /// We will ignore it.
//             if (!view.has_metric(i))
//             {
//                 continue;
//             }
//             abacus::value_type type = view.metric_type(i);

//             std::string value_string;

//             switch (type)
//             {
//             case abacus::value_type::unsigned_integral:
//             {
//                 uint64_t value;
//                 view.metric_value(value, i);
//                 value_string = std::to_string(value);
//                 break;
//             }
//             case abacus::value_type::signed_integral:
//             {
//                 int64_t value;
//                 view.metric_value(value, i);
//                 value_string = std::to_string(value);
//                 break;
//             }
//             case abacus::value_type::boolean:
//             {
//                 bool value;
//                 view.metric_value(value, i);
//                 value_string = std::to_string(value);
//                 break;
//             }
//             case abacus::value_type::floating_point:
//             {
//                 double value;
//                 view.metric_value(value, i);
//                 value_string = std::to_string(value);
//                 break;
//             }
//                 /// Get the name from memory and the address of the value and
//                 /// dereference it.
//                 std::cout << "\t" << view.metric_name(i) << ": " <<
//                 value_string
//                           << std::endl;
//             }
//             std::cout << std::endl;
//         }
//     }

//     /// Or you can use view::to_json(true) for the metrics in json-format:
//     for (std::size_t i = 0; i < car_iterator.view_count(); i++)
//     {
//         auto view = car_iterator.get_view(i);
//         std::cout << "View has the following metrics:" << std::endl;
//         std::cout << view.to_json() << std::endl;
//     }

//     /// Or you can even call the view_iterator::to_json() function to get
//     /// the json-format of all the metrics in the data storage:
//     std::cout << "view_iterator::to_json():" << std::endl
//               << car_iterator.to_json() << std::endl;

//     /// You can reset your metrics if needed with the reset_metrics()
//     /// function.
//     vw.reset_metrics();
//     std::cout << vw.to_json() << std::endl;

//     return 0;
// }
