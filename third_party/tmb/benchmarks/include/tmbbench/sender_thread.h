// Copyright (c) 2014-2015, Quickstep Technologies LLC.
// Copyright (c) 2015, Pivotal Software, Inc.
// All Rights Reserved.

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
