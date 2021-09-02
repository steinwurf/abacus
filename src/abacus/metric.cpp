// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "metric.hpp"

#include <algorithm>
#include <cstring>

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
metric::metric(uint64_t* memory) : m_memory(memory)
{
    assert(m_memory != nullptr);
}

auto metric::operator=(uint64_t value) -> metric&
{
    assert(m_memory != nullptr);

    *m_memory = value;
    return *this;
}

auto metric::operator+=(uint64_t value) -> metric&
{
    assert(m_memory != nullptr);

    *m_memory += value;
    return *this;
}

auto metric::operator++() -> metric&
{
    assert(m_memory != nullptr);

    *m_memory += 1U;
    return *this;
}

auto metric::operator-=(uint64_t value) -> metric&
{
    assert(m_memory != nullptr);
    assert(value <= *m_memory);

    *m_memory -= value;
    return *this;
}

auto metric::operator--() -> metric&
{
    assert(m_memory != nullptr);
    assert(1U <= *m_memory);

    *m_memory -= 1U;
    return *this;
}

auto metric::is_initialized() const -> bool
{
    return m_memory != nullptr;
}
}
}