/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_THREADING_CONDITION_VARIABLE_HPP_
#define QUICKSTEP_THREADING_CONDITION_VARIABLE_HPP_

#include <cstdint>

#include "threading/ThreadingConfig.h"

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief A condition variable associated with a Mutex.
 * @note A ConditionVariable should not be created directly. All
 *       ConditionVariables are created by calling
 *       Mutex::createConditionVariable(), and are owned by the Mutex which
 *       creates them.
 * @note This interface exists to provide a central point of documentation for
 *       platform-specific ConditionVariable implementations, you should never
 *       use it directly. Instead, simply use the ConditionVariable class,
 *       which will be typedefed to the appropriate implementation.
 **/
class ConditionVariableInterface {
 public:
  /**
   * @brief Virtual destructor.
   **/
  virtual ~ConditionVariableInterface() = 0;

  /**
   * @brief Block the calling thread and wait for this condition variable to
   *        be signalled.
   * @note Calling this method unlocks the associated Mutex. The calling thread
   *       will automatically acquire a lock on the Mutex again when returning
   *       from this method.
   * @warning The actual predicate this ConditionVariable represents should be
   *          re-checked after this method returns.
   **/
  virtual void await() = 0;

  /**
   * @brief Block the calling thread and wait for this condition variable to
   *        be signalled or after the specified timeout duration.
   * @param milliseconds The timeout period (in milliseconds).
   * @note Calling this method unlocks the associated Mutex. The calling thread
   *       will automatically acquire a lock on the Mutex again when returning
   *       from this method.
   * @warning The actual predicate this ConditionVariable represents should be
   *          re-checked after this method returns.
   *
   * @return Return true if this condition variable is signalled. Return false
   *         if the specified timeout expired.
   **/
  virtual bool awaitWithTimeout(std::uint64_t milliseconds) = 0;

  /**
   * @brief Wake up a single thread which is waiting on this ConditionVariable.
   * @note This method has no effect if no threads are waiting on this
   *       ConditionVariable.
   * @note This method can be called whether or not the associated Mutex is
   *       locked. If called while holding the lock, the waiting thread will
   *       not actually resume until the Mutex is unlocked so that it can be
   *       reacquired by the waiting thread.
   **/
  virtual void signalOne() = 0;

  /**
   * @brief Wake up all threads which are waiting on this ConditionVariable.
   * @note This method has no effect if no threads are waiting on this
   *       ConditionVariable.
   * @note This method can be called whether or not the associated Mutex is
   *       locked. If called while holding the lock, the waiting threads will
   *       not actually resume until the Mutex is unlocked so that it can be
   *       reacquired by waiting threads.
   * @note If multiple threads are waiting on this condition variable, they
   *       will be woken up one-at-a-time, since each one will re-acquire the
   *       Mutex as it wakes.
   **/
  virtual void signalAll() = 0;
};

/** @} */

}  // namespace quickstep

#ifdef QUICKSTEP_HAVE_CPP11_THREADS
#include "threading/cpp11/ConditionVariable.hpp"
#endif

#ifdef QUICKSTEP_HAVE_POSIX_THREADS
#include "threading/posix/ConditionVariable.hpp"
#endif

#ifdef QUICKSTEP_HAVE_WINDOWS_THREADS
#include "threading/windows/ConditionVariable.hpp"
#endif

#endif  // QUICKSTEP_THREADING_CONDITION_VARIABLE_HPP_
