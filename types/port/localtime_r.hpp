/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_TYPES_PORT_LOCALTIME_R_HPP_
#define QUICKSTEP_TYPES_PORT_LOCALTIME_R_HPP_

#include <ctime>

#include "types/port/TypesPortConfig.h"
#include "utility/Macros.hpp"

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
