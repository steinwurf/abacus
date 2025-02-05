// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "view.hpp"

#include "boolean.hpp"
#include "constant.hpp"
#include "detail/is_constant.hpp"
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
static inline std::size_t get_offset(const protobuf::Metric& m)
{
    switch (m.type_case())
    {
    case protobuf::Metric::kUint64:
        return m.uint64().offset();
    case protobuf::Metric::kInt64:
        return m.int64().offset();
    case protobuf::Metric::kUint32:
        return m.uint32().offset();
    case protobuf::Metric::kInt32:
        return m.int32().offset();
    case protobuf::Metric::kFloat64:
        return m.float64().offset();
    case protobuf::Metric::kFloat32:
        return m.float32().offset();
    case protobuf::Metric::kBoolean:
        return m.boolean().offset();
    case protobuf::Metric::kEnum8:
        return m.enum8().offset();
    case protobuf::Metric::kConstant:
        // This should never be reached
        assert(false);
        return 0;
    case protobuf::Metric::TYPE_NOT_SET:
        // This should never be reached
        assert(false);
        return 0;
    default:
        // This should never be reached
        assert(false);
        return 0;
    }
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
    -> std::conditional_t<detail::is_constant_v<Metric>, typename Metric::type,
                          std::optional<typename Metric::type>>
{
    assert(m_metadata.IsInitialized());
    assert(m_value_data != nullptr);
    auto m = metric(name);
    if constexpr (detail::is_constant_v<Metric>)
    {
        // Check that Metric is constant
        assert(m.has_constant());
        auto constant = m.constant();
        if constexpr (std::is_same_v<Metric, constant::str>)
        {
            if (constant.value_case() != protobuf::Constant::ValueCase::kString)
            {
                throw std::runtime_error("Invalid constant type");
            }
            return constant.string();
        }
        if constexpr (!std::is_same_v<Metric, constant::str>)
        {
            auto constant = m.constant();
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
    }
    if constexpr (!detail::is_constant_v<Metric>)
    {
        auto offset = get_offset(m);
        assert(offset < m_value_bytes);
        auto data = m_value_data + offset;
        assert(data != nullptr);
        if (data[0] == 0)
        {
            // The metric is unset
            return std::nullopt;
        }

        if (m_metadata.endianness() == protobuf::Endianness::BIG)
        {
            return endian::big_endian::get<typename Metric::type>(data + 1);
        }
        else
        {
            return endian::little_endian::get<typename Metric::type>(data + 1);
        }
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

// Constants (no optional)
template auto view::value<abacus::constant::uint64>(
    const std::string& name) const -> abacus::constant::uint64::type;
template auto view::value<abacus::constant::int64>(
    const std::string& name) const -> abacus::constant::int64::type;
template auto view::value<abacus::constant::float64>(
    const std::string& name) const -> abacus::constant::float64::type;
template auto view::value<abacus::constant::boolean>(
    const std::string& name) const -> abacus::constant::boolean::type;
template auto view::value<abacus::constant::str>(const std::string& name) const
    -> abacus::constant::str::type;
}
}
