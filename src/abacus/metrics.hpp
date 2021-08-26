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
class metrics : public view
{
public:
    // The header consists of 42 bits of 3 values:
    // 1. 16 bit denoting the max size of name
    // 2. 16 bit denoting the max number of counters
    // 3. 8 bit denoting the max size of values

    static constexpr std::size_t header_size = 5;

public:
    /// Default constructor
    metrics(uint16_t max_metrics, uint16_t max_name_bytes,
            const std::string& title);

    /// Destructor
    ~metrics();

    /// Set the name of all the metrics contained within
    void set_metrics_title(const std::string& title);

    /// @return The name of a counter as a string
    auto metric_name(std::size_t index) const -> std::string;

    /// @return A specific count
    auto metric_value(std::size_t index) const -> uint64_t;

    /// @return The value of the counter
    auto initialize_metric(std::size_t index, const std::string& name)
        -> metric;

    /// @return True if the counter has been initialized
    auto is_metric_initialized(std::size_t index) const -> bool;

    /// Copies the memory backing the counter storage to a data pointer
    void copy_storage(uint8_t* data) const;

    /// @return The size of the counter storage in bytes
    auto storage_bytes() const -> std::size_t;

    /// Reset all the counters
    void reset_metrics();

    /// Reset specific counter
    void reset_metric(std::size_t index);

    /// @return The number of counters
    auto metrics_count() const -> std::size_t;

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
    // The number of values
    uint16_t m_max_metrics = 0;

    // The number of values
    uint16_t m_max_name_bytes = 0;

    // The level
    uint8_t m_level = 0;

    // The raw memory for the counters (both value and name)
    uint8_t* m_data = nullptr;
};

}
}
