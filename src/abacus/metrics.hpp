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

#include "metric.hpp"
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

    /// Initialize a metric
    /// @param name The name of the metric
    /// @return The metric object
    template <class Metric>
    [[nodiscard]] auto initialize(const std::string& name) -> metric<Metric>;

    /// Check if a metric has been initialized
    /// @param name The name of the metric
    /// @return true if the metric has been initialized
    auto is_initialized(const std::string& name) const -> bool;

    /// @return true if all metrics have been initialized
    auto is_initialized() const -> bool;

    /// Reset all metrics to be unset
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

    /// Map to offset
    std::unordered_map<std::string, std::size_t> m_offsets;

    /// Map to reset functions - only initialized metrics can be reset
    std::unordered_map<std::string, bool> m_initialized;
};
}
}
