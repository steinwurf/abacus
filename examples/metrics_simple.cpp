// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <iostream>

#include <abacus/metrics.hpp>

// Simple example of metrics on a car.

int main()
{
    std::string name0 = "fuel_consumption";
    std::string name1 = "wheels";
    std::string name2 = "days_until_maintenance";
    std::string name3 = "registered";

    abacus::metric_info infos[4] = {
        abacus::metric_info{name0, "Fuel consumption in kilometers per liter",
                            abacus::value_type::float64,
                            abacus::qualifier::constant},
        abacus::metric_info{name1, "Wheels on the car",
                            abacus::value_type::uint64,
                            abacus::qualifier::constant},
        abacus::metric_info{name2,
                            "Days until next maintenance, if less than 0, "
                            "maintenance is overdue",
                            abacus::value_type::int64,
                            abacus::qualifier::non_constant},
        abacus::metric_info{name3, "Is the car registered",
                            abacus::value_type::boolean,
                            abacus::qualifier::non_constant}};

    abacus::metrics car(infos);

    car.initialize_constant("fuel_consumption", (double)22.3);
    car.initialize_constant("wheels", (uint64_t)4);
    auto days_until_maintenance =
        car.initialize_metric<abacus::value_type::int64>(
            "days_until_maintenance");
    auto registered =
        car.initialize_metric<abacus::value_type::boolean>("registered");

    car.push_scope("car");

    // The car should be registered.
    registered = true;

    // The car is overdue maintenance.
    days_until_maintenance = -10;

    /// We can print out the counters neatly.
    std::cout << car.to_json() << std::endl;

    /// We want to export the metrics memory, so we need a new storage
    std::vector<uint8_t> data(car.storage_bytes());

    /// Copy the memory into the new storage
    car.copy_storage(data.data(), data.size());

    return 0;
}
