//   Copyright 2014-2015 Quickstep Technologies LLC.
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

#ifndef TMBBENCH_SENDER_THREAD_H_
#define TMBBENCH_SENDER_THREAD_H_

#include <chrono>  // NOLINT(build/c++11)
#include <cstddef>
#include <vector>

#include "tmb/id_typedefs.h"
#include "tmbbench/thread.h"

namespace tmb { class MessageBus; }

namespace tmbbench {

class SenderThread : public Thread {
 public:
  static constexpr std::size_t kPoisonWindowMask = 0x0FFF;

  SenderThread(tmb::MessageBus *message_bus,
               const std::size_t message_bytes,
               const std::vector<int> &cpu_affinity);

  ~SenderThread() override {
  }

  void AddReceiverID(const tmb::client_id receiver_id) {
    receiver_ids_.push_back(receiver_id);
  }

  void SetReceiverIDs(const std::vector<tmb::client_id> &receiver_ids) {
    receiver_ids_ = receiver_ids;
  }

  tmb::client_id GetClientID() const {
    return me_;
  }

  double GetThroughput() const {
    return static_cast<double>(sent_) / static_cast<double>(elapsed_.count());
  }

 protected:
  void Run() override;

 private:
  tmb::MessageBus *message_bus_;
  const std::size_t message_bytes_;
  tmb::client_id me_;
  std::vector<tmb::client_id> receiver_ids_;

  std::size_t sent_;
  std::chrono::duration<double> elapsed_;

  // Disallow copy and assign:
  SenderThread(const SenderThread &orig) = delete;
  SenderThread& operator=(const SenderThread &rhs) = delete;
};

}  // namespace tmbbench

#endif  // TMBBENCH_SENDER_THREAD_H_
