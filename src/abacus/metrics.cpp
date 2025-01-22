// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "metrics.hpp"

#include "detail/hash_function.hpp"
#include "detail/size_of_type.hpp"

#include "info.hpp"
#include "protocol_version.hpp"
#include "version.hpp"

#include "protobuf/metrics.pb.h"

#include <endian/is_big_endian.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

static inline auto
get_kind(const protobuf::Metric& metric) -> std::optional<protobuf::Kind>
{
    switch (metric.type_case())
    {
    case protobuf::Metric::kUint64:
        return metric.uint64().kind();
    case protobuf::Metric::kInt64:
        return metric.int64().kind();
    case protobuf::Metric::kUint32:
        return metric.uint32().kind();
    case protobuf::Metric::kInt32:
        return metric.int32().kind();
    case protobuf::Metric::kFloat64:
        return metric.float64().kind();
    case protobuf::Metric::kFloat32:
        return metric.float32().kind();
    case protobuf::Metric::kBoolean:
        return metric.boolean().kind();
    case protobuf::Metric::kEnum8:
    default:
        return std::nullopt;
    }
}

metrics::metrics(metrics&& other) noexcept :
    m_data(std::move(other.m_data)), m_metadata(std::move(other.m_metadata)),
    m_hash(other.m_hash), m_metadata_bytes(other.m_metadata_bytes),
    m_value_bytes(other.m_value_bytes),
    m_initialized(std::move(other.m_initialized))
{
    other.m_data = std::vector<uint8_t>();
    other.m_metadata = protobuf::MetricsMetadata();
    other.m_hash = 0;
    other.m_metadata_bytes = 0;
    other.m_value_bytes = 0;
    other.m_initialized.clear();
}

metrics::metrics(const std::map<name, abacus::info>& info)
{
    m_metadata = protobuf::MetricsMetadata();
    m_metadata.set_protocol_version(protocol_version());
    m_metadata.set_endianness(endian::is_big_endian()
                                  ? protobuf::Endianness::BIG
                                  : protobuf::Endianness::LITTLE);

    // The first byte is reserved for the sync value
    m_value_bytes = sizeof(uint32_t);

    for (auto [name, value] : info)
    {
        protobuf::Metric metric;
        metric.set_offset(m_value_bytes);

        // The offset is incremented by one byte which represents whether
        // the metric is set or not.
        m_value_bytes += 1;

        if (auto* m = std::get_if<uint64>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();
            metric.set_optional(is_optional(m->availability));

            metric.mutable_uint64()->set_description(m->description.value);
            metric.mutable_uint64()->set_kind(to_protobuf(m->kind));
            if (!m->unit.empty())
            {
                metric.mutable_uint64()->set_unit(m->unit.value);
            }
            if (m->min.value.has_value())
            {
                metric.mutable_uint64()->set_min(m->min.value.value());
            }
            if (m->max.value.has_value())
            {
                metric.mutable_uint64()->set_max(m->max.value.value());
            }
        }
        else if (auto* m = std::get_if<int64>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();
            metric.set_optional(is_optional(m->availability));

            metric.mutable_int64()->set_description(m->description.value);
            metric.mutable_int64()->set_kind(to_protobuf(m->kind));
            if (!m->unit.empty())
            {
                metric.mutable_int64()->set_unit(m->unit.value);
            }
            if (m->min.value.has_value())
            {
                metric.mutable_int64()->set_min(m->min.value.value());
            }
            if (m->max.value.has_value())
            {
                metric.mutable_int64()->set_max(m->max.value.value());
            }
        }
        else if (auto* m = std::get_if<uint32>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();
            metric.set_optional(is_optional(m->availability));

            metric.mutable_uint32()->set_description(m->description.value);
            metric.mutable_uint32()->set_kind(to_protobuf(m->kind));
            if (!m->unit.empty())
            {
                metric.mutable_uint32()->set_unit(m->unit.value);
            }
            if (m->min.value.has_value())
            {
                metric.mutable_uint32()->set_min(m->min.value.value());
            }
            if (m->max.value.has_value())
            {
                metric.mutable_uint32()->set_max(m->max.value.value());
            }
        }
        else if (auto* m = std::get_if<int32>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();
            metric.set_optional(is_optional(m->availability));

            metric.mutable_int32()->set_description(m->description.value);
            metric.mutable_int32()->set_kind(to_protobuf(m->kind));
            if (!m->unit.empty())
            {
                metric.mutable_int32()->set_unit(m->unit.value);
            }
            if (m->min.value.has_value())
            {
                metric.mutable_int32()->set_min(m->min.value.value());
            }
            if (m->max.value.has_value())
            {
                metric.mutable_int32()->set_max(m->max.value.value());
            }
        }
        else if (auto* m = std::get_if<float64>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();
            metric.set_optional(is_optional(m->availability));

            metric.mutable_float64()->set_description(m->description.value);
            metric.mutable_float64()->set_kind(to_protobuf(m->kind));
            if (!m->unit.empty())
            {
                metric.mutable_float64()->set_unit(m->unit.value);
            }
            if (m->min.value.has_value())
            {
                metric.mutable_float64()->set_min(m->min.value.value());
            }
            if (m->max.value.has_value())
            {
                metric.mutable_float64()->set_max(m->max.value.value());
            }
        }
        else if (auto* m = std::get_if<float32>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();
            metric.set_optional(is_optional(m->availability));

            metric.mutable_float32()->set_description(m->description.value);
            metric.mutable_float32()->set_kind(to_protobuf(m->kind));
            if (!m->unit.empty())
            {
                metric.mutable_float32()->set_unit(m->unit.value);
            }
            if (m->min.value.has_value())
            {
                metric.mutable_float32()->set_min(m->min.value.value());
            }
            if (m->max.value.has_value())
            {
                metric.mutable_float32()->set_max(m->max.value.value());
            }
        }
        else if (auto* m = std::get_if<boolean>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();
            metric.set_optional(is_optional(m->availability));

            metric.mutable_boolean()->set_description(m->description.value);
            metric.mutable_boolean()->set_kind(to_protobuf(m->kind));
        }
        else if (auto* m = std::get_if<enum8>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();
            metric.set_optional(is_optional(m->availability));

            metric.mutable_enum8()->set_description(m->description.value);
            for (auto [key, value] : m->values)
            {
                auto enum_value = protobuf::EnumValue();
                enum_value.set_name(value.name);
                if (!value.description.empty())
                {
                    enum_value.set_description(value.description);
                }

                metric.mutable_enum8()->mutable_values()->insert(
                    {key, enum_value});
            }
        }

        m_metadata.mutable_metrics()->insert({name.value, metric});
        m_initialized[name.value] = false;
    }

    // Set the sync value to 1 so that the size of the metadata is
    // calculated correctly
    m_metadata.set_sync_value(1);

    m_metadata_bytes = m_metadata.ByteSizeLong();
    m_data.reserve(m_metadata_bytes + m_value_bytes);
    m_data.resize(m_metadata_bytes);

    // Serialize the metadata
    m_metadata.SerializeToArray(m_data.data(), m_data.size());

    // Calculate the hash of the metadata
    m_hash = detail::hash_function(m_data.data(), m_data.size());

    // Update the sync value
    m_metadata.set_sync_value(m_hash);

    // Serialize the metadata again to include the sync value
    m_metadata.SerializeToArray(m_data.data(), m_data.size());

    // Make sure the metadata didn't change unexpectedly
    assert(m_metadata.ByteSizeLong() == m_metadata_bytes);

    // Write the sync value to the first byte of the value data (this will
    // be written as the endianess of the system)
    // Consuming code can use the endianness field in the metadata to
    // read the sync value
    m_data.resize(m_metadata_bytes + m_value_bytes);
    std::memcpy(m_data.data() + m_metadata_bytes, &m_hash, sizeof(uint32_t));
}

