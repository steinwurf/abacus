// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "data_points.hpp"
#include "exporter.hpp"

#include <atomic>

namespace abacus
{
namespace time_series
{
/// @brief A lock free queue with support for one consumer and one producer,
/// used for transporting collected data points from a collector to a set of
/// exporters. This makes it possible to offload the export processing to a
/// separate thread in a lock free manner.
struct data_points_queue
{
private:
    struct node
    {
        explicit node() :
            m_points(std::make_shared<data_points>()), m_next(nullptr)
        {
        }
        std::shared_ptr<data_points> m_points;
        node* m_next;
    };

    node* m_head;
    std::atomic<node*> m_divider{}, m_tail{};

public:
    data_points_queue()
    {
        m_head = m_divider = m_tail = new node();
    }

    ~data_points_queue()
    {
        while (m_head != nullptr)
        {
            node* tmp = m_head;
            m_head = tmp->m_next;
            delete tmp;
        }
    }

    auto push(data_points::iterator begin, data_points::iterator end) -> void
    {
        auto new_node = new node();
        std::copy(begin, end, std::back_inserter(*new_node->m_points));

        m_tail.load()->m_next = new_node;
        m_tail = m_tail.load()->m_next;

        while (m_head != m_divider)
        {
            node* tmp = m_head;
            m_head = m_head->m_next;
            delete tmp;
        }
    }

    auto flush_to_exporters(const exporters& in_exporters) -> void
    {
        while (m_divider != m_tail)
        {
            auto& points = *m_divider.load()->m_next->m_points;
            m_divider = m_divider.load()->m_next;

            for (auto& in_exporter : in_exporters)
            {
                in_exporter->receive_data_points(points.begin(), points.end());
            }
        }
    }
};
}
}