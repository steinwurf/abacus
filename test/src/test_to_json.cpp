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
  "metric2" : true,
  "metric3" : 2
})";

TEST(test_to_json, to_json_minimal)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";
    std::string name2 = "metric2";
    std::string name3 = "metric3";

    std::map<abacus::name, abacus::info> infos = {
        {abacus::name{name0},
         abacus::uint64{
             abacus::counter, abacus::description{"An unsigned integer metric"},
             abacus::required, abacus::unit{"bytes"}, abacus::min{uint64_t{0U}},
             abacus::max{uint64_t{100U}}}},
        {abacus::name{name1},
         abacus::int64{abacus::gauge,
                       abacus::description{"A signed integer metric"},
                       abacus::required, abacus::unit{"USD"},
                       abacus::min{int64_t{-100}}, abacus::max{int64_t{100}}}},
        {abacus::name{name2},
         abacus::boolean{abacus::constant,
                         abacus::description{"A boolean constant"},
                         abacus::required}},
        {abacus::name{name3},
         abacus::enum8{abacus::description{"An enum metric"},
                       {{0, {"value0", "The value for 0"}},
                        {1, {"value1", "The value for 1"}},
                        {2, {"value2", "The value for 2"}},
                        {3, {"value3", "The value for 3"}}},
                       abacus::required}}};

    abacus::metrics metrics(infos);

    auto m0 = metrics.initialize_required<abacus::uint64>(name0, 42);
    auto m1 = metrics.initialize_required<abacus::int64>(name1, -42);
    metrics.initialize_constant<abacus::boolean>(name2, true);
    auto m3 = metrics.initialize_required<abacus::enum8>(name3, 2);

    (void)m0;
    (void)m1;
    (void)m3;

    abacus::view view;
    bool success =
        view.set_meta_data(metrics.metadata_data(), metrics.metadata_bytes());
    ASSERT_TRUE(success);
    success = view.set_value_data(metrics.value_data(), metrics.value_bytes());
    ASSERT_TRUE(success);

    auto json_from_view_minimal = abacus::to_json(view, true);
    EXPECT_EQ(json_from_view_minimal, expected_json_minimal);
}

static const char* expected_json = R"({
  "metric0" : {
    "offset" : 4,
    "optional" : false,
    "uint64" : {
      "description" : "An unsigned integer metric",
      "kind" : "COUNTER",
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
    "offset" : 13,
    "optional" : false,
    "value" : -42
  },
  "metric2" : {
    "boolean" : {
      "description" : "A boolean constant",
      "kind" : "CONSTANT"
    },
    "offset" : 22,
    "optional" : false,
    "value" : true
  },
  "metric3" : {
    "enum8" : {
      "description" : "An enum metric",
      "values" : {
        "0" : {
          "description" : "The value for 0",
          "name" : "value0"
        },
        "1" : {
          "description" : "The value for 1",
          "name" : "value1"
        },
        "2" : {
          "description" : "The value for 2",
          "name" : "value2"
        },
        "3" : {
          "description" : "The value for 3",
          "name" : "value3"
        }
      }
    },
    "offset" : 24,
    "optional" : false,
    "value" : 2
  }
})";

namespace
{
enum class test_enum
{
    value0 = 0,
    value1 = 1,
    value2 = 2,
    value3 = 3
};
}

TEST(test_to_json, to_json)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";
    std::string name2 = "metric2";
    std::string name3 = "metric3";

    std::map<abacus::name, abacus::info> infos = {
        {abacus::name{name0},
         abacus::uint64{
             abacus::counter, abacus::description{"An unsigned integer metric"},
             abacus::required, abacus::unit{"bytes"}, abacus::min{uint64_t{0U}},
             abacus::max{uint64_t{100U}}}},
        {abacus::name{name1},
         abacus::int64{abacus::gauge,
                       abacus::description{"A signed integer metric"},
                       abacus::required, abacus::unit{"USD"},
                       abacus::min{int64_t{-100}}, abacus::max{int64_t{100}}}},
        {abacus::name{name2},
         abacus::boolean{abacus::constant,
                         abacus::description{"A boolean constant"},
                         abacus::required}},
        {abacus::name{name3},
         abacus::enum8{abacus::description{"An enum metric"},
                       {{0, {"value0", "The value for 0"}},
                        {1, {"value1", "The value for 1"}},
                        {2, {"value2", "The value for 2"}},
                        {3, {"value3", "The value for 3"}}},
                       abacus::required}}};

    abacus::metrics metrics(infos);

    auto m0 = metrics.initialize_required<abacus::uint64>(name0, 42);
    auto m1 = metrics.initialize_required<abacus::int64>(name1, -42);
    metrics.initialize_constant<abacus::boolean>(name2, true);
    auto m3 = metrics.initialize_required<abacus::enum8>(
        name3, (uint8_t)test_enum::value1);

    m3 = test_enum::value2;

    (void)m0;
    (void)m1;

    abacus::view view;
    bool success =
        view.set_meta_data(metrics.metadata_data(), metrics.metadata_bytes());
    ASSERT_TRUE(success);
    success = view.set_value_data(metrics.value_data(), metrics.value_bytes());
    ASSERT_TRUE(success);

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
