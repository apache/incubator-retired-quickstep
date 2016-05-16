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

/* A standalone command-line interface to Distributed QuickStep Worker */

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "cli/InputParserUtil.hpp"
#include "query_execution/Shiftboss.hpp"
#include "query_execution/Worker.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "storage/StorageConfig.h"  // For QUICKSTEP_HAVE_FILE_MANAGER_HDFS.

#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
#include "storage/FileManagerHdfs.hpp"
#endif

#include "storage/PreloaderThread.hpp"
#include "storage/StorageManager.hpp"
#include "utility/PtrVector.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "grpc/grpc.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/native_net_client_message_bus.h"

using std::fflush;
using std::fprintf;
using std::printf;
using std::size_t;
using std::string;
using std::vector;

using quickstep::InputParserUtil;
using quickstep::Worker;

using tmb::client_id;

namespace quickstep {

#ifdef QUICKSTEP_OS_WINDOWS
static constexpr char kPathSeparator = '\\';
static constexpr char kDefaultStoragePath[] = "qsstor\\";
#else
static constexpr char kPathSeparator = '/';
static constexpr char kDefaultStoragePath[] = "qsstor/";
#endif

DEFINE_int32(num_workers, 1, "Number of worker threads");

DEFINE_bool(preload_buffer_pool, false,
            "If true, pre-load all known blocks into buffer pool before "
            "accepting queries (should also set --buffer_pool_slots to be "
            "large enough to accomodate the entire database).");

DEFINE_string(storage_path, kDefaultStoragePath,
              "Filesystem path for quickstep database storage.");

DEFINE_string(tmb_server_ip, "127.0.0.1", "IP Address of the TMB Server.");

static bool ValidateTmbServerPort(const char *flagname,
                                  std::int32_t value) {
  if (value > 0 && value < 65536) {
    return true;
  } else {
    fprintf(stderr, "--%s must be between 1 and 65535 (inclusive)\n", flagname);
    return false;
  }
}
DEFINE_int32(tmb_server_port, 4575, "Port of the TMB Server.");
static const bool tmb_server_port_dummy
    = gflags::RegisterFlagValidator(&FLAGS_tmb_server_port, &ValidateTmbServerPort);

DEFINE_string(worker_affinities, "",
              "A comma-separated list of CPU IDs to pin worker threads to "
              "(leaving this empty will cause all worker threads to inherit "
              "the affinity mask of the Quickstep process, which typically "
              "means that they will all be runable on any CPU according to "
              "the kernel's own scheduling policy).");

}  // namespace quickstep

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  grpc_init();

  if (quickstep::FLAGS_num_workers > 0) {
    printf("Starting Quickstep with %d worker thread(s)\n",
           quickstep::FLAGS_num_workers);
  } else {
    LOG(FATAL) << "Quickstep needs at least one worker thread";
  }

#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
  if (quickstep::FLAGS_use_hdfs) {
    LOG(INFO) << "Using HDFS as the default persistent storage, with namenode at "
              << quickstep::FLAGS_hdfs_namenode_host << ":"
              << quickstep::FLAGS_hdfs_namenode_port << " and block replication factor "
              << quickstep::FLAGS_hdfs_num_replications << "\n";
  }
#endif

  // Setup the paths used by StorageManager.
  string fixed_storage_path(quickstep::FLAGS_storage_path);
  if (!fixed_storage_path.empty()
      && (fixed_storage_path.back() != quickstep::kPathSeparator)) {
    fixed_storage_path.push_back(quickstep::kPathSeparator);
  }

  quickstep::StorageManager storage_manager(fixed_storage_path);

  tmb::NativeNetClientMessageBus bus;
  bus.AddServer(quickstep::FLAGS_tmb_server_ip.c_str(), quickstep::FLAGS_tmb_server_port);
  bus.Initialize();

  // Parse the CPU affinities for workers and the preloader thread, if enabled
  // to warm up the buffer pool.
  const vector<int> worker_cpu_affinities =
      InputParserUtil::ParseWorkerAffinities(quickstep::FLAGS_num_workers,
                                             quickstep::FLAGS_worker_affinities);

  if (quickstep::FLAGS_preload_buffer_pool) {
    // TODO(quickstep-team): Enable Preloading by reconstructing CatalogDatabase
    // from Catalog in Conductor.
    quickstep::CatalogDatabase database(nullptr, "default");
    quickstep::PreloaderThread preloader(database,
                                         &storage_manager,
                                         worker_cpu_affinities.front());
    printf("Preloading buffer pool... ");
    fflush(stdout);
    preloader.start();
    preloader.join();
    printf("DONE\n");
  }

  // Get the NUMA affinities for workers.
  vector<int> cpu_numa_nodes = InputParserUtil::GetNUMANodesForCPUs();
  if (cpu_numa_nodes.empty()) {
    // libnuma is not present. Assign -1 as the NUMA node for every worker.
    cpu_numa_nodes.assign(worker_cpu_affinities.size(), -1);
  }

  vector<int> worker_numa_nodes;
  quickstep::PtrVector<Worker> workers;
  vector<client_id> worker_client_ids;

  // Initialize the worker threads.
  DCHECK_EQ(static_cast<size_t>(quickstep::FLAGS_num_workers),
            worker_cpu_affinities.size());
  for (size_t worker_idx = 0;
       worker_idx < worker_cpu_affinities.size();
       ++worker_idx) {
    int numa_node_id = -1;
    if (worker_cpu_affinities[worker_idx] >= 0) {
      // This worker can be NUMA affinitized.
      numa_node_id = cpu_numa_nodes[worker_cpu_affinities[worker_idx]];
    }
    worker_numa_nodes.push_back(numa_node_id);

    workers.push_back(new Worker(worker_idx,
                                 &bus,
                                 worker_cpu_affinities[worker_idx]));
    worker_client_ids.push_back(workers.back().getBusClientID());
  }

  // TODO(zuyu): Move WorkerDirectory within Shiftboss once the latter is added.
  quickstep::WorkerDirectory worker_directory(worker_cpu_affinities.size(),
                                              worker_client_ids,
                                              worker_numa_nodes);

  quickstep::Shiftboss shiftboss(&bus,
                                 &storage_manager,
                                 &worker_directory);
  shiftboss.start();

  // Start the worker threads.
  for (Worker &worker : workers) {
    worker.start();
  }

  for (Worker &worker : workers) {
    worker.join();
  }
  shiftboss.join();

  return 0;
}
