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

#include "detail/helpers.hpp"
#include "protobuf/metrics.pb.h"

#include <endian/is_big_endian.hpp>

#include <iostream>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

static inline auto is_optional(const protobuf::Metric& metric) -> bool
{
    return detail::call_type(
        metric,
        [](const auto& metric)
        {
            using Metric = std::decay_t<decltype(metric)>;
            if constexpr (detail::has_kGauge_v<typename Metric::KindCase>)
            {
                if (metric.kind_case() == Metric::KindCase::kGauge)
                {
                    return metric.gauge().optional();
                }
            }
            if constexpr (detail::has_kCounter_v<typename Metric::KindCase>)

            {
                if (metric.kind_case() == Metric::KindCase::kCounter)
                {
                    return metric.counter().optional();
                }
            }

            return false;
        });
}

static inline bool is_constant(const protobuf::Metric& metric)
{
    return detail::call_type(
        metric,
        [](const auto& metric)
        {
            using Metric = std::decay_t<decltype(metric)>;

            if constexpr (detail::has_kConstant_v<typename Metric::KindCase>)
            {
                return metric.kind_case() == Metric::KindCase::kConstant;
            }
            return false;
        });
}

template <class Protobuf, class Kind>
static inline auto set_kind(Protobuf& protobuf, const Kind& kind)
{
    // Check if the Protobuf message has a function called mutable_gauge
    if constexpr (detail::has_mutable_gauge_v<Protobuf> &&
                  detail::is_in_variant_v<gauge, Kind>)
    {
        if (auto* g = std::get_if<gauge>(&kind))
        {
            protobuf.mutable_gauge()->set_optional(
                std::holds_alternative<optional_>(g->availability));
            return;
        }
    }
    // Check if the Protobuf message has a function called mutable_counter
    if constexpr (detail::has_mutable_counter_v<Protobuf> &&
                  detail::is_in_variant_v<counter, Kind>)
    {
        if (auto* c = std::get_if<counter>(&kind))
        {
            protobuf.mutable_counter()->set_optional(
                std::holds_alternative<optional_>(c->availability));
            return;
        }
    }

    if constexpr (detail::has_mutable_constant_v<Protobuf> &&
                  detail::is_in_variant_v<constant, Kind>)
    {
        if (auto* c = std::get_if<constant>(&kind))
        {

            protobuf.mutable_constant();
            return;
        }
    }
}

