// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <iostream>

#include <abacus/metrics.hpp>

// Simple example of metrics on a car.

int main()
{
    uint64_t max_metrics = 10;
    uint64_t max_name_bytes = 32;

    abacus::metrics metrics(max_metrics, max_name_bytes, "Car");

    auto acceleration = metrics.initialize_metric(0, "0 to 100 / s");

    acceleration += 3;

    auto fuel_consumption =
        metrics.initialize_metric(1, "fuel consumption km/L");

    fuel_consumption += 20;

    auto wheels = metrics.initialize_metric(2, "Wheels");

    // Remember the spare tire in the trunk ;-)
    wheels += 5;

    std::cout << metrics.to_json();

    return 0;
}