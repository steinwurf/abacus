// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <vector>

#include "detail/raw.hpp"
#include "detail/value_size_info.hpp"
#include "metric.hpp"
#include "value_types.hpp"
#include "version.hpp"
#include "view.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// This class preallocates memory at construction to hold a header, a title
/// of size max_name_bytes, max_metrics metrics, each having a name of size
/// max_name_bytes, with a metric value of 8 bytes (sizeof(uint64_t)).
///
/// The total preallocated memory is
/// header_size + max_metrics * (max_name_bytes + 8).
///
/// To save memory, take caution in choosing the max_metrics. If your
/// library only uses 10 counters, you should probably not choose
/// max_metrics = 64.
///
/// The header consists of 42 bits of 3 values:
/// 1. 16 bit denoting the number of counters

class metrics
{

public:
    /// Constructor
    /// @param info The info needed to initialize all the metrics with types,
    /// names and descriptions
    metrics(std::vector<metric_info> info);

    /// Destructor
    ~metrics();

    /// @returns the number of metrics in the collection
    auto metric_count() const -> std::size_t;

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

    /// @returns A wrapper for a counter at the given index.

    /// The templated type must match the one given through the metric_info used
    /// for the constructor. The indices of the metrics do not match the ones in
    /// the info, so use metric_type(), metric_name() and metric_index() to get
    /// the correct index, name and type. Please do not hard-code this, as this
    /// may break with changes to your code.

    /// @param index The index of the metric to initialize. Must be less than
    /// metric_count().
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    template <value_type T>
    auto initialize_metric(std::size_t index, std::string name) const
        -> metric<T>
    {
        (void)index;
        (void)name;
        throw std::runtime_error("Unknown metric type");
    }

    /// Initialize a constant uint64_t metric at the given index.
    ///
    /// A constant metric that is initialized with a value. The value is
    /// written into memory at the time of initialization and cannot be altered
    /// within the same runtime-environment. Constants are also not reset, so
    /// these can be utilized for flags, special values or other constant
    /// values.
    ///
    /// Make sure that the type, index and name are correct using metric_type(),
    /// metric_name() and metric_index() to get the correct index, name and
    /// type. Please do not hard-code these values, as this may break with
    /// changes to your code.
    ///
    /// @param index The index of the metric to initialize. Must be less than
    /// metric_count().
    /// @param value The value of the constant. A uint64_t value.
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    void initialize_constant(std::size_t index, uint64_t value,
                             std::string name) const;

    /// Initialize a constant int64_t metric at the given index.
    ///
    /// A constant metric that is initialized with a value. The value is
    /// written into memory at the time of initialization and cannot be altered
    /// within the same runtime-environment. Constants are also not reset, so
    /// these can be utilized for flags, special values or other constant
    /// values.
    ///
    /// Make sure that the type, index and name are correct using metric_type(),
    /// metric_name() and metric_index() to get the correct index, name and
    /// type. Please do not hard-code these values, as this may break with
    /// changes to your code.
    ///
    /// @param index The index of the metric to initialize. Must be less than
    /// metric_count().
    /// @param value The value of the constant. A int64_t value.
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    void initialize_constant(std::size_t index, int64_t value,
                             std::string name) const;

    /// Initialize a constant double metric at the given index.
    ///
    /// A constant metric that is initialized with a value. The value is
    /// written into memory at the time of initialization and cannot be altered
    /// within the same runtime-environment. Constants are also not reset, so
    /// these can be utilized for flags, special values or other constant
    /// values.
    ///
    /// Make sure that the type, index and name are correct using metric_type(),
    /// metric_name() and metric_index() to get the correct index, name and
    /// type. Please do not hard-code these values, as this may break with
    /// changes to your code.
    ///
    /// @param index The index of the metric to initialize. Must be less than
    /// metric_count().
    /// @param value The value of the constant. A double value.
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    void initialize_constant(std::size_t index, double value,
                             std::string name) const;

    /// Initialize a constant bool metric at the given index.
    ///
    /// A constant metric that is initialized with a value. The value is
    /// written into memory at the time of initialization and cannot be altered
    /// within the same runtime-environment. Constants are also not reset, so
    /// these can be utilized for flags, special values or other constant
    /// values.
    ///
    /// Make sure that the type, index and name are correct using metric_type(),
    /// metric_name() and metric_index() to get the correct index, name and
    /// type. Please do not hard-code these values, as this may break with
    /// changes to your code.
    ///
    /// @param index The index of the metric to initialize. Must be less than
    /// metric_count().
    /// @param value The value of the constant. A bool value.
    /// @param name The name of the metric. This is used for a check to ensure
    /// that the name matches the one at the given index.
    void initialize_constant(std::size_t index, bool value,
                             std::string name) const;

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

