// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <iostream>

#include <abacus/metrics.hpp>
#include <abacus/view_iterator.hpp>

// Example of metrics on two cars. A Volkswagen and a BMW.

int main()
{
    /// Choose the constructor values for the metrics class
    uint16_t metric_count = 4;

    std::string name0 = "fuel_consumption";
    std::string name1 = "wheels";
    std::string name2 = "days_until_maintenance";
    std::string name3 = "registered";

    /// Create a vector that will contain the information
    std::vector<abacus::metric_info> infos;

    infos.reserve(metric_count);

    /// Add info of the metric describing the fuel consumption
    infos.push_back(abacus::metric_info{
        name0, "Fuel consumption in kilometers per liter",
        abacus::value_type::floating_point, abacus::CONSTANT});

    /// Add info of the metric describing the number of wheels
    infos.push_back(abacus::metric_info{name1, "Wheels on the car",
                                        abacus::value_type::unsigned_integral,
                                        abacus::CONSTANT});

    /// Add info of the metric describing days until maintenance
    infos.push_back(abacus::metric_info{
        name2,
        "Days until next maintenance, if less than 0, maintenance is overdue",
        abacus::value_type::signed_integral, abacus::NON_CONSTANT});

    /// Add info of the metric describing whether the car is registered
    infos.push_back(abacus::metric_info{name3, "Is the car registered",
                                        abacus::value_type::boolean,
                                        abacus::NON_CONSTANT});

    /// We create two metrics objects. One for a Volkswagen and one for a BMW
    abacus::metrics vw(infos);
    abacus::metrics bmw(infos);

    vw.push_scope("volkswagen");
    bmw.push_scope("bmw");

    /// Initialize the constant metrics
    double vw_fuel = 22.3;
    double bmw_fuel = 16.9;
    vw.initialize_constant(0, vw_fuel, name0);
    bmw.initialize_constant(0, bmw_fuel, name0);

    uint64_t wheels = 4U;
    vw.initialize_constant(1, wheels, name1);
    bmw.initialize_constant(1, wheels, name1);

    /// Initialize the non-constant metrics
    auto vw_maintenance =
        vw.initialize_metric<abacus::value_type::signed_integral>(2, name2);
    auto bmw_maintenance =
        bmw.initialize_metric<abacus::value_type::signed_integral>(2, name2);

    vw_maintenance = -10;
    bmw_maintenance = 240;

    auto vw_registered =
        vw.initialize_metric<abacus::value_type::boolean>(3, name3);
    auto bmw_registered =
        bmw.initialize_metric<abacus::value_type::boolean>(3, name3);

    vw_registered = false;
    bmw_registered = true;

    /// We can print out the metrics neatly.
    std::cout << vw.to_json() << std::endl;
    std::cout << bmw.to_json() << std::endl;

    /// We want to export the metrics memory, so we need a new storage.
    /// We copy both metrics into one data storage.
    std::vector<uint8_t> data(vw.storage_bytes() + bmw.storage_bytes());

    /// Copy the memory into the new storage
    vw.copy_storage(data.data(), vw.storage_bytes());
    bmw.copy_storage(data.data() + vw.storage_bytes(), bmw.storage_bytes());

    /// We can use the view-iterator class to separate the two metrics objects
    /// in the exported data storage.
    abacus::view_iterator car_iterator(data.data(), data.size());

    /// Now we can view both the VW and the BMW metrics using the
    /// view_iterator::get_view(index) function. Lets see what it contains:

    for (std::size_t i = 0; i < car_iterator.view_count(); i++)
    {
        auto view = car_iterator.get_view(i);
        std::cout << "View has the following metrics:" << std::endl;

        for (std::size_t i = 0; i < view.metric_count(); i++)
        {
            /// If a counter in memory has no name, it's not yet addd.
            /// We will ignore it.
            if (!view.is_metric_initialized(i))
            {
                continue;
            }
            abacus::value_type type = view.metric_type(i);

            std::string value_string;

            switch (type)
            {
            case abacus::value_type::unsigned_integral:
            {
                uint64_t value = 0;
                view.metric_value(i, value);
                value_string = std::to_string(value);
                break;
            }
            case abacus::value_type::signed_integral:
            {
                int64_t value = 0;
                view.metric_value(i, value);
                value_string = std::to_string(value);
                break;
            }
            case abacus::value_type::boolean:
            {
                bool value = false;
                view.metric_value(i, value);
                value_string = value ? "true" : "false";
                break;
            }
            case abacus::value_type::floating_point:
            {
                double value = 0.0;
                view.metric_value(i, value);
                value_string = std::to_string(value);
                break;
            }
                /// Get the name from memory and the address of the value and
                /// dereference it.
                std::cout << "\t" << view.metric_name(i) << ": " << value_string
                          << std::endl;
            }
            std::cout << std::endl;
        }
    }

    /// Or you can use view::to_json(true) for the metrics in json-format:
    for (std::size_t i = 0; i < car_iterator.view_count(); i++)
    {
        auto view = car_iterator.get_view(i);
        std::cout << "View has the following metrics:" << std::endl;
        std::cout << view.to_json() << std::endl;
    }

    /// You can reset your non-constant metrics if needed with the
    /// reset_metrics() function.
    vw.reset_metrics();
    std::cout << "Reset volkswagen metrics: " << std::endl;
    std::cout << vw.to_json() << std::endl;

    return 0;
}
