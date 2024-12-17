// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <functional>
#include <map>
#include <string>

#include "detail/value_size_info.hpp"
#include "metric.hpp"
#include "type.hpp"
#include "version.hpp"

#include "protobuf/metrics.pb.h"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
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
    metrics2(const protobuf::MetricsMetadata metadata) : m_metadata(metadata)
    {
        m_value_data = new uint8_t[value_bytes()];
    }

    /// Move assignment
    auto operator=(metrics2&& other) noexcept -> metrics2&;

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
        auto offset = m_metadata.metrics().at(name).value_offset();

        value_type* value_ptr = (value_type*)initialize(offset);

        return metric<MetricType>{value_ptr};
    }

    template <abacus::type MetricType>
    void
    initialize_constant(const std::string& name,
                        typename metric<MetricType>::value_type value) const
    {
        using value_type = typename metric<MetricType>::value_type;
        auto offset = m_metadata.metrics().at(name).value_offset();
        assert(m_metadata.metrics().at(name).kind(index) ==
               abacus::kind::constant);
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
