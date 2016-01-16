/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
