// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "to_json.hpp"

#include <bourne/json.hpp>
#include <google/protobuf/util/json_util.h>

#include "../version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace detail
{
auto to_json(const view2& view, bool slim) -> bourne::json
{
    auto json = bourne::json::object();
    if (slim)
    {
        for (const auto& [name, metric] : view.metadata().metrics())
        {
            switch (metric.type_case())
            {
            case protobuf::Metric::kUint64:
            {
                auto v = view.value<abacus::uint64>(name);
                if (v.has_value())
                {
                    json[name] = v.value();
                }
                break;
            }
            case protobuf::Metric::kInt64:
            {
                auto v = view.value<abacus::int64>(name);
                if (v.has_value())
                {
                    json[name] = v.value();
                }
                break;
            }
            case protobuf::Metric::kUint32:
            {
                auto v = view.value<abacus::uint32>(name);
                if (v.has_value())
                {
                    json[name] = v.value();
                }
                break;
            }
            case protobuf::Metric::kInt32:
            {
                auto v = view.value<abacus::int32>(name);
                if (v.has_value())
                {
                    json[name] = v.value();
                }
                break;
            }
            case protobuf::Metric::kFloat64:
            {
                auto v = view.value<abacus::float64>(name);
                if (v.has_value())
                {
                    json[name] = v.value();
                }
                break;
            }
            case protobuf::Metric::kFloat32:
            {
                auto v = view.value<abacus::float32>(name);
                if (v.has_value())
                {
                    json[name] = v.value();
                }
                break;
            }
            case protobuf::Metric::kBoolean:
            {
                auto v = view.value<abacus::boolean>(name);
                if (v.has_value())
                {
                    json[name] = v.value();
                }
                break;
            }
            case protobuf::Metric::kEnum8:
            {
                auto v = view.value<abacus::enum8>(name);
                if (v.has_value())
                {
                    json[name] = v.value();
                }
                break;
            }
            default:
                break;
            }
        }
    }
    else
    {
        std::string metadata_json;
        auto status = google::protobuf::util::MessageToJsonString(
            view.metadata(), &metadata_json);
        if (status.ok())
        {
            json = bourne::json::parse(metadata_json);
        }
    }

    return json;
}
}
}
}
