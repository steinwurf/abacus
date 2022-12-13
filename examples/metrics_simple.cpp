// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <iostream>

#include <abacus/metrics.hpp>
#include <abacus/to_json.hpp>
#include <abacus/view.hpp>

// Simple example of metrics on a car.

int main()
{
    std::string name0 = "fuel_consumption";
    std::string name1 = "wheels";
    std::string name2 = "days_until_maintenance";
    std::string name3 = "registered";

    abacus::metric_info infos[4] = {
        abacus::metric_info{name0, "Fuel consumption in kilometers per liter",
                            abacus::metric_type::float64,
                            abacus::metric_kind::constant},
        abacus::metric_info{name1, "Wheels on the car",
                            abacus::metric_type::uint64,
                            abacus::metric_kind::constant},
        abacus::metric_info{name2,
                            "Days until next maintenance, if less than 0, "
                            "maintenance is overdue",
                            abacus::metric_type::int64,
                            abacus::metric_kind::gauge},
        abacus::metric_info{name3, "Is the car registered",
                            abacus::metric_type::boolean,
                            abacus::metric_kind::gauge}};

    abacus::metrics car(infos);

    car.initialize_constant("fuel_consumption", (double)22.3);
    car.initialize_constant("wheels", (uint64_t)4);
    auto days_until_maintenance =
        car.initialize_metric<abacus::metric_type::int64>(
            "days_until_maintenance");
    auto registered =
        car.initialize_metric<abacus::metric_type::boolean>("registered");

    // The car should be registered.
    registered = true;

    // The car is overdue maintenance.
    days_until_maintenance = -10;

    /// We want to export the metrics memory, so we need a new storage
    std::vector<uint8_t> meta_data(car.meta_bytes());
    std::vector<uint8_t> value_data(car.value_bytes());

    /// Copy the memory into the new storage
    std::memcpy(meta_data.data(), car.meta_data(), car.meta_bytes());
    std::memcpy(value_data.data(), car.value_data(), car.value_bytes());

    abacus::view car_view;

    car_view.set_meta_data(meta_data.data());
    car_view.set_value_data(value_data.data());

    std::cout << abacus::to_json(car_view) << std::endl;

    return 0;
}
