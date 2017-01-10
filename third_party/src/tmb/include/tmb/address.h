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

#ifndef TMB_ADDRESS_H_
#define TMB_ADDRESS_H_

#include <vector>

#include "tmb/id_typedefs.h"

namespace tmb {

namespace internal {
template <bool enable_deletion_support, bool memory_mirror>
    class MemoryBasedMessageBus;  // NOLINT(runtime/indentation_namespace)
}  // namespace internal

/** \addtogroup TMB
 *  @{
 */

/**
 * @brief An object used by a sender to indicate which receivers should receive
 *        a particular message. Also see MessageStyle, which indicates how
 *        messages should be delivered (particularly when more than one
 *        recipient is specified).
 **/
class Address {
 public:
  /**
   * @brief Constructor. Creates an empty address with no recipients.
   **/
  Address()
      : send_to_all_(false) {
  }

  /**
   * @brief Add a particular recipient to the set of recipients for the
   *        message.
   * @warning It is an error to add the same recipient to an Address twice,
   *          which may cause unexpected behavior.
   * @note This only has an effect if send-to-all mode is disabled. See All()
   *       for more information.
   *
   * @param recipient_id The client_id of the recipient.
   * @return This Address (this is a fluent modifier method).
   **/
  inline Address& AddRecipient(const client_id recipient_id) {
    explicit_recipients_.push_back(recipient_id);
    return *this;
  }

  /**
   * @brief Clear any recipients specified by calls to AddRecipient() and
   *        return this address to an empty state.
   *
   * @return This Address (this is a fluent modifier method).
   **/
  inline Address& Clear() {
    explicit_recipients_.clear();
    return *this;
  }

  /**
   * @brief Enable or disable send-to-all mode. If send-to-all mode is
   *        disabled, then the message will be delivered only to recipients
   *        explicitly specified by calls to AddRecipient(). If send-to-all
   *        mode is enabled, the message will be delivered to ALL connected
   *        receivers which are able to receive the message.
   *
   * @param all Whether send-to-all mode should be used.
   * @return This Address (this is a fluent modifier method).
   **/
  inline Address& All(const bool all) {
    send_to_all_ = all;
    return *this;
  }

 private:
  friend class internal::MemoryBasedMessageBus<false, false>;
  friend class internal::MemoryBasedMessageBus<false, true>;
  friend class internal::MemoryBasedMessageBus<true, false>;
  friend class internal::MemoryBasedMessageBus<true, true>;
  friend class LevelDBMessageBus;
  friend class NativeNetClientMessageBus;
  friend class SQLiteMessageBus;
  friend class VoltDBMessageBus;
  friend class ZookeeperMessageBus;

  bool send_to_all_;
  std::vector<client_id> explicit_recipients_;
};

/** @} */

}  // namespace tmb

#endif  // TMB_ADDRESS_H_
