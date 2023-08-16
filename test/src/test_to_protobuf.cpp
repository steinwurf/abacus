
// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/metrics.hpp>
#include <abacus/to_protobuf.hpp>
#include <abacus/view.hpp>
#include <google/protobuf/util/json_util.h>

TEST(test_to_protobuf, api)
{
    std::string name0 = "metric0";
    std::string name1 = "metric1";
    std::string name2 = "metric2";

    abacus::metric_info infos[3] = {
        abacus::metric_info{name0, "An unsigned integer metric",
                            abacus::type::uint64, abacus::kind::counter,
                            abacus::unit{"bytes"}, abacus::min{uint64_t{0U}},
                            abacus::max{uint64_t{100U}}},
        abacus::metric_info{name1, "A signed integer metric",
                            abacus::type::int64, abacus::kind::gauge,
                            abacus::unit{"USD"}, abacus::min{int64_t{-100}},
                            abacus::max{int64_t{100}}},
        abacus::metric_info{name2, "A boolean constant", abacus::type::boolean,
                            abacus::kind::constant}};

    abacus::metrics metrics(infos);

    auto m0 = metrics.initialize_metric<abacus::type::uint64>(name0);
    auto m1 = metrics.initialize_metric<abacus::type::int64>(name1);
    metrics.initialize_constant(name2, true);

    m0 = 42;
    m1 = -42;

    abacus::view view;
    view.set_meta_data(metrics.meta_data());
    view.set_value_data(metrics.value_data());

    auto protobuf = abacus::to_protobuf(view);
    auto expected = abacus::metrics_message();
    expected.set_protocol_version(1);

    auto metric0 = expected.add_metric();
    auto info0 = metric0->mutable_info();
    info0->set_name(name0);
    info0->set_description(infos[0].description);
    info0->set_kind(static_cast<protobuf::Kind>(infos[0].kind));
    info0->set_type(static_cast<protobuf::Type>(infos[0].type));
    info0->set_unit(infos[0].unit.value);
    metric0->set_uint64_value(42);
    info0->set_uint64_max(infos[0].max.m_uint);
    info0->set_uint64_min(infos[0].min.m_uint);

    auto metric1 = expected.add_metric();
    auto info1 = metric1->mutable_info();
    info1->set_name(name1);
    info1->set_description(infos[1].description);
    info1->set_kind(static_cast<protobuf::Kind>(infos[1].kind));
    info1->set_type(static_cast<protobuf::Type>(infos[1].type));
    info1->set_unit(infos[1].unit.value);
    metric1->set_int64_value(-42);
    info1->set_int64_max(infos[1].max.m_int);
    info1->set_int64_min(infos[1].min.m_int);

    auto metric2 = expected.add_metric();
    auto info2 = metric2->mutable_info();
    info2->set_name(name2);
    info2->set_description(infos[2].description);
    info2->set_kind(static_cast<protobuf::Kind>(infos[2].kind));
    info2->set_type(static_cast<protobuf::Type>(infos[2].type));
    info2->set_unit(infos[2].unit.value);
    metric2->set_bool_value(true);

    std::string expected_output;
    google::protobuf::util::MessageToJsonString(expected, &expected_output);

    std::string protobuf_json;
    google::protobuf::util::MessageToJsonString(protobuf, &protobuf_json);

    EXPECT_EQ(expected_output, protobuf_json);
}
