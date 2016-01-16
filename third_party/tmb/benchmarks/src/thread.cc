// Copyright (c) 2015, Pivotal Software, Inc.
// All Rights Reserved.

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
