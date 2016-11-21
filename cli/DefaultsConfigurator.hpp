/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_CLI_DEFAULTS_CONFIGURATOR_HPP_
#define QUICKSTEP_CLI_DEFAULTS_CONFIGURATOR_HPP_

#include "storage/StorageConfig.h"  // For QUICKSTEP_HAVE_LIBNUMA.

#ifdef QUICKSTEP_HAVE_LIBNUMA
#include <numa.h>
#endif  // QUICKSTEP_HAVE_LIBNUMA

#include <cstddef>
#include <string>
#include <thread>  // NOLINT(build/c++11)

#ifdef QUICKSTEP_HAVE_LIBNUMA
#include <unordered_map>
#endif  // QUICKSTEP_HAVE_LIBNUMA

#include <vector>

#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup CLI
 * @{
 **/

/**
 * @brief A static class for setting the default configuration parameters in
 *        Quickstep.
 **/
class DefaultsConfigurator {
 public:
  /**
   * @brief Get the number of concurrent threads supported by the hardware,
   *
   * @note This function will return 0, if it fails (which it may on some
   *       machines/environments).
   **/
  static std::size_t GetNumHardwareThreads() {
    return std::thread::hardware_concurrency();
  }

  /**
   * @brief Get the number of available numa sockets.
   *
   * @return Number of available numa sockets. Always 1 if the system doesn't
   *         have libnuma.
   **/
  static std::size_t GetNumNUMANodes() {
#ifdef QUICKSTEP_HAVE_LIBNUMA
    // Id of the maximum node.
    return numa_max_node() + 1;
#else
    return 1;
#endif  // QUICKSTEP_HAVE_LIBNUMA
  }

  /**
   * @brief Get the number of NUMA nodes covered by the given worker affinities
   *        to the CPU cores.
   *
   * @param worker_cpu_affinities A vector V where V[i] is the CPU core to which
   *        worker with index i is affinitized.
   *
   * @return The number of NUMA nodes that are covered by the given set of
   *         workers.
   **/
  static std::size_t GetNumNUMANodesCoveredByWorkers(const std::vector<int> &worker_cpu_affinities) {
    if (!worker_cpu_affinities.empty()) {
#ifdef QUICKSTEP_HAVE_LIBNUMA
      // Key = NUMA node, value = whether there is at least one worker whose
      // affinity is set to a core on the given NUMA node.
      std::unordered_map<int, bool> any_worker_on_numa_node;
      for (const int curr_cpu_core_id : worker_cpu_affinities) {
        const int curr_numa_node_id = numa_node_of_cpu(curr_cpu_core_id);
        if (curr_numa_node_id >= 0) {
          // Note - For the purpose of this function, the value is always true.
          // Therefore, we can rely on the size of the map to get the count.
          any_worker_on_numa_node[curr_numa_node_id] = true;
        }
      }
      return any_worker_on_numa_node.size();
#endif  // QUICKSTEP_HAVE_LIBNUMA
    }
    // When libnuma is not available, or worker affinities are not specified,
    // the default return value is 1.
    return 1;
  }

  /**
   * @brief Initialize the default database with no relations.
   *
   * @param storage_path The filesystem directory to store catalog.
   * @param catalog_path The full path of the catalog file.
   **/
  static void InitializeDefaultDatabase(const std::string &storage_path,
                                        const std::string &catalog_path);

 private:
  /**
   * @brief Private constructor to disable instantiation of the class.
   **/
  DefaultsConfigurator();

  DISALLOW_COPY_AND_ASSIGN(DefaultsConfigurator);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_DEFAULTS_CONFIGURATOR_HPP_
