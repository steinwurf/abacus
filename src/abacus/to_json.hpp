// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <vector>

#include "version.hpp"
#include "view.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

/// @return a JSON-formatted string of a single views data.
/// @param single_view A view with access to metrics-data.
auto to_json(const view& single_view, std::string filter = "") -> std::string;

/// @return a JSON-formatted string of the data of the views given.
/// @param views A vector of views with access to metrics-data.
auto to_json(const std::vector<view>& views, std::string filter = "")
    -> std::string;

}
}
