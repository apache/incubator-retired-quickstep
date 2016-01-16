/**
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "cli/InputParserUtil.hpp"

#include <cstddef>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogConfig.h"
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
  if (affinity_string.empty()) {
    affinities.resize(num_workers, -1);
    return affinities;
  }

  if (!ParseIntString(affinity_string, ',', &affinities)) {
    LOG(FATAL) << "--worker_affinities must be a comma-separated list of "
               << "integer CPU ids.\n";
  }

  for (const int affinity : affinities) {
    if (affinity < -1) {
      LOG(FATAL) << "CPU affinities specified by --worker_affinities must be "
                 << "non-negative, or -1 to specify no affinity.\n";
    }
  }

  if (affinities.size() < static_cast<std::size_t>(num_workers)) {
    LOG(WARNING) << "--num_workers is " << num_workers << ", but only "
                 << "specified " << affinities.size() << " CPU affinities "
                 << "with --worker_affinities. "
                 << (num_workers - affinities.size()) << " workers will be "
                 << "unaffinitized.\n";
    affinities.resize(num_workers, -1);
  } else if (affinities.size() > static_cast<std::size_t>(num_workers)) {
    LOG(WARNING) << "--num_workers is " << num_workers << ", but specified "
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
