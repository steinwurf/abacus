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

enum class test_enum
{
    value0 = 0,
    value1 = 1,
    value2 = 2,
    value3 = 3
};

// Simple example of metrics on a car.
int main()
{
    std::map<abacus::name, abacus::info> infos = {
        {abacus::name{"fuel_consumption"},
         abacus::constant{
             abacus::constant::float64{22.3},
             abacus::description{"Fuel consumption in kilometers per liter"},
             abacus::unit{"km/l"}}},
        {abacus::name{"wheels"},
         abacus::constant{abacus::constant::uint64{4},
                          abacus::description{"Wheels on the car"},
                          abacus::unit{"wheels"}}},
        {abacus::name{"days_until_maintenance"},
         abacus::int64{
             abacus::kind::gauge,
             abacus::description{"Days until next maintenance, if less than 0, "
                                 "maintenance is overdue"},
             abacus::unit{"days"}}},
        {abacus::name{"registered"},
         abacus::boolean{abacus::description{"Is the car registered"}}},
        {abacus::name{"license_plate"},
         abacus::constant{abacus::constant::str{"ABC-1234"},
                          abacus::description{"License plate of the car"}}},
        {abacus::name{"some_enum"},
         abacus::enum8{abacus::description{"An enum metric"},
                       {{test_enum::value0, {"value0", "The value for 0"}},
                        {test_enum::value1, {"value1", "The value for 1"}},
                        {test_enum::value2, {"value2", "The value for 2"}},
                        {test_enum::value3, {"value3", "The value for 3"}}}}}};

    abacus::metrics car(infos);

    // The car still has some time before maintenance.
    abacus::metric<abacus::int64> days_until_maintenance =
        car.initialize<abacus::int64>("days_until_maintenance");

    // The car should be registered.
    abacus::metric<abacus::boolean> registered =
        car.initialize<abacus::boolean>("registered");

    // The registration is initialized, but not set.
    assert(registered.is_initialized());
    assert(!registered.has_value());

    abacus::metric<abacus::enum8> some_enum =
        car.initialize<abacus::enum8>("some_enum").set_value(test_enum::value1);

    // Change value
    some_enum = test_enum::value2;

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
    std::memcpy(value_data.data(), car.value_data(), car.value_bytes());

    abacus::view car_view;

    auto success = car_view.set_metadata(metadata);
    assert(success);
    success = car_view.set_value_data(value_data.data(), value_data.size());
    assert(success);
    (void)success;

    std::cout << abacus::to_json(car_view) << std::endl;

    return 0;
}