template <class Metric>
[[nodiscard]] auto
metrics::initialize_optional(const std::string& name,
                             std::optional<typename Metric::type> value) ->
    typename Metric::optional
{
    assert(m_initialized.find(name) != m_initialized.end());
    assert(!m_initialized.at(name));
    m_initialized[name] = true;
    const protobuf::Metric& proto_metric = m_metadata.metrics().at(name);
    assert(proto_metric.optional() == true);
    auto kind = get_kind(proto_metric);
    if (kind.has_value())
    {
        assert(kind.value() != protobuf::Kind::CONSTANT);
    }

    auto offset = proto_metric.offset();

    if (value.has_value())
    {
        m_initial_values[name] = value.value();
        return typename Metric::optional(
            m_data.data() + m_metadata_bytes + offset, value.value());
    }
    else
    {
        return typename Metric::optional(m_data.data() + m_metadata_bytes +
                                         offset);
    }
}

// Explicit instantiations for the expected types
template auto metrics::initialize_optional<uint64>(
    const std::string& name,
    std::optional<uint64::type> value) -> uint64::optional;
template auto metrics::initialize_optional<int64>(
    const std::string& name,
    std::optional<int64::type> value) -> int64::optional;
template auto metrics::initialize_optional<uint32>(
    const std::string& name,
    std::optional<uint32::type> value) -> uint32::optional;
template auto metrics::initialize_optional<int32>(
    const std::string& name,
    std::optional<int32::type> value) -> int32::optional;
template auto metrics::initialize_optional<float64>(
    const std::string& name,
    std::optional<float64::type> value) -> float64::optional;
template auto metrics::initialize_optional<float32>(
    const std::string& name,
    std::optional<float32::type> value) -> float32::optional;
template auto metrics::initialize_optional<boolean>(
    const std::string& name,
    std::optional<boolean::type> value) -> boolean::optional;
template auto metrics::initialize_optional<enum8>(
    const std::string& name,
    std::optional<enum8::type> value) -> enum8::optional;

