/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "threading/Thread.hpp"

#include <cstddef>

namespace quickstep {

ThreadInterface::~ThreadInterface() {}

namespace threading_internal {
void executeRunMethodForThreadReturnNothing(void *thread_ptr) {
  static_cast<ThreadInterface*>(thread_ptr)->run();
}

void *executeRunMethodForThreadReturnNull(void *thread_ptr) {
  static_cast<ThreadInterface*>(thread_ptr)->run();
  return NULL;
}
}  // namespace threading_internal

}  // namespace quickstep
