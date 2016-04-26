/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_UTILITY_SHARDED_LOCK_MANAGER_HPP_
#define QUICKSTEP_UTILITY_SHARDED_LOCK_MANAGER_HPP_

#include <array>
#include <cstddef>
#include <functional>

#include "storage/StorageConstants.hpp"
#include "threading/SharedMutex.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Maintains a fixed number of SharedMutex objects and an interface for
 *        mapping instances of the template class T to one of the underlying
 *        SharedMutex's. The mapping is done by calling std::hash<T>::operator()
 *        on the object.
 *
 * @param T The class to map to SharedMutex's.
 * @param N The number of SharedMutex's to use in the underlying array.
 * @param SharedMutexT The shared mutex class to use for locks.
 */
template <typename T, std::size_t N, typename SharedMutexT = SharedMutex>
class ShardedLockManager {
 public:
  /**
   * @brief Default constructor.
   */
  ShardedLockManager() { }

  /**
   * @brief Get the SharedMutex corresponding to the provided key.
   * @param key The key to map to a SharedMutex.
   * @param has_collision Whether accessing the given key would result in a
   *        hash collision. Used in StorageManager::makeRoomForBlock only.
   * @return The corresponding SharedMutex if there is no collision; otherwise,
   *         the collision SharedMutex.
   */
  SharedMutexT* get(const T key, bool *has_collision = nullptr) {
    const std::size_t shard = hash_(key) % N;

    if (has_collision != nullptr) {
      // In StorageManager::makeRoomForBlock, check whether the evicting block
      // or blob has a shard collision with existing referenced shards.
      SpinSharedMutexSharedLock<false> read_lock(shards_mutex_);
      if (shards_.find(shard) != shards_.end()) {
        *has_collision = true;
        return &collision_mutex_;
      }
    }

    {
      SpinSharedMutexExclusiveLock<false> write_lock(shards_mutex_);

      // Check one more time for the evicting block or blob if there is a shard
      // collision.
      if (has_collision != nullptr && shards_.find(shard) != shards_.end()) {
        *has_collision = true;
        return &collision_mutex_;
      }

      shards_.insert(shard);
    }
    return &sharded_mutexes_[shard];
  }

  /**
   * @brief Release the shard corresponding to the provided key.
   * @param key The key to compute the shard.
   */
  void release(const T key) {
    SpinSharedMutexExclusiveLock<false> write_lock(shards_mutex_);
    shards_.erase(hash_(key) % N);
  }

 private:
  std::hash<T> hash_;
  std::array<SharedMutexT, N> sharded_mutexes_;

  // The placeholder mutex used whenever there is a hash collision.
  SharedMutexT collision_mutex_;

  // Bookkeep all shards referenced by StorageManager in multiple threads.
  std::unordered_set<std::size_t> shards_;
  alignas(kCacheLineBytes) mutable SpinSharedMutex<false> shards_mutex_;

  DISALLOW_COPY_AND_ASSIGN(ShardedLockManager);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_SHARDED_LOCK_MANAGER_HPP_
