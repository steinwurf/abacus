// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <map>
#include <vector>

#include "kind.hpp"
#include "max.hpp"
#include "min.hpp"
#include "protobuf/metrics.pb.h"
#include "type.hpp"
#include "type2.hpp"
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
/// The class cannot manipulate the memory, only access the
/// pointed to values
///
/// Note that this class has no constructor, so it can only be declared and
/// then view.set_meta_data() can be called to initialize the view with the
/// meta data, subsequently view.set_value_data() can be called to populate
/// the view with the value data. To update the view with new value data
/// view.set_value_data() can be called again.
class view2
{

public:
    /// Sets the meta data pointer
    /// @param meta_data The meta data pointer
    void set_meta_data(const uint8_t* meta_data);

    /// Sets the value data pointer
    /// @param value_data The value data pointer
    void set_value_data(const uint8_t* value_data);

    /// Gets the meta data pointer
    /// @return The meta data pointer
    const uint8_t* meta_data() const;

    /// Gets the value data pointer
    /// @return The value data pointer
    const uint8_t* value_data() const;

    /// Gets the meta data size in bytes
    /// @return The meta data size in bytes
    std::size_t meta_bytes() const;

    /// Gets the value data size in bytes
    /// @return The value data size in bytes
    std::size_t value_bytes() const;

    /// @return the number of metrics from in a metrics data pointer
    auto count() const -> uint16_t;

    /// Gets the protocol version of the metrics
    /// @return The protocol version of the metrics
    auto protocol_version() const -> uint8_t;

    auto names() const -> std::vector<std::string>;

    auto type() const -> std::optional<type2>;

    template <typename T>
    auto
    value(const std::string& name) const -> std::optional<typename T::type>;

    const protobuf::Metric& metric_info(const std::string& name) const;

private:
    /// The meta data pointer
    const uint8_t* m_meta_data;

    /// The value data pointer
    const uint8_t* m_value_data;
};
}
}
