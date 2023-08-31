// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <functional>
#include <map>
#include <string>

#include "delegate.hpp"
#include "detail/value_size_info.hpp"
#include "metric.hpp"
#include "type.hpp"
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
/// 1. Header of 10 bytes
///     * Is big endian (1 byte)
///     * Protocol version (1 byte)
///     * Total name bytes (2 bytes)
///     * Total description bytes (2 bytes)
///     * Number of 8-byte-value metrics (2 bytes)
///     * Number of 1-byte-value metrics (2 bytes)
/// 2. The name sizes in bytes (2 bytes per metric)
/// 3. The description sizes in bytes (2 bytes per metric)
/// 4. The names of the metrics (? bytes)
/// 5. The descriptions of the metrics (? bytes)
/// 6. The types of metrics (1 byte per metric)
/// 7. The flags of the metrics; initialized, constant. (1 byte per metric)
/// 8. Alignment padding (if needed, max 7 bytes)
/// 9. The 8-byte-values (8 bytes per 8-byte metric)
/// 10. The 1-byte-values (1 byte per 1-byte metric)
/// 11. A bitmap of the initialized metrics (1 byte per 8 metrics)
///
/// 1-8 are static and are only written once, 9-11 are dynamic and are updated
/// whenever a metric is initialized and updated.
///
class metrics
{

public:
    /// Default constructor
    /// No metrics will be contained within this object.
    metrics();

    /// Constructor
    /// @param info The info of the metrics in a pointer.
    /// @param count The number of infos.
    metrics(const metric_info* info, std::size_t count);

    /// Delegate Constructor. Will pass a size-deduced array to the pointer/size
    /// constructor
    /// @param info The info of the metrics that will be contained within this
    /// object with types, names and descriptions
    template <std::size_t N>
    metrics(const metric_info (&info)[N]) : metrics(info, N)
    {
    }

    /// Constructor.
    /// @param infos The infos of the metrics.
    metrics(const std::vector<metric_info>& infos) :
        metrics(infos.data(), infos.size())
    {
    }

    /// Move-constructor
    /// @param other The metrics to move from.
    metrics(metrics&& other);

    /// Destructor
    ~metrics();

    /// @returns the pointer to the meta data of the metrics.
    auto meta_data() const -> const uint8_t*;

    /// @return the size of the meta data of the metrics.
    auto meta_bytes() const -> std::size_t;

    /// @returns the pointer to the value data of the metrics.
    auto value_data() const -> const uint8_t*;

    /// @return the size of the value data of the metrics.
    auto value_bytes() const -> std::size_t;

    /// @returns the number of metrics
    auto count() const -> std::size_t;

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

    /// @returns the unit of the metric at the given index.
    /// @param index The index of the metric to check. Must be less than
    /// count() and initialized with initialize_metric<>() or
    /// initialize_constant().
    auto unit(std::size_t index) const -> std::string;

    /// @returns true if the value of the metric is of type uint64_t
    /// @param index The index of the metric to check. Must be less than
    /// count() and initialized with initialize_metric<>() or
    /// initialize_constant().
    auto is_uint64(std::size_t index) const -> bool;

    /// @returns true if the value of the metric is of type int64_t
    /// @param index The index of the metric to check. Must be less than
    /// count() and initialized with initialize_metric<>() or
    /// initialize_constant().
    auto is_int64(std::size_t index) const -> bool;

    /// @returns true if the value of the metric is of type double
    /// @param index The index of the metric to check. Must be less than
    /// count() and initialized with initialize_metric<>() or
    /// initialize_constant().
    auto is_float64(std::size_t index) const -> bool;

    /// @returns true if the value of the metric is of type bool
    /// @param index The index of the metric to check. Must be less than
    /// count() and initialized with initialize_metric<>() or
    /// initialize_constant().
    auto is_boolean(std::size_t index) const -> bool;

    /// @returns The kind of metric at the given index. counter, gauge or
    /// constant
    /// @param index The index of the metric to check. Must be less than
    /// count().
    auto kind(std::size_t index) const -> abacus::kind;

    /// @returns A wrapper for a counter at the given index with type
    /// appropriate with given enum.
    ///
    /// This function writes the name, description, value, etc. for this index
    /// into memory, such that only data for initialized metrics will be
    /// available when extracting the memory through copy_storage().
    ///
    /// The templated type must match the one given through the
    /// metric_info used for the constructor. The indices of the metrics do not
    /// match the ones in the info, so use index() to get the correct
    /// index. Please do not hard-code this, as this may break with changes to
    /// yours or our code.
    ///
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    template <abacus::type MetricType>
    auto initialize_metric(const std::string& name) const -> metric<MetricType>
    {
        using value_type = typename metric<MetricType>::value_type;

        auto index = metrics::index(name);
        value_type* value_ptr = (value_type*)initialize(index);

        return metric<MetricType>{value_ptr};
    }

    /// This function registers a callback that will be called when the metric
    /// data is sampled, i.e when value_data() or value() is called. The
    /// callback must return the value of the metric.
    ///
    /// The passed callback will give the value of the metric and this value
    /// will then be written into memory behind the scenes.
    ///
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    /// @param callback The callback that will be called when the metric data
    /// is sampled.
    void observe_metric(const std::string& name,
                        delegate<uint64_t()> callback) const;

