/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "threading/Mutex.hpp"

namespace quickstep {

// Strange but true: C++ requires pure-virtual destructors to have a definition.

MutexInterface::~MutexInterface() {}
MutexLockInterface::~MutexLockInterface() {}

}  // namespace quickstep
