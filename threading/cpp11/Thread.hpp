/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_THREADING_CPP11_THREAD_HPP_
#define QUICKSTEP_THREADING_CPP11_THREAD_HPP_

#include <thread>  // NOLINT(build/c++11)

#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Threading
 *  @{
 */

/**
 * @brief Implementation of Thread using C++11 threads.
 **/
class ThreadImplCPP11 : public ThreadInterface {
 public:
  inline ThreadImplCPP11() {
  }

  inline virtual ~ThreadImplCPP11() {
  }

  inline void start() {
    internal_thread_ = std::thread(threading_internal::executeRunMethodForThreadReturnNothing, this);
  }

  inline void join() {
    internal_thread_.join();
  }

 private:
  std::thread internal_thread_;

  DISALLOW_COPY_AND_ASSIGN(ThreadImplCPP11);
};
typedef ThreadImplCPP11 Thread;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_CPP11_THREAD_HPP_
