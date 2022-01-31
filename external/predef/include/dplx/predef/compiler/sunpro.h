/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef DPLX_PREDEF_COMPILER_SUNPRO_H
#define DPLX_PREDEF_COMPILER_SUNPRO_H

#include <dplx/predef/version_number.h>
#include <dplx/predef/make.h>

/* tag::reference[]
= `DPLX_COMP_SUNPRO`

http://en.wikipedia.org/wiki/Oracle_Solaris_Studio[Oracle Solaris Studio] compiler.
Version number available as major, minor, and patch.

[options="header"]
|===
| {predef_symbol} | {predef_version}

| `+__SUNPRO_CC+` | {predef_detection}
| `+__SUNPRO_C+` | {predef_detection}

| `+__SUNPRO_CC+` | V.R.P
| `+__SUNPRO_C+` | V.R.P
| `+__SUNPRO_CC+` | VV.RR.P
| `+__SUNPRO_C+` | VV.RR.P
|===
*/ // end::reference[]

#define DPLX_COMP_SUNPRO DPLX_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__SUNPRO_CC) || defined(__SUNPRO_C)
#   if !defined(DPLX_COMP_SUNPRO_DETECTION) && defined(__SUNPRO_CC)
#       if (__SUNPRO_CC < 0x5100)
#           define DPLX_COMP_SUNPRO_DETECTION DPLX_PREDEF_MAKE_0X_VRP(__SUNPRO_CC)
#       else
#           define DPLX_COMP_SUNPRO_DETECTION DPLX_PREDEF_MAKE_0X_VVRRP(__SUNPRO_CC)
#       endif
#   endif
#   if !defined(DPLX_COMP_SUNPRO_DETECTION) && defined(__SUNPRO_C)
#       if (__SUNPRO_C < 0x5100)
#           define DPLX_COMP_SUNPRO_DETECTION DPLX_PREDEF_MAKE_0X_VRP(__SUNPRO_C)
#       else
#           define DPLX_COMP_SUNPRO_DETECTION DPLX_PREDEF_MAKE_0X_VVRRP(__SUNPRO_C)
#       endif
#   endif
#   if !defined(DPLX_COMP_SUNPRO_DETECTION)
#       define DPLX_COMP_SUNPRO_DETECTION DPLX_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#ifdef DPLX_COMP_SUNPRO_DETECTION
#   if defined(DPLX_PREDEF_DETAIL_COMP_DETECTED)
#       define DPLX_COMP_SUNPRO_EMULATED DPLX_COMP_SUNPRO_DETECTION
#   else
#       undef DPLX_COMP_SUNPRO
#       define DPLX_COMP_SUNPRO DPLX_COMP_SUNPRO_DETECTION
#   endif
#   define DPLX_COMP_SUNPRO_AVAILABLE
#   include <dplx/predef/detail/comp_detected.h>
#endif

#define DPLX_COMP_SUNPRO_NAME "Oracle Solaris Studio"

#endif

#include <dplx/predef/detail/test.h>
DPLX_PREDEF_DECLARE_TEST(DPLX_COMP_SUNPRO,DPLX_COMP_SUNPRO_NAME)

#ifdef DPLX_COMP_SUNPRO_EMULATED
#include <dplx/predef/detail/test.h>
DPLX_PREDEF_DECLARE_TEST(DPLX_COMP_SUNPRO_EMULATED,DPLX_COMP_SUNPRO_NAME)
#endif
