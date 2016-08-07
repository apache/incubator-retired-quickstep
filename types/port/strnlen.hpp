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

#ifndef QUICKSTEP_TYPES_PORT_STRNLEN_HPP_
#define QUICKSTEP_TYPES_PORT_STRNLEN_HPP_

#include <cstddef>
#include <cstring>

#include "types/port/TypesPortConfig.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

#ifdef QUICKSTEP_HAVE_STRNLEN
inline std::size_t strnlen(const char *c, const std::size_t maxlen) {
  return ::strnlen(c, maxlen);
}
#else
inline std::size_t strnlen(const char *c, const std::size_t maxlen) {
  const char *loc = static_cast<const char*>(std::memchr(c, '\0', maxlen));
  if (loc == nullptr) {
    return maxlen;
  } else {
    return (loc - c);
  }
}
#endif

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_PORT_STRNLEN_HPP_
