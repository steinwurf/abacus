// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <any>
#include <cassert>
#include <map>
#include <string>
#include <vector>

#include "info.hpp"
#include "name.hpp"
#include "version.hpp"

#include "optional_metric.hpp"
#include "protobuf/metrics.pb.h"
#include "required_metric.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
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
    metrics(const std::map<name, abacus::info>& info);

    /// Destructor
    ~metrics();

    /// Initialize a required metric
    /// @param name The name of the metric
    /// @param value Optional initial value of the metric
    /// @return The metric object
    template <class Metric, class Value>
    [[nodiscard]] auto initialize_required(const std::string& name, Value value)
        -> required_metric<Metric>
    {
        assert(!is_initialized(name));
        required_metric<Metric> metric{metric_memory(name)};
        metric.set_value(value);

        // Store lambda to reset metric if needed
        m_initialized.emplace(name, [metric, value]() mutable
                              { metric.set_value(value); });

        return metric
    }

    /// Initialize a metric
    /// @param name The name of the metric
    /// @return The metric object
    template <class Metric>
    [[nodiscard]] auto
    initialize_optional(const std::string& name) -> optional_metric<Metric>
    {
        assert(!is_initialized(name));
        optional_metric<Metric> metric{metric_memory(name)};

        // Store lambda to reset metric if needed
        m_initialized.emplace(name, [metric]() mutable { metric.reset(); });

        return metric;
    }

    void initialize(const std::string_view& name,
                    optional_metric<abacus::enum8>& metric);

    void initialize(const std::string_view& name,
                    optional_metric<abacus::boolean>& metric);

    void initialize(const std::string_view& name,
                    optional_metric<abacus::float32>& metric);

    void initialize(const std::string_view& name,
                    optional_metric<abacus::float64>& metric);

    void initialize(const std::string_view& name,
                    optional_metric<abacus::int32>& metric);

    void initialize(const std::string_view& name,
                    optional_metric<abacus::int64>& metric);

    void initialize(const std::string_view& name,
                    optional_metric<abacus::uint32>& metric);

    void initialize(const std::string_view& name,
                    optional_metric<abacus::uint64>& metric);

    void initialize(const std::string_view& name,
                    required_metric<abacus::enum8>& metric, uint8_t value);

    void initialize(const std::string_view& name,
                    required_metric<abacus::boolean>& metric, bool value);

    void initialize(const std::string_view& name,
                    required_metric<abacus::float32>& metric, float value);

    void initialize(const std::string_view& name,
                    required_metric<abacus::float64>& metric, double value);

    void initialize(const std::string_view& name,
                    required_metric<abacus::int32>& metric, int32_t value);

    void initialize(const std::string_view& name,
                    required_metric<abacus::int64>& metric, int64_t value);

    /// Check if a metric has been initialized
    /// @param name The name of the metric
    /// @return true if the metric has been initialized
    auto is_initialized(const std::string& name) const -> bool;

    /// @return true if all metrics have been initialized
    auto is_initialized() const -> bool;

    /// Reset all metrics to their initial values
    /// Optional values will be reset to std::nullopt unless they were set
    /// during initialization.
    auto reset() -> void;

    /// @return the pointer to the metadata part of the metrics.
    auto metadata_data() const -> const uint8_t*;

    /// @return the size of the metadata part of the metrics.
    auto metadata_bytes() const -> std::size_t;

    /// @return the const pointer to the value data of the metrics.
    auto value_data() const -> const uint8_t*;

    /// @return the size of the value data of the metrics.
    auto value_bytes() const -> std::size_t;

    /// @return the metadata part of the metrics.
    auto metadata() const -> const protobuf::MetricsMetadata&;

private:
    /// @param offset The offset of the value data
    /// @return the pointer to the value data of the metrics.
    auto value_data(std::size_t offset) -> uint8_t*;

    /// No copy
    metrics(metrics&) = delete;

    /// No copy assignment
    metrics& operator=(metrics&) = delete;

private:
    /// The info of the metrics separated by byte-sizes
    protobuf::MetricsMetadata m_metadata;

    /// Data of the metrics, both metadata and values
    std::vector<uint8_t> m_data;

    /// The size of the metadata in bytes
    std::size_t m_metadata_bytes;

    /// The hash of the metadata
    uint32_t m_hash;

    /// The size of the value data in bytes
    std::size_t m_value_bytes;

    struct metric_state
    {
        /// The offset of the metric in the value data
        std::size_t offset;

        /// Is optional?
        bool optional;

        /// Reset function (will always be set if the metric is intialized
        /// otherwise not)
        std::function<void()> reset;
    };

    /// A vector of reset functions for the metrics
    std::unordered_map<std::string, metric_info> m_initialized;
};
}
}
