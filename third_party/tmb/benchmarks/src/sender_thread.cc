// Copyright (c) 2015, Pivotal Software, Inc.
// All Rights Reserved.

#include "tmbbench/sender_thread.h"

#include <chrono>  // NOLINT(build/c++11)
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <random>
#include <vector>

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/tagged_message.h"
#include "tmbbench/messages.h"
#include "tmbbench/thread.h"

namespace tmbbench {

SenderThread::SenderThread(tmb::MessageBus *message_bus,
                           const std::size_t message_bytes,
                           const std::vector<int> &cpu_affinity)
    : Thread(cpu_affinity),
      message_bus_(message_bus),
      message_bytes_(message_bytes),
      sent_(0) {
  me_ = message_bus_->Connect();
  message_bus_->RegisterClientAsSender(me_, 0);
  message_bus_->RegisterClientAsReceiver(me_, 1);
}

void SenderThread::Run() {
  tmb::Address address;
  tmb::MessageStyle style;
  void *message_buffer = std::malloc(message_bytes_);
  std::memset(message_buffer, 0, message_bytes_);

  // Use a fast subtract-with-carry engine to randomly choose receiver
  // addresses. Use the output of a hardware RNG (if available) as seed.
  std::random_device hw_rand;
  std::ranlux24_base random_engine(hw_rand());
  std::uniform_int_distribution<std::vector<tmb::client_id>::size_type>
      receiver_distribution(0, receiver_ids_.size() - 1);

  std::chrono::time_point<std::chrono::high_resolution_clock> start
      = std::chrono::high_resolution_clock::now();
  for (;;) {
    if ((sent_ & kPoisonWindowMask) == 0) {
      tmb::AnnotatedMessage received;
      bool finishing = false;
      while (message_bus_->ReceiveIfAvailable(me_, &received)) {
        if ((received.tagged_message.message_type() == 1)
            && (received.tagged_message.message_bytes() == sizeof(size_t))
            && (*static_cast<const std::size_t*>(
                    received.tagged_message.message()) == kPoisonMessage)) {
          finishing = true;
          break;
        }
      }
      if (finishing) {
        break;
      }
    }

    address.Clear().AddRecipient(
        receiver_ids_[receiver_distribution(random_engine)]);

    *static_cast<std::size_t*>(message_buffer) = sent_;
    tmb::TaggedMessage message(message_buffer, message_bytes_, 0);

    message_bus_->Send(me_, address, style, std::move(message));
    ++sent_;
  }
  std::chrono::time_point<std::chrono::high_resolution_clock> end
      = std::chrono::high_resolution_clock::now();
  elapsed_ = end - start;

  // Disconnect.
  message_bus_->Disconnect(me_);
  std::free(message_buffer);
}

}  // namespace tmbbench
