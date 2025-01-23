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

#include "protobuf/metrics.pb.h"

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

    /// @return the const pointer to the value data of the metrics.
    auto value_data(std::size_t offset = 0) const -> const uint8_t*;

    /// @return the size of the value data of the metrics.
    auto value_bytes() const -> std::size_t;

    /// @return the metadata of the metrics.
    auto metadata() const -> const protobuf::MetricsMetadata&;

    /// Initialize a required metric
    /// @param name The name of the metric
    /// @param value Optional initial value of the metric
    /// @return The metric object
    template <class Metric>
    [[nodiscard]] auto initialize_required(const std::string& name,
                                           typename Metric::type value) ->
        typename Metric::required;

    /// Initialize a metric
    /// @param name The name of the metric
    /// @param value Optional initial value of the metric
    /// @return The metric object
    template <class Metric>
    [[nodiscard]] auto initialize_optional(const std::string& name) ->
        typename Metric::optional;

    /// Initialize a constant metric
    /// @param name The name of the metric
    /// @param value The value of the metric
    template <class Metric>
    void initialize_constant(const std::string& name,
                             typename Metric::type value);

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

    /// Get the protobuf representation of the metrics
    /// @return The protobuf representation of the metrics
    auto protobuf() const -> const protobuf::Metrics&;

private:
    /// @param offset The offset of the value data
    /// @return the pointer to the value data of the metrics.
    auto value_data(std::size_t offset = 0) -> uint8_t*;

    /// No copy
    metrics(metrics&) = delete;

    /// No copy assignment
    metrics& operator=(metrics&) = delete;

private:
    /// The info of the metrics separated by byte-sizes
    protobuf::Metrics m_proto_metrics;

    /// The hash of the metadata
    uint32_t m_hash;

    /// The size of the metadata in bytes
    std::size_t m_metadata_bytes;

    /// The size of the value data in bytes
    std::size_t m_value_bytes;

    /// A map of the metrics and whether they have been initialized
    std::map<std::string, bool> m_initialized;

    /// The initial values of the metrics
    std::map<std::string, std::any> m_initial_values;
};
}
}
