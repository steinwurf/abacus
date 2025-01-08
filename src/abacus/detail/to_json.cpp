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
auto to_json(const view& view, bool minimal) -> bourne::json
{
    auto json = bourne::json::object();
    if (minimal)
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
        auto metadata = view.metadata();
        // Go through each metric and remove the offset
        for (auto& [name, metric] : *metadata.mutable_metrics())
        {
            metric.clear_offset();
        }

        auto status = google::protobuf::util::MessageToJsonString(
            metadata, &metadata_json);
        if (!status.ok())
        {
            return json;
        }
        json = bourne::json::parse(metadata_json);
        if (json.has_key("metrics"))
        {
            json = json["metrics"];
        }
        // Call the minimal version recursively to get the values
        auto values = to_json(view, true);
        for (const auto& [key, value] : values.object_range())
        {
            json[key]["value"] = value;
        }
    }

    return json;
}
}
}
}
