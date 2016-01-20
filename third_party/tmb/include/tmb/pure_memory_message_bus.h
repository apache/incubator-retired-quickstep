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

#ifndef TMB_PURE_MEMORY_MESSAGE_BUS_H_
#define TMB_PURE_MEMORY_MESSAGE_BUS_H_

#include <atomic>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/cache_info.h"
#include "tmb/internal/memory_based_message_bus.h"
#include "tmb/internal/net_message_removal_interface.h"

namespace tmb {

namespace internal {
class ContainerPusher;
template <typename T> class IteratorAdapter;
}  // namespace internal

class Address;
class CancellationToken;
class MessageStyle;

/** \addtogroup TMB
 *  @{
 */

/**
 * @brief A pure-memory implementation of MessageBus. This implements the full
 *        MessageBus interface, but data is not persistent in any underlying
 *        storage.
 * @note PureMemoryMessageBus is threadsafe so long as the same client does not
 *       make concurrent calls to methods of PureMemoryMessageBus (concurrent
 *       calls from different clients are OK).
 **/
template <bool enable_deletion_support>
class PureMemoryMessageBus
    : public internal::MemoryBasedMessageBus<enable_deletion_support, false>,
      public internal::NetMessageRemovalInterface {
 public:
  /**
   * @brief Constructor.
   *
   * @param enable_net_support Enable extra bookkeeping functionality needed to
   *        serve as a backend for NetServiceImpl (unless you are dealing with
   *        the internals of TMB networking, you can safely ignore this and
   *        leave it as false, which is the default).
   **/
  explicit PureMemoryMessageBus(const bool enable_net_support = false)
      : internal::MemoryBasedMessageBus<enable_deletion_support, false>(
            enable_net_support),
        client_id_sequence_(0),
        message_id_sequence_(-1) {
  }

  ~PureMemoryMessageBus() {
  }

  bool Initialize() override {
    return true;
  }

  void ResetBus() override;

  client_id Connect() override;

  bool Disconnect(const client_id client) override;

  bool RegisterClientAsSender(const client_id sender_id,
                              const message_type_id message_type) override;

  bool RegisterClientAsReceiver(const client_id receiver_id,
                                const message_type_id message_type) override;

  MessageBus::SendStatus Send(
      const client_id sender_id,
      const Address &destination_address,
      const MessageStyle &style,
      TaggedMessage &&message,  // NOLINT(whitespace/operators)
      const Priority priority = kDefaultPriority,
      CancellationToken *cancellation_token = nullptr) override;

  void CancelMessage(const client_id sender_id,
                     const CancellationToken &cancellation_token) override;

  bool SupportsNetMessageRemovalInterface() const override {
    return this->net_support_enabled_;
  }

  internal::NetMessageRemovalInterface* GetNetMessageRemovalInterface()
      override {
    return this;
  }

  // NetMessageRemovalInterface implementations:
  void DeleteById(
      const client_id receiver_id,
      const std::vector<std::int64_t> &message_ids) override;

  void SenderCancelById(
      const client_id sender_id,
      const std::vector<std::int64_t> &message_ids) override;

  void ReceiverCancelById(
      const client_id receiver_id,
      const std::vector<std::int64_t> &message_ids) override;

 protected:
  std::size_t ReceiveImpl(const client_id receiver_id,
                          const Priority minimum_priority,
                          const std::size_t max_messages,
                          const bool delete_immediately,
                          internal::ContainerPusher *pusher) override;

  std::size_t ReceiveIfAvailableImpl(const client_id receiver_id,
                                     const Priority minimum_priority,
                                     const std::size_t max_messages,
                                     const bool delete_immediately,
                                     internal::ContainerPusher *pusher)
                                         override;

  void DeleteImpl(const client_id receiver_id,
                  internal::IteratorAdapter<const AnnotatedMessage> *adapter)
                      override;

  void CancelImpl(const client_id receiver_id,
                  internal::IteratorAdapter<const AnnotatedMessage> *adapter)
                      override;

 private:
  typedef MessageBus::SendStatus SendStatus;
  typedef typename internal::MemoryBasedMessageBus<enable_deletion_support,
                                                   false>::ClientHandle
          ClientHandle;

  typedef typename internal::MemoryBasedMessageBus<enable_deletion_support,
                                                   false>::ClientMap
          ClientMap;
  typedef typename internal::MemoryBasedMessageBus<enable_deletion_support,
                                                   false>::ReceiverDirectory
          ReceiverDirectory;
  typedef typename internal::MemoryBasedMessageBus<enable_deletion_support,
                                                   false>::RegistrationState
          RegistrationState;

  std::atomic<client_id> client_id_sequence_;

  // This counts downwards through negative numbers.
  alignas(internal::kCacheLineBytes) std::atomic<std::int64_t>
      message_id_sequence_;

  // Disallow copy and assign:
  PureMemoryMessageBus(const PureMemoryMessageBus &orig) = delete;
  PureMemoryMessageBus& operator=(const PureMemoryMessageBus &rhs) = delete;
};

/** @} */

}  // namespace tmb

#endif  // TMB_PURE_MEMORY_MESSAGE_BUS_H_
