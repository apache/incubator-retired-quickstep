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

#ifndef TMB_MEMORY_MIRROR_MESSAGE_BUS_H_
#define TMB_MEMORY_MIRROR_MESSAGE_BUS_H_

#include <cstdint>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/memory_based_message_bus.h"
#include "tmb/internal/net_message_removal_interface.h"
#include "tmb/internal/persistent_bus_state_interface.h"

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
 * @brief An implementation of MessageBus that caches its complete state in
 *        memory, but persists all transactions to durable underlying storage.
 * @note MemoryMirrorMessageBus is threadsafe so long as the same client does
 *       not make concurrent calls to methods of MemoryMirrorMessageBus
 *       (concurrent calls from different clients are OK).
 **/
template <bool enable_deletion_support>
class MemoryMirrorMessageBus
    : public internal::MemoryBasedMessageBus<enable_deletion_support, true>,
      public internal::NetMessageRemovalInterface {
 public:
  /**
   * @brief Constructor.
   *
   * @param underlying_bus The underlying MessageBus to provide a memory mirror
   *        in front of. The MemoryMirrorMessageBus takes ownership of
   *        underlying_bus.
   * @param enable_net_support Enable extra bookkeeping functionality needed to
   *        serve as a backend for NetServiceImpl (unless you are dealing with
   *        the internals of TMB networking, you can safely ignore this and
   *        leave it as false, which is the default).
   **/
  explicit MemoryMirrorMessageBus(
      internal::PersistentBusStateInterface *underlying_bus,
      const bool enable_net_support = false)
      : internal::MemoryBasedMessageBus<enable_deletion_support, true>(
            enable_net_support),
        underlying_bus_(underlying_bus) {
  }

  virtual ~MemoryMirrorMessageBus() {
  }

  bool Initialize() override;

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
                                                   true>::ClientHandle
          ClientHandle;

  typedef typename internal::MemoryBasedMessageBus<enable_deletion_support,
                                                   true>::ClientMap
          ClientMap;
  typedef typename internal::MemoryBasedMessageBus<enable_deletion_support,
                                                   true>::ReceiverDirectory
          ReceiverDirectory;
  typedef typename internal::MemoryBasedMessageBus<enable_deletion_support,
                                                   true>::RegistrationState
          RegistrationState;

  std::unique_ptr<internal::PersistentBusStateInterface> underlying_bus_;

  // Disallow copy and assign:
  MemoryMirrorMessageBus(const MemoryMirrorMessageBus &orig) = delete;
  MemoryMirrorMessageBus& operator=(const MemoryMirrorMessageBus &rhs)
      = delete;
};

/** @} */

}  // namespace tmb

#endif  // TMB_MEMORY_MIRROR_MESSAGE_BUS_H_
