
// Copyright 2022-2023 Henrik Steffen Gaßmann
//
// Distributed under the Boost Software License, Version 1.0.
//         (See accompanying file LICENSE or copy at
//           https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <outcome/experimental/status_result.hpp>
#include <outcome/result.hpp>
#include <outcome/try.hpp>
#include <status-code/error.hpp>

#ifndef DPLX_TRY
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DPLX_TRY(...) OUTCOME_TRY(__VA_ARGS__)
#endif

namespace dplx
{

namespace system_error = SYSTEM_ERROR2_NAMESPACE;
namespace outcome = OUTCOME_V2_NAMESPACE;

// we use a distinct policy type in order to shorten the symbol length
template <typename T>
struct status_code_throw_policy
    : outcome::experimental::policy::status_code_throw<
              T,
              system_error::erased_errored_status_code<
                      typename system_error::system_code::value_type>,
              void>
{
};

template <typename R>
using result = outcome::experimental::status_result<
        R,
        system_error::erased_errored_status_code<
                typename system_error::system_code::value_type>,
        status_code_throw_policy<R>>;

} // namespace dplx

namespace dplx::cncr::detail
{

template <typename B>
concept boolean_testable_impl = std::convertible_to<B, bool>;
// clang-format off
template <typename B>
concept boolean_testable
        = boolean_testable_impl<B>
        && requires(B b)
        {
            { !static_cast<B &&>(b) }
                -> boolean_testable_impl;
        };
// clang-format on

template <typename T>
void check_copy_initializable(T) noexcept;

} // namespace dplx::cncr::detail

namespace dplx::cncr
{

// clang-format off
template <typename T>
concept tryable
    = requires(T t)
{
    // has value
    { outcome::try_operation_has_value(t) }
        -> detail::boolean_testable;

    // failure propagation
    outcome::try_operation_return_as(static_cast<T &&>(t));
    // result<void> rx = outcome::try_operation_return_as(static_cast<T &&>(t));
    detail::check_copy_initializable<result<void>>(
        outcome::try_operation_return_as(static_cast<T &&>(t)));

    // value extraction; note that this doesn't assert anything about value_type
    outcome::try_operation_extract_value(static_cast<T &&>(t));
};
// clang-format on

template <tryable T>
using result_value_t
        = std::remove_cvref_t<decltype(outcome::try_operation_extract_value(
                std::declval<T &&>()))>;

template <typename T, typename R>
concept tryable_result
        = tryable<T>
       && (std::is_same_v<R, result_value_t<T>> // this handles R=void
           || requires(T rx) {
                  detail::check_copy_initializable<R>(
                          outcome::try_operation_extract_value(
                                  static_cast<T &&>(rx)));
              });

} // namespace dplx::cncr
