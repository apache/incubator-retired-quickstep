/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
