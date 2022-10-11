// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <map>
#include <vector>

#include "metric_type.hpp"
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
/// then view.set_meta_data() can be called to initialize the view with the
/// meta data, subsequently view.set_value_data() can be called to populate
/// the view with the value data. To update the view with new value data
/// view.set_value_data() can be called again.
class view
{

public:
    /// Sets the meta data pointer
    /// @param meta_data The meta data pointer
    void set_meta_data(const uint8_t* meta_data);

    /// Sets the value data pointer
    /// @param value_data The value data pointer
    void set_value_data(const uint8_t* value_data);

    /// Gets the meta data pointer
    /// @return The meta data pointer
    const uint8_t* meta_data() const;

    /// Gets the value data pointer
    /// @return The value data pointer
    const uint8_t* value_data() const;

    /// Gets the meta data size in bytes
    /// @return The meta data size in bytes
    std::size_t meta_bytes() const;

    /// Gets the value data size in bytes
    /// @return The value data size in bytes
    std::size_t value_bytes() const;

    /// @return the number of metrics from in a metrics data pointer
    auto count() const -> uint16_t;

    /// Gets the protocol version of the metrics
    /// @return The protocol version of the metrics
    auto protocol_version() const -> uint8_t;

    /// @returns true if the metric is initialized, that is if
    /// initialize_metric() has been called for the given index.
    /// @param index The index of the metric to check. Must be less than
    /// count().
    auto is_initialized(std::size_t index) const -> bool;

    /// @returns the name of the metric at the given index.
    /// The name is not written into memory until the metric is initialized with
    /// either initialize_metric<>() or initialize_constant().
    /// @param index The index of the metric to check. Must be less than
    /// count().
    auto name(std::size_t index) const -> std::string;

    /// @returns the description of the metric at the given index.
    /// @param index The index of the metric to check. Must be less than
    /// count() and initialized with initialize_metric<>() or
    /// initialize_constant().
    auto description(std::size_t index) const -> std::string;

    /// @returns the type of the metric at the given index.
    /// @param index The index of the metric to check. Must be less than
    /// count().
    auto type(std::size_t index) const -> metric_type;

    /// @returns true if the metric at the given index is a constant, otherwise
    /// false.
    /// @param index The index of the metric to check. Must be less than
    /// count().
    auto is_constant(std::size_t index) const -> bool;

    /// Copy the value of the uint64_t metric into a passed reference. This is
    /// used to extract the values during runtime.
    ///
    /// Make sure that the type and index are correct using type()
    /// and index() to get the correct index and
    /// type. Please do not hard-code these values, as this may break with
    /// changes to your code.
    ///
    /// @param index The index of the metric to copy. Must be less than
    /// count().
    /// @param value The variable to copy the value into. A uint64_t reference.
    void value(std::size_t index, uint64_t& value) const;

    /// Copy the value of the int64_t metric into a passed reference. This is
    /// used to extract the values during runtime.
    ///
    /// Make sure that the type and index are correct using type()
    /// and index() to get the correct index and
    /// type. Please do not hard-code these values, as this may break with
    /// changes to your code.
    ///
    /// @param index The index of the metric to copy. Must be less than
    /// count().
    /// @param value The variable to copy the value into. A int64_t reference.
    void value(std::size_t index, int64_t& value) const;

    /// Copy the value of the double metric into a passed reference. This is
    /// used to extract the values during runtime.
    ///
    /// Make sure that the type and index are correct using type()
    /// and index() to get the correct index and
    /// type. Please do not hard-code these values, as this may break with
    /// changes to your code.
    ///
    /// @param index The index of the metric to copy. Must be less than
    /// count().
    /// @param value The variable to copy the value into. A double reference.
    void value(std::size_t index, double& value) const;

    /// Copy the value of the bool metric into a passed reference. This is used
    /// to extract the values during runtime.
    ///
    /// Make sure that the type and index are correct using type()
    /// and index() to get the correct index and
    /// type. Please do not hard-code these values, as this may break with
    /// changes to your code.
    ///
    /// @param index The index of the metric to copy. Must be less than
    /// count().
    /// @param value The variable to copy the value into. A bool reference.
    void value(std::size_t index, bool& value) const;

    /// @param name The name of the counter to get the index of
    /// @return The index of the counter with the given name
    auto index(const std::string& name) const -> std::size_t;

private:
    /// The meta data pointer
    const uint8_t* m_meta_data;

    /// The value data pointer
    const uint8_t* m_value_data;

    /// Map to get index from names
    std::map<std::string, std::size_t> m_name_to_index;
};
}
}
