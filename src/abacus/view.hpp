// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <map>
#include <vector>

#include <endian/big_endian.hpp>
#include <endian/is_big_endian.hpp>
#include <endian/little_endian.hpp>

#include "detail/hash_function.hpp"
#include "max.hpp"
#include "min.hpp"
#include "protobuf/metrics.pb.h"
#include "type.hpp"
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
/// then view.set_meta_data() can be called to initialize the view with the
/// meta data, subsequently view.set_value_data() can be called to populate
/// the view with the value data. To update the view with new value data
/// view.set_value_data() can be called again.
class view
{
public:
    /// Sets the meta data pointer
    /// @param meta_data The meta data pointer
    void set_meta_data(const uint8_t* metadata_data, std::size_t metadata_bytes)
    {
        assert(metadata_data != nullptr);
        m_metadata_data = metadata_data;
        m_metadata_bytes = metadata_bytes;
        m_value_data = nullptr;
        m_value_bytes = 0;

        m_metadata.ParseFromArray(metadata_data, metadata_bytes);
    }

    /// Sets the value data pointer
    /// @param value_data The value data pointer
    /// @return true if the hash is correct otherwise false
    auto set_value_data(const uint8_t* value_data,
                        std::size_t value_bytes) -> bool
    {
        assert(m_metadata_data != nullptr);
        assert(value_data != nullptr);

        // Check that the hash is correct
        uint32_t value_data_hash = 0;
        switch (m_metadata.endianness())
        {
        case protobuf::Endianness::BIG:
            endian::big_endian::get(value_data_hash, value_data);
            break;
        case protobuf::Endianness::LITTLE:
            endian::little_endian::get(value_data_hash, value_data);
            break;
        default:
            assert(false);
        }

        if (m_metadata.sync_value() != value_data_hash)
        {
            return false;
        }
        m_value_data = value_data;
        m_value_bytes = value_bytes;
        return true;
    }

    /// Gets the meta data pointer
    /// @return The meta data pointer
    auto metadata_data() const -> const uint8_t*
    {
        return m_metadata_data;
    }

    /// Gets the value data pointer
    /// @return The value data pointer
    const uint8_t* value_data() const
    {
        return m_value_data;
    }

    /// Gets the meta data size in bytes
    /// @return The meta data size in bytes
    std::size_t metadata_bytes() const
    {
        return m_metadata_bytes;
    }

    /// Gets the value data size in bytes
    /// @return The value data size in bytes
    std::size_t value_bytes() const
    {
        return m_value_bytes;
    }

    /// Gets the meta data
    /// @return The meta data
    auto metadata() const -> const protobuf::MetricsMetadata&
    {
        return m_metadata;
    }

    /// Gets the value of a metric
    /// @param name The name of the metric
    /// @return The value of the metric or std::nullopt if the metric has no
    /// value
    template <class Metric>
    auto
    value(const std::string& name) const -> std::optional<typename Metric::type>
    {
        assert(m_value_data != nullptr);
        assert(m_metadata_data != nullptr);

        auto endianness = m_metadata.endianness();

        auto offset = metric(name).offset();
        assert(offset < m_value_bytes);

        if (m_value_data[offset] == 0)
        {
            return std::nullopt;
        }

        typename Metric::type value;

        switch (m_metadata.endianness())
        {
        case protobuf::Endianness::BIG:
            endian::big_endian::get(value, m_value_data + offset + 1);
            break;
        case protobuf::Endianness::LITTLE:
            endian::little_endian::get(value, m_value_data + offset + 1);
            break;
        default:
            assert(false);
        }
        return value;
    }

    /// Gets the metric
    /// @param name The name of the metric
    /// @return The metric
    const protobuf::Metric& metric(const std::string& name) const
    {
        return m_metadata.metrics().at(name);
    }

private:
    /// The meta data pointer
    const uint8_t* m_metadata_data;

    /// The meta data size in bytes
    std::size_t m_metadata_bytes;

    /// The meta data
    protobuf::MetricsMetadata m_metadata;

    /// The value data pointer
    const uint8_t* m_value_data;

    /// The value data size in bytes
    std::size_t m_value_bytes;
};
}
}
