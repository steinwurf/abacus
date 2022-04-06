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

    abacus::metrics car(max_metrics, max_name_bytes);

    car.push_scope("car");

    /// A car has headlights. Two of them usually
    auto headlights = car.add_metric<bool>("has_headlights");

    headlights = true;

    /// What about the gas mileage?
    auto fuel_consumption = car.add_metric<double>("fuel consumption km/L");

    fuel_consumption += 21.8;

    /// Most cars are 4-wheelers as well
    auto wheels = car.add_metric<uint64_t>("Wheels");

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

    for (std::size_t i = 0; i < car_view.count(); i++)
    {
        /// If a counter in memory has no name, it's not yet addd.
        /// We will ignore it.
        if (!car_view.has_metric(i))
        {
            continue;
        }
        abacus::value_type type = car_view.metric_type(i);

        std::string value_string;

        switch (type)
        {
        case abacus::value_type::unsigned_integral:
        {
            uint64_t value;
            car_view.metric_value(value, i);
            value_string = std::to_string(value);
            break;
        }
        case abacus::value_type::signed_integral:
        {
            int64_t value;
            car_view.metric_value(value, i);
            value_string = std::to_string(value);
            break;
        }
        case abacus::value_type::boolean:
        {
            bool value;
            car_view.metric_value(value, i);
            value_string = std::to_string(value);
            break;
        }
        case abacus::value_type::floating_point:
        {
            double value;
            car_view.metric_value(value, i);
            value_string = std::to_string(value);
            break;
        }
        }
        /// Get the name from memory and the address of the value and
        /// dereference it.
        std::cout << "\t" << car_view.metric_name(i) << ": " << value_string
                  << std::endl;
    }

    return 0;
}
