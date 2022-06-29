// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <iostream>

#include <abacus/metrics.hpp>
#include <abacus/to_json.hpp>
#include <abacus/view.hpp>

// Example of metrics on two cars. A Volkswagen and a BMW.

int main()
{
    std::string name0 = "fuel_consumption";
    std::string name1 = "wheels";
    std::string name2 = "days_until_maintenance";
    std::string name3 = "registered";

    abacus::metric_info infos[4] = {
        abacus::metric_info{name0, "Fuel consumption in kilometers per liter",
                            abacus::metric_type::float64,
                            abacus::qualifier::constant},
        abacus::metric_info{name1, "Wheels on the car",
                            abacus::metric_type::uint64,
                            abacus::qualifier::constant},
        abacus::metric_info{name2,
                            "Days until next maintenance, if less than 0, "
                            "maintenance is overdue",
                            abacus::metric_type::int64,
                            abacus::qualifier::non_constant},
        abacus::metric_info{name3, "Is the car registered",
                            abacus::metric_type::boolean,
                            abacus::qualifier::non_constant}};

    /// We create two metrics objects. One for a Volkswagen and one for a BMW
    abacus::metrics vw(infos);
    abacus::metrics bmw(infos);

    vw.push_scope("volkswagen");
    bmw.push_scope("bmw");

    /// Initialize the constant metrics
    double vw_fuel = 22.3;
    double bmw_fuel = 16.9;
    vw.initialize_constant(name0, vw_fuel);
    bmw.initialize_constant(name0, bmw_fuel);

    uint64_t wheels = 4U;
    vw.initialize_constant(name1, wheels);
    bmw.initialize_constant(name1, wheels);

    /// Initialize the non-constant metrics
    auto vw_maintenance =
        vw.initialize_metric<abacus::metric_type::int64>(name2);
    auto bmw_maintenance =
        bmw.initialize_metric<abacus::metric_type::int64>(name2);

    vw_maintenance = -10;
    bmw_maintenance = 240;

    auto vw_registered =
        vw.initialize_metric<abacus::metric_type::boolean>(name3);
    auto bmw_registered =
        bmw.initialize_metric<abacus::metric_type::boolean>(name3);

    vw_registered = false;
    bmw_registered = true;

    /// We want to export the metrics memory, so we need a new storage.
    /// We copy both metrics into one data storage.
    std::vector<uint8_t> vw_data(vw.storage_bytes());
    std::vector<uint8_t> bmw_data(bmw.storage_bytes());

    /// Copy the memory into the new storage
    vw.copy_storage(vw_data.data(), vw.storage_bytes());
    bmw.copy_storage(bmw_data.data(), bmw.storage_bytes());

    /// We can use the view-iterator class to separate the two metrics objects
    /// in the exported data storage.
    std::vector<abacus::view> car_views;
    car_views.reserve(2);

    abacus::view vw_view;
    abacus::view bmw_view;

    vw_view.set_data(vw_data.data());
    bmw_view.set_data(bmw_data.data());

    car_views.push_back(vw_view);
    car_views.push_back(bmw_view);

    /// We can use the to_json() function to see the metrics in json-format:
    std::string json_data = abacus::to_json(car_views);

    std::cout << json_data << std::endl;

    return 0;
}
