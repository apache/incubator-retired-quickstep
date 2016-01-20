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
