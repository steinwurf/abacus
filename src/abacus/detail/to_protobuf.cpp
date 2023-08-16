// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "to_protobuf.hpp"

#include "../to_string.hpp"
#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
auto to_protobuf(const view& view) -> metrics_message
{
    auto metrics = metrics_message();
    uint64_t uint_value = 0U;
    int64_t int_value = 0;
    bool bool_value = false;
    double float_value = 0.0;
    metrics.set_protocol_version(view.protocol_version());

    for (std::size_t i = 0; i < view.count(); ++i)
    {

        auto metric = metrics.add_metric();
        auto info = metric->mutable_info();
        info->set_kind(static_cast<protobuf::Kind>(view.kind(i)));
        info->set_type(static_cast<protobuf::Type>(view.type(i)));
        info->set_name(view.name(i));
        info->set_description(view.description(i));
        if (!view.unit(i).empty())
        {
            info->set_unit(view.unit(i));
        }

        // If metric is not initialized we set it to null
        if ((view.is_initialized(i)))
        {
            switch (view.type(i))
            {
            case abacus::type::uint64:
                view.value(i, uint_value);
                metric->set_uint64_value(uint_value);
                info->set_uint64_max(view.max(i).m_uint);
                info->set_uint64_min(view.min(i).m_uint);
                break;
            case abacus::type::int64:
                view.value(i, int_value);
                metric->set_int64_value(int_value);
                info->set_int64_max(view.max(i).m_int);
                info->set_int64_min(view.min(i).m_int);
                break;
            case abacus::type::boolean:
                view.value(i, bool_value);
                metric->set_bool_value(bool_value);
                info->set_uint64_max(view.max(i).m_uint);
                info->set_uint64_max(view.min(i).m_uint);
                break;
            case abacus::type::float64:
                view.value(i, float_value);
                metric->set_float64_value(float_value);
                info->set_float64_max(view.max(i).m_double);
                info->set_float64_min(view.min(i).m_double);
                break;
            default:
                assert(false);
                break;
            }
        }
    }
    return metrics;
}
}
}
}
