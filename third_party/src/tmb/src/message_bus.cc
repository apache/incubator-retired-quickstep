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

#include "tmb/message_bus.h"

#include <chrono>  // NOLINT(build/c++11)
#include <cstdio>
#include <cstdlib>
#include <thread>  // NOLINT(build/c++11)

#include "gflags/gflags.h"

namespace tmb {

static bool ValidateTmbReceivePollInterval(const char *flagname,
                                           std::int32_t value) {
  if (value > 0) {
    return true;
  } else {
    std::fprintf(stderr, "--%s must be at least 1\n", flagname);
    return false;
  }
}
DEFINE_int32(tmb_receive_poll_interval, 50,
             "The number of milliseconds to sleep between calls to ReceiveIfAvailableImpl() "
             "in the default active-polling implementation of ReceiveImpl().");
static const bool tmb_receive_poll_interval_dummy = gflags::RegisterFlagValidator(
    &FLAGS_tmb_receive_poll_interval,
    &ValidateTmbReceivePollInterval);

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
        std::chrono::milliseconds(FLAGS_tmb_receive_poll_interval));
    received = ReceiveIfAvailableImpl(receiver_id,
                                      minimum_priority,
                                      max_messages,
                                      delete_immediately,
                                      pusher);
  }

  return received;
}

}  // namespace tmb
