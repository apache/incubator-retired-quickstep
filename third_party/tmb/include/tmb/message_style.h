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

#ifndef TMB_MESSAGE_STYLE_H_
#define TMB_MESSAGE_STYLE_H_

#include <chrono>  // NOLINT(build/c++11)

namespace tmb {

template <bool enable_deletion_support> class MemoryMirrorMessageBus;
template <bool enable_deletion_support> class PureMemoryMessageBus;

namespace internal {
template <bool enable_deletion_support, bool memory_mirror>
    class MemoryBasedMessageBus;  // NOLINT(runtime/indentation_namespace)
}  // namespace internal

/** \addtogroup TMB
 *  @{
 */

/**
 * @brief An object used by a sender to indicate the style of delivery which
 *        should be used for a particular message.
 **/
class MessageStyle {
 public:
  /**
   * @brief Constructor. Creates a default MessageStyle with broadcast turned
   *        off, and with no timeout.
   **/
  MessageStyle()
      : broadcast_(false) {
  }

  /**
   * @brief Indicate that broadcast-style messaging should be used. If enabled,
   *        all possible recipients of a message (as indicated by an Address)
   *        will receive a copy of it. If disabled, any one (but only one)
   *        recipient will receive it.
   *
   * @param broadcast Whether broadcast should be enabled.
   * @return This MessageStyle (this is a fluent modifier method).
   **/
  MessageStyle& Broadcast(const bool broadcast) {
    broadcast_ = broadcast;
    return *this;
  }

  /**
   * @brief Set a timeout for a message. If a timeout is set, then a message
   *        will not be delivered to any recipient after the specified
   *        expiration_time.
   * @note This method does not place any restriction on how long a receiver
   *       may take to process a message and possibly respond to it.
   *
   * @param expiration_time A time point after which a message should not be
   *        delivered to any receiver. An expiration_time equal to the epoch
   *        indicates no timeout.
   * @return This MessageStyle (this is a fluent modifier method).
   **/
  MessageStyle& Timeout(
      const std::chrono::time_point<std::chrono::high_resolution_clock>
          &expiration_time) {
    expiration_time_ = expiration_time;
    return *this;
  }

 private:
  friend class internal::MemoryBasedMessageBus<false, false>;
  friend class internal::MemoryBasedMessageBus<false, true>;
  friend class internal::MemoryBasedMessageBus<true, false>;
  friend class internal::MemoryBasedMessageBus<true, true>;
  friend class LevelDBMessageBus;
  friend class MemoryMirrorMessageBus<false>;
  friend class MemoryMirrorMessageBus<true>;
  friend class NativeNetClientMessageBus;
  friend class PureMemoryMessageBus<false>;
  friend class PureMemoryMessageBus<true>;
  friend class SQLiteMessageBus;
  friend class VoltDBMessageBus;
  friend class ZookeeperMessageBus;

  bool broadcast_;

  // expiration_time_ set to the epoch indicates no timeout.
  std::chrono::time_point<std::chrono::high_resolution_clock> expiration_time_;
};

/** @} */

}  // namespace tmb

#endif  // TMB_MESSAGE_STYLE_H_
