// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

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
