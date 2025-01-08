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

static const char* expected_json_minimal = R"({
  "metric0" : 42,
  "metric1" : -42,
  "metric2" : true
})";

TEST(test_to_json, to_json_minimal)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";
    std::string name2 = "metric2";

    std::map<std::string, abacus::type> infos = {
        {name0,
         abacus::uint64{abacus::kind::COUNTER, "An unsigned integer metric",
                        abacus::unit{"bytes"}, abacus::min{uint64_t{0U}},
                        abacus::max{uint64_t{100U}}}},
        {name1, abacus::int64{abacus::kind::GAUGE, "A signed integer metric",
                              abacus::unit{"USD"}, abacus::min{int64_t{-100}},
                              abacus::max{int64_t{100}}}},
        {name2, abacus::boolean{abacus::kind::CONSTANT, "A boolean constant"}}};

    abacus::metrics metrics(infos);

    auto m0 = metrics.initialize_metric<abacus::uint64>(name0, 42);
    auto m1 = metrics.initialize_metric<abacus::int64>(name1, -42);
    metrics.initialize_constant<abacus::boolean>(name2, true);

    abacus::view view;
    view.set_meta_data(metrics.metadata_data(), metrics.metadata_bytes());
    view.set_value_data(metrics.value_data(), metrics.value_bytes());

    auto json_from_view_minimal = abacus::to_json(view, true);
    EXPECT_EQ(json_from_view_minimal, expected_json_minimal);
}

static const char* expected_json = R"({
  "metric0" : {
    "uint64" : {
      "description" : "An unsigned integer metric",
      "max" : "100",
      "min" : "0",
      "unit" : "bytes"
    },
    "value" : 42
  },
  "metric1" : {
    "int64" : {
      "description" : "A signed integer metric",
      "kind" : "GAUGE",
      "max" : "100",
      "min" : "-100",
      "unit" : "USD"
    },
    "value" : -42
  },
  "metric2" : {
    "boolean" : {
      "description" : "A boolean constant",
      "kind" : "CONSTANT"
    },
    "value" : true
  }
})";

TEST(test_to_json, to_json)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";
    std::string name2 = "metric2";

    std::map<std::string, abacus::type> infos = {
        {name0,
         abacus::uint64{abacus::kind::COUNTER, "An unsigned integer metric",
                        abacus::unit{"bytes"}, abacus::min{uint64_t{0U}},
                        abacus::max{uint64_t{100U}}}},
        {name1, abacus::int64{abacus::kind::GAUGE, "A signed integer metric",
                              abacus::unit{"USD"}, abacus::min{int64_t{-100}},
                              abacus::max{int64_t{100}}}},
        {name2, abacus::boolean{abacus::kind::CONSTANT, "A boolean constant"}}};

    abacus::metrics metrics(infos);

    auto m0 = metrics.initialize_metric<abacus::uint64>(name0, 42);
    auto m1 = metrics.initialize_metric<abacus::int64>(name1, -42);
    metrics.initialize_constant<abacus::boolean>(name2, true);

    abacus::view view;
    view.set_meta_data(metrics.metadata_data(), metrics.metadata_bytes());
    view.set_value_data(metrics.value_data(), metrics.value_bytes());

    auto json_from_view = abacus::to_json(view);

    std::error_code error;
    bourne::json::parse(json_from_view, error);
    EXPECT_FALSE(error);

    auto json_from_data =
        abacus::to_json(metrics.metadata_data(), metrics.metadata_bytes(),
                        metrics.value_data(), metrics.value_bytes());

    EXPECT_EQ(json_from_view, json_from_data);
    EXPECT_EQ(json_from_view, expected_json) << json_from_view;
}
