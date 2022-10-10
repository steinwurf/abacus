// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics.hpp>
#include <abacus/to_json.hpp>
#include <abacus/view.hpp>

#include <bourne/json.hpp>

static const char* expected_json = R"({
  "metric0" : {
    "description" : "An unsigned integer metric",
    "is_constant" : false,
    "value" : 42
  },
  "metric1" : {
    "description" : "A signed integer metric",
    "is_constant" : false,
    "value" : -42
  },
  "protocol_version" : 0
})";

static const char* expected_json_slim = R"({
  "metric0" : 42,
  "metric1" : -42
})";

TEST(test_to_json, api)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";

    abacus::metric_info infos[2] = {
        abacus::metric_info{name0, "An unsigned integer metric",
                            abacus::metric_type::uint64},
        abacus::metric_info{name1, "A signed integer metric",
                            abacus::metric_type::int64}};

    abacus::metrics metrics(infos);

    auto m0 = metrics.initialize_metric<abacus::metric_type::uint64>(name0);
    auto m1 = metrics.initialize_metric<abacus::metric_type::int64>(name1);

    m0 = 42;
    m1 = -42;

    abacus::view view;
    view.set_meta_data(metrics.meta_data());
    view.set_value_data(metrics.value_data());

    auto json_from_view = abacus::to_json(view);

    std::error_code error;
    bourne::json::parse(json_from_view, error);
    EXPECT_FALSE(error);

    auto json_from_data =
        abacus::to_json(metrics.meta_data(), metrics.value_data());

    EXPECT_EQ(json_from_view, json_from_data);
    EXPECT_EQ(json_from_view, expected_json);

    auto json_from_view_slim = abacus::to_json(view, true);
    EXPECT_EQ(json_from_view_slim, expected_json_slim);
}
