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
public:
    static constexpr std::size_t header_size = 8;

public:
    void set_data(uint8_t* data);

    auto max_metrics() const -> const uint16_t;

    auto max_name_bytes() const -> const uint16_t;

    auto level() const -> const uint8_t;

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

    auto view_bytes() const -> std::size_t;

private:
    uint8_t* m_data = nullptr;
};
}
}