// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "to_json.hpp"

#include <bourne/json.hpp>

#include "../to_string.hpp"
#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
auto to_json(const view& view, bool slim) -> bourne::json
{
    auto json = bourne::json::object();
    uint64_t uint_value = 0U;
    int64_t int_value = 0;
    bool bool_value = false;
    double float_value = 0.0;
    if (!slim)
    {
        json["protocol_version"] = view.protocol_version();
    }

    for (std::size_t i = 0; i < view.count(); ++i)
    {
        auto value = bourne::json::null();
        auto max = bourne::json::null();
        auto min = bourne::json::null();
        // If metric is not initialized we set it to null
        if ((view.is_initialized(i)))
        {
            switch (view.type(i))
            {
            case abacus::type::uint64:
                view.value(i, uint_value);
                value = uint_value;
                max = view.max(i).m_uint;
                min = view.min(i).m_uint;
                break;
            case abacus::type::int64:
                view.value(i, int_value);
                value = int_value;
                max = view.max(i).m_int;
                min = view.min(i).m_int;
                break;
            case abacus::type::boolean:
                view.value(i, bool_value);
                value = bool_value;
                max = view.max(i).m_uint;
                min = view.min(i).m_uint;
                break;
            case abacus::type::float64:
                view.value(i, float_value);
                value = float_value;
                max = view.max(i).m_double;
                min = view.min(i).m_double;
                break;
            default:
                assert(false);
                break;
            }
        }
        if (slim)
        {
            json[view.name(i)] = value;
        }
        else
        {
            std::string kind = to_string(view.kind(i));

            if (view.unit(i) == "")
            {
                if (min == 0U && max == 0U)
                {
                    json[view.name(i)] = {
                        "description", view.description(i),
                        "kind",        kind,
                        "value",       value,
                    };
                }
                else
                {
                    json[view.name(i)] = {
                        "description", view.description(i),
                        "kind",        kind,
                        "min",         min,
                        "max",         max,
                        "value",       value,
                    };
                }
            }
            else
            {
                if (min == 0U && max == 0U)
                {
                    json[view.name(i)] = {
                        "description", view.description(i), "kind",  kind,
                        "unit",        view.unit(i),        "value", value,
                    };
                }
                else
                {
                    json[view.name(i)] = {
                        "description", view.description(i),
                        "kind",        kind,
                        "unit",        view.unit(i),
                        "min",         min,
                        "max",         max,
                        "value",       value,
                    };
                }
            }
        }
    }
    return json;
}
}
}
}
