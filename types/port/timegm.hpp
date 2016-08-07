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

#ifndef QUICKSTEP_TYPES_PORT_TIMEGM_HPP_
#define QUICKSTEP_TYPES_PORT_TIMEGM_HPP_

#include <ctime>

#include "types/port/TypesPortConfig.h"
#include "utility/Macros.hpp"

namespace quickstep {

#if defined(QUICKSTEP_HAVE_TIMEGM)

/**
 * @brief timegm implementation which uses system's timegm implementation.
 *
 * @param tm Broken-time struct.
 *
 * @return Seconds since epoch.
 **/
inline std::time_t timegmWithOriginal(struct tm *tm) {
  return ::timegm(tm);
}
#endif  // defined(QUICKSTEP_HAVE_TIMEGM)

#if defined(QUICKSTEP_HAVE_SETENV) && defined(QUICKSTEP_HAVE_TZSET)
// This is statically initialized before main() in timegm.cpp
extern const bool kGMTSetup;

/**
 * @brief timegm implementation sets TZ to GMT and invokes std::mktime.
 *
 * @param tm Broken-time struct.
 *
 * @return Seconds since epoch.
 **/
inline std::time_t timegmWithTZGMT(struct tm *tm) {
  // Reference kGMTSetup here so that any executable that links in timegm will
  // call SetupGMT() before main().
  if (kGMTSetup) {
    return std::mktime(tm);
  } else {
    FATAL_ERROR("SetupGMT() did not run at program startup.");
  }
}
#endif  // defined(QUICKSTEP_HAVE_SETENV) && defined(QUICKSTEP_HAVE_TZSET)

// This is statically initialized before main() in timegm.cpp
extern const std::time_t kLocalTimeOffset;

/**
 * @brief timegm implementation which pre-computes the time zone offset and uses
 *        std::mktime.
 *
 * @param tm Broken-time struct.
 *
 * @return Seconds since epoch.
 **/
inline std::time_t timegmWithPrecomputedOffset(struct tm *tm) {
  return std::mktime(tm) + kLocalTimeOffset;
}

/**
 * @brief Custom timegm implementation which avoids performance issues with
 *        system implementation.
 *
 * @param tm Broken-time struct.
 *
 * @return Seconds since epoch.
 *
 * @note This implementation does not correct values of broken-time values out
 *       of range, unlike the timegm() / mktime() implementation. Also, this
 *       implementation does not do any bounds checking, i.e., return -1 when
 *       the broken-time given can not be represented using std::time_t.
 **/
std::time_t timegmCustom(struct tm *tm);

/**
 * @brief System-based timegm implementation, i.e., uses some system implemented
 * method to convert broken-time to seconds since epoch.
 *
 * @param tm Broken-time struct.
 *
 * @return Seconds since epoch.
 **/
inline std::time_t timegmSystemImplemented(struct tm *tm) {
#if defined(QUICKSTEP_HAVE_TIMEGM)
  return timegmWithOriginal(tm);
#elif defined(QUICKSTEP_HAVE_SETENV) && defined(QUICKSTEP_HAVE_TZSET)
  return timegmWithTZGMT(tm);
#else
  return timegmWithPrecomputedOffset(tm);
#endif
}

/**
 * @brief Converts broken-time to seconds since epoch in GMT.
 *
 * @param tm Broken-time struct.
 *
 * @return Seconds since epoch.
 **/
inline std::time_t timegm(struct tm *tm) {
  return timegmCustom(tm);
}

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_PORT_TIMEGM_HPP_
