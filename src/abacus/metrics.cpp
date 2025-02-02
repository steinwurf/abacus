// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "metrics.hpp"

#include "detail/hash_function.hpp"
#include "detail/size_of_type.hpp"

#include "info.hpp"
#include "protocol_version.hpp"
#include "version.hpp"

#include "detail/helpers.hpp"
#include "detail/overload.hpp"
#include "protobuf/metrics.pb.h"

#include <endian/is_big_endian.hpp>

#include <iostream>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

metrics::metrics(metrics&& other) noexcept :
    m_metadata(std::move(other.m_metadata)), m_data(std::move(other.m_data)),
    m_metadata_bytes(other.m_metadata_bytes), m_hash(other.m_hash),
    m_value_bytes(other.m_value_bytes),
    m_initialized(std::move(other.m_initialized))
{
    other.m_metadata = protobuf::MetricsMetadata();
    other.m_data.clear();
    other.m_metadata_bytes = 0;
    other.m_hash = 0;
    other.m_value_bytes = 0;
    other.m_initialized.clear();
}

metrics::metrics(const std::map<name, abacus::info>& infos)
{
    m_metadata = protobuf::MetricsMetadata();
    m_metadata.set_protocol_version(protocol_version());
    m_metadata.set_endianness(endian::is_big_endian()
                                  ? protobuf::Endianness::BIG
                                  : protobuf::Endianness::LITTLE);

    // The first byte is reserved for the sync value
    m_value_bytes = sizeof(uint32_t);

    for (auto [name, info] : infos)
    {
        protobuf::Metric metric;
        metric_state state;

        m_initialized.emplace(name.value, std::nullptr);

        std::visit(
            detail::overload{
                [&](const uint64& m)
                {
                    state.optional = m.availability == availability::optional;
                    state.offset = m_value_bytes;

                    auto* typed_metric = metric.mutable_uint64();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
                    typed_metric->set_optional(state.optional);
                    typed_metric->set_kind(static_cast<protobuf::Kind>(m.kind));

                    if (!m.unit.empty())
                    {
                        typed_metric->set_unit(m.unit.value);
                    }
                    if (m.min.value.has_value())
                    {
                        typed_metric->set_min(m.min.value.value());
                    }
                    if (m.max.value.has_value())
                    {
                        typed_metric->set_max(m.max.value.value());
                    }
                    // The offset is incremented by the size of the type
                    m_value_bytes += sizeof(uint64::type);

                    // The offset is incremented by one byte which represents
                    // whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const int64& m)
                {
                    auto* typed_metric = metric.mutable_int64();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
                    typed_metric->set_optional(m.availability ==
                                               availability::optional);
                    typed_metric->set_kind(static_cast<protobuf::Kind>(m.kind));

                    if (!m.unit.empty())
                    {
                        typed_metric->set_unit(m.unit.value);
                    }
                    if (m.min.value.has_value())
                    {
                        typed_metric->set_min(m.min.value.value());
                    }
                    if (m.max.value.has_value())
                    {
                        typed_metric->set_max(m.max.value.value());
                    }
                    // The offset is incremented by the size of the type
                    m_value_bytes += sizeof(int64::type);

                    // The offset is incremented by one byte which represents
                    // whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const uint32& m)
                {
                    auto* typed_metric = metric.mutable_uint32();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
                    typed_metric->set_optional(m.availability ==
                                               availability::optional);
                    typed_metric->set_kind(static_cast<protobuf::Kind>(m.kind));

                    if (!m.unit.empty())
                    {
                        typed_metric->set_unit(m.unit.value);
                    }
                    if (m.min.value.has_value())
                    {
                        typed_metric->set_min(m.min.value.value());
                    }
                    if (m.max.value.has_value())
                    {
                        typed_metric->set_max(m.max.value.value());
                    }
                    // The offset is incremented by the size of the type
                    m_value_bytes += sizeof(uint32::type);

                    // The offset is incremented by one byte which represents
                    // whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const int32& m)
                {
                    auto* typed_metric = metric.mutable_int32();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
                    typed_metric->set_optional(m.availability ==
                                               availability::optional);
                    typed_metric->set_kind(static_cast<protobuf::Kind>(m.kind));

                    if (!m.unit.empty())
                    {
                        typed_metric->set_unit(m.unit.value);
                    }
                    if (m.min.value.has_value())
                    {
                        typed_metric->set_min(m.min.value.value());
                    }
                    if (m.max.value.has_value())
                    {
                        typed_metric->set_max(m.max.value.value());
                    }
                    // The offset is incremented by the size of the type
                    m_value_bytes += sizeof(int32::type);

                    // The offset is incremented by one byte which represents
                    // whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const float64& m)
                {
                    auto* typed_metric = metric.mutable_float64();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
                    typed_metric->set_optional(m.availability ==
                                               availability::optional);
                    typed_metric->set_kind(static_cast<protobuf::Kind>(m.kind));

                    if (!m.unit.empty())
                    {
                        typed_metric->set_unit(m.unit.value);
                    }
                    if (m.min.value.has_value())
                    {
                        typed_metric->set_min(m.min.value.value());
                    }
                    if (m.max.value.has_value())
                    {
                        typed_metric->set_max(m.max.value.value());
                    }
                    // The offset is incremented by the size of the type
                    m_value_bytes += sizeof(float64::type);

                    // The offset is incremented by one byte which represents
                    // whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const float32& m)
                {
                    auto* typed_metric = metric.mutable_float32();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
                    typed_metric->set_optional(m.availability ==
                                               availability::optional);
                    typed_metric->set_kind(static_cast<protobuf::Kind>(m.kind));

                    if (!m.unit.empty())
                    {
                        typed_metric->set_unit(m.unit.value);
                    }
                    if (m.min.value.has_value())
                    {
                        typed_metric->set_min(m.min.value.value());
                    }
                    if (m.max.value.has_value())
                    {
                        typed_metric->set_max(m.max.value.value());
                    }
                    // The offset is incremented by the size of the type
                    m_value_bytes += sizeof(float32::type);

                    // The offset is incremented by one byte which represents
                    // whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const boolean& m)
                {
                    auto* typed_metric = metric.mutable_boolean();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
                    typed_metric->set_optional(m.availability ==
                                               availability::optional);
                    m_value_bytes += sizeof(boolean::type);

                    // The offset is incremented by one byte which represents
                    // whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const enum8& m)
                {
                    auto* typed_metric = metric.mutable_enum8();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
                    typed_metric->set_optional(m.availability ==
                                               availability::optional);
                    for (auto [key, value] : m.values)
                    {
                        auto enum_value = protobuf::Enum8Metric::EnumValue();
                        enum_value.set_name(value.name);
                        if (!value.description.empty())
                        {
                            enum_value.set_description(value.description);
                        }

                        typed_metric->mutable_values()->insert(
                            {key, enum_value});
                    }
                    m_value_bytes += sizeof(enum8::type);

                    // The offset is incremented by one byte which represents
                    // whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const constant& m)
                {
                    auto* typed_metric = metric.mutable_constant();
                    typed_metric->set_description(m.description.value);
                    if (!m.unit.empty())
                    {
                        typed_metric->set_unit(m.unit.value);
                    }
                    // We expect the metric to be a constant
                    std::visit(
                        detail::overload{
                            [typed_metric](uint64_t value)
                            { typed_metric->set_uint64(value); },
                            [typed_metric](int64_t value)
                            { typed_metric->set_int64(value); },
                            [typed_metric](double value)
                            { typed_metric->set_float64(value); },
                            [typed_metric](const std::string& value)
                            { typed_metric->set_string(value); },
                            [typed_metric](bool value)
                            { typed_metric->set_boolean(value); }, [](auto)
                            { assert(false && "Unsupported constant type"); }},
                        m.value);
                },
                [&](const auto&)
                { assert(false && "Unsupported metric type"); }},
            info);

        m_metadata.mutable_metrics()->insert({name.value, metric});
    }

    // Set the sync value to 1 so that the size of the metadata is
    // calculated correctly
    m_metadata.set_sync_value(1);

    m_metadata_bytes = metadata().ByteSizeLong();

    m_data.resize(m_metadata_bytes + m_value_bytes);

    // Serialize the metadata
    metadata().SerializeToArray(m_data.data(), m_metadata_bytes);

    // Calculate the hash of the metadata
    m_hash = detail::hash_function(m_data.data(), m_metadata_bytes);

    // Update the sync value
    m_metadata.set_sync_value(m_hash);

    // Serialize the metadata again to include the sync value
    metadata().SerializeToArray(m_data.data(), m_metadata_bytes);

    // Make sure the metadata didn't change unexpectedly
    assert(metadata().ByteSizeLong() == m_metadata_bytes);

    // Write the sync value to the first byte of the value data (this
    // will be written as the endianess of the system) Consuming code
    // can use the endianness field in the metadata to read the sync
    // value
    std::memcpy(value_data(0), &m_hash, sizeof(uint32_t));
}

static inline auto is_optional(const protobuf::Metric& metric) -> bool
{

    if (metric.has_uint64())
    {
        return metric.uint64().optional();
    }
    else if (metric.has_int64())
    {
        return metric.int64().optional();
    }
    else if (metric.has_uint32())
    {
        return metric.uint32().optional();
    }
    else if (metric.has_int32())
    {
        return metric.int32().optional();
    }
    else if (metric.has_float64())
    {
        return metric.float64().optional();
    }
    else if (metric.has_float32())
    {
        return metric.float32().optional();
    }
    else if (metric.has_boolean())
    {
        return metric.boolean().optional();
    }
    else if (metric.has_enum8())
    {
        return metric.enum8().optional();
    }
    else
    {
        assert(false && "Unsupported metric type");
        return false;
    }
}

metrics::~metrics()
{
}

auto metrics::value_data() const -> const uint8_t*
{
    return m_data.data() + m_metadata_bytes;
}

auto metrics::value_data(std::size_t offset) -> uint8_t*
{
    return m_data.data() + m_metadata_bytes + offset;
}

auto metrics::value_bytes() const -> std::size_t
{
    return m_value_bytes;
}

auto metrics::metadata() const -> const protobuf::MetricsMetadata&
{
    return m_metadata;
}

auto metrics::metadata_data() const -> const uint8_t*
{
    return m_data.data();
}

auto metrics::metadata_bytes() const -> std::size_t
{
    return m_metadata_bytes;
}

auto metrics::is_initialized(const std::string& name) const -> bool
{
    auto it = m_initialized.find(name);
    assert(it != m_initialized.end());

    if (it->second)
    {
        return true;
    }
    else
    {
        return false;
    }
}

auto metrics::is_initialized() const -> bool
{
    for (const auto& [name, info] : m_initialized)
    {
        (void)name;
        if (!info.reset)
        {
            return false;
        }
    }
    return true;
}

auto metrics::reset() -> void
{
    for (auto& [name, info] : m_initialized)
    {
        (void)name;
        if (info.reset)
        {
            info.reset();
        }
    }
}
}
}
