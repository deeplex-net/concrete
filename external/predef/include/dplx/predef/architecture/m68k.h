/*
Copyright Rene Rivera 2008-2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef DPLX_PREDEF_ARCHITECTURE_M68K_H
#define DPLX_PREDEF_ARCHITECTURE_M68K_H

#include <dplx/predef/version_number.h>
#include <dplx/predef/make.h>

/* tag::reference[]
= `DPLX_ARCH_M68K`

http://en.wikipedia.org/wiki/M68k[Motorola 68k] architecture.

[options="header"]
|===
| {predef_symbol} | {predef_version}

| `+__m68k__+` | {predef_detection}
| `M68000` | {predef_detection}

| `+__mc68060__+` | 6.0.0
| `mc68060` | 6.0.0
| `+__mc68060+` | 6.0.0
| `+__mc68040__+` | 4.0.0
| `mc68040` | 4.0.0
| `+__mc68040+` | 4.0.0
| `+__mc68030__+` | 3.0.0
| `mc68030` | 3.0.0
| `+__mc68030+` | 3.0.0
| `+__mc68020__+` | 2.0.0
| `mc68020` | 2.0.0
| `+__mc68020+` | 2.0.0
| `+__mc68010__+` | 1.0.0
| `mc68010` | 1.0.0
| `+__mc68010+` | 1.0.0
| `+__mc68000__+` | 0.0.1
| `mc68000` | 0.0.1
| `+__mc68000+` | 0.0.1
|===
*/ // end::reference[]

#define DPLX_ARCH_M68K DPLX_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__m68k__) || defined(M68000)
#   undef DPLX_ARCH_M68K
#   if !defined(DPLX_ARCH_M68K) && (defined(__mc68060__) || defined(mc68060) || defined(__mc68060))
#       define DPLX_ARCH_M68K DPLX_VERSION_NUMBER(6,0,0)
#   endif
#   if !defined(DPLX_ARCH_M68K) && (defined(__mc68040__) || defined(mc68040) || defined(__mc68040))
#       define DPLX_ARCH_M68K DPLX_VERSION_NUMBER(4,0,0)
#   endif
#   if !defined(DPLX_ARCH_M68K) && (defined(__mc68030__) || defined(mc68030) || defined(__mc68030))
#       define DPLX_ARCH_M68K DPLX_VERSION_NUMBER(3,0,0)
#   endif
#   if !defined(DPLX_ARCH_M68K) && (defined(__mc68020__) || defined(mc68020) || defined(__mc68020))
#       define DPLX_ARCH_M68K DPLX_VERSION_NUMBER(2,0,0)
#   endif
#   if !defined(DPLX_ARCH_M68K) && (defined(__mc68010__) || defined(mc68010) || defined(__mc68010))
#       define DPLX_ARCH_M68K DPLX_VERSION_NUMBER(1,0,0)
#   endif
#   if !defined(DPLX_ARCH_M68K) && (defined(__mc68000__) || defined(mc68000) || defined(__mc68000))
#       define DPLX_ARCH_M68K DPLX_VERSION_NUMBER_AVAILABLE
#   endif
#   if !defined(DPLX_ARCH_M68K)
#       define DPLX_ARCH_M68K DPLX_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if DPLX_ARCH_M68K
#   define DPLX_ARCH_M68K_AVAILABLE
#endif

#if DPLX_ARCH_M68K
#   undef DPLX_ARCH_WORD_BITS_32
#   define DPLX_ARCH_WORD_BITS_32 DPLX_VERSION_NUMBER_AVAILABLE
#endif

#define DPLX_ARCH_M68K_NAME "Motorola 68k"

#endif

#include <dplx/predef/detail/test.h>
DPLX_PREDEF_DECLARE_TEST(DPLX_ARCH_M68K,DPLX_ARCH_M68K_NAME)
