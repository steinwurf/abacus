// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "view.hpp"

#include "boolean.hpp"
#include "constant.hpp"
#include "detail/helpers.hpp"
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
namespace
{
template <class Metric>
inline auto get_value(const uint8_t* data, bool is_big_endian)
    -> std::optional<typename Metric::type>
{
    assert(data != nullptr);
    if (data[0] == 0)
    {
        // The metric is unset
        return std::nullopt;
    }

    if (is_big_endian)
    {
        return endian::big_endian::get<typename Metric::type>(data + 1);
    }
    else
    {
        return endian::little_endian::get<typename Metric::type>(data + 1);
    }
}

template <>
inline auto get_value<constant::str>(const uint8_t*,
                                     bool) -> std::optional<constant::str::type>
{
    // This function should never be called, but we need it to silence the
    // compiler
    assert(false);
    return std::nullopt;
}

template <class Metric>
inline auto get_constant_value(const protobuf::Constant& constant) ->
    typename Metric::type
{
    switch (constant.value_case())
    {
    case protobuf::Constant::ValueCase::kUint64:
        return constant.uint64();
    case protobuf::Constant::ValueCase::kInt64:
        return constant.int64();
    case protobuf::Constant::ValueCase::kFloat64:
        return constant.float64();
    case protobuf::Constant::ValueCase::kBoolean:
        return constant.boolean();
    default:
        throw std::runtime_error("Invalid constant type");
    }
}
template <>
inline auto get_constant_value<constant::str>(
    const protobuf::Constant& constant) -> constant::str::type
{
    assert(constant.value_case() == protobuf::Constant::ValueCase::kString);
    return constant.string();
}
}

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
    if (m.has_constant())
    {
        // Check that Metric is one of the expected constant types
        assert((detail::is_in_variant_v<Metric, decltype(constant::value)>));
        return get_constant_value<Metric>(m.constant());
    }
    else
    {
        auto offset = detail::get_offset(m);
        assert(offset < m_value_bytes);
        return get_value<Metric>(m_value_data + offset,
                                 m_metadata.endianness() ==
                                     protobuf::Endianness::BIG);
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
// Constants
template auto view::value<abacus::constant::uint64>(const std::string& name)
    const -> std::optional<abacus::constant::uint64::type>;
template auto view::value<abacus::constant::int64>(const std::string& name)
    const -> std::optional<abacus::constant::int64::type>;
template auto view::value<abacus::constant::float64>(const std::string& name)
    const -> std::optional<abacus::constant::float64::type>;
template auto view::value<abacus::constant::boolean>(const std::string& name)
    const -> std::optional<abacus::constant::boolean::type>;
template auto view::value<abacus::constant::str>(const std::string& name) const
    -> std::optional<abacus::constant::str::type>;

}
}
