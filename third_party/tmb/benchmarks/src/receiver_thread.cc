//   Copyright 2015 Pivotal Software, Inc.
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

#include "tmbbench/receiver_thread.h"

#include <chrono>  // NOLINT(build/c++11)
#include <cstddef>
#include <vector>

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"
#include "tmbbench/messages.h"

namespace tmbbench {

ReceiverThreadBase::ReceiverThreadBase(tmb::MessageBus *message_bus,
                                       const std::vector<int> &cpu_affinity)
    : Thread(cpu_affinity),
      message_bus_(message_bus),
      received_(0) {
  me_ = message_bus_->Connect();
  message_bus_->RegisterClientAsReceiver(me_, 0);
  message_bus_->RegisterClientAsReceiver(me_, 1);
}

template <bool delete_immediately>
void ReceiverThread<delete_immediately>::Run() {
  std::vector<tmb::AnnotatedMessage> received_batch;
  bool running = true;
  std::chrono::time_point<std::chrono::high_resolution_clock> start
      = std::chrono::high_resolution_clock::now();
  while (running) {
    received_ += message_bus_->ReceiveBatch(me_,
                                            &received_batch,
                                            0,
                                            0,
                                            delete_immediately);
    for (const tmb::AnnotatedMessage &received : received_batch) {
      if ((received.tagged_message.message_type() == 1)
          && (received.tagged_message.message_bytes() == sizeof(size_t))
          && (*static_cast<const std::size_t*>(
                  received.tagged_message.message()) == kPoisonMessage)) {
        running = false;
        break;
      }
    }
    if (!delete_immediately) {
      message_bus_->DeleteMessages(me_,
                                   received_batch.begin(),
                                   received_batch.end());
    }
    received_batch.clear();
  }
  std::chrono::time_point<std::chrono::high_resolution_clock> end
      = std::chrono::high_resolution_clock::now();
  elapsed_ = end - start;

  // Disconnect.
  message_bus_->Disconnect(me_);
}

template class ReceiverThread<false>;
template class ReceiverThread<true>;

}  // namespace tmbbench
