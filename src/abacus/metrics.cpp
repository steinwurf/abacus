// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "metrics.hpp"

#include "detail/hash_function.hpp"

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
    m_value_bytes(other.m_value_bytes), m_offsets(std::move(other.m_offsets)),
    m_resets(std::move(other.m_resets))
{
    other.m_metadata = protobuf::MetricsMetadata();
    other.m_data.clear();
    other.m_metadata_bytes = 0;
    other.m_hash = 0;
    other.m_value_bytes = 0;
    other.m_offsets.clear();
    other.m_resets.clear();
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

        // Save the offset of the metric
        m_offsets.emplace(name.value, m_value_bytes);

        std::visit(
            detail::overload{
                [&](const uint64& m)
                {
                    auto* typed_metric = metric.mutable_uint64();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
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

                    // The offset is incremented by one byte which
                    // represents whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const int64& m)
                {
                    auto* typed_metric = metric.mutable_int64();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
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

                    // The offset is incremented by one byte which
                    // represents whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const uint32& m)
                {
                    auto* typed_metric = metric.mutable_uint32();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
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

                    // The offset is incremented by one byte which
                    // represents whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const int32& m)
                {
                    auto* typed_metric = metric.mutable_int32();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
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

                    // The offset is incremented by one byte which
                    // represents whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const float64& m)
                {
                    auto* typed_metric = metric.mutable_float64();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
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

                    // The offset is incremented by one byte which
                    // represents whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const float32& m)
                {
                    auto* typed_metric = metric.mutable_float32();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
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

                    // The offset is incremented by one byte which
                    // represents whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const boolean& m)
                {
                    auto* typed_metric = metric.mutable_boolean();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
                    m_value_bytes += sizeof(boolean::type);

                    // The offset is incremented by one byte which
                    // represents whether the metric is set or not.
                    m_value_bytes += 1;
                },
                [&](const enum8& m)
                {
                    auto* typed_metric = metric.mutable_enum8();
                    typed_metric->set_offset(m_value_bytes);
                    typed_metric->set_description(m.description.value);
                    for (auto [key, value] : m.values)
                    {
                        auto enum_value = protobuf::Enum8Metric::EnumValue();
                        enum_value.set_name(value.name);
                        if (!value.description.empty())
                        {
                            enum_value.set_description(value.description);
                        }

                        typed_metric->mutable_values()->insert(
                            {key.value, enum_value});
                    }
                    m_value_bytes += sizeof(enum8::type);

                    // The offset is incremented by one byte which
                    // represents whether the metric is set or not.
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
                            [typed_metric](constant::uint64 c)
                            { typed_metric->set_uint64(c.value); },
                            [typed_metric](constant::int64 c)
                            { typed_metric->set_int64(c.value); },
                            [typed_metric](constant::float64 c)
                            { typed_metric->set_float64(c.value); },
                            [typed_metric](constant::str c)
                            { typed_metric->set_string(c.value); },
                            [typed_metric](constant::boolean c)
                            { typed_metric->set_boolean(c.value); },
                            [](const auto&)
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
    std::memcpy(m_data.data() + m_metadata_bytes, &m_hash, sizeof(uint32_t));
}

template <class Metric>
[[nodiscard]] auto
metrics::initialize(const std::string& name) -> metric<Metric>
{
    assert(m_resets.find(name) == m_resets.end());
    assert(m_offsets.find(name) != m_offsets.end());

    std::size_t offset = m_offsets.at(name);
    metric<Metric> m(m_data.data() + m_metadata_bytes + offset);

    m_resets[name] = [m]() mutable { m.reset(); };
    return m;
}

// Explicit instantiations for the expected types
template auto
metrics::initialize<uint64>(const std::string& name) -> metric<uint64>;

template auto
metrics::initialize<int64>(const std::string& name) -> metric<int64>;

template auto
metrics::initialize<uint32>(const std::string& name) -> metric<uint32>;

template auto
metrics::initialize<int32>(const std::string& name) -> metric<int32>;

template auto
metrics::initialize<float64>(const std::string& name) -> metric<float64>;

template auto
metrics::initialize<float32>(const std::string& name) -> metric<float32>;

template auto
metrics::initialize<boolean>(const std::string& name) -> metric<boolean>;

template auto
metrics::initialize<enum8>(const std::string& name) -> metric<enum8>;

auto metrics::value_data() const -> const uint8_t*
{
    return m_data.data() + m_metadata_bytes;
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
    return m_resets.find(name) != m_resets.end();
}

auto metrics::is_initialized() const -> bool
{
    for (const auto& [name, offset] : m_offsets)
    {
        (void)offset;
        if (!is_initialized(name))
        {
            return false;
        }
    }
    return true;
}

auto metrics::reset() -> void
{
    for (const auto& [name, reset] : m_resets)
    {
        (void)name;
        reset();
    }
}
}
}
