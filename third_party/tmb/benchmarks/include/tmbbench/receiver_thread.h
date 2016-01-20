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

#ifndef TMBBENCH_RECEIVER_THREAD_H_
#define TMBBENCH_RECEIVER_THREAD_H_

#include <chrono>  // NOLINT(build/c++11)
#include <cstddef>
#include <vector>

#include "tmb/id_typedefs.h"
#include "tmbbench/thread.h"

namespace tmb { class MessageBus; }

namespace tmbbench {

class ReceiverThreadBase : public Thread {
 public:
  ReceiverThreadBase(tmb::MessageBus *message_bus,
                     const std::vector<int> &cpu_affinity);

  virtual ~ReceiverThreadBase() {
  }

  tmb::client_id GetClientID() const {
    return me_;
  }

  double GetThroughput() const {
    return static_cast<double>(received_)
           / static_cast<double>(elapsed_.count());
  }

 protected:
  tmb::MessageBus *message_bus_;
  tmb::client_id me_;

  std::size_t received_;
  std::chrono::duration<double> elapsed_;

 private:
  // Disallow copy and assign:
  ReceiverThreadBase(const ReceiverThreadBase &orig) = delete;
  ReceiverThreadBase& operator=(const ReceiverThreadBase &rhs) = delete;
};

template <bool delete_immediately>
class ReceiverThread : public ReceiverThreadBase {
 public:
  ReceiverThread(tmb::MessageBus *message_bus,
                 const std::vector<int> &cpu_affinity)
      : ReceiverThreadBase(message_bus, cpu_affinity) {
  }

  ~ReceiverThread() override {
  }

 protected:
  void Run() override;

 private:
  // Disallow copy and assign:
  ReceiverThread(const ReceiverThread &orig) = delete;
  ReceiverThread& operator=(const ReceiverThread &rhs) = delete;
};

}  // namespace tmbbench

#endif  // TMBBENCH_RECEIVER_THREAD_H_
