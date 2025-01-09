// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "metrics.hpp"

#include "detail/hash_function.hpp"
#include "detail/size_of_type.hpp"

#include "protocol_version.hpp"
#include "type.hpp"
#include "version.hpp"

#include "protobuf/metrics.pb.h"

#include <endian/is_big_endian.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

metrics::metrics(metrics&& other) noexcept :
    m_data(other.m_data), m_metadata(std::move(other.m_metadata)),
    m_hash(other.m_hash), m_metadata_bytes(other.m_metadata_bytes),
    m_value_bytes(other.m_value_bytes),
    m_initialized(std::move(other.m_initialized))
{
    other.m_data = nullptr;
    other.m_metadata = protobuf::MetricsMetadata();
    other.m_hash = 0;
    other.m_metadata_bytes = 0;
    other.m_value_bytes = 0;
    other.m_initialized.clear();
}

metrics::metrics(const std::map<std::string, type>& info)
{
    m_metadata = protobuf::MetricsMetadata();
    m_metadata.set_protocol_version(protocol_version());
    m_metadata.set_endianness(endian::is_big_endian()
                                  ? protobuf::Endianness::BIG
                                  : protobuf::Endianness::LITTLE);

    // The first byte is reserved for the sync value
    m_value_bytes = sizeof(uint32_t);

    for (auto [name, value] : info)
    {
        protobuf::Metric metric;
        metric.set_offset(m_value_bytes);

        // The offset is incremented by one byte which represents whether
        // the metric is set or not.
        m_value_bytes += 1;

        if (auto* m = std::get_if<uint64>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();

            metric.mutable_uint64()->set_description(m->description);
            metric.mutable_uint64()->set_kind(m->kind);
            if (!m->unit.empty())
            {
                metric.mutable_uint64()->set_unit(m->unit.value);
            }
            if (m->min.value.has_value())
            {
                metric.mutable_uint64()->set_min(m->min.value.value());
            }
            if (m->max.value.has_value())
            {
                metric.mutable_uint64()->set_max(m->max.value.value());
            }
        }
        else if (auto* m = std::get_if<int64>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();

            metric.mutable_int64()->set_description(m->description);
            metric.mutable_int64()->set_kind(m->kind);
            if (!m->unit.empty())
            {
                metric.mutable_int64()->set_unit(m->unit.value);
            }
            if (m->min.value.has_value())
            {
                metric.mutable_int64()->set_min(m->min.value.value());
            }
            if (m->max.value.has_value())
            {
                metric.mutable_int64()->set_max(m->max.value.value());
            }
        }
        else if (auto* m = std::get_if<uint32>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();

            metric.mutable_uint32()->set_description(m->description);
            metric.mutable_uint32()->set_kind(m->kind);
            if (!m->unit.empty())
            {
                metric.mutable_uint32()->set_unit(m->unit.value);
            }
            if (m->min.value.has_value())
            {
                metric.mutable_uint32()->set_min(m->min.value.value());
            }
            if (m->max.value.has_value())
            {
                metric.mutable_uint32()->set_max(m->max.value.value());
            }
        }
        else if (auto* m = std::get_if<int32>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();

            metric.mutable_int32()->set_description(m->description);
            metric.mutable_int32()->set_kind(m->kind);
            if (!m->unit.empty())
            {
                metric.mutable_int32()->set_unit(m->unit.value);
            }
            if (m->min.value.has_value())
            {
                metric.mutable_int32()->set_min(m->min.value.value());
            }
            if (m->max.value.has_value())
            {
                metric.mutable_int32()->set_max(m->max.value.value());
            }
        }
        else if (auto* m = std::get_if<float64>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();

            metric.mutable_float64()->set_description(m->description);
            metric.mutable_float64()->set_kind(m->kind);
            if (!m->unit.empty())
            {
                metric.mutable_float64()->set_unit(m->unit.value);
            }
            if (m->min.value.has_value())
            {
                metric.mutable_float64()->set_min(m->min.value.value());
            }
            if (m->max.value.has_value())
            {
                metric.mutable_float64()->set_max(m->max.value.value());
            }
        }
        else if (auto* m = std::get_if<float32>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();

            metric.mutable_float32()->set_description(m->description);
            metric.mutable_float32()->set_kind(m->kind);
            if (!m->unit.empty())
            {
                metric.mutable_float32()->set_unit(m->unit.value);
            }
            if (m->min.value.has_value())
            {
                metric.mutable_float32()->set_min(m->min.value.value());
            }
            if (m->max.value.has_value())
            {
                metric.mutable_float32()->set_max(m->max.value.value());
            }
        }
        else if (auto* m = std::get_if<boolean>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();

            metric.mutable_boolean()->set_description(m->description);
            metric.mutable_boolean()->set_kind(m->kind);
        }
        else if (auto* m = std::get_if<enum8>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();

            metric.mutable_enum8()->set_description(m->description);
            for (auto [key, value] : m->values)
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
        m_initialized[name] = false;
    }

    // Set the sync value to 1 so that the size of the metadata is
    // calculated correctly
    m_metadata.set_sync_value(1);

    m_metadata_bytes = m_metadata.ByteSizeLong();
    m_data = new uint8_t[m_metadata_bytes + m_value_bytes];

    // Serialize the metadata
    m_metadata.SerializeToArray(m_data, m_metadata_bytes);

    // Calculate the hash of the metadata
    m_hash = detail::hash_function(m_data, m_metadata_bytes);

    // Update the sync value
    m_metadata.set_sync_value(m_hash);

    // Serialize the metadata again to include the sync value
    m_metadata.SerializeToArray(m_data, m_metadata_bytes);

    // Make sure the metadata didn't change unexpectedly
    assert(m_metadata.ByteSizeLong() == m_metadata_bytes);

    // Write the sync value to the first byte of the value data (this will
    // be written as the endianess of the system)
    // Consuming code can use the endianness field in the metadata to
    // read the sync value
    std::memcpy(m_data + m_metadata_bytes, &m_hash, sizeof(uint32_t));
}

metrics::~metrics()
{
    if (m_data != nullptr)
        delete[] m_data;
}

auto metrics::value_data() const -> const uint8_t*
{
    return m_data + m_metadata_bytes;
}

auto metrics::value_bytes() const -> std::size_t
{
    return m_value_bytes;
}

auto metrics::metadata_data() const -> const uint8_t*
{
    return m_data;
}

auto metrics::metadata_bytes() const -> std::size_t
{
    return m_metadata_bytes;
}

auto metrics::metadata() const -> const protobuf::MetricsMetadata&
{
    return m_metadata;
}

auto metrics::is_initialized(const std::string& name) const -> bool
{
    assert(m_initialized.find(name) != m_initialized.end());
    return m_initialized.at(name);
}

auto metrics::is_initialized() const -> bool
{
    for (auto [name, initialized] : m_initialized)
    {
        if (!initialized)
        {
            return false;
        }
    }
    return true;
}

void metrics::reset_metrics()
{
    // Reset the value data
    std::memset(m_data + m_metadata_bytes + sizeof(uint32_t), 0,
                m_value_bytes - sizeof(uint32_t));
}

}
}
