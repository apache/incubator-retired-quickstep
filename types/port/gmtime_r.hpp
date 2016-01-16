/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_PORT_GMTIME_R_HPP_
#define QUICKSTEP_TYPES_PORT_GMTIME_R_HPP_

#include <ctime>

#include "types/port/TypesPortConfig.h"

#if defined(QUICKSTEP_HAVE_GMTIME_S)
#include "utility/Macros.hpp"
#endif  // QUICKSTEP_HAVE_GMTIME_S

namespace quickstep {

#if defined(QUICKSTEP_HAVE_GMTIME_R)
inline struct tm* gmtime_r(const std::time_t *timep, struct tm *result) {
  return ::gmtime_r(timep, result);
}
#elif defined(QUICKSTEP_HAVE_GMTIME_S)
inline struct tm* gmtime_r(const std::time_t *timep, struct tm *result) {
  DO_AND_DEBUG_ASSERT_ZERO(gmtime_s(result, timep));
  return result;
}
#endif

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_PORT_GMTIME_R_HPP_
