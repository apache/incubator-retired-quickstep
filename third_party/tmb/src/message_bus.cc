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

#include "tmb/message_bus.h"

#include <chrono>  // NOLINT(build/c++11)
#include <cstdio>
#include <cstdlib>
#include <thread>  // NOLINT(build/c++11)

namespace tmb {

const unsigned int MessageBus::kReceivePollIntervalMS;

internal::NetMessageRemovalInterface*
    MessageBus::GetNetMessageRemovalInterface() {
  std::fprintf(stderr,
               "FATAL: TMB internal error. Called "
               "GetNetMessageRemovalInterface() on a MessageBus that does not "
               "support it.");
  std::exit(1);
}

std::size_t MessageBus::ReceiveImpl(const client_id receiver_id,
                                    const Priority minimum_priority,
                                    const std::size_t max_messages,
                                    const bool delete_immediately,
                                    internal::ContainerPusher *pusher) {
  std::size_t received = ReceiveIfAvailableImpl(receiver_id,
                                                minimum_priority,
                                                max_messages,
                                                delete_immediately,
                                                pusher);
  while (received == 0) {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(kReceivePollIntervalMS));
    received = ReceiveIfAvailableImpl(receiver_id,
                                      minimum_priority,
                                      max_messages,
                                      delete_immediately,
                                      pusher);
  }

  return received;
}

}  // namespace tmb
