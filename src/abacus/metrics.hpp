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
class metrics
{
    /// This class preallocates memory at construction to hold a header, a title
    /// of size max_name_bytes, max_metrics metrics, each having a name of size
    /// max_name_bytes, with a metric value of 8 bytes (sizeof(uint64_t)).

    /// The total preallocated memory is
    /// header_size + max_name_bytes + max_metrics * (max_name_bytes + 8)

    /// To save memory, take caution in choosing the max_metrics. If your
    /// library only uses 10 counters, you should probably not choose
    /// max_metrics = 64. You can update the max_metrics value as you go to
    /// ensure optimal memory usage.

    /// The header consists of 42 bits of 3 values:
    /// 1. 16 bit denoting the max size of name
    /// 2. 16 bit denoting the max number of counters
    /// 3. 8 bit denoting the max size of values

public:
    /// Default constructor
    /// @param max_metrics The maximum number of metrics this object will
    /// contain. Must be a number that can fit in 2 bytes.
    /// @param max_name_bytes The maximum length in bytes the title/names of the
    /// counters will contain. Must be a number that can fit in 2 bytes.
    /// @param title The title of the metrics object
    metrics(std::size_t max_metrics, std::size_t max_name_bytes,
            const std::string& title);

    /// Destructor
    ~metrics();

    /// Return the maximum number of metrics that was provided to the
    /// constructor
    auto max_metrics() const -> std::size_t;

    /// Return the maximum number of bytes used for a metric name that was
    /// provided to the constructor
    auto max_name_bytes() const -> std::size_t;

    /// Set the name of all the metrics contained within
    /// @param title The title of the metrics object
    void set_metrics_title(const std::string& title);

    /// @param index The index of a counter. Must be less than max_metrics.
    /// @return The name of a counter as a string
    auto metric_name(std::size_t index) const -> std::string;

    /// @param index The index of a counter. Must be less than max_metrics.
    /// @return A specific count
    auto metric_value(std::size_t index) const -> uint64_t;

    /// @param index The index of the new counter. Must be less than
    /// max_metrics.
    /// @param name The name of the new counter. Must be less than
    /// max_name_bytes bytes
    /// @return The value of the counter
    auto initialize_metric(std::size_t index, const std::string& name)
        -> metric;

    /// @param index The index of the new counter. Must be less than
    /// max_metrics.
    /// @return True if the counter has been initialized
    auto is_metric_initialized(std::size_t index) const -> bool;

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
    /// The number of values
    std::size_t m_max_metrics = 0;

    /// The number of values
    std::size_t m_max_name_bytes = 0;

    /// The raw memory for the counters (both value and name)
    uint8_t* m_data = nullptr;
};

}
}
