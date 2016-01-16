/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_UTILITY_CHECK_SNPRINTF_HPP_
#define QUICKSTEP_UTILITY_CHECK_SNPRINTF_HPP_

#include <cstddef>

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Do DEBUG-mode checks after a call to snprintf() to make sure that it
 *        succeeded. Checks that snprintf() didn't return an error code, and
 *        that the total string length would not have overflowed a buffer's
 *        size, causing snprintf() to stop.
 *
 * @param snprintf_result The value returned by a call to snprintf().
 * @param buffer_size The size of the buffer used by snprintf().
 * @param total_written A pointer to a count of the total number of characters
 *        written into the buffer used by snprintf(). This function will
 *        add the number of characters successfully written by snprintf() to
 *        *total_written.
 **/
inline void CheckSnprintf(const int snprintf_result,
                          const std::size_t buffer_size,
                          std::size_t *total_written) {
  DCHECK_GE(snprintf_result, 0) << "snprintf() encountered an encoding error";
  *total_written += snprintf_result;
  DCHECK_LT(*total_written, buffer_size)
      << "snprintf() would have written a string of length " << *total_written
      << " (plus null-terminator), but buffer size is only " << buffer_size;
}

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_CHECK_SNPRINTF_HPP_
