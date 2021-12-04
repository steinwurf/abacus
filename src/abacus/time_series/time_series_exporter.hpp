// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "exporter.hpp"

namespace abacus
{
namespace time_series
{

struct time_series_exporter : public exporter
{
    struct time_index_record
    {
        friend time_series_exporter;

        explicit time_index_record(const time_type& time,
                                   std::size_t data_point_index) :
            m_time(time),
            m_data_point_index(data_point_index)
        {
        }

        time_type m_time;
        std::size_t m_data_point_index;
    };

    struct time_index : std::vector<time_index_record>
    {
        static const std::size_t NOT_FOUND;

        /// @return The position of the nearest record rounding up.
        auto find_near(const time_type& time) -> std::size_t
        {
            if (empty())
            {
                return NOT_FOUND;
            }

            if (time <= front().m_time)
            {
                return 0;
            }
            else if (time >= back().m_time)
            {
                return size() - 1;
            }

            std::size_t low = 0, high = size() - 1;
            std::size_t mid;

            while (low <= high)
            {
                mid = (high + low) / 2;

                if (time < this->operator[](mid).m_time)
                {
                    high = mid - 1;
                }
                else if (time > this->operator[](mid).m_time)
                {
                    low = mid + 1;
                }
                else
                {
                    return mid;
                }
            }
            return (this->operator[](low).m_time - time) <=
                           (time - this->operator[](high).m_time)
                       ? low
                       : high;
        }
    };

    /// @param capacity See description of method `set_capacity(...)` for
    /// explanation.
    explicit time_series_exporter(std::size_t capacity = 0) :
        m_time_index_records_capacity(capacity)
    {
    }

    auto
    receive_data_points(std::vector<data_point>::iterator data_points_start,
                        std::vector<data_point>::iterator data_points_end)
        -> void override
    {
        time_type previous_time = 0;
        if (m_time_index_records_capacity > 0)
        {
            // Count number of unique time points in given data points
            std::size_t unique_time_points = 0;
            previous_time = -1;
            for (auto itr = data_points_start; itr != data_points_end; itr++)
            {
                if (previous_time != itr->metrics.time())
                {
                    unique_time_points++;
                }
                previous_time = itr->metrics.time();
            }

            if (unique_time_points >= m_time_index_records_capacity)
            {
                // In the case that we're receiving more unique time points than
                // we have capacity for we simply just clear the index and data
                // points to make room for the new points.
                clear();

                if (unique_time_points != m_time_index_records_capacity)
                {
                    // Now we also need to limit the received points. We do this
                    // by starting from the back of the points and update the
                    // `data_points_start` iterator until we reach the time
                    // points' capacity.
                    unique_time_points = 0;
                    previous_time = -1;
                    for (auto itr = data_points_end - 1;
                         itr != data_points_start; itr--)
                    {
                        if (previous_time != itr->metrics.time())
                        {
                            // Only first check for capacity when we find the
                            // next unique time point. This makes sure we get
                            // all data points for the last possible time point.
                            if (unique_time_points ==
                                m_time_index_records_capacity)
                            {
                                data_points_start = itr + 1;
                                break;
                            }
                            unique_time_points++;
                        }
                        previous_time = itr->metrics.time();
                    }
                }
            }
            else
            {
                // In the case we are receiving less unique time points than the
                // capacity.
                if (m_time_index.size() + unique_time_points >
                    m_time_index_records_capacity)
                {
                    auto erase_time_index_records_count =
                        m_time_index.size() + unique_time_points -
                        m_time_index_records_capacity;

                    // Count number of data points exist for the unique time
                    // points being erased.
                    std::size_t erase_data_points_count = 0;
                    for (std::size_t i = 0; i < erase_time_index_records_count;
                         i++)
                    {
                        auto& record = m_time_index[i];
                        for (std::size_t j = record.m_data_point_index;
                             j < m_data_points.size(); j++)
                        {
                            if (m_data_points[j].metrics.time() !=
                                record.m_time)
                            {
                                break;
                            }

                            erase_data_points_count++;
                        }
                    }

                    // Erase the time index records for the discarded data
                    // points.
                    m_time_index.erase(m_time_index.begin(),
                                       m_time_index.begin() +
                                           (time_index::difference_type)
                                               erase_time_index_records_count);

                    // Erase the discarded data points.
                    m_data_points.erase(m_data_points.begin(),
                                        m_data_points.begin() +
                                            (data_points::difference_type)
                                                erase_data_points_count);

                    // Update all the time index records by offsetting their
                    // data point indices by the number of deleted data points.
                    for (auto& record : m_time_index)
                    {
                        record.m_data_point_index -= erase_data_points_count;
                    }
                }
            }
        }

        // If no record exist in the time index, we'll need to make sure that
        // one is present for the first data point as the next step requires the
        // time index not to be empty.
        if (m_time_index.empty() && data_points_start != data_points_end)
        {
            m_time_index.emplace_back(data_points_start->metrics.time(), 0);
            previous_time = data_points_start->metrics.time();
        }

        // Insert every data point into the internal data points container and
        // create a record in the time index for each unique time point.
        for (auto itr = data_points_start; itr != data_points_end; itr++)
        {
            assert(itr->metrics.time() >= previous_time);

            m_data_points.emplace_back(*itr);

            if (m_time_index.back().m_time != itr->metrics.time())
            {
                auto time = itr->metrics.time();
                m_time_index.emplace_back(time, m_data_points.size() - 1);
                previous_time = time;
            }
        }
    }

