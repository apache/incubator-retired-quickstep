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

#ifndef QUICKSTEP_THREADING_CPP11_MUTEX_HPP_
#define QUICKSTEP_THREADING_CPP11_MUTEX_HPP_

#include <mutex>  // NOLINT(build/c++11)

#include "threading/Mutex.hpp"
#include "threading/cpp11/ConditionVariable.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief Implementation of Mutex and RecursiveMutex using C++11 threads.
 **/
template <class InternalMutexType>
class MutexImplCPP11 {
 public:
  inline MutexImplCPP11() {
  }

  inline ~MutexImplCPP11() {
  }

  inline void lock() {
    internal_mutex_.lock();
  }

  inline void unlock() {
    internal_mutex_.unlock();
  }

  ConditionVariableImplCPP11<InternalMutexType>* createConditionVariable() {
    ConditionVariableImplCPP11<InternalMutexType> *new_condition
        = new ConditionVariableImplCPP11<InternalMutexType>(&internal_mutex_);
    condition_vars_.push_back(new_condition);
    return new_condition;
  }

 private:
  InternalMutexType internal_mutex_;
  PtrVector<ConditionVariableImplCPP11<InternalMutexType> > condition_vars_;

  DISALLOW_COPY_AND_ASSIGN(MutexImplCPP11);
};
typedef MutexImplCPP11<std::mutex> Mutex;
typedef MutexImplCPP11<std::recursive_mutex> RecursiveMutex;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_CPP11_MUTEX_HPP_
