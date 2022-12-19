// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <cstring>
#include <gtest/gtest.h>

#include <abacus/detail/raw.hpp>
#include <abacus/metrics.hpp>

TEST(test_raw, header_data)
{
    using namespace abacus;

    // The header data is 10 bytes
    EXPECT_EQ(detail::header_bytes(), 12U);

    // The first byte is the is_big_endian flag
    EXPECT_EQ(detail::is_big_endian_byte_offset(), 0U);

    // The second byte is the protocol version
    EXPECT_EQ(detail::protocol_version_offset(), 1U);
}

TEST(test_raw, meta_data)
{
    const uint16_t metric_count = 6;

    std::string name0 = "metric0";
    std::string name1 = "metric1";
    std::string name2 = "metric2";
    std::string name3 = "metric3";
    std::string name4 = "metric4";
    std::string name5 = "metric5";

    std::string desc0 = "A boolean metric";
    std::string desc1 = "An unsigned integer metric";
    std::string desc2 = "A signed integer metric";
    std::string desc3 = "A floating point metric";
    std::string desc4 = "A constant boolean metric";
    std::string desc5 = "A constant floating point metric";

    abacus::type type0 = abacus::type::boolean;
    abacus::type type1 = abacus::type::uint64;
    abacus::type type2 = abacus::type::int64;
    abacus::type type3 = abacus::type::float64;
    abacus::type type4 = abacus::type::boolean;
    abacus::type type5 = abacus::type::float64;

    abacus::kind kind0 = abacus::kind::gauge;
    abacus::kind kind1 = abacus::kind::counter;
    abacus::kind kind2 = abacus::kind::counter;
    abacus::kind kind3 = abacus::kind::gauge;
    abacus::kind kind4 = abacus::kind::constant;
    abacus::kind kind5 = abacus::kind::constant;

    abacus::unit unit1 = abacus::unit{"bytes"};
    abacus::unit unit2 = abacus::unit{"USD"};
    abacus::unit unit3 = abacus::unit{"ms"};
    abacus::unit unit5 = abacus::unit{"us"};

    abacus::metric_info infos[metric_count] = {
        abacus::metric_info{name0, desc0, type0, kind0},
        abacus::metric_info{name1, desc1, type1, kind1, unit1},
        abacus::metric_info{name2, desc2, type2, kind2, unit2},
        abacus::metric_info{name3, desc3, type3, kind3, unit3},
        abacus::metric_info{name4, desc4, type4, kind4},
        abacus::metric_info{name5, desc5, type5, kind5, unit5}};

    abacus::metrics metrics(infos);

    auto meta_data = metrics.meta_data();

    EXPECT_EQ(4U, abacus::detail::eight_byte_count(meta_data));
    EXPECT_EQ(2U, abacus::detail::one_byte_count(meta_data));

    EXPECT_EQ(metric_count, abacus::detail::metric_count(meta_data));

    EXPECT_EQ(name1.size(), abacus::detail::name_size(meta_data, 0));
    EXPECT_EQ(name2.size(), abacus::detail::name_size(meta_data, 1));
    EXPECT_EQ(name3.size(), abacus::detail::name_size(meta_data, 2));
    EXPECT_EQ(name5.size(), abacus::detail::name_size(meta_data, 3));
    EXPECT_EQ(name0.size(), abacus::detail::name_size(meta_data, 4));
    EXPECT_EQ(name4.size(), abacus::detail::name_size(meta_data, 5));

    EXPECT_EQ(desc1.size(), abacus::detail::description_size(meta_data, 0));
    EXPECT_EQ(desc2.size(), abacus::detail::description_size(meta_data, 1));
    EXPECT_EQ(desc3.size(), abacus::detail::description_size(meta_data, 2));
    EXPECT_EQ(desc5.size(), abacus::detail::description_size(meta_data, 3));
    EXPECT_EQ(desc0.size(), abacus::detail::description_size(meta_data, 4));
    EXPECT_EQ(desc4.size(), abacus::detail::description_size(meta_data, 5));

    EXPECT_EQ(type1, abacus::detail::type(meta_data, 0));
    EXPECT_EQ(type2, abacus::detail::type(meta_data, 1));
    EXPECT_EQ(type3, abacus::detail::type(meta_data, 2));
    EXPECT_EQ(type5, abacus::detail::type(meta_data, 3));
    EXPECT_EQ(type0, abacus::detail::type(meta_data, 4));
    EXPECT_EQ(type4, abacus::detail::type(meta_data, 5));

    EXPECT_EQ(kind1, abacus::detail::kind(meta_data, 0));
    EXPECT_EQ(kind2, abacus::detail::kind(meta_data, 1));
    EXPECT_EQ(kind3, abacus::detail::kind(meta_data, 2));
    EXPECT_EQ(kind5, abacus::detail::kind(meta_data, 3));
    EXPECT_EQ(kind0, abacus::detail::kind(meta_data, 4));
    EXPECT_EQ(kind4, abacus::detail::kind(meta_data, 5));

    EXPECT_EQ(name1, std::string(abacus::detail::name(meta_data, 0),
                                 abacus::detail::name_size(meta_data, 0)));
    EXPECT_EQ(name2, std::string(abacus::detail::name(meta_data, 1),
                                 abacus::detail::name_size(meta_data, 1)));
    EXPECT_EQ(name3, std::string(abacus::detail::name(meta_data, 2),
                                 abacus::detail::name_size(meta_data, 2)));
    EXPECT_EQ(name5, std::string(abacus::detail::name(meta_data, 3),
                                 abacus::detail::name_size(meta_data, 3)));
    EXPECT_EQ(name0, std::string(abacus::detail::name(meta_data, 4),
                                 abacus::detail::name_size(meta_data, 4)));
    EXPECT_EQ(name4, std::string(abacus::detail::name(meta_data, 5),
                                 abacus::detail::name_size(meta_data, 5)));

    EXPECT_EQ(desc1,
              std::string(abacus::detail::description(meta_data, 0),
                          abacus::detail::description_size(meta_data, 0)));
    EXPECT_EQ(desc2,
              std::string(abacus::detail::description(meta_data, 1),
                          abacus::detail::description_size(meta_data, 1)));
    EXPECT_EQ(desc3,
              std::string(abacus::detail::description(meta_data, 2),
                          abacus::detail::description_size(meta_data, 2)));
    EXPECT_EQ(desc5,
              std::string(abacus::detail::description(meta_data, 3),
                          abacus::detail::description_size(meta_data, 3)));
    EXPECT_EQ(desc0,
              std::string(abacus::detail::description(meta_data, 4),
                          abacus::detail::description_size(meta_data, 4)));
    EXPECT_EQ(desc4,
              std::string(abacus::detail::description(meta_data, 5),
                          abacus::detail::description_size(meta_data, 5)));

    EXPECT_EQ(unit1.value,
              std::string(abacus::detail::unit(meta_data, 0),
                          abacus::detail::unit_size(meta_data, 0)));
    EXPECT_EQ(unit2.value,
              std::string(abacus::detail::unit(meta_data, 1),
                          abacus::detail::unit_size(meta_data, 1)));
    EXPECT_EQ(unit3.value,
              std::string(abacus::detail::unit(meta_data, 2),
                          abacus::detail::unit_size(meta_data, 2)));
    EXPECT_EQ(unit5.value,
              std::string(abacus::detail::unit(meta_data, 3),
                          abacus::detail::unit_size(meta_data, 3)));
    EXPECT_EQ("", std::string(abacus::detail::unit(meta_data, 4),
                              abacus::detail::unit_size(meta_data, 4)));
    EXPECT_EQ("", std::string(abacus::detail::unit(meta_data, 5),
                              abacus::detail::unit_size(meta_data, 5)));
}
