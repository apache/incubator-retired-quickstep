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

#ifndef TMB_NATIVE_LOGGING_MESSAGE_BUS_H_
#define TMB_NATIVE_LOGGING_MESSAGE_BUS_H_

#include <string>

#include "tmb/memory_mirror_message_bus.h"

#include "tmb/internal/native_transaction_log.h"

namespace tmb {

/** \addtogroup TMB
 *  @{
 */

/**
 * @brief An implementation of MessageBus that caches its complete state in
 *        memory, but persists all transactions to the TMB library's native
 *        write-ahead logging format.
 * @note This is simply a convenience wrapper for a MemoryMirrorMessageBus
 *       backed by a NativeTransactionLog.
 **/

template <bool enable_deletion_support>
class NativeLoggingMessageBus
    : public MemoryMirrorMessageBus<enable_deletion_support> {
 public:
  explicit NativeLoggingMessageBus(const std::string &log_filename,
                                   const bool sync_writes = true,
                                   const bool enable_net_support = false)
      : MemoryMirrorMessageBus<enable_deletion_support>(
          new internal::NativeTransactionLog(log_filename, sync_writes),
          enable_net_support) {
  }

  ~NativeLoggingMessageBus() {
  }

 private:
  // Disallow copy and assign:
  NativeLoggingMessageBus(const NativeLoggingMessageBus &orig) = delete;
  NativeLoggingMessageBus& operator=(const NativeLoggingMessageBus &rhs)
      = delete;
};

/** @} */

}  // namespace tmb

#endif  // TMB_NATIVE_LOGGING_MESSAGE_BUS_H_