    /// @param from The range start time. The search is done with equal or
    /// greater than.
    /// @param to The range end time. The search is done with equal or less
    /// than.
    /// @return Collection of data points found in the given time range
    auto get(time_type from, time_type to) -> data_points
    {
        auto range = range_from_to(from, to);

        data_points filtered_points;
        std::copy(range.from, range.to, std::back_inserter(filtered_points));
        return filtered_points;
    }

    /// @param count The number of latest points based on the time, meaning that
    /// if the value is 10 and there is two data points per unique time point
    /// then a collection of 20 data points is returned.
    /// @return Collection of the n latest data points.
    auto get(std::size_t count) -> data_points
    {
        auto range = range_latest(count);

        data_points filtered_points;
        std::copy(range.from, range.to, std::back_inserter(filtered_points));
        return filtered_points;
    }

    /// @param from The range start time. The search is done with equal or
    /// greater than.
    /// @param to The range end time. The search is done with equal or less
    /// than.
    /// @return JSON string of an array holding the collection of data points
    /// for the given range.
    auto get_json(time_type from, time_type to) -> std::string
    {
        if (m_time_index.empty())
        {
            return "[]";
        }

        auto range = range_from_to(from, to);

        std::stringstream ss;
        json_data_points<true>(ss, range.from, range.to);
        return ss.str();
    }

    /// @param count The number of latest points based on the time, meaning that
    /// if the value is 10 and there is two data points per unique time point
    /// then a collection of 20 data points is returned.
    /// @return JSON string of an array holding the collection of the n latest
    /// points.
    auto get_json(std::size_t count) -> std::string
    {
        if (m_time_index.empty())
        {
            return "[]";
        }

        auto range = range_latest(count);

        std::stringstream ss;
        json_data_points<true>(ss, range.from, range.to);
        return ss.str();
    }

    /// Sets the maximum number of unique time points. This mean that if the
    /// capacity is set to 100, then 100 data points with unique time points is
    /// kept and the oldest data points is discarded. If there is multiple data
    /// points for a single unique time point they are all kept, meaning that if
    /// there is 2 data point per unique time point and there is 100 unique time
    /// points then 200 data points is kept.
    auto set_capacity(std::size_t capacity) -> void
    {
        m_time_index_records_capacity = capacity;
    }

    /// Clears all data points flushed to the exporter.
    auto clear() -> void
    {
        m_data_points.clear();
        m_time_index.clear();
    }

private:
    /// @brief Used to represent a range of data points by the iterators of the
    /// start and end.
    struct data_points_range
    {
        data_points::iterator from, to;
    };

    /// Returns a `data_points_range` struct with the data point iterators
    /// nearest the start and end position of the range given. Data points at
    /// the edges is included.
    /// @param from The range start position. The search is done with equal or
    /// greater than.
    /// @param to The range end position. The search is done with equal or less
    /// than.
    /// @return The two iterators of the outer most data point given for the
    /// given time range.
    auto range_from_to(time_type from, time_type to) -> data_points_range
    {

        if (m_time_index.empty() ||
            (from > m_time_index.back().m_time &&
             to > m_time_index.back().m_time) ||
            (from < m_time_index.front().m_time &&
             to < m_time_index.front().m_time))
        {
            return {m_data_points.end(), m_data_points.end()};
        }

        assert(from <= to);

        auto from_time_record = m_time_index.find_near(from);

        auto from_offset = (data_points ::difference_type)(
                               m_time_index.begin() +
                               (time_index::difference_type)from_time_record)
                               ->m_data_point_index;

        auto to_time_record = m_time_index.find_near(to);

        if (to_time_record == m_time_index.size() - 1)
        {
            return {m_data_points.begin() + from_offset, m_data_points.end()};
        }
        else
        {
            auto to_offset =
                (data_points ::difference_type)(
                    m_time_index.begin() +
                    (time_index::difference_type)(to_time_record + 1))
                    ->m_data_point_index;

            return {m_data_points.begin() + from_offset,
                    m_data_points.begin() + to_offset};
        }
    }

    /// Returns the range of the latest data points based on unique time points.
    /// Meaning that if there is two data point at the latest time point but is
    /// returned if the method is called with a count argument of 1.
    /// @param count The maximum number of time points
    /// @return The two iterators of the range where the `from` is the end
    /// iterator offset by the given count and the `to` is the the end iterator.
    auto range_latest(std::size_t count) -> data_points_range
    {
        if (m_time_index.empty() || count == 0)
        {
            return {m_data_points.end(), m_data_points.end()};
        }

        auto from_time_record =
            m_time_index.end() - ((time_index::difference_type)std::min(
                                     m_time_index.size(), (std::size_t)count));

        auto from_offset =
            (data_points::difference_type)from_time_record->m_data_point_index;

        return {m_data_points.begin() + from_offset, m_data_points.end()};
    };

    data_points m_data_points;
    time_index m_time_index;
    std::size_t m_time_index_records_capacity;
};

const std::size_t time_series_exporter::time_index::NOT_FOUND = (std::size_t)-1;
}
}