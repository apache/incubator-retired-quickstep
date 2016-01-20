/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_UTILITY_MACROS_HPP_
#define QUICKSTEP_UTILITY_MACROS_HPP_

#ifdef QUICKSTEP_DEBUG
#include <cassert>
#endif
#include <cstdlib>
#include <iostream>

/**
 * This macro prevents instances of a class from being copied or assigned
 * by automatically-generated (dumb, shallow) copy constructors and assignment
 * operators. It should be placed in a class' private section, and given the
 * name of the class as its argument.
 **/
#define DISALLOW_COPY_AND_ASSIGN(classname) \
  classname(const classname &orig) = delete;\
  classname & operator=(const classname &rhs) = delete

/**
 * This macro supresses warnings that sometimes occur when GCC does not
 * recognize that the end of a non-void function is unreachable.
 **/
#if defined(__GNUC__) && !(defined(__clang__) || defined(__INTEL_COMPILER))
#define QUICKSTEP_UNREACHABLE()  \
  std::exit(1)
#else
#define QUICKSTEP_UNREACHABLE()  \
  (void)0
#endif

// TODO(chasseur): Start using a proper logging library like glog instead of
//                 these macros.

/**
 * This macro calls assert() with the given statement when QUICKSTEP_DEBUG is
 * defined (i.e. this is a debug build), otherwise it is a no-op. Remember: for
 * release builds, the provided statement is NOT called, so don't use this for
 * statements that have any side effects.
 **/
#ifdef QUICKSTEP_DEBUG
#define DEBUG_ASSERT(statement) \
  assert(statement)
#else
#define DEBUG_ASSERT(statement)
#endif

/**
 * This macro always calls the provided statement. When QUICKSTEP_DEBUG is
 * defined (i.e. this is a debug build), an assertion checks that the provided
 * statement returns 0.
 **/
#ifdef QUICKSTEP_DEBUG
#define DO_AND_DEBUG_ASSERT_ZERO(statement) \
  assert(statement == 0)
#else
#define DO_AND_DEBUG_ASSERT_ZERO(statement) \
  statement
#endif

/**
 * This macro logs the provided error message to STDERR and causes the program
 * to exit. We use a macro instead of a function so that compilers will realize
 * that calling FATAL_ERROR terminates the program, and it is OK to not return
 * from the calling function.
 **/
#define FATAL_ERROR(message) \
  std::cerr << "FATAL ERROR: " << message << "\n";\
  std::exit(1)

/**
 * This macro logs a warning message to STDERR. Note that race conditions are
 * possible when multiple threads call this, potentially garbling output.
 **/
#define LOG_WARNING(message) \
  std::cerr << "WARNING: " << message << "\n";

/**
 * This macro logs a warning message to STDERR. This is similar to LOG_WARNING,
 * but is intended for developer-specific messages indicating that there is
 * potentially a problem with some quickstep code.
 **/
#define DEV_WARNING(message) \
  std::cerr << "DEVELOPER WARNING: " << message << "\n";

#endif  // QUICKSTEP_UTILITY_MACROS_HPP_
