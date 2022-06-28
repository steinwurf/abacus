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
        vw.initialize_metric<abacus::value_type::int64>(name2);
    auto bmw_maintenance =
        bmw.initialize_metric<abacus::value_type::int64>(name2);

    vw_maintenance = -10;
    bmw_maintenance = 240;

    auto vw_registered =
        vw.initialize_metric<abacus::value_type::boolean>(name3);
    auto bmw_registered =
        bmw.initialize_metric<abacus::value_type::boolean>(name3);

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
            case abacus::value_type::uint64:
            {
                uint64_t value = 0;
                view.metric_value(i, value);
                value_string = std::to_string(value);
                break;
            }
            case abacus::value_type::int64:
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
            case abacus::value_type::float64:
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
