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

    std::map<abacus::name, abacus::type> infos = {
        {abacus::name{name0},
         abacus::float64{abacus::kind::CONSTANT,
                         "Fuel consumption in kilometers per liter",
                         abacus::required, abacus::unit{"km/l"}}},
        {abacus::name{name1},
         abacus::uint64{abacus::kind::CONSTANT, "Wheels on the car",
                        abacus::required, abacus::unit{"wheels"}}},
        {abacus::name{name2},
         abacus::int64{abacus::kind::GAUGE,
                       "Days until next maintenance, if less than 0, "
                       "maintenance is overdue",
                       abacus::required, abacus::unit{"days"}}},
        {abacus::name{name3},
         abacus::boolean{abacus::kind::GAUGE, "Is the car registered",
                         abacus::required}}};

    abacus::metrics car(infos);

    car.initialize_constant<abacus::float64>("fuel_consumption", 22.3);
    car.initialize_constant<abacus::uint64>("wheels", 4);

    // The car still has some time before maintenance.
    auto days_until_maintenance =
        car.initialize_required<abacus::int64>("days_until_maintenance", 10);

    // The car should be registered.
    auto registered =
        car.initialize_required<abacus::boolean>("registered", false);

    // The car has been registered.
    registered = true;

    // The car has been driven for a while, and now maintenance is overdue.
    days_until_maintenance = -1;

    /// We want to export the metrics memory, so we need a new storage
    std::vector<uint8_t> meta_data(car.metadata_bytes());
    std::vector<uint8_t> value_data(car.value_bytes());

    /// Copy the memory into the new storage
    std::memcpy(meta_data.data(), car.metadata_data(), car.metadata_bytes());
    std::memcpy(value_data.data(), car.value_data(), car.value_bytes());

    abacus::view car_view;

    car_view.set_meta_data(meta_data.data(), meta_data.size());
    auto success =
        car_view.set_value_data(value_data.data(), value_data.size());
    (void)success;
    assert(success);

    std::cout << abacus::to_json(car_view) << std::endl;

    return 0;
}
