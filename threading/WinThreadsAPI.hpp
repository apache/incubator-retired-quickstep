/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_THREADING_WIN_THREADS_API_HPP_
#define QUICKSTEP_THREADING_WIN_THREADS_API_HPP_

#define WIN32_LEAN_AND_MEAN

// Magic numbers from WinBase.h
#define CREATE_SUSPENDED 0x00000004
#define INFINITE 0xFFFFFFFF

// Define a symbol for the CPU architecture. This is normally done by
// windows.h, but the modular headers introduced in the Windows 8 API still
// need it.

// 64-bit x86
#if defined(__x86_64__) || defined(_M_X64)
#define _AMD64_
#endif

// 32-bit x86
#if !defined(_AMD64_) && (defined(__i386__) || defined(_M_IX86))
#define _X86_
#endif

// ARM
#if !defined(_AMD64_) && !defined(_X86_) && (defined(__arm__) || defined(_M_ARM))
#define _ARM_
#endif

// Itanium
#if !defined(_AMD64_) && !defined(_X86_) && !defined(_ARM_)  \
    && (defined(__itanium__) || defined(__ia64) || defined(_M_IA64))
#define _IA64_
#endif

// Minimal headers needed for windows threads. These modular headers were
// introduced in the Windows 8 SDK, but will work with previous Windows
// versions as long as the more recent SDK is installed on the build machine.
#include <handleapi.h>
#include <processthreadsapi.h>
#include <synchapi.h>

#endif  // QUICKSTEP_THREADING_WIN_THREADS_API_HPP_
