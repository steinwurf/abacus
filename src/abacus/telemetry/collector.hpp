// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "data_points.hpp"
#include "exporter.hpp"

#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace abacus
{
namespace telemetry
{
/// @brief The Abacus Telemetry collector
/// The collector purpose is to collect metrics as data points in time series
/// from Abacus instances by creating new data points in the collector with
/// `new_data_point(...)` and copy the metrics from a Abacus instance to that
/// new data point. For the collector to actually output anything one would need
/// to attach one or more exporters. The collected data points is not pushed to
/// the exporters before the collector buffer is flushed by calling `flush()`.
/// This may then make one realise that the purpose of the collector is to act
/// as an entry point and a buffer, or a sink, that then will flush the data out
/// into one or more exporters, or pipes. All the exporters will receive the
/// same data. When flushing the data the exporters is free to do whatever
/// processing they require and therefore will inevitable introduce further
/// computation time when calling `flush()`. So flushing the data could be moved
/// into its own thread to avoid affecting other more time sensitive
/// computation. Do notice that the collector is not thread-safe. For
/// concurrency use make sure to guard calls made to it's methods by some
/// locking mechanism like mutexes.
class collector
{
public:
    /// Default constructor
    collector() : m_data_points_count(0)
    {
    }

    /// Attach an exporter. The collector will keep an internal shared
    /// pointer to the instance.
    auto add_exporter(const std::shared_ptr<exporter>& exporter_in) -> void
    {
        m_exporters.emplace_back(exporter_in);
    }

    auto new_aggregate(const std::string& aggregate_string)
        -> const aggregate_ptr&
    {
        auto& agg = *new aggregate();

        size_t start, end = 0;
        while ((start = aggregate_string.find_first_not_of('|', end)) !=
               std::string::npos)
        {
            end = aggregate_string.find('|', start);
            agg.emplace_back(aggregate_string.substr(start, end - start));
        }

        m_aggregates.emplace_back(&agg);
        return m_aggregates.back();
    }

    /// Creates a new data point in the time series at the given time of the
    /// parameter `now` and allocates a block of memory of given parameter
    /// `size`. Internally the collector reuses already allocated memory for
    /// flushed data points. If the memory is sparse and the cached memory needs
    /// to be freed, please see `clear()` for further details.
    ///
    /// @param aggregate The aggregate for the new data point
    ///
    /// @param now The time of now, given in milliseconds, will be used for the
    /// new time series data point
    ///
    /// @param size The allocated memory for the metrics of the data point
    ///
    /// @return A pointer to allocated memory for the data point. To copy the
    /// Abacus metrics to this one could use a simple `memcpy(...)`
    auto new_data_point(const aggregate_ptr& agg, time_type now,
                        std::size_t size) -> uint8_t*
    {
        uint8_t* data;
        auto size_with_time = size + sizeof(decltype(now));

        if (m_data_points.size() > m_data_points_count)
        {
            // If there is any free data points we shall reuse them
            auto& data_point = m_data_points[m_data_points_count];
            data_point.metrics.resize(size_with_time);
            data = data_point.metrics.data();

            data_point.aggregate = agg;
        }
        else
        {
            // If there is no freed data points we'll need to create a new one
            m_data_points.emplace_back(agg, size_with_time);
            data = m_data_points.back().metrics.data();
        }

        // Copy time to front of data point
        memcpy(data, (uint8_t*)&now, sizeof(decltype(now)));

        m_data_points_count++;

        return data + sizeof(decltype(now));
    }

    /// Flushes all the collected data point since initialization, or the latest
    /// flush, to all attached exporters, each receiving all data points.
    auto flush() -> void
    {
        for (auto& exporter_in : m_exporters)
        {
            exporter_in->receive_data_points(
                m_data_points.begin(),
                m_data_points.begin() +
                    (data_points::difference_type)m_data_points_count);
        }
        m_data_points_count = 0;
    }

    /// Removes all collected data points and deallocates all cached memory
    /// blocks.
    ///
    /// The internal buffer of the collectors is optimized to reuse memory and
    /// therefore keeps the memory for freed data points to prevent allocating
    /// new memory for future data points. This may not be optimal for machines
    /// with limited resources. If that is your case we suggest that your clear
    /// the buffer after each flush.
    auto clear() -> void
    {
        m_data_points.resize(0);
    }

private:
    std::vector<std::shared_ptr<exporter>> m_exporters;
    std::vector<aggregate_ptr> m_aggregates;
    std::size_t m_data_points_count;
    data_points m_data_points;
};
}
}