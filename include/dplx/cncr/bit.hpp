
// Copyright Henrik Steffen Gaßmann 2019
//
// Distributed under the Boost Software License, Version 1.0.
//         (See accompanying file LICENSE or copy at
//           https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstddef>

#include <limits>
#include <type_traits>

#if __cplusplus > 201703L && __has_include(<version>)
#include <version>
#endif

// #cpp_version_ODR_violation
#if __cplusplus > 201703L && __cpp_lib_bitops >= 201907L
#include <bit>

#include <dplx/predef/compiler.h>

namespace dplx::cncr
{

using std::countl_one;
using std::countl_zero;
using std::countr_one;
using std::countr_zero;

} // namespace dplx::cncr

#else

namespace dplx::cncr::detail
{

template <class U1, class U2>
inline constexpr int unsigned_digit_distance_v
        = std::numeric_limits<U1>::digits - std::numeric_limits<U2>::digits;

}

namespace dplx::cncr
{

template <class T>
inline auto countl_zero(T x) noexcept -> int
{
    static_assert(std::is_integral_v<T>);
    static_assert(std::is_unsigned_v<T>);
    static_assert(sizeof(T) <= sizeof(unsigned long long));

    using limits = std::numeric_limits<T>;

    if (x == 0)
    {
        return limits::digits;
    }

#if defined DPLX_COMP_MSVC_AVAILABLE

    unsigned long result;
    if constexpr (sizeof(T) <= sizeof(unsigned long))
    {
        _BitScanReverse(&result, static_cast<unsigned long>(x));
        result -= detail::unsigned_digit_distance_v<unsigned long, T>;
    }
    else if constexpr (sizeof(T) <= sizeof(unsigned long long))
    {
#if defined(_M_ARM64) || defined(_M_AMD64)

        _BitScanReverse64(&result, static_cast<unsigned long long>(x));
        result -= detail::unsigned_digit_distance_v<unsigned long long, T>;

#else
        static_assert(sizeof(unsigned long) * 2 == sizeof(unsigned long long));

        using ulong_limits = std::numeric_limits<unsigned long>;
        if (_BitScanReverse(&result, static_cast<unsigned long>(
                                             x >> ulong_limits::digits)))
        {
            result -= detail::unsigned_digit_distance_v<unsigned long long, T>;
        }
        else
        {
            _BitScanReverse(&result, static_cast<unsigned long>(x));
            result += ulong_limits::digits;
            result -= detail::unsigned_digit_distance_v<unsigned long long, T>;
        }
#endif
    }
    return static_cast<int>(result);

#elif defined DPLX_COMP_GCC_AVAILABLE || defined DPLX_COMP_CLANG_AVAILABLE

    int result;
    if constexpr (sizeof(T) <= sizeof(unsigned int))
    {
        result = __builtin_countlz(static_cast<unsigned int>(x));
        result -= detail::unsigned_digit_distance_v<unsigned int, T>;
    }
    else if constexpr (sizeof(T) <= sizeof(unsigned long))
    {
        result = __builtin_countlzl(static_cast<unsigned long>(x));
        result -= detail::unsigned_digit_distance_v<unsigned long, T>;
    }
    else if constexpr (sizeof(T) <= sizeof(unsigned long long))
    {
        result = __builtin_countlzll(static_cast<unsigned long long>(x));
        result -= detail::unsigned_digit_distance_v<unsigned long long, T>;
    }
    return result;

#else
#error countl_zero has not been ported to this compiler
#endif
}

template <class T>
inline auto countr_zero(T x) noexcept -> int
{
    static_assert(std::is_integral_v<T>);
    static_assert(std::is_unsigned_v<T>);
    static_assert(sizeof(T) <= sizeof(unsigned long long));

    using limits = std::numeric_limits<T>;

    if (x == 0)
    {
        return limits::digits;
    }

#if defined DPLX_COMP_MSVC_AVAILABLE

    unsigned long result;
    if constexpr (sizeof(T) <= sizeof(unsigned long))
    {
        _BitScanForward(&result, static_cast<unsigned long>(x));
        result -= detail::unsigned_digit_distance_v<unsigned long, T>;
    }
    else if constexpr (sizeof(T) <= sizeof(unsigned long long))
    {
#if defined(_M_ARM64) || defined(_M_AMD64)

        _BitScanForward64(&result, static_cast<unsigned long long>(x));
        result -= detail::unsigned_digit_distance_v<unsigned long long, T>;

#else
        static_assert(sizeof(unsigned long) * 2 == sizeof(unsigned long long));

        using ulong_limits = std::numeric_limits<unsigned long>;
        if (!_BitScanForward(&result, static_cast<unsigned long>(x)))
        {
            _BitScanForward(&result, static_cast<unsigned long>(
                                             x >> ulong_limits::digits));
            result += ulong_limits::digits;
        }
#endif
    }
    return static_cast<int>(result);

#elif defined DPLX_COMP_GCC_AVAILABLE || defined DPLX_COMP_CLANG_AVAILABLE

    int result;
    if constexpr (sizeof(T) <= sizeof(unsigned int))
    {
        result = __builtin_countffs(static_cast<unsigned int>(x));
    }
    else if constexpr (sizeof(T) <= sizeof(unsigned long))
    {
        result = __builtin_countffsl(static_cast<unsigned long>(x));
    }
    else if constexpr (sizeof(T) <= sizeof(unsigned long long))
    {
        result = __builtin_countffsll(static_cast<unsigned long long>(x));
    }
    return result;

#else
#error countr_zero has not been ported to this compiler
#endif
}

template <typename T>
int countl_one(T x) noexcept
{
    return countl_zero(~x);
}
template <typename T>
int countr_one(T x) noexcept
{
    return countr_zero(~x);
}

} // namespace dplx::cncr

#endif
