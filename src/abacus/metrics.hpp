// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <map>
#include <vector>

#include "detail/value_size_info.hpp"
#include "metric.hpp"
#include "metric_type.hpp"
#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// This class is used for creating descriptive counters that are contiguous in
/// memory, to allow for fast access fast and fast arithmetic operations.
///
/// The class preallocates memory at construction with the following
/// layout
///
/// 1. Header of 12 bytes
///     * Number of metrics (2 bytes)
///     * Endianness (2 byte)
///     * Total name bytes (2 bytes)
///     * Total description bytes (2 bytes)
///     * Number of 8-byte-value metrics (2 bytes)
///     * Number of 1-byte-value metrics (2 bytes)
///
/// 2. The name sizes in bytes (2 * count bytes)
/// 3. The description sizes in bytes (2 * count bytes)
/// 4. The names of the metrics (? bytes)
/// 5. The descriptions of the metrics (? bytes)
/// 6. The types of the metrics (count bytes)
/// 7. The state of the metrics, constant or not (count bytes)
/// 8. Alignment padding (if needed, max 7 bytes)
/// 9. The 8-byte-values (8 * 8-byte-count bytes)
/// 10. The 1-byte-values (1-byte-count bytes)

class metrics
{

public:
    /// Constructor
    /// @param info The info of the metrics in a pointer.
    /// @param size The size of the pointer in elements.
    metrics(const metric_info* info, std::size_t size);

    /// Delegate Constructor. Will pass a size-deduced array to the pointer/size
    /// constructor
    /// @param info The info of the metrics that will be contained within this
    /// object with types, names and descriptions
    template <std::size_t N>
    metrics(const metric_info (&info)[N]) : metrics(info, N)
    {
    }

    /// Destructor
    ~metrics();

    /// @returns the underlying data of the metrics object.
    auto data() const -> const uint8_t*;

    /// @returns the number of metrics in the collection
    auto metric_count() const -> std::size_t;

    /// @return the bytes used for metric names from a metrics data pointer
    auto name_bytes() const -> uint16_t;

    /// @return the bytes used for metric descriptions from a metrics data
    /// pointer
    auto description_bytes() const -> uint16_t;

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

    /// @returns true if the value of the metric is of type uint64_t
    /// @param index The index of the metric to check. Must be less than
    /// metric_count() and initialized with initialize_metric<>() or
    /// initialize_constant().
    auto is_metric_uint64(std::size_t index) const -> bool;

    /// @returns true if the value of the metric is of type int64_t
    /// @param index The index of the metric to check. Must be less than
    /// metric_count() and initialized with initialize_metric<>() or
    /// initialize_constant().
    auto is_metric_int64(std::size_t index) const -> bool;

    /// @returns true if the value of the metric is of type double
    /// @param index The index of the metric to check. Must be less than
    /// metric_count() and initialized with initialize_metric<>() or
    /// initialize_constant().
    auto is_metric_float64(std::size_t index) const -> bool;

    /// @returns true if the value of the metric is of type bool
    /// @param index The index of the metric to check. Must be less than
    /// metric_count() and initialized with initialize_metric<>() or
    /// initialize_constant().
    auto is_metric_boolean(std::size_t index) const -> bool;

    /// @returns true if the metric at the given index is a constant, otherwise
    /// false.
    /// @param index The index of the metric to check. Must be less than
    /// metric_count().
    auto metric_is_constant(std::size_t index) const -> bool;

    /// @returns A wrapper for a counter at the given index with type
    /// appropriate with given enum.
    ///
    /// This function writes the name, description, value, etc. for this index
    /// into memory, such that only data for initialized metrics will be
    /// available when extracting the memory through copy_storage().
    ///
    /// The templated type must match the one given through the
    /// metric_info used for the constructor. The indices of the metrics do not
    /// match the ones in the info, so use metric_index() to get the correct
    /// index. Please do not hard-code this, as this may break with changes to
    /// yours or our code.
    ///
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    template <metric_type MetricType>
    auto initialize_metric(std::string name) const -> metric<MetricType>
    {
        using value_type = typename metric<MetricType>::value_type;

        value_type* value_ptr = (value_type*)initialize(name);
        return metric<MetricType>{value_ptr};
    }

