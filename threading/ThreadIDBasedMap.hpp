/**
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

#ifndef QUICKSTEP_THREADING_THREAD_ID_BASED_MAP_HPP_
#define QUICKSTEP_THREADING_THREAD_ID_BASED_MAP_HPP_

#include <unordered_map>

#include "threading/ThreadingConfig.h"

#ifdef QUICKSTEP_HAVE_POSIX_THREADS
#include <pthread.h>
#endif

#ifdef QUICKSTEP_HAVE_CPP11_THREADS
#include <thread>  // NOLINT(build/c++11)
#endif

#ifdef QUICKSTEP_HAVE_WINDOWS_THREADS
#include "threading/WinThreadsAPI.hpp"
#endif

#include "threading/Mutex.hpp"
#include "threading/SharedMutex.hpp"
#include "threading/SpinSharedMutex.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup QueryExecution
 *  @{
 */
/**
 * @brief A map which uses caller thread's ID as a key based on a singleton
 *        pattern.
 *
 * @note This is in a way a simulation of thread-local object storage.
 **/
template <class V, char... name>
class ThreadIDBasedMap {
 public:
#ifdef QUICKSTEP_HAVE_CPP11_THREADS
  typedef std::thread::id thread_id_t;
#endif
#ifdef QUICKSTEP_HAVE_POSIX_THREADS
  typedef pthread_t thread_id_t;
#endif
#ifdef QUICKSTEP_HAVE_WINDOWS_THREADS
  typedef DWORD thread_id_t;
#endif
  /**
   * @brief Get the instance of the map.
   *
   * @warning The initialization of the instance is not thread safe.
   *
   * @return A pointer to the instance.
   **/
  static ThreadIDBasedMap* Instance() {
    static ThreadIDBasedMap instance;
    return &instance;
  }

  /**
   * @brief Add a value to the map.
   *
   * @param value The value to be added.
   **/
  void addValue(const V &value) {
    const thread_id_t key = getKey();
    {
      SpinSharedMutexExclusiveLock<false> lock(map_mutex_);
      DCHECK(map_.find(key) == map_.end());
      map_[key] = value;
    }
  }

  /**
   * @brief Remove the value for the caller thread.
   **/
  void removeValue() {
    const thread_id_t key = getKey();
    {
      SpinSharedMutexExclusiveLock<false> lock(map_mutex_);
      // Use the following way to avoid exception in erase() call.
      auto remove_iter = map_.find(key);
      DCHECK(remove_iter != map_.end());
      map_.erase(remove_iter);
    }
  }

  /**
   * @brief Get the value corresponding to the caller thread.
   *
   * @return The value for which the key is the caller thread's ID.
   **/
  const V& getValue() {
    const thread_id_t key = getKey();
    {
      SpinSharedMutexSharedLock<false> lock(map_mutex_);
      DCHECK(map_.find(key) != map_.end());
      return map_[key];
    }
  }

 private:
  /**
   * @brief Constructor.
   **/
  ThreadIDBasedMap() {}

  /**
   * @brief Destructor.
   *
   * @note The destructor is private in order to prevent any caller that holds
   *       a pointer to the map from accidentally deleting it.
   **/
  ~ThreadIDBasedMap() {}

  /**
   * @brief Return a unique key corresponding to the caller thread.
   *
   * @return A unique key.
   **/
  static thread_id_t getKey() {
#ifdef QUICKSTEP_HAVE_CPP11_THREADS
    return std::this_thread::get_id();
#endif
#ifdef QUICKSTEP_HAVE_POSIX_THREADS
    return pthread_self();
#endif
#ifdef QUICKSTEP_HAVE_WINDOWS_THREADS
    return GetCurrentThreadId();
#endif
  }

  std::unordered_map<thread_id_t, V> map_;

  SpinSharedMutex<false> map_mutex_;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_THREADING_THREAD_ID_BASED_MAP_HPP_
