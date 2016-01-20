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

#ifndef QUICKSTEP_THREADING_WINDOWS_CONDITION_VARIABLE_HPP_
#define QUICKSTEP_THREADING_WINDOWS_CONDITION_VARIABLE_HPP_

#include "threading/ConditionVariable.hpp"
#include "threading/WinThreadsAPI.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief Implementation of ConditionVariable using MS Windows threads.
 **/
class ConditionVariableImplWindows {
 public:
  ~ConditionVariableImplWindows() {
  }

  inline void await() {
    SleepConditionVariableCS(&condition_variable_,
                             parent_critical_section_,
                             INFINITE);
  }

  inline bool awaitWithTimeout(std::uint64_t milliseconds) {
    return SleepConditionVariableCS(&condition_variable_,
                                    parent_critical_section_,
                                    milliseconds);
  }

  inline void signalOne() {
    WakeConditionVariable(&condition_variable_);
  }

  inline void signalAll() {
    WakeAllConditionVariable(&condition_variable_);
  }

 private:
  explicit ConditionVariableImplWindows(CRITICAL_SECTION *parent_critical_section)
      : parent_critical_section_(parent_critical_section) {
  }

  CRITICAL_SECTION *parent_critical_section_;
  CONDITION_VARIABLE condition_variable_;

  friend class RecursiveMutexImplWindows;

  DISALLOW_COPY_AND_ASSIGN(ConditionVariableImplWindows);
};
typedef ConditionVariableImplWindows ConditionVariable;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_WINDOWS_CONDITION_VARIABLE_HPP_