template <class Metric>
[[nodiscard]] auto metrics::initialize_required(const std::string& name,
                                                typename Metric::type value) ->
    typename Metric::required
{
    assert(m_initialized.find(name) != m_initialized.end());
    assert(!m_initialized.at(name));
    m_initialized[name] = true;
    const protobuf::Metric& proto_metric = m_metadata.metrics().at(name);
    assert(proto_metric.optional() == false);
    auto kind = get_kind(proto_metric);
    if (kind.has_value())
    {
        assert(kind.value() != protobuf::Kind::CONSTANT);
    }

    auto offset = proto_metric.offset();

    m_initial_values[name] = value;
    return typename Metric::required(m_data.data() + m_metadata_bytes + offset,
                                     value);
}

// Explicit instantiations for the expected types
template auto
metrics::initialize_required<uint64>(const std::string& name,
                                     uint64::type value) -> uint64::required;
template auto
metrics::initialize_required<int64>(const std::string& name,
                                    int64::type value) -> int64::required;
template auto
metrics::initialize_required<uint32>(const std::string& name,
                                     uint32::type value) -> uint32::required;
template auto
metrics::initialize_required<int32>(const std::string& name,
                                    int32::type value) -> int32::required;
template auto
metrics::initialize_required<float64>(const std::string& name,
                                      float64::type value) -> float64::required;
template auto
metrics::initialize_required<float32>(const std::string& name,
                                      float32::type value) -> float32::required;
template auto
metrics::initialize_required<boolean>(const std::string& name,
                                      boolean::type value) -> boolean::required;
template auto
metrics::initialize_required<enum8>(const std::string& name,
                                    enum8::type value) -> enum8::required;

template <class Metric>
void metrics::initialize_constant(const std::string& name,
                                  typename Metric::type value)
{
    assert(m_initialized.find(name) != m_initialized.end());
    assert(!m_initialized.at(name));
    m_initialized[name] = true;

    const protobuf::Metric& proto_metric = m_metadata.metrics().at(name);
    assert(proto_metric.optional() == false &&
           "Constant metrics cannot be optional");
    auto offset = proto_metric.offset();
    auto kind = get_kind(proto_metric);
    assert(kind.has_value() && kind.value() == protobuf::Kind::CONSTANT);

    m_initial_values[name] = value;
    typename Metric::required(m_data.data() + m_metadata_bytes + offset, value);
}

// Explicit instantiations for the expected types
template void metrics::initialize_constant<uint64>(const std::string& name,
                                                   uint64::type value);
template void metrics::initialize_constant<int64>(const std::string& name,
                                                  int64::type value);
template void metrics::initialize_constant<uint32>(const std::string& name,
                                                   uint32::type value);
template void metrics::initialize_constant<int32>(const std::string& name,
                                                  int32::type value);
template void metrics::initialize_constant<float64>(const std::string& name,
                                                    float64::type value);
template void metrics::initialize_constant<float32>(const std::string& name,
                                                    float32::type value);
template void metrics::initialize_constant<boolean>(const std::string& name,
                                                    boolean::type value);
template void metrics::initialize_constant<enum8>(const std::string& name,
                                                  enum8::type value);

metrics::~metrics()
{
}

auto metrics::value_data() const -> const uint8_t*
{
    return m_data.data() + m_metadata_bytes;
}

auto metrics::value_bytes() const -> std::size_t
{
    return m_value_bytes;
}

auto metrics::metadata_data() const -> const uint8_t*
{
    return m_data.data();
}

auto metrics::metadata_bytes() const -> std::size_t
{
    return m_metadata_bytes;
}

auto metrics::metadata() const -> const protobuf::MetricsMetadata&
{
    return m_metadata;
}

auto metrics::is_initialized(const std::string& name) const -> bool
{
    assert(m_initialized.find(name) != m_initialized.end());
    return m_initialized.at(name);
}

auto metrics::is_initialized() const -> bool
{
    for (auto [name, initialized] : m_initialized)
    {
        if (!initialized)
        {
            return false;
        }
    }
    return true;
}
auto metrics::reset() -> void
{
    for (auto [name, value] : m_initial_values)
    {
        const protobuf::Metric& metric = m_metadata.metrics().at(name);
        auto kind = get_kind(metric);
        if (!kind.has_value() || kind.value() == protobuf::Kind::CONSTANT)
        {
            continue;
        }

        auto offset = metric.offset();
        if (metric.has_boolean())
        {
            auto v = std::any_cast<boolean::type>(value);
        }
        else if (metric.has_enum8())
        {
            auto v = std::any_cast<enum8::type>(value);
        }
        else if (metric.has_float32())
        {
            auto v = std::any_cast<float32::type>(value);
        }
        else if (metric.has_float64())
        {
            auto v = std::any_cast<float64::type>(value);
        }
        else if (metric.has_int32())
        {
            auto v = std::any_cast<int32::type>(value);
        }
        else if (metric.has_int64())
        {
            auto v = std::any_cast<int64::type>(value);
        }
        else if (metric.has_uint32())
        {
            auto v = std::any_cast<uint32::type>(value);
        }
        else if (metric.has_uint64())
        {
            auto v = std::any_cast<uint64::type>(value);
        }
    }
}

}
}
