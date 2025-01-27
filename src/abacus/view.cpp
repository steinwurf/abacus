// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "view.hpp"

#include "boolean.hpp"
#include "enum8.hpp"
#include "float32.hpp"
#include "float64.hpp"
#include "int32.hpp"
#include "int64.hpp"
#include "protocol_version.hpp"
#include "uint32.hpp"
#include "uint64.hpp"

#include <cassert>
#include <map>
#include <vector>

#include <endian/big_endian.hpp>
#include <endian/is_big_endian.hpp>
#include <endian/little_endian.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
[[nodiscard]] auto
view::set_metadata(const protobuf::MetricsMetadata& metadata) -> bool
{
    assert(metadata.IsInitialized());
    m_metadata = metadata;
    if (m_metadata.protocol_version() != protocol_version())
    {
        m_metadata.Clear();
        return false;
    }
    return true;
}

[[nodiscard]] auto view::set_value_data(const uint8_t* value_data,
                                        std::size_t value_bytes) -> bool
{
    assert(m_metadata.IsInitialized());
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

const uint8_t* view::value_data() const
{
    return m_value_data;
}

std::size_t view::value_bytes() const
{
    return m_value_bytes;
}

auto view::metadata() const -> const protobuf::MetricsMetadata&
{
    return m_metadata;
}

const protobuf::Metric& view::metric(const std::string& name) const
{
    assert(m_metadata.metrics().count(name) != 0);
    return m_metadata.metrics().at(name);
}

template <class Metric>
auto view::value(const std::string& name) const
    -> std::optional<typename Metric::type>
{
    assert(m_metadata.IsInitialized());
    assert(m_value_data != nullptr);
    auto m = metric(name);
    auto offset = m.offset();
    assert(offset < m_value_bytes);

    if (m_value_data[offset] == 0)
    {
        // Either the metric is unintialized or it's optional and has no value
        return std::nullopt;
    }

    if (m_metadata.endianness() == protobuf::Endianness::BIG)
    {
        return endian::big_endian::get<typename Metric::type>(m_value_data +
                                                              offset + 1);
    }
    else
    {
        assert(m_metadata.endianness() == protobuf::Endianness::LITTLE);
        return endian::little_endian::get<typename Metric::type>(m_value_data +
                                                                 offset + 1);
    }
}

// Explicit instantiations for the expected types
template auto view::value<abacus::uint64>(const std::string& name) const
    -> std::optional<abacus::uint64::type>;
template auto view::value<abacus::int64>(const std::string& name) const
    -> std::optional<abacus::int64::type>;
template auto view::value<abacus::uint32>(const std::string& name) const
    -> std::optional<abacus::uint32::type>;
template auto view::value<abacus::int32>(const std::string& name) const
    -> std::optional<abacus::int32::type>;
template auto view::value<abacus::float64>(const std::string& name) const
    -> std::optional<abacus::float64::type>;
template auto view::value<abacus::float32>(const std::string& name) const
    -> std::optional<abacus::float32::type>;
template auto view::value<abacus::boolean>(const std::string& name) const
    -> std::optional<abacus::boolean::type>;
template auto view::value<abacus::enum8>(const std::string& name) const
    -> std::optional<abacus::enum8::type>;
}
}
