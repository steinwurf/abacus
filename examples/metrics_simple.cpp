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
    uint16_t metric_count = 4;

    std::string name0 = "fuel_consumption";
    std::string name1 = "wheels";
    std::string name2 = "days_until_maintenance";
    std::string name3 = "registered";

    std::vector<abacus::metric_info> infos;

    infos.reserve(metric_count);

    infos.push_back(abacus::metric_info{
        name0, "Fuel consumption in kilometers per liter",
        abacus::value_type::floating_point, abacus::CONSTANT});
    infos.push_back(abacus::metric_info{name1, "Wheels on the car",
                                        abacus::value_type::unsigned_integral,
                                        abacus::CONSTANT});
    infos.push_back(abacus::metric_info{
        name2,
        "Days until next maintenance, if less than 0, maintenance is overdue",
        abacus::value_type::signed_integral, abacus::NON_CONSTANT});
    infos.push_back(abacus::metric_info{name3, "Is the car registered",
                                        abacus::value_type::boolean,
                                        abacus::NON_CONSTANT});
    abacus::metrics car(infos);

    car.initialize_constant(0, (double)22.3, "fuel_consumption");
    car.initialize_constant(1, (uint64_t)4, "wheels");
    auto days_until_maintenance =
        car.initialize_metric<abacus::value_type::signed_integral>(
            2, "days_until_maintenance");
    auto registered =
        car.initialize_metric<abacus::value_type::boolean>(3, "registered");

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
