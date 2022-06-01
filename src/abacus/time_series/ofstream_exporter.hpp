// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "data_points.hpp"
#include "exporter.hpp"

#include <fstream>
#include <iterator>
#include <sstream>
#include <utility>

namespace abacus
{
namespace time_series
{
/// @brief Outputs all data points as a printed JSON array in the ostream
class ofstream_exporter final : public exporter
{
public:
    enum class format
    {
        BINARY,
        JSON
    };

    explicit ofstream_exporter(std::string file_path,
                               format format_in = format::BINARY) :
        m_file_path(std::move(file_path)),
        m_format(format_in)
    {
    }

    auto receive_data_points(data_points::iterator data_points_start,
                             data_points::iterator data_points_end)
        -> void override
    {
        // Start of by creating the file for the data points
        std::ofstream create_file(m_file_path, std::fstream::app);
        if (!create_file.is_open())
        {
            // The file could not be created
            // TODO: How do we handle this?
            throw std::exception();
        }
        create_file.close();

        switch (m_format)
        {
        case format::BINARY:
        {
            std::ofstream file_stream(m_file_path,
                                      std::fstream::binary | std::fstream::app);
            std::basic_stringbuf<char> stringbuf;
            for (auto itr = data_points_start; itr != data_points_end; itr++)
            {
                file_stream << (uint8_t)itr->aggregate->size();
                for (auto& part : *itr->aggregate)
                {
                    file_stream << part;
                }
                stringbuf.pubsetbuf((char*)itr->metrics.data(),
                                    (std::streamsize)itr->metrics.size());
                file_stream << &stringbuf;
            }
        }
        break;

        case format::JSON:
        {
            std::fstream file_stream(m_file_path, std::fstream::out |
                                                      std::fstream::in |
                                                      std::fstream::binary);

            std::stringstream string_stream;
            string_stream << "[" << std::endl;

            auto file_start = std::istreambuf_iterator<char>(file_stream);
            auto file_end = std::istreambuf_iterator<char>();
            if (file_start != file_end)
            {
                advance(file_start, 2); // Skip first line with "["
                advance(file_end, -2);  // Skip last line with "]"
                std::copy(file_start, file_end,
                          std::ostreambuf_iterator<char>(string_stream));
                string_stream.seekp(-3, std::stringstream::end);
                string_stream << "," << std::endl;
            }
            json_data_points(string_stream, data_points_start, data_points_end,
                             true);
            string_stream << "]" << std::endl;
            file_stream.seekp(0, std::fstream::beg);
            file_stream << string_stream.rdbuf();
        }
        break;
        }
    }

private:
    std::string m_file_path;
    bool m_use_delta_values;
    format m_format;
};
}
}