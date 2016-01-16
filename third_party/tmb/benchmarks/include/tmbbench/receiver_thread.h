// Copyright (c) 2014-2015, Quickstep Technologies LLC.
// Copyright (c) 2015, Pivotal Software, Inc.
// All Rights Reserved.

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
