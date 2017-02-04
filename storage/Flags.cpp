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

#include "storage/Flags.hpp"

#include "storage/StorageConfig.h"

#include "gflags/gflags.h"

namespace quickstep {

#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
DEFINE_bool(use_hdfs, false, "Use HDFS as the persistent storage, instead of the local disk.");

DEFINE_string(hdfs_namenode_host, "localhost", "Host name or IP address of HDFS namenode.");

static bool ValidateHdfsNameNodePort(const char *flagname,
                                     std::int32_t value) {
  if (value > 0 && value < 65536) {
    return true;
  } else {
    std::fprintf(stderr, "--%s must be between 1 and 65535 (inclusive)\n", flagname);
    return false;
  }
}
DEFINE_int32(hdfs_namenode_port, 9000, "Port of HDFS namenode.");
static const bool hdfs_namenode_port_dummy
    = gflags::RegisterFlagValidator(&FLAGS_hdfs_namenode_port, &ValidateHdfsNameNodePort);

static bool ValidateHdfsNumReplications(const char *flagname,
                                        std::int32_t value) {
  // The default dfs.namenode.replication.min is 1, while the default
  // dfs.replication.max is 512.
  if (value > 0) {
    return true;
  } else {
    std::fprintf(stderr, "--%s must be at least 1\n", flagname);
    return false;
  }
}
DEFINE_int32(hdfs_num_replications, 1, "Number of HDFS file replications.");
static const bool hdfs_num_replications_dummy
    = gflags::RegisterFlagValidator(&FLAGS_hdfs_num_replications, &ValidateHdfsNumReplications);
#endif

}  // namespace quickstep