    /// Initialize a constant uint64_t metric at the given index.
    ///
    /// A constant metric that is initialized with a value. The value is
    /// written into memory at the time of initialization and cannot be altered
    /// within the same runtime-environment. Constants are also not reset, so
    /// these can be utilized for flags, special values or other constant
    /// values.
    ///
    /// The indices of the metrics do not
    /// match the ones in the info, so use metric_index() to get the correct
    /// index. Please do not hard-code this, as this may break with changes to
    /// yours or our code.
    ///
    /// @param value The value of the constant. A uint64_t value.
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    void initialize_constant(std::string name, uint64_t value) const;

    /// Initialize a constant int64_t metric at the given index.
    ///
    /// A constant metric that is initialized with a value. The value is
    /// written into memory at the time of initialization and cannot be altered
    /// within the same runtime-environment. Constants are also not reset, so
    /// these can be utilized for flags, special values or other constant
    /// values.
    ///
    /// The indices of the metrics do not
    /// match the ones in the info, so use metric_index() to get the correct
    /// index. Please do not hard-code this, as this may break with changes to
    /// yours or our code.
    ///
    /// @param value The value of the constant. A int64_t value.
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    void initialize_constant(std::string name, int64_t value) const;

    /// Initialize a constant double metric at the given index.
    ///
    /// A constant metric that is initialized with a value. The value is
    /// written into memory at the time of initialization and cannot be altered
    /// within the same runtime-environment. Constants are also not reset, so
    /// these can be utilized for flags, special values or other constant
    /// values.
    ///
    /// The indices of the metrics do not
    /// match the ones in the info, so use metric_index() to get the correct
    /// index. Please do not hard-code this, as this may break with changes to
    /// yours or our code.
    ///
    /// @param value The value of the constant. A double value.
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    void initialize_constant(std::string name, double value) const;

    /// Initialize a constant bool metric at the given index.
    ///
    /// A constant metric that is initialized with a value. The value is
    /// written into memory at the time of initialization and cannot be altered
    /// within the same runtime-environment. Constants are also not reset, so
    /// these can be utilized for flags, special values or other constant
    /// values.
    ///
    /// The indices of the metrics do not
    /// match the ones in the info, so use metric_index() to get the correct
    /// index. Please do not hard-code this, as this may break with changes to
    /// yours or our code.
    ///
    /// @param value The value of the constant. A bool value.
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    void initialize_constant(std::string name, bool value) const;

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

    /// @returns The index of a metric with the given name. Throws an assert if
    /// no metric is found with the given name. Returns the numeric max of
    /// std::size_t if asserts are disabled.
    ///
    /// @param name The name of the metric.
    auto metric_index(std::string name) const -> std::size_t;

    /// Copies the memory of the metrics into the given data buffer.
    /// The storage_bytes() function is used to allocate the exact amount of
    /// memory needed. Example code to illustrade intented use:
    ///
    ///     std::vector<uint8_t> copied_data(metrics.storage_bytes());
    ///     metrics.copy_storage(copied_data.data(), copied_data.size());
    ///
    /// @param data the buffer to copy data into.
    /// @param size the size of the buffer. Must be equal to storage_bytes() to
    /// ensure memory alignment.
    void copy_storage(uint8_t* data, std::size_t size) const;

    /// @return The size in bytes of the metrics data
    auto storage_bytes() const -> std::size_t;

    /// Resets the values of all initialized and non-constant metrics.
    void reset_metrics();

    /// Resets the value of the metric at the given index. The metric must not
    /// be constant and must be initialized.
    ///
    /// @param index the index of the metric to reset. Must be less than
    /// metric_count()
    void reset_metric(std::size_t index);

private:
    /// No copy
    metrics(metrics&) = delete;

    /// No copy assignment
    metrics& operator=(metrics&) = delete;

    /// No move
    metrics(metrics&&) = delete;

    /// No move assignment
    metrics& operator=(metrics&&) = delete;

private:
    ///
    auto initialize(std::string name) const -> void*;

private:
    /// The info of the metrics seperated by byte-sizes
    detail::value_size_info m_info;

    /// Map to get index from names
    std::map<std::string, std::size_t> m_name_to_index;

    /// The sizes of the names of the metrics
    std::vector<uint16_t> m_name_sizes;

    /// The sizes of the descriptions of the metrics
    std::vector<uint16_t> m_description_sizes;

    /// storage_size
    std::size_t m_storage_bytes = 0;

    /// The current number of metrics that have been initialized
    uint16_t m_count = 0;
    /// The raw memory for the counters (both value and name)

    uint8_t* m_data = nullptr;
};
}
}
