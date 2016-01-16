// Copyright (c) 2014-2015, Quickstep Technologies LLC.
// Copyright (c) 2015, Pivotal Software, Inc.
// All Rights Reserved.

#ifndef TMBBENCH_THREAD_H_
#define TMBBENCH_THREAD_H_

#include <thread>  // NOLINT(build/c++11)
#include <vector>

namespace tmbbench {

// Object-oriented wrapper for std::thread.
class Thread {
 public:
  explicit Thread(const std::vector<int> &cpu_affinity)
      : cpu_affinity_(cpu_affinity) {
  }

  virtual ~Thread() {
  }

  void Start() {
    internal_thread_ = std::thread(Thread::ExecuteRunMethodOfThread, this);
  }

  void Join() {
    internal_thread_.join();
  }

 protected:
  static void ExecuteRunMethodOfThread(Thread* thread) {
    thread->Affinitize();
    thread->Run();
  }

  virtual void Run() = 0;

 private:
  void Affinitize();

  std::thread internal_thread_;
  const std::vector<int> cpu_affinity_;

  // Disallow copy and assign:
  Thread(const Thread &orig) = delete;
  Thread& operator=(const Thread &rhs) = delete;
};

}  // namespace tmbbench

#endif  // TMBBENCH_THREAD_H_
