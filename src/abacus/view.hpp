// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "detail/is_constant.hpp"
#include "protobuf/metrics.pb.h"
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
/// The class cannot manipulate the memory, only access the values.
///
/// Note that this class has no constructor, so it can only be declared and
/// then view.set_metadata() can be called to initialize the view with the
/// meta data, subsequently view.set_value_data() can be called to populate
/// the view with the value data. To update the view with new value data
/// view.set_value_data() can be called again.
class view
{
public:
    /// Sets the meta data
    /// @param metadata The meta data
    /// @return true if the meta data was unpacked correctly otherwise false
    [[nodiscard]]
    auto set_metadata(const protobuf::MetricsMetadata& metadata) -> bool;

    /// Sets the value data pointer
    /// @param value_data The value data pointer
    /// @param value_bytes The value data size in bytes
    /// @return true if the hash is correct otherwise false
    [[nodiscard]] auto set_value_data(const uint8_t* value_data,
                                      std::size_t value_bytes) -> bool;

    /// Gets the value data pointer
    /// @return The value data pointer
    const uint8_t* value_data() const;

    /// Gets the value data size in bytes
    /// @return The value data size in bytes
    std::size_t value_bytes() const;

    /// Gets the metric
    /// @param name The name of the metric
    /// @return The metric
    const protobuf::Metric& metric(const std::string& name) const;

    /// Gets the meta data
    /// @return The meta data
    auto metadata() const -> const protobuf::MetricsMetadata&;

    /// Gets the value of a metric
    /// @param name The name of the metric
    /// @return The value of the metric or std::nullopt if the metric has no
    /// value
    template <class Metric>
    auto value(const std::string& name) const
        -> std::conditional_t<detail::is_constant_v<Metric>,
                              typename Metric::type,
                              std::optional<typename Metric::type>>;

private:
    /// The meta data
    protobuf::MetricsMetadata m_metadata;

    /// The value data pointer
    const uint8_t* m_value_data;

    /// The value data size in bytes
    std::size_t m_value_bytes;
};
}
}
