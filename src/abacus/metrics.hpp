// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <vector>

#include "detail/raw.hpp"
#include "detail/value_size_info.hpp"
#include "metric.hpp"
#include "value_types.hpp"
#include "version.hpp"
#include "view.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
/// This class preallocates memory at construction to hold a header, a title
/// of size max_name_bytes, max_metrics metrics, each having a name of size
/// max_name_bytes, with a metric value of 8 bytes (sizeof(uint64_t)).
///
/// The total preallocated memory is
/// header_size + max_metrics * (max_name_bytes + 8).
///
/// To save memory, take caution in choosing the max_metrics. If your
/// library only uses 10 counters, you should probably not choose
/// max_metrics = 64.
///
/// The header consists of 42 bits of 3 values:
/// 1. 16 bit denoting the number of counters

class metrics
{

public:
    /// Constructor
    /// @param info The info needed to initialize all the metrics with types,
    /// names and descriptions
    metrics(std::vector<metric_info> info);

    /// Destructor
    ~metrics();

    /// @returns the number of metrics in the collection
    auto metric_count() const -> std::size_t;

    auto is_metric_initialized(std::size_t index) const -> bool;

    auto metric_name(std::size_t index) const -> std::string;

    auto metric_description(std::size_t index) const -> std::string;

    auto metric_type(std::size_t index) const -> value_type;

    auto metric_is_constant(std::size_t index) const -> bool;

    template <value_type T>
    auto initialize_metric(std::size_t index) const -> metric<T>
    {
        (void)index;
        throw std::runtime_error("Unknown metric type");
    }

private:
    /// No copy
    metrics(metrics&) = delete;

    /// No copy assignment
    metrics& operator=(metrics&) = delete;

    /// No move
    metrics(metrics&&) = delete;

    /// No move assignment
    metrics& operator=(metrics&&) = delete;

private:
    /// The info of the metrics seperated by byte-sizes
    detail::value_size_info m_info;

    /// The sizes of the names of the metrics
    std::vector<uint16_t> m_name_sizes;

    /// The sizes of the descriptions of the metrics
    std::vector<uint16_t> m_description_sizes;

    /// storage_size
    std::size_t m_storage_bytes = 0;

    /// The current number of metrics that have been initialized
    std::size_t m_count = 0;

    std::string m_scope = "";

    /// The raw memory for the counters (both value and name)
    uint8_t* m_data = nullptr;

    /// The scopes prepended to the metrics object
    std::vector<std::string> m_scopes;
};

template <>
inline auto metrics::initialize_metric<value_type::unsigned_integral>(
    std::size_t index) const -> metric<value_type::unsigned_integral>
{
    assert(index < m_count);
    assert(!is_metric_initialized(index));

    const char* name_ptr = detail::raw_name(m_data, index);
    std::memcpy(&name_ptr, m_info[index].name.c_str(), m_name_sizes[index]);

    auto description_ptr = detail::raw_description(m_data, index);
    std::memcpy(&description_ptr, m_info[index].description.c_str(),
                m_description_sizes[index]);

    uint64_t* value_ptr = detail::raw_value<uint64_t>(m_data, index);

    *value_ptr = 0U;

    return metric<value_type::unsigned_integral>{value_ptr};
}

template <>
inline auto
metrics::initialize_metric<value_type::signed_integral>(std::size_t index) const
    -> metric<value_type::signed_integral>
{
    assert(index < m_count);
    assert(!is_metric_initialized(index));

    auto name_ptr = detail::raw_name(m_data, index);
    std::memcpy(name_ptr, m_info[index].name.c_str(), m_name_sizes[index]);

    auto description_ptr = detail::raw_description(m_data, index);
    std::memcpy(description_ptr, m_info[index].description.c_str(),
                m_description_sizes[index]);

    int64_t* value_ptr = detail::raw_value<int64_t>(m_data, index);

    *value_ptr = 0;

    return metric<value_type::signed_integral>{value_ptr};
}

template <>
inline auto
metrics::initialize_metric<value_type::floating_point>(std::size_t index) const
    -> metric<value_type::floating_point>
{
    assert(index < m_count);
    assert(!is_metric_initialized(index));

    auto name_ptr = detail::raw_name(m_data, index);
    std::memcpy(name_ptr, m_info[index].name.c_str(), m_name_sizes[index]);

    auto description_ptr = detail::raw_description(m_data, index);
    std::memcpy(description_ptr, m_info[index].description.c_str(),
                m_description_sizes[index]);

    double* value_ptr = detail::raw_value<double>(m_data, index);

    *value_ptr = 0.0;

    return metric<value_type::floating_point>{value_ptr};
}

template <>
inline auto
metrics::initialize_metric<value_type::boolean>(std::size_t index) const
    -> metric<value_type::boolean>
{
    assert(index < m_count);
    assert(!is_metric_initialized(index));

    auto name_ptr = detail::raw_name(m_data, index);
    std::memcpy(name_ptr, m_info[index].name.c_str(), m_name_sizes[index]);

    auto description_ptr = detail::raw_description(m_data, index);
    std::memcpy(description_ptr, m_info[index].description.c_str(),
                m_description_sizes[index]);

    bool* value_ptr = detail::raw_value<bool>(m_data, index);

    *value_ptr = false;

    return metric<value_type::boolean>{value_ptr};
}

}
}
