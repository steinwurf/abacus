// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <iostream>

#include <abacus/metrics.hpp>

// Simple example of metrics on a car.

int main()
{
    /// Choose the constructor values for the metrics class
    uint64_t max_metrics = 10;
    uint64_t max_name_bytes = 32;

    abacus::metrics car(max_metrics, max_name_bytes, "Car");

    /// A car has headlights. Two of them usually
    auto headlights = car.initialize_metric("headlights");

    headlights += 2;

    /// What about the gas mileage?
    auto fuel_consumption = car.initialize_metric("fuel consumption km/L");

    fuel_consumption += 20;

    /// Most cars are 4-wheelers as well
    auto wheels = car.initialize_metric("Wheels");

    wheels += 4;

    /// We can print out the counters neatly.
    std::cout << car.to_json() << std::endl;

    /// We want to export the metrics memory, so we need a new storage
    std::vector<uint8_t> data(car.storage_bytes());

    /// Copy the memory into the new storage
    car.copy_storage(data.data());

    /// We can use the view class to read the pointed-to values
    abacus::view car_view;

    /// The view should operate on the copied storage
    car_view.set_data(data.data());

    /// Lets see what it contains:
    std::cout << "Car has the following metrics:" << std::endl;

    for (std::size_t i = 0; i < car_view.metrics_count(); i++)
    {
        /// If a counter in memory has no name, it's not yet initialized.
        /// We will ignore it.
        if (!car_view.is_metric_initialized(i))
        {
            continue;
        }
        /// Get the name from memory and the address of the value and
        /// dereference it.
        std::cout << "\t" << car_view.metric_name(i) << ": "
                  << car_view.metric_value(i) << std::endl;
    }

    return 0;
}
