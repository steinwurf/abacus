// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <array>
#include <vector>

#include "view.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

/// @return a JSON-formatted string of a single views data.
///
/// The keys in the JSON string will be "scope + metric_name", and
/// the values will be a JSON-object with keys "description", "value" and
/// "is_constant". Example output with scope "car" could be:
///
///     {
///         "car.fuel_consumption": {
/// 	        "description": "Fuel consumption in kilometers per liter",
/// 	        "value": 22.300000,
/// 	        "constant": true,
///         },
///         "car.wheels": {
/// 	        "description": "Wheels on the car",
/// 	        "value": 4,
/// 	        "constant": true,
///         }
///     }
/// @param views A vector of views.
auto to_json(view single_view, bool closed_brackets = true) -> std::string;

/// @return a JSON-formatted string of the data of the views given.
///
/// The keys in the JSON string will be "scope + metric_name", and
/// the values will be a JSON-object with keys "description", "value" and
/// "is_constant". Example output with scope "car" could be:
///
///     {
///         "car.fuel_consumption": {
/// 	        "description": "Fuel consumption in kilometers per liter",
/// 	        "value": 22.300000,
/// 	        "constant": true,
///         },
///         "car.wheels": {
/// 	        "description": "Wheels on the car",
/// 	        "value": 4,
/// 	        "constant": true,
///         }
///     }
/// @param views A vector of views.
auto to_json(std::vector<view> views) -> std::string;

}
}
