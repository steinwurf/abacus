#pragma once

#include <array>
#include <cassert>
#include <cstdint>

#include "version.hpp"

namespace abacus
{
inline namespace STEINWURF_ABACUS_VERSION
{

/// Main delegate template
template <class T, std::size_t MaxBytes = sizeof(void*) * 4,
          std::size_t Alignment = alignof(std::decay_t<T>)>
class delegate;

/// Specialization for function types
///
/// The delegate class stores an invocable object (e.g. a function,
/// member function, lambda).
///
/// The following examples show how to use the delegate class.
///
/// Invoke a lambda:
///
///    auto f = [](int x) { return x * x; };
///    delegate<int(int)> d(f);
///    d(2); // 4
///
/// Invoke a member function:
///
///    struct foo {
///      int square(int x) { return x * x; }
///    };
///    foo f;
///    delegate<int(int)> d(&f, &foo::square);
///    d(2); // 4
///
/// Invoke a function:
///
///    int square(int x) { return x * x; }
///    delegate<int(int)> d(square);
///    d(2); // 4
///
template <class R, class... Args, std::size_t MaxBytes, std::size_t Alignment>
class delegate<R(Args...), MaxBytes, Alignment>
{
public:
    /// Default Constructor
    delegate() = default;

    /// Construct a delegate from the callable object F. Typically a lambda or
    /// functor. The enable_if is needed to avoid interfering with the
    /// implicitly declared copy constructor.
    template <class F, typename = std::enable_if_t<
                           !std::is_same<std::decay_t<F>, delegate>::value>>
    delegate(F&& f)
    {
        static_assert(sizeof(std::decay_t<F>) <= MaxBytes,
                      "Delegate closure F is too big");

        static_assert(alignof(std::decay_t<F>) <= Alignment,
                      "Delegate closure F has stricter alignment");

        static_assert(std::is_trivially_copyable<std::decay_t<F>>::value,
                      "Delegate closure F not trivially copyable!");

        static_assert(std::is_trivially_destructible<std::decay_t<F>>::value,
                      "Delegate closure F not trivially destructible!");

        // We do not have to clean-up (i.e. call any destructors since F is
        // trivially destructible)
        new (&m_storage) std::decay_t<F>{std::forward<F>(f)};

        // Initialize the function pointer - the non-capturing lambda will decay
        // to the function pointer type
        m_invoke = [](const void* ptr, Args... args) -> R
        {
            return (*reinterpret_cast<const std::decay_t<F>*>(ptr))(
                std::forward<Args>(args)...);
        };
    }

    /// Construct a delegate from a point to function argument
    delegate(R (*const func)(Args...)) :
        delegate([func](Args... args) -> R
                 { return func(std::forward<Args>(args)...); })
    {
        assert(func != nullptr && "Delegate function pointer is null");
    }

    /// Construct a delegate to a member fuction in class C
    /// @param c Pointer to object of type C
    /// @param func Pointer to member function
    template <class C>
    delegate(std::decay_t<C>* const c, R (C::*const func)(Args...)) :
        delegate([c, func](Args... args) -> R
                 { return (c->*func)(std::forward<Args>(args)...); })
    {
        assert(c != nullptr && "Delegate object pointer is null");
        assert(func != nullptr && "Delegate member function pointer is null");
    }

    /// Construct a delegate to a const member fuction in class C
    /// @param c Pointer to const object of type C
    /// @param func Pointer to const member function
    template <class C>
    delegate(const std::decay_t<C>* const c,
             R (C::*const func)(Args...) const) :
        delegate([c, func](Args... args) -> R
                 { return (c->*func)(std::forward<Args>(args)...); })
    {
        assert(c != nullptr && "Delegate object pointer is null");
        assert(func != nullptr && "Delegate member function pointer is null");
    }

    /// Construct a delegate from an exiting invoke function and constructed
    /// function object
    delegate(R (*invoke)(const void*, Args...),
             std::array<uint8_t, MaxBytes> storage) :
        m_invoke(invoke),
        m_storage(storage)
    {
    }

    /// Call operator for invoking the delegate
    auto operator()(Args... args) const -> R
    {
        assert(m_invoke);
        return m_invoke(&m_storage, std::forward<Args>(args)...);
    }

    /// Operator bool to check for validity
    explicit operator bool() const noexcept
    {
        return m_invoke != nullptr;
    }

    /// Call operator for invoking the delegate
    auto call(Args... args) const -> R
    {
        assert(m_invoke);
        return m_invoke(&m_storage, std::forward<Args>(args)...);
    }

    /// @return The invoke pointer to function
    auto invoke() const -> R (*)(const void*, Args...)
    {
        return m_invoke;
    }

    /// @return The storage array
    auto storage() const -> std::array<uint8_t, MaxBytes>
    {
        return m_storage;
    }

private:
    /// The function pointer used to invoke the c
    R (*m_invoke)(const void*, Args...){nullptr};

    /// Storage for the closure object
    alignas(Alignment) std::array<uint8_t, MaxBytes> m_storage;
};

}
}
