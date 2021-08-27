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

    abacus::metrics vw(max_metrics, max_name_bytes, "Volkswagen");
    abacus::metrics bmw(max_metrics, max_name_bytes, "BMW");

    /// A car has headlights. Two of them usually
    auto headlights1 = vw.initialize_metric(0, "headlights");
    auto headlights2 = bmw.initialize_metric(0, "headlights");

    headlights1 += 2;
    headlights2 += 2;

    /// What about the gas mileage?
    auto fuel_consumption1 = vw.initialize_metric(1, "fuel consumption km/L");
    auto fuel_consumption2 = bmw.initialize_metric(1, "fuel consumption km/L");

    fuel_consumption1 += 20;
    fuel_consumption2 += 15;

    /// Most cars are 4-wheelers as well
    auto wheels1 = vw.initialize_metric(2, "Wheels");
    auto wheels2 = bmw.initialize_metric(2, "Wheels");

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
        std::cout << view.raw_title()
                  << " has the following metrics:" << std::endl;

        for (std::size_t i = 0; i < view.max_metrics(); i++)
        {
            /// If a counter in memory has no name, it's not yet initialized.
            /// We will ignore it.
            if (view.raw_name(i)[0] == 0)
            {
                continue;
            }
            /// Get the name from memory and the address of the value and
            /// dereference it.
            std::cout << "\t" << view.raw_name(i) << ": " << *view.raw_value(i)
                      << std::endl;
        }
        std::cout << std::endl;
    }

    /// Or you can use view::to_json() for the metrics in json-format:
    for (std::size_t i = 0; i < car_iterator.view_count(); i++)
    {
        auto view = car_iterator.get_view(i);
        std::cout << view.raw_title()
                  << " has the following metrics:" << std::endl;
        std::cout << view.to_json() << std::endl;
    }
    return 0;
}