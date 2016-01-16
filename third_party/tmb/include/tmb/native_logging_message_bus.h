// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

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
