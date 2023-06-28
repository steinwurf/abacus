// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cassert>
#include <cstdint>
#include <cstring>

#include "detail/raw.hpp"
#include "kind.hpp"
#include "max.hpp"
#include "metric_info.hpp"
#include "metrics.hpp"
#include "min.hpp"
#include "protocol_version.hpp"
#include "type.hpp"

#include <endian/is_big_endian.hpp>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

metrics::metrics() : metrics(nullptr, 0)
{
}

metrics::metrics(const metric_info* info, std::size_t count) :
    m_info(info, count)
{
    assert(count <= std::numeric_limits<uint16_t>::max());
    assert(count == 0 || info != nullptr);

    std::size_t name_bytes = 0;
    std::size_t description_bytes = 0;
    std::size_t unit_bytes = 0;

    // First calculate the total size of header
    std::size_t meta_bytes = detail::header_bytes();
    for (std::size_t i = 0; i < m_info.count(); i++)
    {
        const auto& info = m_info[i];
        m_name_to_index.emplace(info.name, i);
        name_bytes += info.name.size();
        description_bytes += info.description.size();
        unit_bytes += info.unit.value.size();

        // name_size
        meta_bytes += sizeof(uint16_t);
        // description size
        meta_bytes += sizeof(uint16_t);
        // unit size
        meta_bytes += sizeof(uint16_t);
        // name
        meta_bytes += info.name.size();
        // description
        meta_bytes += info.description.size();
        // unit
        meta_bytes += info.unit.value.size();
        // type
        meta_bytes += sizeof(uint8_t);
        // kind
        meta_bytes += sizeof(uint8_t);
        // min
        meta_bytes += sizeof(uint64_t);
        // max
        meta_bytes += sizeof(uint64_t);
    }
    // Add padding to ensure alignment for the values.
    std::size_t alignment = detail::alignment_padding(meta_bytes);

    std::size_t value_bytes = 0;
    // Add the bytes needed for the values.
    value_bytes +=
        8 * m_info.eight_byte_metrics_count() + m_info.one_byte_metrics_count();

    // Finally add the bytes needed for the initalized bitmap
    value_bytes += (m_info.count() + 7) / 8;

    std::size_t storage_bytes = meta_bytes + alignment + value_bytes;

    // Allocate the memory needed.
    m_meta_data = static_cast<uint8_t*>(::operator new(storage_bytes));

    // Make sure that the data is 8-byte-aligned
    assert(reinterpret_cast<uint64_t>(m_meta_data) % 8U == 0U);

    // Zero out all memory
    std::memset(m_meta_data, 0, storage_bytes);

    // Write the header
    // The endianness of the data, 0 for little-endian, 1 for big-endian
    new (m_meta_data) uint8_t(endian::is_big_endian());
    // The version of the data protocol
    new (m_meta_data + 1) uint8_t(abacus::protocol_version());

    // The total bytes used for names
    assert(name_bytes <= std::numeric_limits<uint16_t>::max());
    new (m_meta_data + 2) uint16_t((uint16_t)name_bytes);
    // The total bytes used for descriptions
    assert(description_bytes <= std::numeric_limits<uint16_t>::max());
    new (m_meta_data + 4) uint16_t((uint16_t)description_bytes);
    // The total bytes used for units
    assert(unit_bytes <= std::numeric_limits<uint16_t>::max());
    new (m_meta_data + 6) uint16_t((uint16_t)unit_bytes);
    // The number of 8-byte metric values (uint64_t, int64_t and double types)
    new (m_meta_data + 8) uint16_t((uint16_t)m_info.eight_byte_metrics_count());
    // The number of 1-byte metric values (bool type)
    new (m_meta_data + 10) uint16_t((uint16_t)m_info.one_byte_metrics_count());

    // Write the name sizes into memory
    uint8_t* name_sizes_ptr = m_meta_data + detail::name_sizes_offset();

    // Write the description sizes into memory
    uint8_t* description_sizes_ptr =
        m_meta_data + detail::description_sizes_offset(m_meta_data);

    // Write the unit sizes into memory
    uint8_t* unit_sizes_ptr =
        m_meta_data + detail::unit_sizes_offset(m_meta_data);

    // Write the names into memory
    uint8_t* names_ptr = m_meta_data + detail::names_offset(m_meta_data);

    // Write the descriptions into memory
    uint8_t* descriptions_ptr =
        m_meta_data + detail::descriptions_offset(m_meta_data);

    // Write the units into memory
    uint8_t* units_ptr = m_meta_data + detail::units_offset(m_meta_data);

    // Write the types into memory
    uint8_t* types_ptr = m_meta_data + detail::types_offset(m_meta_data);

    // Write the kind into memory
    uint8_t* kind_ptr = m_meta_data + detail::kind_offset(m_meta_data);

    // Write the min into memory
    uint8_t* min_ptr = m_meta_data + detail::min_offset(m_meta_data);

    // Write the max into memory
    uint8_t* max_ptr = m_meta_data + detail::max_offset(m_meta_data);

    for (std::size_t i = 0; i < m_info.count(); i++)
    {
        const auto& info = m_info[i];
        uint16_t name_size = static_cast<uint16_t>(info.name.size());
        uint16_t description_size =
            static_cast<uint16_t>(info.description.size());
        uint16_t unit_size = static_cast<uint16_t>(info.unit.value.size());

        std::memcpy(name_sizes_ptr, &name_size, sizeof(name_size));
        name_sizes_ptr += sizeof(name_size);

        std::memcpy(description_sizes_ptr, &description_size,
                    sizeof(description_size));
        description_sizes_ptr += sizeof(description_size);

        std::memcpy(unit_sizes_ptr, &unit_size, sizeof(unit_size));
        unit_sizes_ptr += sizeof(unit_size);

        std::memcpy(names_ptr, info.name.data(), info.name.size());
        names_ptr += info.name.size();

        std::memcpy(descriptions_ptr, info.description.data(),
                    info.description.size());
        descriptions_ptr += info.description.size();

        std::memcpy(units_ptr, info.unit.value.data(), info.unit.value.size());
        units_ptr += info.unit.value.size();

        std::memcpy(kind_ptr, &info.kind, sizeof(info.kind));
        kind_ptr += sizeof(info.kind);

        std::memcpy(types_ptr, &info.type, sizeof(info.type));
        types_ptr += sizeof(info.type);

        std::memcpy(min_ptr, &info.min, sizeof(info.min));
        min_ptr += sizeof(info.min);

        std::memcpy(max_ptr, &info.max, sizeof(info.max));
        max_ptr += sizeof(info.max);
    }

    m_value_data = m_meta_data + meta_bytes + alignment;

    assert(storage_bytes ==
           detail::meta_bytes(m_meta_data) +
               detail::alignment_padding(detail::meta_bytes(m_meta_data)) +
               detail::value_bytes(m_meta_data));
}

