// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <map>
#include <string>

#include "type.hpp"
#include "version.hpp"

#include "protobuf/metrics.pb.h"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace
{
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
    metrics(metrics&& other) noexcept;

    /// Constructor
    /// @param info The info of the metrics to create.
    metrics(const std::map<std::string, type>& info);

    /// Destructor
    ~metrics();

    /// @return the pointer to the value data of the metrics.
    auto value_data() const -> const uint8_t*;

    /// @return the size of the value data of the metrics.
    auto value_bytes() const -> std::size_t;

    /// @return the pointer to the metadata data of the metrics.
    auto metadata_data() const -> const uint8_t*;

    /// @return the size of the metadata data of the metrics.
    auto metadata_bytes() const -> std::size_t;

    /// @return the metadata of the metrics.
    auto metadata() const -> const protobuf::MetricsMetadata&;

    /// Initialize a metric
    /// @param name The name of the metric
    /// @param value Optional initial value of the metric
    /// @return The metric object
    template <class Metric>
    [[nodiscard]] auto initialize_metric(
        const std::string& name,
        std::optional<typename Metric::type> value = std::nullopt) ->
        typename Metric::metric
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
    auto is_initialized(const std::string& name) const -> bool;

    /// @return true if all metrics have been initialized
    auto is_initialized() const -> bool;

    /// Reset all metrics
    /// This will set all metrics has_value() to false.
    void reset_metrics();

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
