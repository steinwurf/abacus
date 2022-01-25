// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <iostream>

#include <abacus/metrics.hpp>
#include <abacus/view_iterator.hpp>

// Example of metrics on two cars. A Volkswagen and a BMW.

int main()
{
    /// Choose the constructor values for the metrics class
    uint64_t max_metrics = 10;
    uint64_t max_name_bytes = 32;
    uint64_t max_category_bytes = 32;
    abacus::metrics vw(max_metrics, max_name_bytes, max_category_bytes,
                       "volkswagen");
    abacus::metrics bmw(max_metrics, max_name_bytes, max_category_bytes, "bmw");

    vw.add_prefix("car1");
    bmw.add_prefix("car2");
    /// A car has headlights. Two of them usually
    auto headlights1 = vw.initialize_metric("headlights");
    auto headlights2 = bmw.initialize_metric("headlights");

    headlights1 += 2;
    headlights2 += 2;

    /// What about the gas mileage?
    auto fuel_consumption1 = vw.initialize_metric("fuel consumption km/L");
    auto fuel_consumption2 = bmw.initialize_metric("fuel consumption km/L");

    fuel_consumption1 += 20;
    fuel_consumption2 += 15;

    /// Most cars are 4-wheelers as well
    auto wheels1 = vw.initialize_metric("wheels");
    auto wheels2 = bmw.initialize_metric("wheels");

    wheels1 += 4;
    wheels2 += 4;

    /// We can print out the counters neatly.
    std::cout << vw.to_json() << std::endl;
    std::cout << bmw.to_json() << std::endl;

    /// We want to export the metrics memory, so we need a new storage.
    /// We copy both metrics into one data storage.
    std::vector<uint8_t> data(vw.storage_bytes() + bmw.storage_bytes());

    /// Copy the memory into the new storage
    vw.copy_storage(data.data());
    bmw.copy_storage(data.data() + vw.storage_bytes());

    /// We can use the view-iterator class to separate the two metrics objects
    /// in the exported data storage.
    abacus::view_iterator car_iterator(data.data(), data.size());

    /// Now we can view both the VW and the BMW metrics using the
    /// view_iterator::get_view(index) function. Lets see what it contains:

    for (std::size_t i = 0; i < car_iterator.view_count(); i++)
    {
        auto view = car_iterator.get_view(i);
        std::cout << view.get_category()
                  << " has the following metrics:" << std::endl;

        for (std::size_t i = 0; i < view.metrics_count(); i++)
        {
            /// If a counter in memory has no name, it's not yet initialized.
            /// We will ignore it.
            if (!view.is_metric_initialized(i))
            {
                continue;
            }
            /// Get the name from memory and the address of the value and
            /// dereference it.
            std::cout << "\t" << view.metric_name(i) << ": "
                      << view.metric_value(i) << std::endl;
        }
        std::cout << std::endl;
    }

    /// Or you can use view::to_json() for the metrics in json-format:
    for (std::size_t i = 0; i < car_iterator.view_count(); i++)
    {
        auto view = car_iterator.get_view(i);
        std::cout << view.get_category()
                  << " has the following metrics:" << std::endl;
        std::cout << view.to_json() << std::endl;
    }

    /// Or you can even call the view_iterator::to_json() function to get the
    /// json-format of all the metrics in the data storage:
    std::cout << "view_iterator::to_json():" << std::endl
              << car_iterator.to_json() << std::endl;

    return 0;
}