metrics::~metrics()
{
    ::operator delete(m_meta_data);
}

auto metrics::meta_data() const -> const uint8_t*
{
    return m_meta_data;
}

auto metrics::meta_bytes() const -> std::size_t
{
    return detail::meta_bytes(m_meta_data);
}

auto metrics::value_data() const -> const uint8_t*
{
    for (auto const& item : m_observer_map)
    {
        item.second();
    }
    return m_value_data;
}

auto metrics::value_bytes() const -> std::size_t
{
    return detail::value_bytes(m_meta_data);
}

auto metrics::count() const -> std::size_t
{
    return m_info.count();
}

auto metrics::protocol_version() const -> uint8_t
{
    return detail::protocol_version(m_meta_data);
}

auto metrics::is_initialized(std::size_t index) const -> bool
{
    assert(index < count());
    return detail::is_metric_initialized(m_meta_data, m_value_data, index);
}

auto metrics::name(std::size_t index) const -> std::string
{
    assert(index < count());
    return m_info[index].name;
}

auto metrics::description(std::size_t index) const -> std::string
{
    assert(index < count());
    return m_info[index].description;
}

auto metrics::unit(std::size_t index) const -> std::string
{
    assert(index < count());
    return m_info[index].unit.value;
}

auto metrics::is_boolean(std::size_t index) const -> bool
{
    assert(index < count());
    return m_info[index].type == abacus::type::boolean;
}

auto metrics::is_uint64(std::size_t index) const -> bool
{
    assert(index < count());
    return m_info[index].type == abacus::type::uint64;
}

auto metrics::is_int64(std::size_t index) const -> bool
{
    assert(index < count());
    return m_info[index].type == abacus::type::int64;
}

auto metrics::is_float64(std::size_t index) const -> bool
{
    assert(index < count());
    return m_info[index].type == abacus::type::float64;
}

auto metrics::kind(std::size_t index) const -> abacus::kind
{
    assert(index < count());
    return m_info[index].kind;
}

void metrics::observe_metric(const std::string& name,
                             delegate<uint64_t()> callback) const
{
    assert(m_observer_map.find(name) == m_observer_map.end());
    auto metric_index = index(name);
    assert(is_uint64(metric_index));
    auto value_ptr = static_cast<uint64_t*>(initialize(metric_index));
    std::function<void()> mem_callback(
        [callback, value_ptr]()
        {
            uint64_t value = callback();
            *value_ptr = value;
        });
    m_observer_map.emplace(name, mem_callback);
}

void metrics::observe_metric(const std::string& name,
                             delegate<int64_t()> callback) const
{
    assert(m_observer_map.find(name) == m_observer_map.end());
    auto metric_index = index(name);
    assert(is_int64(metric_index));
    auto value_ptr = static_cast<int64_t*>(initialize(metric_index));
    std::function<void()> mem_callback(
        [callback, value_ptr]()
        {
            int64_t value = callback();
            *value_ptr = value;
        });
    m_observer_map.emplace(name, mem_callback);
}

