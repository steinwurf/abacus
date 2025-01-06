// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>
#include <variant>

#include "kind.hpp"
#include "max.hpp"
#include "min.hpp"
#include "unit.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{
namespace
{
template <typename T>
struct common
{
    using type = typename T::type;
    using metric = typename T::metric;
    /// Default constructor
    common() = default;

    common(uint8_t* memory) : m_memory(memory)
    {
        assert(m_memory != nullptr);
        memset(m_memory, 0, sizeof(type) + 1);
    }

    common(uint8_t* memory, type value) : m_memory(memory)
    {
        assert(m_memory != nullptr);
        set_value(value);
    }

    auto is_initialized() const -> bool
    {
        return m_memory != nullptr;
    }

    auto has_value() const -> bool
    {
        assert(is_initialized());
        return m_memory[0] == 1U;
    }

    auto reset() -> void
    {
        assert(is_initialized());
        m_memory[0] = 0;
    }

    auto value() const -> type
    {
        assert(is_initialized());
        assert(has_value());
        type value;
        std::memcpy(&value, m_memory + 1, sizeof(value));
        return value;
    }

    auto set_value(type value) -> void
    {
        assert(is_initialized());
        m_memory[0] = 1;
        std::memcpy(m_memory + 1, &value, sizeof(value));
    }

    /// Assign the counter a new value
    /// @param value The value to assign
    /// @return a counter with the new value
    auto operator=(type value) -> metric&
    {
        assert(is_initialized());
        set_value(value);
        return (metric&)*this;
    }

    /// Increment the counter
    /// @param value The value to add
    /// @return The result of the arithmetic
    auto operator+=(type value) -> metric&
    {
        assert(has_value());
        auto new_value = this->value() + value;
        std::memcpy(m_memory + 1, &new_value, sizeof(new_value));
        return (metric&)*this;
    }

    /// Decrement the counter
    /// @param value The value to subtract
    /// @return The result of the arithmetic
    auto operator-=(type value) -> metric&
    {
        assert(has_value());
        auto new_value = this->value() - value;
        std::memcpy(m_memory + 1, &new_value, sizeof(new_value));
        return (metric&)*this;
    }

    /// Increment the value of the counter
    /// @return The result of the arithmetic
    auto operator++() -> metric&
    {
        assert(has_value());
        auto new_value = value() + 1;
        std::memcpy(m_memory + 1, &new_value, sizeof(new_value));
        return (metric&)*this;
    }

    /// Decrement the value of the counter
    /// @return The result of the arithmetic
    auto operator--() -> metric&
    {
        auto new_value = value() - 1;
        std::memcpy(m_memory + 1, &new_value, sizeof(new_value));
        return (metric&)*this;
    }

protected:
    /// The metric memory
    uint8_t* m_memory = nullptr;
};
}

struct uint64
{
    using type = uint64_t;
    struct metric : public common<uint64>
    {
        using common<uint64>::common;
        using common<uint64>::operator=;
    };

    abacus::kind kind;
    std::string description;
    abacus::unit unit;
    abacus::min2<type> min;
    abacus::max2<type> max;
};
struct int64
{
    using type = int64_t;
    struct metric : public common<int64>
    {
        using common<int64>::common;
        using common<int64>::operator=;
    };
    abacus::kind kind;
    std::string description;
    abacus::unit unit;
    abacus::min2<type> min;
    abacus::max2<type> max;
};
struct uint32
{
    using type = uint32_t;
    struct metric : public common<uint32>
    {
        using common<uint32>::common;
        using common<uint32>::operator=;
    };
    abacus::kind kind;
    std::string description;
    abacus::unit unit;
    abacus::min2<type> min;
    abacus::max2<type> max;
};
struct int32
{
    using type = int32_t;
    struct metric : public common<int32>
    {
        using common<int32>::common;
        using common<int32>::operator=;
    };
    abacus::kind kind;
    std::string description;
    abacus::unit unit;
    abacus::min2<type> min;
    abacus::max2<type> max;
};
struct float64
{
    using type = double;
    struct metric : public common<float64>
    {
        using common<float64>::common;
        using common<float64>::operator=;
    };
    abacus::kind kind;
    std::string description;
    abacus::unit unit;
    abacus::min2<type> min;
    abacus::max2<type> max;
};
struct float32
{
    using type = float;
    struct metric : public common<float32>
    {
        using common<float32>::common;
        using common<float32>::operator=;
    };
    abacus::kind kind;
    std::string description;
    abacus::unit unit;
    abacus::min2<type> min;
    abacus::max2<type> max;
};
struct boolean
{
    using type = bool;
    struct metric
    {
        metric() = default;

        metric(uint8_t* memory) : m_memory(memory)
        {
            assert(m_memory != nullptr);
            memset(m_memory, 0, sizeof(type) + 1);
        }

        metric(uint8_t* memory, type value) : m_memory(memory)
        {
            assert(m_memory != nullptr);
            set_value(value);
        }

        auto is_initialized() const -> bool
        {
            return m_memory != nullptr;
        }

        auto has_value() const -> bool
        {
            assert(is_initialized());
            return m_memory[0] == 1U;
        }

        auto reset() -> void
        {
            assert(is_initialized());
            m_memory[0] = 0;
        }

        auto value() const -> type
        {
            assert(is_initialized());
            return m_memory[1];
        }

        auto set_value(type value) -> void
        {
            assert(is_initialized());
            m_memory[0] = 1;
            m_memory[1] = value;
        }

        /// Assign the counter a new value
        /// @param value The value to assign
        /// @return a counter with the new value
        auto operator=(type value) -> metric&
        {
            assert(is_initialized());
            set_value(value);
            return *this;
        }

    private:
        /// The metric memory
        uint8_t* m_memory = nullptr;
    };
    abacus::kind kind;
    std::string description;
};

struct enum8
{
    using type = uint8_t;
    struct metric
    {
        metric() = default;

        metric(uint8_t* memory) : m_memory(memory)
        {
            assert(m_memory != nullptr);
            memset(m_memory, 0, sizeof(type) + 1);
        }

        metric(uint8_t* memory, type value) : m_memory(memory)
        {
            assert(m_memory != nullptr);
            set_value(value);
        }

        auto is_initialized() const -> bool
        {
            return m_memory != nullptr;
        }

        auto has_value() const -> bool
        {
            assert(is_initialized());
            return m_memory[0] == 1U;
        }

        auto reset() -> void
        {
            assert(is_initialized());
            m_memory[0] = 0;
        }

        auto value() const -> type
        {
            assert(is_initialized());
            return m_memory[1];
        }

        auto set_value(type value) -> void
        {
            assert(is_initialized());
            m_memory[0] = 1;
            m_memory[1] = value;
        }

        /// Assign the counter a new value
        /// @param value The value to assign
        /// @return a counter with the new value
        auto operator=(type value) -> metric&
        {
            assert(is_initialized());
            set_value(value);
            return *this;
        }

    private:
        /// The metric memory
        uint8_t* m_memory = nullptr;
    };

    struct value
    {
        std::string name;
        std::string description;
    };
    std::string description;
    std::map<uint32_t, value> values;
    abacus::unit unit;
};
using type2 = std::variant<uint64, int64, uint32, int32, float64, float32,
                           boolean, enum8>;
}
}
