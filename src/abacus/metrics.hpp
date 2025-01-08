// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <functional>
#include <limits>
#include <map>
#include <string>
#include <variant>

#include "detail/hash_function.hpp"
#include "protocol_version.hpp"
#include "type.hpp"
#include "version.hpp"

#include "protobuf/metrics.pb.h"

#include <endian/is_big_endian.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace
{
template <typename T>
static auto size_of_type() -> std::size_t
{
    return sizeof(typename std::remove_pointer<T>::type::type);
}

static inline auto
get_kind(const protobuf::Metric& metric) -> std::optional<protobuf::Kind>
{
    switch (metric.type_case())
    {
    case protobuf::Metric::kUint64:
        return metric.uint64().kind();
    case protobuf::Metric::kInt64:
        return metric.int64().kind();
    case protobuf::Metric::kUint32:
        return metric.uint32().kind();
    case protobuf::Metric::kInt32:
        return metric.int32().kind();
    case protobuf::Metric::kFloat64:
        return metric.float64().kind();
    case protobuf::Metric::kFloat32:
        return metric.float32().kind();
    case protobuf::Metric::kBoolean:
        return metric.boolean().kind();
    case protobuf::Metric::kEnum8:
    default:
        return std::nullopt;
    }
}
}

/// This class is used for creating descriptive counters that are contiguous in
/// memory, to allow for fast access and arithmetic operations.
class metrics
{
public:
    /// Default constructor
    /// No metrics will be contained within this object.
    metrics() = default;

    /// Move constructor
    /// @param other The metrics to move from.
    metrics(metrics&& other) noexcept :
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

    /// Constructor
    /// @param info The info of the metrics in a pointer.
    /// @param count The number of infos.
    metrics(const std::map<std::string, type>& infos)
    {
        m_metadata = protobuf::MetricsMetadata();
        m_metadata.set_protocol_version(protocol_version());
        m_metadata.set_endianness(endian::is_big_endian()
                                      ? protobuf::Endianness::BIG
                                      : protobuf::Endianness::LITTLE);

        // The first byte is reserved for the sync value
        m_value_bytes = sizeof(uint32_t);

        for (auto [name, value] : infos)
        {
            protobuf::Metric metric;
            metric.set_offset(m_value_bytes);

            // The offset is incremented by one byte which represents whether
            // the metric is set or not.
            m_value_bytes += 1;

            if (auto* m = std::get_if<uint64>(&value))
            {
                // The offset is incremented by the size of the type
                m_value_bytes += size_of_type<decltype(m)>();

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
                m_value_bytes += size_of_type<decltype(m)>();

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
                m_value_bytes += size_of_type<decltype(m)>();

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
                m_value_bytes += size_of_type<decltype(m)>();

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
                m_value_bytes += size_of_type<decltype(m)>();

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
                m_value_bytes += size_of_type<decltype(m)>();

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
                m_value_bytes += size_of_type<decltype(m)>();

                metric.mutable_boolean()->set_description(m->description);
                metric.mutable_boolean()->set_kind(m->kind);
            }
            else if (auto* m = std::get_if<enum8>(&value))
            {
                // The offset is incremented by the size of the type
                m_value_bytes += size_of_type<decltype(m)>();

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

    /// Destructor
    ~metrics()
    {
        if (m_data != nullptr)
            delete[] m_data;
    }

    /// @return the pointer to the value data of the metrics.
    auto value_data() const -> const uint8_t*
    {
        return m_data + m_metadata_bytes;
    }

    /// @return the size of the value data of the metrics.
    auto value_bytes() const -> std::size_t
    {
        return m_value_bytes;
    }

    /// @return the pointer to the metadata data of the metrics.
    auto metadata_data() const -> const uint8_t*
    {
        return m_data;
    }

    /// @return the size of the metadata data of the metrics.
    auto metadata_bytes() const -> std::size_t
    {
        return m_metadata_bytes;
    }

    /// @return the metadata of the metrics.
    auto metadata() const -> const protobuf::MetricsMetadata&
    {
        return m_metadata;
    }

    /// Initialize a metric
    /// @param name The name of the metric
    /// @param value Optional initial value of the metric
    /// @return The metric object
    template <class Metric>
    auto initialize_metric(const std::string& name,
                           std::optional<typename Metric::type> value =
                               std::nullopt) -> typename Metric::metric
    {
        assert(m_initialized.find(name) != m_initialized.end());
        assert(!m_initialized.at(name));
        m_initialized[name] = true;
        const protobuf::Metric& proto_metric = m_metadata.metrics().at(name);
        auto kind = get_kind(proto_metric);
        if (kind.has_value())
        {
            assert(kind.value() != protobuf::Kind::CONSTANT);
        }

        auto offset = proto_metric.offset();

        if (value.has_value())
        {
            return typename Metric::metric(m_data + m_metadata_bytes + offset,
                                           value.value());
        }
        else
        {
            return typename Metric::metric(m_data + m_metadata_bytes + offset);
        }
    }

    /// Initialize a constant metric
    /// @param name The name of the metric
    /// @param value The value of the metric
    template <class Metric>
    void initialize_constant(const std::string& name,
                             typename Metric::type value)
    {
        assert(m_initialized.find(name) != m_initialized.end());
        assert(!m_initialized.at(name));
        m_initialized[name] = true;

        const protobuf::Metric& proto_metric = m_metadata.metrics().at(name);
        auto offset = proto_metric.offset();
        auto kind = get_kind(proto_metric);
        assert(kind.has_value() && kind.value() == protobuf::Kind::CONSTANT);
        typename Metric::metric(m_data + m_metadata_bytes + offset, value);
    }

    /// Check if a metric has been initialized
    /// @param name The name of the metric
    /// @return true if the metric has been initialized
    auto is_initialized(const std::string& name) const -> bool
    {
        assert(m_initialized.find(name) != m_initialized.end());
        return m_initialized.at(name);
    }

    /// @return true if all metrics have been initialized
    auto is_initialized() const -> bool
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

    /// Reset all metrics
    /// This will set all metrics has_value() to false.
    void reset_metrics()
    {
        // Reset the value data
        std::memset(m_data + m_metadata_bytes + sizeof(uint32_t), 0,
                    m_value_bytes - sizeof(uint32_t));
    }

private:
    /// No copy
    metrics(metrics&) = delete;

    /// No copy assignment
    metrics& operator=(metrics&) = delete;

private:
    /// The raw memory for the metadata and value data
    uint8_t* m_data = nullptr;

    /// The info of the metrics separated by byte-sizes
    protobuf::MetricsMetadata m_metadata;

    /// The hash of the metadata
    uint32_t m_hash;

    /// The size of the metadata in bytes
    std::size_t m_metadata_bytes;

    /// The size of the value data in bytes
    std::size_t m_value_bytes;

    /// A map of the metrics and whether they have been initialized
    std::map<std::string, bool> m_initialized;
};
}
}
