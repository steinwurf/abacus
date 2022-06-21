// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <map>
#include <vector>

#include "value_type.hpp"
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

    /// @return the number of metrics from in a metrics data pointer
    auto metric_count() const -> uint16_t;

    /// @return the size of the scope name from a metrics data pointer
    auto scope_size() const -> uint16_t;

    /// @return the bytes used for metric names from a metrics data pointer
    auto name_bytes() const -> uint16_t;

    /// @return the bytes used for metric descriptions from a metrics data
    /// pointer
    auto description_bytes() const -> uint16_t;

    /// @return the number of metrics with 8-byte values (double, uint64_t or
    /// int64_t) from a metrics data pointer
    auto eight_byte_count() const -> uint16_t;

    /// @return the number of metrics with 1-byte values (bool) from a metrics
    /// data pointer
    auto one_byte_count() const -> uint16_t;

    /// @returns true if the metric is initialized, that is if
    /// initialize_metric() has been called for the given index.
    /// @param index The index of the metric to check. Must be less than
    /// metric_count().
    auto is_metric_initialized(std::size_t index) const -> bool;

    /// @returns the name of the metric at the given index.
    /// The name is not written into memory until the metric is initialized with
    /// either initialize_metric<>() or initialize_constant().
    /// @param index The index of the metric to check. Must be less than
    /// metric_count().
    auto metric_name(std::size_t index) const -> std::string;

    /// @returns the description of the metric at the given index.
    /// @param index The index of the metric to check. Must be less than
    /// metric_count() and initialized with initialize_metric<>() or
    /// initialize_constant().
    auto metric_description(std::size_t index) const -> std::string;

    /// @returns the type of the metric at the given index.
    /// @param index The index of the metric to check. Must be less than
    /// metric_count().
    auto metric_type(std::size_t index) const -> value_type;

    /// @returns true if the metric at the given index is a constant, otherwise
    /// false.
    /// @param index The index of the metric to check. Must be less than
    /// metric_count().
    auto metric_is_constant(std::size_t index) const -> bool;

    /// Copy the value of the uint64_t metric into a passed reference. This is
    /// used to extract the values during runtime.
    ///
    /// Make sure that the type and index are correct using metric_type()
    /// and metric_index() to get the correct index and
    /// type. Please do not hard-code these values, as this may break with
    /// changes to your code.
    ///
    /// @param index The index of the metric to copy. Must be less than
    /// metric_count().
    /// @param value The variable to copy the value into. A uint64_t reference.
    void metric_value(std::size_t index, uint64_t& value) const;

    /// Copy the value of the int64_t metric into a passed reference. This is
    /// used to extract the values during runtime.
    ///
    /// Make sure that the type and index are correct using metric_type()
    /// and metric_index() to get the correct index and
    /// type. Please do not hard-code these values, as this may break with
    /// changes to your code.
    ///
    /// @param index The index of the metric to copy. Must be less than
    /// metric_count().
    /// @param value The variable to copy the value into. A int64_t reference.
    void metric_value(std::size_t index, int64_t& value) const;

    /// Copy the value of the double metric into a passed reference. This is
    /// used to extract the values during runtime.
    ///
    /// Make sure that the type and index are correct using metric_type()
    /// and metric_index() to get the correct index and
    /// type. Please do not hard-code these values, as this may break with
    /// changes to your code.
    ///
    /// @param index The index of the metric to copy. Must be less than
    /// metric_count().
    /// @param value The variable to copy the value into. A double reference.
    void metric_value(std::size_t index, double& value) const;

    /// Copy the value of the bool metric into a passed reference. This is used
    /// to extract the values during runtime.
    ///
    /// Make sure that the type and index are correct using metric_type()
    /// and metric_index() to get the correct index and
    /// type. Please do not hard-code these values, as this may break with
    /// changes to your code.
    ///
    /// @param index The index of the metric to copy. Must be less than
    /// metric_count().
    /// @param value The variable to copy the value into. A bool reference.
    void metric_value(std::size_t index, bool& value) const;

    /// @param name The name of the counter to get the index of
    /// @return The index of the counter with the given name
    auto metric_index(const std::string& name) const -> std::size_t;

    /// @return The scope of the metrics data as a string
    auto scope() const -> std::string;

    /// @return The number of bytes in the view memory
    auto view_bytes() const -> std::size_t;

    /// @return a JSON-formatted string of the counters.
    ///
    /// The keys in the JSON string will be "scope + metric_name", and
    /// the values will be a JSON-object with keys "description", "value" and
    /// "is_constant". Example output with scope "car" could be:
    ///
    ///     {
    ///         "car.fuel_consumption": {
    /// 	        "description": "Fuel consumption in kilometers per liter",
    /// 	        "value": 22.300000,
    /// 	        "constant": true,
    ///         },
    ///         "car.wheels": {
    /// 	        "description": "Wheels on the car",
    /// 	        "value": 4,
    /// 	        "constant": true,
    ///         }
    ///     }
    auto to_json() const -> std::string;

private:
    /// The raw memory from the metrics counters
    const uint8_t* m_data;

    /// Map to get index from names
    std::map<std::string, std::size_t> m_name_to_index;

    std::string m_scope;
};
}
}
