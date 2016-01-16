/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_PORT_LOCALTIME_R_HPP_
#define QUICKSTEP_TYPES_PORT_LOCALTIME_R_HPP_

#include <ctime>

#include "types/port/TypesPortConfig.h"

#if defined(QUICKSTEP_HAVE_LOCALTIME_S)
#include "utility/Macros.hpp"
#endif  // QUICKSTEP_HAVE_LOCALTIME_S

namespace quickstep {

#if defined(QUICKSTEP_HAVE_LOCALTIME_R)
inline struct tm* localtime_r(const std::time_t *timep, struct tm *result) {
  return ::localtime_r(timep, result);
}
#elif defined(QUICKSTEP_HAVE_LOCALTIME_S)
inline struct tm* localtime_r(const std::time_t *timep, struct tm *result) {
  DO_AND_DEBUG_ASSERT_ZERO(localtime_s(result, timep));
  return result;
}
#endif

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_PORT_LOCALTIME_R_HPP_
