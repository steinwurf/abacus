// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <vector>

#include "metric_types.hpp"
#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// This class contains utility functions used to read a metrics data
/// buffer.
///
/// It's main use-case is for when the data of a metrics object is
/// copied to a data buffer. abacus::view can then be used to extract the
/// information from the metrics-data.
///
/// The class cannot manipulate the memory, only access the
/// pointed to values
///
/// Note that this class has no constructor, so it can only be declared and
/// then view.set_data() can be called to update the data
class view
{

public:
    /// Sets the data pointer of the view to read the memory of
    /// @param data The data pointer to read the memory of
    auto set_data(const uint8_t* data) -> void;

    /// @return the data pointer for memory used by the view
    auto data() const -> const uint8_t*;

    /// @return the maximum name size from a metrics data pointer
    auto max_name_bytes() const -> uint16_t;

    /// @return the maximum number of metrics from a metrics data pointer
    auto max_metrics() const -> uint16_t;

    /// @return the size of the scope name from a metrics data pointer
    auto scope_size() const -> uint8_t;

    /// @param index The index of the new counter. Must be less than
    /// max_metrics().
    /// @return The name of a counter as a string
    auto metric_name(std::size_t index) const -> std::string;

    /// @param index The index of a counter. Must be less than max_metrics.
    /// @return The type of a counter as an enum class
    auto metric_type(std::size_t index) const -> value_type;

    /// @param value The variable to assign the value of a bool counter to
    /// @param index The index of a counter. Must be less than max_metrics.
    void metric_value(bool& value, std::size_t index) const;

    /// @param value The variable to assign the value of a unsigned integral
    /// counter to.
    /// @param index The index of a counter. Must be less than max_metrics.
    void metric_value(uint64_t& value, std::size_t index) const;

    /// @param value The variable to assign the value of a signed integral
    /// counter to
    /// @param index The index of a counter. Must be less than max_metrics.
    void metric_value(int64_t& value, std::size_t index) const;

    /// @param value The variable to assign the value of a floating point
    /// counter to
    /// @param index The index of a counter. Must be less than max_metrics.
    void metric_value(double& value, std::size_t index) const;

    /// @param name The name of the counter to get the index of
    /// @return The index of the counter with the given name
    auto metric_index(const std::string& name) const -> std::size_t;

    /// @return The number of metrics currently initialized in the object
    auto count() const -> std::size_t;

    /// @return The scope of the metrics data as a string
    auto scope() const -> std::string;

    /// @param index The index of the counter to check. Must be less than
    /// max_metrics().
    /// @return True if the counter has been initialized
    auto has_metric(std::size_t index) const -> bool;

    /// @return The number of bytes in the view memory
    auto view_bytes() const -> std::size_t;

    /// @param closed If true, the json produced will be closed by brackets.
    /// Intented to be used with the view_iterator class to gather all metrics
    /// in a JSON object.
    /// @return All counters in json format
    auto to_json(bool closed = true) const -> std::string;

private:
    /// The raw memory from the metrics counters
    const uint8_t* m_data;

    std::string m_scope;
};
}
}
