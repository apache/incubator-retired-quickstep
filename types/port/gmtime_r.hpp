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
