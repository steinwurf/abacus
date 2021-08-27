// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <vector>

#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
class view
{
    /// This class contains utility functions that are inherited by the metrics
    /// class.

    /// It's main use-case is for when the data of a metrics object is
    /// copied to a data buffer. abacus::view can then be used to extract the
    /// information from the metrics-data.

    /// Note that this class has no constructor, so it can only be declared and
    /// then view.set_data() can be called to update the data
public:
    static constexpr std::size_t header_size = 5;

public:
    /// Sets the data pointer of the view to read the memory of
    void set_data(uint8_t* data);

    /// Accesses the maximum name size from a metrics data pointer
    auto max_name_bytes() const -> uint16_t;

    /// Accesses the maximum number of metrics from a metrics data pointer
    auto max_metrics() const -> uint16_t;

    /// @return A pointer to the title of the counter
    auto raw_title() const -> const char*;

    /// @return A pointer to the title of the counter
    auto raw_title() -> char*;

    /// @return A pointer to the name of the counter
    auto raw_name(std::size_t index) const -> const char*;

    /// @return A pointer to the name of the counter
    auto raw_name(std::size_t index) -> char*;

    /// @return A pointer to the value of the counter
    auto raw_value(std::size_t index) const -> const uint64_t*;

    /// @return A pointer to the value of the counter
    auto raw_value(std::size_t index) -> uint64_t*;

    /// @return The byte offset to the title section
    auto title_offset() const -> std::size_t;

    /// @return The byte offset to the level section
    auto level_offset() const -> std::size_t;

    /// @return The byte offset to the names section
    auto names_offset() const -> std::size_t;

    /// @return The byte offset to the values section
    auto values_offset() const -> std::size_t;

    /// @return The number of bytes in the view memory
    auto view_bytes() const -> std::size_t;

    /// @return All counters in json format
    auto to_json() const -> std::string;

private:
    uint8_t* m_data = nullptr;
};
}
}