// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

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
