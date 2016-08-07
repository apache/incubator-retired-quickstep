// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "tmbbench/thread.h"

#if defined(TMB_AFFINITY_LINUX)

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include <pthread.h>
#include <sched.h>

#elif defined(TMB_AFFINITY_FREEBSD)

#ifndef _BSD_SOURCE
#define _BSD_SOURCE 1
#endif
#include <pthread.h>
#include <pthread_np.h>
#include <sched.h>

#elif defined(TMB_AFFINITY_NETBSD)

#ifndef _BSD_SOURCE
#define _BSD_SOURCE 1
#endif
#include <pthread.h>
#include <sched.h>

#endif

#include <cassert>
#include <vector>

namespace tmbbench {

void Thread::Affinitize() {
  if (cpu_affinity_.empty()) {
    return;
  }

#if defined(TMB_AFFINITY_LINUX) || defined(TMB_AFFINITY_FREEBSD)

#ifdef TMB_AFFINITY_LINUX
  cpu_set_t cpuset;
#else
  cpuset_t cpuset;
#endif
  CPU_ZERO(&cpuset);
  for (const int cpu_id : cpu_affinity_) {
    CPU_SET(cpu_id, &cpuset);
  }

  int rc = pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset);
  assert(rc == 0);

#elif defined(TMB_AFFINITY_NETBSD)

  cpuset_t *cset = cpuset_create();
  int rc = 0;
  for (const int cpu_id : cpu_affinity_) {
    rc = cpuset_set(cpu_id, cset);
    assert(rc != 0);
  }

  rc = pthread_setaffinity_np(pthread_self(), cpuset_size(cset), cset);
  assert(rc != 0);
  cpuset_destroy(cset);

#endif
}

}  // namespace tmbbench
