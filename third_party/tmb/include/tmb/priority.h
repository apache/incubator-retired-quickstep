//   Copyright 2014-2015 Quickstep Technologies LLC.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#ifndef TMB_PRIORITY_H_
#define TMB_PRIORITY_H_

#include <cstdint>
#include <limits>

namespace tmb {

/** \addtogroup TMB
 *  @{
 */

/**
 * @brief A priority level for a message. Higher-priority messages are
 *        delivered before those with lower priority.
 **/
typedef std::uint8_t Priority;

/**
 * @brief A special priority level used for "synchronous" responses that bypass
 *        any pending asynchronous messages.
 **/
const Priority kSyncResponsePriority = std::numeric_limits<Priority>::max();

/**
 * @brief The highest priority level for normal messages.
 **/
const Priority kMaxAsyncPriority = kSyncResponsePriority - 1;

/**
 * @brief The default "neutral" priority level.
 **/
const Priority kDefaultPriority = 1 << 7;  // 128

/** @} */

namespace internal {

const std::int16_t kAboveMaxPriority
    = static_cast<std::int16_t>(std::numeric_limits<Priority>::max()) + 1;

const std::int16_t kBelowMinPriority
    = static_cast<std::int16_t>(std::numeric_limits<Priority>::min()) - 1;

}  // namespace internal

}  // namespace tmb

#endif  // TMB_PRIORITY_H_