void metrics::observe_metric(const std::string& name,
                             delegate<double()> callback) const
{
    assert(m_observer_map.find(name) == m_observer_map.end());
    auto metric_index = index(name);
    assert(is_float64(metric_index));
    auto value_ptr = static_cast<double*>(initialize(metric_index));
    std::function<void()> mem_callback(
        [callback, value_ptr]()
        {
            double value = callback();
            *value_ptr = value;
        });
    m_observer_map.emplace(name, mem_callback);
}

void metrics::observe_metric(const std::string& name,
                             delegate<bool()> callback) const
{
    assert(m_observer_map.find(name) == m_observer_map.end());
    auto metric_index = index(name);
    assert(is_boolean(metric_index));
    auto value_ptr = static_cast<bool*>(initialize(metric_index));
    std::function<void()> mem_callback(
        [callback, value_ptr]()
        {
            bool value = callback();
            *value_ptr = value;
        });
    m_observer_map.emplace(name, mem_callback);
}

void metrics::initialize_constant(const std::string& name, uint64_t value) const
{
    auto index = metrics::index(name);
    assert(kind(index) == abacus::kind::constant);
    assert(is_uint64(index));
    *static_cast<uint64_t*>(initialize(index)) = value;
}

void metrics::initialize_constant(const std::string& name, int64_t value) const
{
    auto index = metrics::index(name);
    assert(kind(index) == abacus::kind::constant);
    assert(is_int64(index));
    *static_cast<int64_t*>(initialize(index)) = value;
}

void metrics::initialize_constant(const std::string& name, double value) const
{
    auto index = metrics::index(name);
    assert(kind(index) == abacus::kind::constant);
    assert(is_float64(index));
    *static_cast<double*>(initialize(index)) = value;
}

void metrics::initialize_constant(const std::string& name, bool value) const
{
    auto index = metrics::index(name);
    assert(kind(index) == abacus::kind::constant);
    assert(is_boolean(index));
    *static_cast<bool*>(initialize(index)) = value;
}

void metrics::value(std::size_t index, uint64_t& value) const
{
    assert(index < count());
    assert(is_initialized(index));
    assert(is_uint64(index));
    if (m_observer_map.find(m_info[index].name) != m_observer_map.end())
        m_observer_map[m_info[index].name]();
    value = *static_cast<uint64_t*>(
        detail::value_ptr(m_meta_data, m_value_data, index));
}

void metrics::value(std::size_t index, int64_t& value) const
{
    assert(index < count());
    assert(is_initialized(index));
    assert(is_int64(index));
    if (m_observer_map.find(m_info[index].name) != m_observer_map.end())
        m_observer_map[m_info[index].name]();
    value = *static_cast<int64_t*>(
        detail::value_ptr(m_meta_data, m_value_data, index));
}

void metrics::value(std::size_t index, double& value) const
{
    assert(index < count());
    assert(is_initialized(index));
    assert(is_float64(index));
    if (m_observer_map.find(m_info[index].name) != m_observer_map.end())
        m_observer_map[m_info[index].name]();
    value = *static_cast<double*>(
        detail::value_ptr(m_meta_data, m_value_data, index));
}

void metrics::value(std::size_t index, bool& value) const
{
    assert(index < count());
    assert(is_initialized(index));
    assert(is_boolean(index));
    if (m_observer_map.find(m_info[index].name) != m_observer_map.end())
        m_observer_map[m_info[index].name]();
    value = *static_cast<bool*>(
        detail::value_ptr(m_meta_data, m_value_data, index));
}

auto metrics::index(const std::string& name) const -> std::size_t
{
    auto it = m_name_to_index.find(name);
    assert(it != m_name_to_index.end() && "metric name not found.");
    return it->second;
}

void metrics::reset_metric(std::size_t index)
{
    assert(index < count());
    assert(is_initialized(index));
    assert(kind(index) != abacus::kind::constant);

    switch (detail::type(m_meta_data, index))
    {
    case abacus::type::boolean:
    {
        bool* value_data = static_cast<bool*>(
            detail::value_ptr(m_meta_data, m_value_data, index));
        *value_data = false;
        break;
    }
    case abacus::type::uint64:
    {
        uint64_t* value_data = static_cast<uint64_t*>(
            detail::value_ptr(m_meta_data, m_value_data, index));
        *value_data = 0U;
        break;
    }
    case abacus::type::int64:
    {
        int64_t* value_data = static_cast<int64_t*>(
            detail::value_ptr(m_meta_data, m_value_data, index));
        *value_data = 0;
        break;
    }
    case abacus::type::float64:
    {
        double* value_data = static_cast<double*>(
            detail::value_ptr(m_meta_data, m_value_data, index));
        *value_data = 0.0;
        break;
    }
    default:
        assert(false && "Invalid metric type.");
    }
}

void metrics::reset_metrics()
{
    memset(m_value_data, 0, detail::metrics_bytes(m_meta_data));
}

auto metrics::initialize(std::size_t index) const -> void*
{
    assert(index < count());
    assert(!is_initialized(index));
    // Write that the metric has been initialized into memory
    detail::set_metric_initialized(m_meta_data, m_value_data, index);
    assert(is_initialized(index));
    return detail::value_ptr(m_meta_data, m_value_data, index);
}
}
}
