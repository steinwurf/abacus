// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "data_points.hpp"
#include "exporter.hpp"

#include <functional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace abacus
{
namespace time_series
{
/// @brief The collector is used for collecting abacus metrics into to time
/// series. The collector acts as a sink that timestamps every data point. To
/// make the data points available for consumption one needs to flush the
/// collected data points, when the collector has been flushed, the collector
/// reuses the allocated memory for the previous flushed data points. The
/// collector is not thread-safe, so flushing in a different thread than the one
/// that is collecting will result in undefined behaviour, if not the ownership
/// of the data points is moved to thread that is flushing. See the example for
/// the `flush(...)` method for how to use the `data_points_queue` to do lock
/// free concurrency.
class collector
{
public:
    /// Default constructor
    collector() : m_data_points_sink_count(0)
    {
    }

    /// Creates a new time series aggregate for the collector.
    /// @return A pointer for the created aggregate.
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

        if (m_data_points_sink.size() > m_data_points_sink_count)
        {
            // If there is any free data points we shall reuse them
            auto& data_point = m_data_points_sink[m_data_points_sink_count];
            data_point.metrics.resize(size_with_time);
            data = data_point.metrics.data();

            data_point.aggregate = agg;
        }
        else
        {
            // If there is no freed data points we'll need to create a new one
            m_data_points_sink.emplace_back(agg, size_with_time);
            data = m_data_points_sink.back().metrics.data();
        }

        // Copy time to front of data point
        memcpy(data, (uint8_t*)&now, sizeof(decltype(now)));

        m_data_points_sink_count++;

        return data + sizeof(decltype(now));
    }

    /// Flushes the collected data points to the given callback method.
    /// @param callback Use this function to process the data points
    ///
    /// Example of usage with the look free queue `data_point_queue`:
    ///
    ///     data_point_queue queue;
    ///     collector.flush([&queue](auto begin, auto end) {
    ///         queue.push(begin, end);
    ///     });
    auto flush(const std::function<void(data_points::iterator begin,
                                        data_points::iterator end)>& callback)
        -> void
    {
        callback(m_data_points_sink.begin(),
                 m_data_points_sink.begin() +
                     (data_points::difference_type)m_data_points_sink_count);

        m_data_points_sink.clear();
        m_data_points_sink_count = 0;
    }

    /// Flushes all collected data points to a set of exporters. The exporters
    /// may process the data points introducing additional workload to the
    /// current thread. To avoid this use `flush(...)`, and you may then use the
    /// `data_point_queue` which is a lock free queue that can flush to
    /// exporters in a separate thread that then moves the export processing
    /// workload to that thread instead.
    auto flush_to_exporters(const exporters& in_exporters) -> void
    {
        for (auto& in_exporter : in_exporters)
        {
            in_exporter->receive_data_points(
                m_data_points_sink.begin(),
                m_data_points_sink.begin() +
                    (data_points::difference_type)m_data_points_sink_count);
        }
        m_data_points_sink.clear();
        m_data_points_sink_count = 0;
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
        m_data_points_sink.resize(0);
    }

private:
    std::vector<aggregate_ptr> m_aggregates;
    std::size_t m_data_points_sink_count;
    data_points m_data_points_sink;
};
}
}