// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <sstream>

#include "to_json.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

auto to_json(view single_view, bool closed_brackets) -> std::string
{
    std::string space = " ";
    std::string newline = "\n";
    std::string tab = "\t";

    uint64_t uint_value = 0U;
    int64_t int_value = 0;
    bool bool_value = false;
    double float_value = 0.0;

    auto metric_count = single_view.metric_count();
    assert(metric_count > 0U);

    std::string scope = single_view.scope();

    std::stringstream json_stream;

    if (closed_brackets)
    {
        json_stream << "{" << newline;
    }

    for (std::size_t i = 0; i < metric_count; ++i)
    {
        if ((!single_view.is_metric_initialized(i)))
        {
            continue;
        }

        auto n = single_view.metric_name(i);
        auto d = single_view.metric_description(i);
        auto t = single_view.get_metric_type(i);
        auto c = single_view.metric_is_constant(i);
        std::string value_string;
        switch (t)
        {
        case metric_type::uint64:
            single_view.metric_value(i, uint_value);
            value_string = std::to_string(uint_value);
            break;
        case metric_type::int64:
            single_view.metric_value(i, int_value);
            value_string = std::to_string(int_value);
            break;
        case metric_type::boolean:
            single_view.metric_value(i, bool_value);
            value_string = bool_value ? "true" : "false";
            break;
        case metric_type::float64:
            single_view.metric_value(i, float_value);
            value_string = std::to_string(float_value);
            break;
        }

        // The key is the scope + the name
        json_stream << tab << "\"" << scope + "." + n << "\": {";

        // Add a JSON object with description, value and is_constant bool.
        json_stream << newline << tab << tab << "\"description\": \"" << d
                    << "\",";
        json_stream << newline << tab << tab << "\"value\": " << value_string
                    << ",";
        json_stream << newline << tab << tab
                    << "\"is_constant\": " << (c ? "true" : "false") << ",";

        // Close the brackets.
        json_stream << newline << tab << "}";

        if (i != (metric_count - 1U))
        {
            json_stream << "," << newline;
        }
    }

    if (closed_brackets)
    {
        json_stream << newline << "}";
    }
    return json_stream.str();
}

auto to_json(std::vector<view> views) -> std::string
{
    std::stringstream json_stream;

    std::string newline = "\n";

    json_stream << "{" << newline;

    for (auto v : views)
    {
        auto view_json = to_json(v, false);
        json_stream << view_json << "," << newline;
    }

    json_stream << newline << "}";

    return json_stream.str();
}
}
}
