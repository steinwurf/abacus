// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <iostream>

#include <abacus/metrics.hpp>

// Simple example of metrics on a .

int main()
{
    /// Choose the constructor values for the metrics class
    uint64_t max_metrics = 10;
    uint64_t max_name_bytes = 32;

    abacus::metrics stream(max_metrics, max_name_bytes, "Stream");

    /// The stream has some bytes processed. Could be Video data and Audio data
    /// e.g
    auto video_bytes = stream.initialize_metric(0, "Video Bytes Processed",
                                                abacus::unit::bytes);
    video_bytes += 100;

    auto audio_bytes = stream.initialize_metric(1, "Audio Bytes Processed",
                                                abacus::unit::bytes);
    audio_bytes += 200;

    /// What about the amount of packets?
    auto video_packets =
        stream.initialize_metric(2, "Video packets", abacus::unit::packets);

    auto audio_packets =
        stream.initialize_metric(3, "Audio packets", abacus::unit::packets);

    video_packets += 20;
    audio_packets += 40;

    /// Maybe there are some lost video packets?
    auto lost_video_packets = stream.initialize_metric(4, "Video Packets lost",
                                                       abacus::unit::packets);

    lost_video_packets += 4;

    auto lost_audio_packets = stream.initialize_metric(5, "Audio Packets lost",
                                                       abacus::unit::packets);

    lost_audio_packets += 8;

    /// We can print out the counters neatly.
    std::cout << stream.to_json_with_units() << std::endl;

    /// We want to export the metrics memory, so we need a new storage
    std::vector<uint8_t> data(stream.storage_bytes());

    /// Copy the memory into the new storage
    stream.copy_storage(data.data());

    /// We can use the view class to read the pointed-to values
    abacus::view stream_view;

    /// The view should operate on the copied storage
    stream_view.set_data(data.data());

    /// Lets see what it contains:
    std::cout << "stream has the following metrics:" << std::endl;

    for (std::size_t i = 0; i < stream_view.max_metrics(); i++)
    {
        /// If a counter in memory has no name, it's not yet initialized.
        /// We will ignore it.
        if (!stream_view.is_metric_initialized(i))
        {
            continue;
        }
        /// Get the name from memory and the address of the value and
        /// dereference it.
        std::cout << "\t" << stream_view.metric_name(i) << ": "
                  << stream_view.metric_value(i) << " "
                  << stream_view.metric_unit(i) << std::endl;
    }

    return 0;
}
