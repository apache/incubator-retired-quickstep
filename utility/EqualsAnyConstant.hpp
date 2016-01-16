/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_UTILITY_EQUALS_ANY_CONSTANT_HPP_
#define QUICKSTEP_UTILITY_EQUALS_ANY_CONSTANT_HPP_

#include <type_traits>

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief EqualsAnyConstant is an overloaded family of function templates that
 *        simply see if a runtime 'check' value is equal to any number of
 *        compile-time constants (which are given as template parameters).
 * @note Obviously there are a number of ways to accomplish such a simple task.
 *       Each of them has some problems, however:
 *         1. A series of if/else statements - very verbose and pontentially
 *            slow for a large number of constants (although GCC and Clang are
 *            smart enough to collapse this down into a jump table at -O3).
 *         2. A switch statement - potentially faster than if/else statements,
 *            because the compiler will convert it to a jump table if there are
 *            more than a few cases. Still verbose, though.
 *         3. Looping over an array of constants. Less verbose (particularly if
 *            using a standard-library template from <algorithm> like
 *            std::find), but just as slow if not worse than the if/else
 *            statements.
 *       The approach here is to have the concise invocations of
 *       EqualsAnyConstant expand out to switch statements for reasonable
 *       numbers of constants (currently 8 or less). This gives us the
 *       efficiency of option #2 with conciseness even better than option #3.
 *       For larger numbers of constants, a variadic template is included that
 *       will (at compile time, not run time) recursively expand into switch
 *       statements that each cover up to 8 constants a piece (a smart compiler
 *       will even collapse them down into a single jump table; GCC and Clang
 *       do, at the very least).
 **/
template <typename T>
inline bool EqualsAnyConstant(const T check) {
  return false;
}

template <typename T, T c1>
inline bool EqualsAnyConstant(const T check) {
  return check == c1;
}

template <typename T, T c1, T c2>
inline bool EqualsAnyConstant(const T check) {
  switch (check) {
    case c1:
    case c2:
      return true;
    default:
      return false;
  }
}

template <typename T, T c1, T c2, T c3>
inline bool EqualsAnyConstant(const T check) {
  switch (check) {
    case c1:
    case c2:
    case c3:
      return true;
    default:
      return false;
  }
}


template <typename T, T c1, T c2, T c3, T c4>
inline bool EqualsAnyConstant(const T check) {
  switch (check) {
    case c1:
    case c2:
    case c3:
    case c4:
      return true;
    default:
      return false;
  }
}

template <typename T, T c1, T c2, T c3, T c4, T c5>
inline bool EqualsAnyConstant(const T check) {
  switch (check) {
    case c1:
    case c2:
    case c3:
    case c4:
    case c5:
      return true;
    default:
      return false;
  }
}

template <typename T, T c1, T c2, T c3, T c4, T c5, T c6>
inline bool EqualsAnyConstant(const T check) {
  switch (check) {
    case c1:
    case c2:
    case c3:
    case c4:
    case c5:
    case c6:
      return true;
    default:
      return false;
  }
}

template <typename T, T c1, T c2, T c3, T c4, T c5, T c6, T c7>
inline bool EqualsAnyConstant(const T check) {
  switch (check) {
    case c1:
    case c2:
    case c3:
    case c4:
    case c5:
    case c6:
    case c7:
      return true;
    default:
      return false;
  }
}

template <typename T, T c1, T c2, T c3, T c4, T c5, T c6, T c7, T c8, T ...tail_constants>
bool EqualsAnyConstant(const T check) {
  switch (check) {
    case c1:
    case c2:
    case c3:
    case c4:
    case c5:
    case c6:
    case c7:
    case c8:
      return true;
    default:
      return EqualsAnyConstant<T, tail_constants...>(check);
  }
}

/**
 * @brief Helper macro for EqualsAnyConstant that adds a little bit of
 *        syntactic sugar to deduce the type of the checked expression.
 **/
#define QUICKSTEP_EQUALS_ANY_CONSTANT(check, ...)                                             \
  (EqualsAnyConstant<                                                                         \
       typename std::remove_reference<typename std::remove_cv<decltype(check)>::type>::type,  \
       __VA_ARGS__ >(check))

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_EQUALS_ANY_CONSTANT_HPP_