    /// This function registers a callback that will be called when the metric
    /// data is sampled, i.e when value_data() or value() is called. The
    /// callback must return the value of the metric.
    ///
    /// The passed callback will give the value of the metric and this value
    /// will then be written into memory behind the scenes.
    ///
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    /// @param callback The callback that will be called when the metric data
    /// is sampled.
    void observe_metric(const std::string& name,
                        delegate<int64_t()> callback) const;

    /// This function registers a callback that will be called when the metric
    /// data is sampled, i.e when value_data() or value() is called. The
    /// callback must return the value of the metric.
    ///
    /// The passed callback will give the value of the metric and this value
    /// will then be written into memory behind the scenes.
    ///
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    /// @param callback The callback that will be called when the metric data
    /// is sampled.
    void observe_metric(const std::string& name,
                        delegate<double()> callback) const;

    /// This function registers a callback that will be called when the metric
    /// data is sampled, i.e when value_data() or value() is called. The
    /// callback must return the value of the metric.
    ///
    /// The passed callback will give the value of the metric and this value
    /// will then be written into memory behind the scenes.
    ///
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    /// @param callback The callback that will be called when the metric data
    /// is sampled.
    void observe_metric(const std::string& name,
                        delegate<bool()> callback) const;

    /// Initialize a constant uint64_t metric at the given index.
    ///
    /// A constant metric that is initialized with a value and never reset.
    /// The value is written into memory at the time of initialization and
    /// cannot be altered within the same runtime-environment.
    ///
    /// The indices of the metrics do not
    /// match the ones in the info, so use index() to get the correct
    /// index. Please do not hard-code this, as this may break with changes to
    /// yours or our code.
    ///
    /// @param value The value of the constant. A uint64_t value.
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    void initialize_constant(const std::string& name, uint64_t value) const;

    /// Initialize a constant int64_t metric at the given index.
    ///
    /// A constant metric that is initialized with a value and never reset.
    /// The value is written into memory at the time of initialization and
    /// cannot be altered within the same runtime-environment.
    ///
    /// The indices of the metrics do not
    /// match the ones in the info, so use index() to get the correct
    /// index. Please do not hard-code this, as this may break with changes to
    /// yours or our code.
    ///
    /// @param value The value of the constant. A int64_t value.
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    void initialize_constant(const std::string& name, int64_t value) const;

    /// Initialize a constant double metric at the given index.
    ///
    /// A constant metric that is initialized with a value and never reset.
    /// The value is written into memory at the time of initialization and
    /// cannot be altered within the same runtime-environment.
    ///
    /// The indices of the metrics do not
    /// match the ones in the info, so use index() to get the correct
    /// index. Please do not hard-code this, as this may break with changes to
    /// yours or our code.
    ///
    /// @param value The value of the constant. A double value.
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    void initialize_constant(const std::string& name, double value) const;

    /// Initialize a constant bool metric at the given index.
    ///
    /// A constant metric that is initialized with a value and never reset.
    /// The value is written into memory at the time of initialization and
    /// cannot be altered within the same runtime-environment.
    ///
    /// The indices of the metrics do not
    /// match the ones in the info, so use index() to get the correct
    /// index. Please do not hard-code this, as this may break with changes to
    /// yours or our code.
    ///
    /// @param value The value of the constant. A bool value.
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    void initialize_constant(const std::string& name, bool value) const;

    /// Copy the value of the uint64_t metric into a passed reference. This is
    /// used to extract the values during runtime.
    ///
    /// Make sure that the type and index are correct using is_uint64()
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
    /// Make sure that the type and index are correct using is_int64()
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
    /// Make sure that the type and index are correct using is_float64()
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
    /// Make sure that the type and index are correct using is_boolean()
    /// and index() to get the correct index and
    /// type. Please do not hard-code these values, as this may break with
    /// changes to your code.
    ///
    /// @param index The index of the metric to copy. Must be less than
    /// count().
    /// @param value The variable to copy the value into. A bool reference.
    void value(std::size_t index, bool& value) const;

    /// @returns The index of a metric with the given name. Throws an assert if
    /// no metric is found with the given name. Returns the numeric max of
    /// std::size_t if asserts are disabled.
    ///
    /// @param name The name of the metric.
    auto index(const std::string& name) const -> std::size_t;

    /// Resets the values of all initialized and non-constant metrics.
    void reset_metrics();

    /// Resets the value of the metric at the given index. The metric must not
    /// be constant and must be initialized.
    ///
    /// @param index the index of the metric to reset. Must be less than
    /// count()
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
    /// initialize the metrics
    auto initialize(std::size_t index) const -> void*;

private:
    /// The info of the metrics seperated by byte-sizes
    detail::value_size_info m_info;

    /// Map to get index from names
    std::map<std::string, std::size_t> m_name_to_index;

    /// The raw memory for the meta data
    uint8_t* m_meta_data = nullptr;

    /// The raw memory for the value data
    uint8_t* m_value_data = nullptr;

    /// Map of the observed metrics and the callbacks that are called on
    /// value_data() and value()
    mutable std::map<std::string, std::function<void()>> m_observer_map;
};
}
}
