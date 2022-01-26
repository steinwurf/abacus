// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <vector>

#include "metric.hpp"
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
/// header_size + max_name_bytes + max_metrics * (max_name_bytes + 8).
///
/// To save memory, take caution in choosing the max_metrics. If your
/// library only uses 10 counters, you should probably not choose
/// max_metrics = 64. You can update the max_metrics value as you go to
/// ensure optimal memory usage.
///
/// The header consists of 42 bits of 3 values:
/// 1. 16 bit denoting the max size of name
/// 2. 16 bit denoting the max number of counters
/// 3. 16 bit denoting the max size of scope
/// 4. 8 bit denoting the max size of values
class metrics
{

public:
    /// Default constructor
    /// @param max_metrics The maximum number of metrics this object will
    /// contain. Must be a number that can fit in 2 bytes.
    /// @param max_name_bytes The maximum length in bytes the title/names of the
    /// counters will contain. Must be a number that can fit in 2 bytes.
    /// @param max_scope_bytes The maximum length in bytes for the scope
    /// of the metrics object. Must be a number that can fit in 2 bytes. When
    /// adding scopees, make sure that the new scope size does not exceed
    /// this size.
    /// @param scope The scope of the metrics object
    metrics(std::size_t max_metrics, std::size_t max_name_bytes,
            std::size_t max_scope_bytes, const std::string& scope);

    /// Destructor
    ~metrics();

    /// @return the maximum number of metrics that was provided to the
    /// constructor
    auto max_metrics() const -> std::size_t;

    /// @return the maximum number of bytes used for the scope of the metrics
    /// object
    auto max_scope_bytes() const -> std::size_t;

    /// @return the maximum number of bytes used for a metric name that was
    /// provided to the constructor
    auto max_name_bytes() const -> std::size_t;

    /// @param index The index of a counter. Must be less than max_metrics.
    /// @return The name of a counter as a string
    auto metric_name(std::size_t index) const -> std::string;

    /// @param index The index of a counter. Must be less than max_metrics.
    /// @return A specific count
    auto metric_value(std::size_t index) const -> uint64_t;

    /// @param name The name of a counter with full scope. The name must be
    /// of the form scope.metric_name
    /// @return The index of a counter.
    auto metric_index(const std::string& name) const -> std::size_t;

    /// @return The scope of the counters as a string.
    auto scope() const -> std::string;

    /// @param name The name of the new counter. Must be less than
    /// max_name_bytes bytes
    /// @return The value of the counter
    auto initialize_metric(const std::string& name) -> metric;

    /// @param index The index of the new counter. Must be less than
    /// max_metrics.
    /// @return True if the counter has been initialized
    auto is_metric_initialized(std::size_t index) const -> bool;

    /// @return The number of metrics currently initialized in the object
    auto metrics_count() const -> std::size_t;

    /// @param text The text to add to the scope of the metrics object.
    ///             The extra scopes can be used to access different metrics
    ///             objects. For example if you have a metrics object with
    ///             scope "bar", calling add_scope("foo") will make
    ///             the scope of the metrics object "baz", "foo.bar.baz".
    void add_scope(const std::string& text);

    /// Copies the memory backing the counter storage to a data pointer
    /// @param data The data pointer to copy the raw memory to
    void copy_storage(uint8_t* data) const;

    /// @return The size of the counter storage in bytes
    auto storage_bytes() const -> std::size_t;

    /// Reset all the counters
    void reset_metrics();

    /// Reset specific counter
    /// @param index The index of the new counter. Must be less than
    /// max_metrics.
    void reset_metric(std::size_t index);

    /// @return All counters in json format
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
    /// The maximum number of metrics that can be stored in the object
    std::size_t m_max_metrics = 0;

    /// The current number of metrics that have been initialized
    std::size_t m_metrics_count = 0;

    /// The maximum number of bytes that can be used for the name of each metric
    std::size_t m_max_name_bytes = 0;

    /// The maximum number of bytes that can be used for the scope of the
    /// metrics object
    std::size_t m_max_scope_bytes = 0;

    /// The raw memory for the counters (both value and name)
    uint8_t* m_data = nullptr;

    /// The scopees prepended to the metrics object
    std::vector<std::string> m_scopes;
};

}
}
