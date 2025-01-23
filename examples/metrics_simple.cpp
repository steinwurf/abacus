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

    std::map<abacus::name, abacus::info> infos = {
        {abacus::name{name0},
         abacus::float64{
             abacus::constant,
             abacus::description{"Fuel consumption in kilometers per liter"},
             abacus::required, abacus::unit{"km/l"}}},
        {abacus::name{name1},
         abacus::uint64{abacus::constant,
                        abacus::description{"Wheels on the car"},
                        abacus::required, abacus::unit{"wheels"}}},
        {abacus::name{name2},
         abacus::int64{
             abacus::gauge,
             abacus::description{"Days until next maintenance, if less than 0, "
                                 "maintenance is overdue"},
             abacus::required, abacus::unit{"days"}}},
        {abacus::name{name3},
         abacus::boolean{abacus::gauge,
                         abacus::description{"Is the car registered"},
                         abacus::optional}}};

    abacus::metrics car(infos);

    car.initialize_constant<abacus::float64>("fuel_consumption", 22.3);
    car.initialize_constant<abacus::uint64>("wheels", 4);

    // The car still has some time before maintenance.
    abacus::int64::required days_until_maintenance =
        car.initialize_required<abacus::int64>("days_until_maintenance", 10);

    // The car should be registered.
    abacus::boolean::optional registered =
        car.initialize_optional<abacus::boolean>("registered");

    // The registration is initialized, but not set.
    assert(registered.is_initialized());
    assert(!registered.has_value());

    // The car hasn't been registered.
    registered = false;

    // The registration is now set.
    assert(registered.has_value());

    // We can reset the registration.
    registered.reset();
    assert(!registered.has_value());

    // The car is now registered.
    registered = true;

    // The car has been driven for a while, and now maintenance is overdue.
    days_until_maintenance = -10;

    // We want to export the metrics memory, so we need a new storage
    auto metadata = car.metadata();
    std::vector<uint8_t> value_data(car.value_bytes());

    // Copy the memory into the new storage
    std::memcpy(value_data.data(), ((const abacus::metrics&)car).value_data(),
                car.value_bytes());

    abacus::view car_view;

    auto success = car_view.set_meta_data(metadata);
    assert(success);
    success = car_view.set_value_data(value_data.data(), value_data.size());
    assert(success);
    (void)success;

    std::cout << abacus::to_json(car_view) << std::endl;

    return 0;
}
