
// Copyright 2022-2023 Henrik Steffen Gaßmann
//
// Distributed under the Boost Software License, Version 1.0.
//         (See accompanying file LICENSE or copy at
//           https://www.boost.org/LICENSE_1_0.txt)

#pragma once

// the configuration generated by cmake is fully optional
#if __has_include(<dplx/cncr/detail/config.hpp>)
#include <dplx/cncr/detail/config.hpp>
#endif

#if !defined(DPLX_CNCR_DISABLE_WORKAROUNDS)
#define DPLX_CNCR_DISABLE_WORKAROUNDS 0
#endif
#if !defined(DPLX_CNCR_FLAG_OUTDATED_WORKAROUNDS)
#define DPLX_CNCR_FLAG_OUTDATED_WORKAROUNDS 0
#endif
