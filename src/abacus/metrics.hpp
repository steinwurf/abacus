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
    // The header consists of two 64 bit values:
    // 1. 8 bit endian flag
    // 2. 8 bit size of values
    // 3. 16 bit size of name
    // 4. 16 bit number of counters
    // 5. 8 bit level
    // 6. 16 bit title
    static constexpr std::size_t header_size = 9;

    /// The maximum number of counters supported
    // static constexpr std::size_t max_counters = 64;

    /// The maximum size of a name in bytes
    // static constexpr std::size_t max_name_size = 64;

    /// The offset to the names
    // static constexpr std::size_t names_offset = header_size;

    /// The offset to the values
    // static constexpr std::size_t values_offset =
    //   header_size + (max_counters * max_name_size);

public:
    /// Default constructor
    metrics(uint16_t max_metrics, uint16_t max_name_bytes,
            const std::string& title, uint8_t level);

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

    /// @return All counters in json format
    auto to_json() const -> std::string;

    /// Reset all the counters
    void reset_metrics();

    /// Reset specific counter
    void reset_metric(std::size_t index);

    /// @return The number of counters
    auto metrics_count() const -> std::size_t;

    /// @return The level of the metrics
    auto metrics_level() const -> uint8_t;

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
