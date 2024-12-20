// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <functional>
#include <map>
#include <string>
#include <variant>

#include "detail/value_size_info.hpp"
#include "metric.hpp"
#include "metric_info2.hpp"
#include "protocol_version.hpp"
#include "type.hpp"
#include "version.hpp"

#include "protobuf/metrics.pb.h"

#include <endian/is_big_endian.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
template <typename T>
static auto size_of_type() -> std::size_t
{
    return sizeof(typename T::type);
}

static inline auto to_protobuf_kind(abacus::kind kind) -> protobuf::Kind
{
    switch (kind)
    {
    case abacus::kind::counter:
        return protobuf::Kind::COUNTER;
    case abacus::kind::constant:
        return protobuf::Kind::CONSTANT;
    case abacus::kind::gauge:
        return protobuf::Kind::GAUGE;
    }
}

/// This class is used for creating descriptive counters that are contiguous in
/// memory, to allow for fast access and arithmetic operations.
class metrics2
{

public:
    /// Default constructor
    /// No metrics will be contained within this object.
    metrics2();

    /// Constructor
    /// @param info The info of the metrics in a pointer.
    /// @param count The number of infos.
    metrics2(const metric_info2& infos)
    {
        m_metadata = protobuf::MetricsMetadata();
        m_metadata.set_protocol_version(protocol_version());
        m_metadata.set_endianness(endian::is_big_endian()
                                      ? protobuf::Endianness::BIG
                                      : protobuf::Endianness::LITTLE);
        std::size_t offset = 0;
        for (auto [name, value] : infos)
        {
            // metric_info2 is a variant type, so we need to use a visitor to
            // extract the correct type

            protobuf::Metric metric;
            metric.set_offset(offset);
            offset += sizeof(size_of_type<decltype(value)>());
            if (std::holds_alternative<uint64>(value))
            {
                auto m = std::get<uint64>(value);
                metric.mutable_uint64()->set_description(m.description);
                metric.mutable_uint64()->set_kind(to_protobuf_kind(m.kind));
                if (!m.unit.empty())
                {
                    metric.mutable_uint64()->set_unit(m.unit);
                }
                if (m.min.has_value())
                {
                    metric.mutable_uint64()->set_min(m.min.value());
                }
                if (m.max.has_value())
                {
                    metric.mutable_uint64()->set_max(m.max.value());
                }
            }
            else if (std::holds_alternative<int64>(value))
            {
                auto m = std::get<int64>(value);
                metric.mutable_int64()->set_description(m.description);
                metric.mutable_int64()->set_kind(to_protobuf_kind(m.kind));
                if (!m.unit.empty())
                {
                    metric.mutable_int64()->set_unit(m.unit);
                }
                if (m.min.has_value())
                {
                    metric.mutable_int64()->set_min(m.min.value());
                }
                if (m.max.has_value())
                {
                    metric.mutable_int64()->set_max(m.max.value());
                }
            }
            else if (std::holds_alternative<uint32>(value))
            {
                auto m = std::get<uint32>(value);
                metric.mutable_uint32()->set_description(m.description);
                metric.mutable_uint32()->set_kind(to_protobuf_kind(m.kind));
                if (!m.unit.empty())
                {
                    metric.mutable_uint32()->set_unit(m.unit);
                }
                if (m.min.has_value())
                {
                    metric.mutable_uint32()->set_min(m.min.value());
                }
                if (m.max.has_value())
                {
                    metric.mutable_uint32()->set_max(m.max.value());
                }
            }
            else if (std::holds_alternative<int32>(value))
            {
                auto m = std::get<int32>(value);
                metric.mutable_int32()->set_description(m.description);
                metric.mutable_int32()->set_kind(to_protobuf_kind(m.kind));
                if (!m.unit.empty())
                {
                    metric.mutable_int32()->set_unit(m.unit);
                }
                if (m.min.has_value())
                {
                    metric.mutable_int32()->set_min(m.min.value());
                }
                if (m.max.has_value())
                {
                    metric.mutable_int32()->set_max(m.max.value());
                }
            }
            else if (std::holds_alternative<float64>(value))
            {
                auto m = std::get<float64>(value);
                metric.mutable_float64()->set_description(m.description);
                metric.mutable_float64()->set_kind(to_protobuf_kind(m.kind));
                if (!m.unit.empty())
                {
                    metric.mutable_float64()->set_unit(m.unit);
                }
                if (m.min.has_value())
                {
                    metric.mutable_float64()->set_min(m.min.value());
                }
                if (m.max.has_value())
                {
                    metric.mutable_float64()->set_max(m.max.value());
                }
            }
            else if (std::holds_alternative<float32>(value))
            {
                auto m = std::get<float32>(value);
                metric.mutable_float32()->set_description(m.description);
                metric.mutable_float32()->set_kind(to_protobuf_kind(m.kind));
                if (!m.unit.empty())
                {
                    metric.mutable_float32()->set_unit(m.unit);
                }
                if (m.min.has_value())
                {
                    metric.mutable_float32()->set_min(m.min.value());
                }
                if (m.max.has_value())
                {
                    metric.mutable_float32()->set_max(m.max.value());
                }
            }
            else if (std::holds_alternative<boolean>(value))
            {
                auto m = std::get<boolean>(value);
                metric.mutable_boolean()->set_description(m.description);
                metric.mutable_boolean()->set_kind(to_protobuf_kind(m.kind));
            }
            else if (std::holds_alternative<enum8>(value))
            {
                auto m = std::get<enum8>(value);
                metric.mutable_enum8()->set_description(m.description);
                for (auto [key, value] : m.values)
                {

                    auto enum_value = protobuf::EnumValue();
                    enum_value.set_name(value.name);
                    if (!value.description.empty())
                    {
                        enum_value.set_description(value.description);
                    }

                    metric.mutable_enum8()->mutable_values()->insert(
                        {key, enum_value});
                }
            }

            m_metadata.mutable_metrics()->insert({name, metric});
        }

        // @
        int hash = std::hash<std::string>{}(m_metadata.SerializeAsString());
        m_metadata.set_sync_value(hash);
    }

    /// Destructor
    ~metrics2();

    /// @return the pointer to the value data of the metrics.
    auto value_data() const -> const uint8_t*;

    /// @return the size of the value data of the metrics.
    auto value_bytes() const -> std::size_t;

    /// @return the meta data of the metrics.
    auto metadata() const -> const protobuf::MetricsMetadata&;

    template <abacus::type MetricType>
    auto initialize_metric(const std::string& name) const -> metric<MetricType>
    {
        using value_type = typename metric<MetricType>::value_type;
        auto offset = m_metadata.metrics().at(name).offset();

        value_type* value_ptr = (value_type*)initialize(offset);

        return metric<MetricType>{value_ptr};
    }

    template <abacus::type MetricType>
    void
    initialize_constant(const std::string& name,
                        typename metric<MetricType>::value_type value) const
    {
        using value_type = typename metric<MetricType>::value_type;
        auto offset = m_metadata.metrics().at(name).offset();
        *static_cast<value_type*>(initialize(offset)) = value;
    }

    void reset_metrics();
    void reset_metric(const std::string& name);

private:
    auto initialize(std::size_t offset) const -> void*;

    /// No copy
    metrics2(metrics2&) = delete;

    /// No copy assignment
    metrics2& operator=(metrics2&) = delete;

private:
    /// The info of the metrics separated by byte-sizes
    protobuf::MetricsMetadata m_metadata;

    /// The raw memory for the value data
    uint8_t* m_value_data = nullptr;
};
}
}