    /// @returns The scope of the metrics.
    ///
    /// The scope of the metrics is copied onto memory when using the
    /// copy_storage() function. It can be used to distinguish different metrics
    /// objects and to identify the metrics in the memory.
    auto scope() const -> std::string;

    auto scope_size() const -> std::size_t;

    void push_scope(const std::string& text);

    void pop_scope();

    void copy_storage(uint8_t* data) const;

    auto storage_bytes() const -> std::size_t;

    void reset_metrics();

    void reset_metric(std::size_t index);

    auto to_json() const -> std::string;

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
    /// The info of the metrics seperated by byte-sizes
    detail::value_size_info m_info;

    /// The sizes of the names of the metrics
    std::vector<uint16_t> m_name_sizes;

    /// The sizes of the descriptions of the metrics
    std::vector<uint16_t> m_description_sizes;

    /// storage_size
    std::size_t m_storage_bytes = 0;

    /// The current number of metrics that have been initialized
    uint16_t m_count = 0;

    std::string m_scope = "";

    /// The raw memory for the counters (both value and name)
    uint8_t* m_data = nullptr;

    /// The scopes prepended to the metrics object
    std::vector<std::string> m_scopes;
};

template <>
inline auto metrics::initialize_metric<value_type::unsigned_integral>(
    std::size_t index, std::string name) const
    -> metric<value_type::unsigned_integral>
{
    assert(index < m_count);
    assert(!is_metric_initialized(index));
    assert(!metric_is_constant(index));
    assert(name == m_info[index].name);

    char* name_ptr = detail::raw_name(m_data, index);
    std::memcpy(name_ptr, m_info[index].name.c_str(), m_name_sizes[index]);

    auto description_ptr = detail::raw_description(m_data, index);
    std::memcpy(description_ptr, m_info[index].description.c_str(),
                m_description_sizes[index]);

    uint64_t* value_ptr = detail::raw_value<uint64_t>(m_data, index);

    *value_ptr = 0U;

    return metric<value_type::unsigned_integral>{value_ptr};
}

template <>
inline auto
metrics::initialize_metric<value_type::signed_integral>(std::size_t index,
                                                        std::string name) const
    -> metric<value_type::signed_integral>
{
    assert(index < m_count);
    assert(!is_metric_initialized(index));
    assert(!metric_is_constant(index));
    assert(name == m_info[index].name);

    auto name_ptr = detail::raw_name(m_data, index);
    std::memcpy(name_ptr, m_info[index].name.c_str(), m_name_sizes[index]);

    auto description_ptr = detail::raw_description(m_data, index);
    std::memcpy(description_ptr, m_info[index].description.c_str(),
                m_description_sizes[index]);

    int64_t* value_ptr = detail::raw_value<int64_t>(m_data, index);

    *value_ptr = 0;

    return metric<value_type::signed_integral>{value_ptr};
}

template <>
inline auto
metrics::initialize_metric<value_type::floating_point>(std::size_t index,
                                                       std::string name) const
    -> metric<value_type::floating_point>
{
    assert(index < m_count);
    assert(!is_metric_initialized(index));
    assert(!metric_is_constant(index));
    assert(name == m_info[index].name);

    auto name_ptr = detail::raw_name(m_data, index);
    std::memcpy(name_ptr, m_info[index].name.c_str(), m_name_sizes[index]);

    auto description_ptr = detail::raw_description(m_data, index);
    std::memcpy(description_ptr, m_info[index].description.c_str(),
                m_description_sizes[index]);

    double* value_ptr = detail::raw_value<double>(m_data, index);

    *value_ptr = 0.0;

    return metric<value_type::floating_point>{value_ptr};
}

template <>
inline auto metrics::initialize_metric<value_type::boolean>(
    std::size_t index, std::string name) const -> metric<value_type::boolean>
{
    assert(index < m_count);
    assert(!is_metric_initialized(index));
    assert(!metric_is_constant(index));
    assert(name == m_info[index].name);

    auto name_ptr = detail::raw_name(m_data, index);
    std::memcpy(name_ptr, m_info[index].name.c_str(), m_name_sizes[index]);

    auto description_ptr = detail::raw_description(m_data, index);
    std::memcpy(description_ptr, m_info[index].description.c_str(),
                m_description_sizes[index]);

    bool* value_ptr = detail::raw_value<bool>(m_data, index);

    *value_ptr = false;

    return metric<value_type::boolean>{value_ptr};
}

}
}
