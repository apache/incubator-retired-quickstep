/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
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

#ifndef QUICKSTEP_STORAGE_PRELOADER_THREAD_HPP_
#define QUICKSTEP_STORAGE_PRELOADER_THREAD_HPP_

#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class StorageManager;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A thread that preloads all known blocks in a database into a
 *        StorageManager's buffer pool. A separate thread is used so that we
 *        can affinitize it like a worker and get allocations for buffer memory
 *        that are NUMA-local for at least some workers where possible.
 **/
class PreloaderThread : public Thread {
 public:
  /**
   * @brief Constructor.
   *
   * @param database All blocks in all non-temporary relations in this database
   *        will be loaded into the buffer pool.
   * @param storage_manager Blocks will be loaded into this StorageManager's
   *        buffer pool. Note that if the total blocks in the database exceed
   *        the size of the StorageManager's buffer pool, some blocks will be
   *        evicted and the database will not be fully preloaded.
   * @param cpu_id The ID of the CPU to affinitize this thread to, or -1 to
   *        indicate no affinity.
   **/
  PreloaderThread(const CatalogDatabase &database,
                  StorageManager *storage_manager,
                  const int cpu_id)
      : database_(database),
        storage_manager_(storage_manager),
        cpu_id_(cpu_id) {
  }

  ~PreloaderThread() override {
  }

 protected:
  void run() override;

 private:
  const CatalogDatabase &database_;
  StorageManager *storage_manager_;

  const int cpu_id_;

  DISALLOW_COPY_AND_ASSIGN(PreloaderThread);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_PRELOADER_THREAD_HPP_
