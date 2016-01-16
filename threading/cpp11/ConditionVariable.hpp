/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
