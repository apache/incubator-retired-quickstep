/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_THREADING_CPP11_CONDITION_VARIABLE_HPP_
#define QUICKSTEP_THREADING_CPP11_CONDITION_VARIABLE_HPP_

#include <chrono>
#include <cstdint>
#include <condition_variable>  // NOLINT(build/c++11)
#include <mutex>  // NOLINT(build/c++11)

#include "threading/ConditionVariable.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

template <class InternalMutexType> class MutexImplCPP11;

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief Implementation of ConditionVariable using C++11 threads.
 **/
template <typename InternalMutexType>
class ConditionVariableImplCPP11 {
 public:
  inline void await() {
    internal_condition_var_.wait(*parent_mutex_);
  }

  inline bool awaitWithTimeout(std::uint64_t milliseconds) {
    std::cv_status status = internal_condition_var_.wait_for(*parent_mutex_,
                                                             std::chrono::milliseconds(milliseconds));
    return std::cv_status::no_timeout == status;
  }

  inline void signalOne() {
    internal_condition_var_.notify_one();
  }

  inline void signalAll() {
    internal_condition_var_.notify_all();
  }

 private:
  explicit ConditionVariableImplCPP11(InternalMutexType *parent_mutex)
      : parent_mutex_(parent_mutex) {
  }

  InternalMutexType *parent_mutex_;
  std::condition_variable_any internal_condition_var_;

  friend class MutexImplCPP11<std::mutex>;
  friend class MutexImplCPP11<std::recursive_mutex>;

  DISALLOW_COPY_AND_ASSIGN(ConditionVariableImplCPP11);
};
typedef ConditionVariableImplCPP11<std::mutex> ConditionVariable;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_CPP11_CONDITION_VARIABLE_HPP_
