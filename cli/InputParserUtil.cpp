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

#include "cli/InputParserUtil.hpp"

#include <cstddef>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogConfig.h"
#include "storage/StorageConfig.h"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

#ifdef QUICKSTEP_HAVE_LIBNUMA
#include <numa.h>
#endif

using std::string;

namespace quickstep {

std::vector<int> InputParserUtil::ParseWorkerAffinities(
    const int num_workers,
    const string &affinity_string) {
  std::vector<int> affinities;
  bool switch_to_default_affinities = false;
  if (affinity_string.empty()) {
    switch_to_default_affinities = true;
    LOG(INFO) << "Empty worker affinities provided, switching to default "
                 "worker affinities";
  } else if (!ParseIntString(affinity_string, ',', &affinities)) {
      switch_to_default_affinities = true;
      LOG(INFO) << "Invalid worker affinities provided, switching to default "
                   "affinities";
  }

  for (const int affinity : affinities) {
    if (affinity < -1) {
      switch_to_default_affinities = true;
      LOG(INFO) << "CPU affinities specified by --worker_affinities must be "
                   "non-negative, or -1 to specify no affinity. Switching to "
                   "default worker affinities";
      break;
    }
  }

  if (switch_to_default_affinities) {
    // Set default affinities.
    // If the number of worker threads is less than the maximum parallelism on
    // the box, we try to balance workers on all sockets. The intention is to
    // balance the memory bandwidth usage across all sockets. This may however
    // hurt the performance (due to poor data locality) when the machine has
    // many sockets and data is not partitioned.
#ifdef QUICKSTEP_HAVE_LIBNUMA
    // This code is inspired from the print_node_cpus() function of numactl.
    // WARNING - If some NUMA sockets are disabled, we can't detect it.
    const int num_sockets = numa_num_configured_nodes();
    CHECK_GT(num_sockets, 0);
    // A vector V where V[i] denotes a vector of CPU cores that belong to the
    // socket i.
    std::vector<std::vector<int>> cpus_from_sockets;
    cpus_from_sockets.resize(num_sockets);
    for (int curr_socket = 0; curr_socket < num_sockets; ++curr_socket) {
      std::unique_ptr<struct bitmask> cpus(numa_allocate_cpumask());
      const int err = numa_node_to_cpus(curr_socket, cpus.get());
      if (err >= 0) {
        for (int i = 0; i < static_cast<int>(cpus->size); i++) {
          if (numa_bitmask_isbitset(cpus.get(), i)) {
            // The current CPU belongs to curr_socket.
            cpus_from_sockets[curr_socket].push_back(i);
          }
        }
      }
    }
    // Now assign affinity to each worker, picking one CPU from each socket in a
    // round robin manner.
    int curr_socket = 0;
    std::size_t iteration = 0;
    for (int curr_worker = 0; curr_worker < num_workers; ++curr_worker) {
      if (iteration < cpus_from_sockets[curr_socket].size()) {
        const int curr_worker_affinity =
            cpus_from_sockets[curr_socket][iteration];
        affinities.push_back(curr_worker_affinity);
      }
      // Increase iteration number only when we are at the last socket.
      iteration = iteration + ((curr_socket + 1) / num_sockets);
      curr_socket = (curr_socket + 1) % num_sockets;
    }
#endif
  }

  if (affinities.size() < static_cast<std::size_t>(num_workers)) {
    std::cout << "--num_workers is " << num_workers << ", but only "
              << "specified " << affinities.size() << " CPU affinities "
              << "with --worker_affinities. "
              << (num_workers - affinities.size()) << " workers will be "
              << "unaffinitized.\n";
    affinities.resize(num_workers, -1);
  } else if (affinities.size() > static_cast<std::size_t>(num_workers)) {
    std::cout << "--num_workers is " << num_workers << ", but specified "
              << affinities.size() << " CPU affinities with "
              << "--worker_affinities. Extra affinities will be ignored.\n";
    affinities.resize(num_workers);
  }

  return affinities;
}

std::vector<int> InputParserUtil::GetNUMANodesForCPUs() {
  std::vector<int> numa_nodes_of_cpus;
#ifdef QUICKSTEP_HAVE_LIBNUMA
  const int num_cpus = numa_num_configured_cpus();
  numa_nodes_of_cpus.reserve(num_cpus);
  for (int curr_cpu = 0; curr_cpu < num_cpus; ++curr_cpu) {
    numa_nodes_of_cpus.push_back(numa_node_of_cpu(curr_cpu));
  }
#endif
  return numa_nodes_of_cpus;
}

}  // namespace quickstep