metrics::metrics(metrics&& other) noexcept :
    m_metadata(std::move(other.m_metadata)), m_data(std::move(other.m_data)),
    m_metadata_bytes(other.m_metadata_bytes), m_hash(other.m_hash),
    m_value_bytes(other.m_value_bytes),
    m_initialized(std::move(other.m_initialized))
{
    other.m_metadata = protobuf::MetricsMetadata();
    other.m_data.clear();
    other.m_metadata_bytes = 0;
    other.m_hash = 0;
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

        // The offset is incremented by one byte which represents
        // whether the metric is set or not.
        m_value_bytes += 1;

        if (auto* m = std::get_if<uint64>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();
            metric.mutable_uint64()->set_description(m->description.value);

            set_kind(*metric.mutable_uint64(), m->kind);

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
            metric.mutable_int64()->set_description(m->description.value);

            set_kind(*metric.mutable_int64(), m->kind);

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
            metric.mutable_uint32()->set_description(m->description.value);

            set_kind(*metric.mutable_uint32(), m->kind);

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
            metric.mutable_int32()->set_description(m->description.value);

            set_kind(*metric.mutable_int32(), m->kind);

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

            metric.mutable_float64()->set_description(m->description.value);

            set_kind(*metric.mutable_float64(), m->kind);

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

            metric.mutable_float32()->set_description(m->description.value);

            set_kind(*metric.mutable_float32(), m->kind);

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
            metric.mutable_boolean()->set_description(m->description.value);

            set_kind(*metric.mutable_boolean(), m->kind);
        }
        else if (auto* m = std::get_if<enum8>(&value))
        {
            // The offset is incremented by the size of the type
            m_value_bytes += detail::size_of_type<decltype(m)>();

            metric.mutable_enum8()->set_description(m->description.value);

            set_kind(*metric.mutable_enum8(), m->kind);

            for (auto [key, value] : m->values)
            {
                auto enum_value = protobuf::Enum8Metric::EnumValue();
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

    m_metadata_bytes = metadata().ByteSizeLong();

    m_data.resize(m_metadata_bytes + m_value_bytes);

    // Serialize the metadata
    metadata().SerializeToArray(m_data.data(), m_metadata_bytes);

    // Calculate the hash of the metadata
    m_hash = detail::hash_function(m_data.data(), m_metadata_bytes);

    // Update the sync value
    m_metadata.set_sync_value(m_hash);

    // Serialize the metadata again to include the sync value
    metadata().SerializeToArray(m_data.data(), m_metadata_bytes);

    // Make sure the metadata didn't change unexpectedly
    assert(metadata().ByteSizeLong() == m_metadata_bytes);

    // Write the sync value to the first byte of the value data (this
    // will be written as the endianess of the system) Consuming code
    // can use the endianness field in the metadata to read the sync
    // value
    std::memcpy(value_data(0), &m_hash, sizeof(uint32_t));
}

template <class Metric>
[[nodiscard]] auto
metrics::initialize_optional(const std::string& name) -> optional_metric<Metric>
{
    assert(m_initialized.find(name) != m_initialized.end());
    assert(!m_initialized.at(name));
    m_initialized[name] = true;
    const protobuf::Metric& proto_metric = metadata().metrics().at(name);
    assert(is_optional(proto_metric));
    // assert(get_kind(proto_metric) != protobuf::Kind::CONSTANT);

    auto offset = proto_metric.offset();

    return {value_data(offset)};
}

// Explicit instantiations for the expected types
template auto metrics::initialize_optional<uint64>(const std::string& name)
    -> optional_metric<uint64>;
template auto metrics::initialize_optional<int64>(const std::string& name)
    -> optional_metric<int64>;
template auto metrics::initialize_optional<uint32>(const std::string& name)
    -> optional_metric<uint32>;
template auto metrics::initialize_optional<int32>(const std::string& name)
    -> optional_metric<int32>;
template auto metrics::initialize_optional<float64>(const std::string& name)
    -> optional_metric<float64>;
template auto metrics::initialize_optional<float32>(const std::string& name)
    -> optional_metric<float32>;
template auto metrics::initialize_optional<boolean>(const std::string& name)
    -> optional_metric<boolean>;
template auto metrics::initialize_optional<enum8>(const std::string& name)
    -> optional_metric<enum8>;

template <class Metric>
[[nodiscard]] auto metrics::initialize_required(const std::string& name,
                                                typename Metric::type value)
    -> required_metric<Metric>
{
    assert(m_initialized.find(name) != m_initialized.end());
    assert(!m_initialized.at(name));
    m_initialized[name] = true;
    const protobuf::Metric& proto_metric = metadata().metrics().at(name);
    assert(!is_optional(proto_metric));
    // assert(get_kind(proto_metric) != protobuf::Kind::CONSTANT);

    auto offset = proto_metric.offset();

    m_initial_values[name] = value;
    return {value_data(offset), value};
}

// Explicit instantiations for the expected types
template auto metrics::initialize_required<uint64>(
    const std::string& name, uint64::type value) -> required_metric<uint64>;
template auto metrics::initialize_required<int64>(
    const std::string& name, int64::type value) -> required_metric<int64>;
template auto metrics::initialize_required<uint32>(
    const std::string& name, uint32::type value) -> required_metric<uint32>;
template auto metrics::initialize_required<int32>(
    const std::string& name, int32::type value) -> required_metric<int32>;
template auto metrics::initialize_required<float64>(
    const std::string& name, float64::type value) -> required_metric<float64>;
template auto metrics::initialize_required<float32>(
    const std::string& name, float32::type value) -> required_metric<float32>;
template auto metrics::initialize_required<boolean>(
    const std::string& name, boolean::type value) -> required_metric<boolean>;
template auto metrics::initialize_required<enum8>(
    const std::string& name, enum8::type value) -> required_metric<enum8>;

template <class Metric>
void metrics::initialize_constant(const std::string& name,
                                  typename Metric::type value)
{
    assert(m_initialized.find(name) != m_initialized.end());
    assert(!m_initialized.at(name));
    m_initialized[name] = true;

    const protobuf::Metric& proto_metric = metadata().metrics().at(name);
    auto offset = proto_metric.offset();
    // assert(get_kind(proto_metric) == protobuf::Kind::CONSTANT);

    required_metric<Metric>(value_data(offset), value);
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

auto metrics::value_data(std::size_t offset) -> uint8_t*
{
    return m_data.data() + offset + m_metadata_bytes;
}

auto metrics::value_bytes() const -> std::size_t
{
    return m_value_bytes;
}

auto metrics::metadata() const -> const protobuf::MetricsMetadata&
{
    return m_metadata;
}

auto metrics::metadata_data() const -> const uint8_t*
{
    return m_data.data();
}

auto metrics::metadata_bytes() const -> std::size_t
{
    return m_metadata.ByteSizeLong();
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
    for (const auto& [name, metric] : metadata().metrics())
    {
        if (!is_initialized(name))
        {
            // Skip metrics that have not been initialized
            continue;
        }

        if (is_constant(metric))
        {
            // Skip constant metrics
            continue;
        }

        auto offset = metric.offset();
        if (is_optional(metric))
        {
            // Set the has value byte to 0
            value_data(offset)[0] = 0;
        }
        else
        {
            // Set the value to what it was initialized to
            if (metric.has_uint64())
            {
                uint64::set_value(
                    value_data(offset),
                    std::any_cast<uint64::type>(m_initial_values.at(name)));
            }
            else if (metric.has_int64())
            {
                int64::set_value(
                    value_data(offset),
                    std::any_cast<int64::type>(m_initial_values.at(name)));
            }
            else if (metric.has_uint32())
            {
                uint32::set_value(
                    value_data(offset),
                    std::any_cast<uint32::type>(m_initial_values.at(name)));
            }
            else if (metric.has_int32())
            {
                int32::set_value(
                    value_data(offset),
                    std::any_cast<int32::type>(m_initial_values.at(name)));
            }
            else if (metric.has_float64())
            {
                float64::set_value(
                    value_data(offset),
                    std::any_cast<float64::type>(m_initial_values.at(name)));
            }
            else if (metric.has_float32())
            {
                float32::set_value(
                    value_data(offset),
                    std::any_cast<float32::type>(m_initial_values.at(name)));
            }
            else if (metric.has_boolean())
            {
                boolean::set_value(
                    value_data(offset),
                    std::any_cast<boolean::type>(m_initial_values.at(name)));
            }
            else if (metric.has_enum8())
            {
                enum8::set_value(
                    value_data(offset),
                    std::any_cast<enum8::type>(m_initial_values.at(name)));
            }
        }
    }
}
}
}
