/*
Copyright Charly Chevalier 2015
Copyright Joel Falcou 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef DPLX_PREDEF_HARDWARE_SIMD_X86_AMD_H
#define DPLX_PREDEF_HARDWARE_SIMD_X86_AMD_H

#include <dplx/predef/version_number.h>
#include <dplx/predef/hardware/simd/x86_amd/versions.h>

/* tag::reference[]
= `DPLX_HW_SIMD_X86_AMD`

The SIMD extension for x86 (AMD) (*if detected*).
Version number depends on the most recent detected extension.

[options="header"]
|===
| {predef_symbol} | {predef_version}

| `+__SSE4A__+` | {predef_detection}

| `+__FMA4__+` | {predef_detection}

| `+__XOP__+` | {predef_detection}

| `DPLX_HW_SIMD_X86` | {predef_detection}
|===

[options="header"]
|===
| {predef_symbol} | {predef_version}

| `+__SSE4A__+` | DPLX_HW_SIMD_X86_SSE4A_VERSION

| `+__FMA4__+` | DPLX_HW_SIMD_X86_FMA4_VERSION

| `+__XOP__+` | DPLX_HW_SIMD_X86_XOP_VERSION

| `DPLX_HW_SIMD_X86` | DPLX_HW_SIMD_X86
|===

NOTE: This predef includes every other x86 SIMD extensions and also has other
more specific extensions (FMA4, XOP, SSE4a). You should use this predef
instead of `DPLX_HW_SIMD_X86` to test if those specific extensions have
been detected.

*/ // end::reference[]

#define DPLX_HW_SIMD_X86_AMD DPLX_VERSION_NUMBER_NOT_AVAILABLE

// AMD CPUs also use x86 architecture. We first try to detect if any AMD
// specific extension are detected, if yes, then try to detect more recent x86
// common extensions.

#undef DPLX_HW_SIMD_X86_AMD
#if !defined(DPLX_HW_SIMD_X86_AMD) && defined(__XOP__)
#   define DPLX_HW_SIMD_X86_AMD DPLX_HW_SIMD_X86_AMD_XOP_VERSION
#endif
#if !defined(DPLX_HW_SIMD_X86_AMD) && defined(__FMA4__)
#   define DPLX_HW_SIMD_X86_AMD DPLX_HW_SIMD_X86_AMD_FMA4_VERSION
#endif
#if !defined(DPLX_HW_SIMD_X86_AMD) && defined(__SSE4A__)
#   define DPLX_HW_SIMD_X86_AMD DPLX_HW_SIMD_X86_AMD_SSE4A_VERSION
#endif

#if !defined(DPLX_HW_SIMD_X86_AMD)
#   define DPLX_HW_SIMD_X86_AMD DPLX_VERSION_NUMBER_NOT_AVAILABLE
#else
    // At this point, we know that we have an AMD CPU, we do need to check for
    // other x86 extensions to determine the final version number.
#   include <dplx/predef/hardware/simd/x86.h>
#   if DPLX_HW_SIMD_X86 > DPLX_HW_SIMD_X86_AMD
#      undef DPLX_HW_SIMD_X86_AMD
#      define DPLX_HW_SIMD_X86_AMD DPLX_HW_SIMD_X86
#   endif
#   define DPLX_HW_SIMD_X86_AMD_AVAILABLE
#endif

#define DPLX_HW_SIMD_X86_AMD_NAME "x86 (AMD) SIMD"

#endif

#include <dplx/predef/detail/test.h>
DPLX_PREDEF_DECLARE_TEST(DPLX_HW_SIMD_X86_AMD, DPLX_HW_SIMD_X86_AMD_NAME)
