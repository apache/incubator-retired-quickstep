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

#include "cli/Flags.hpp"

#include <cstddef>
#include <cstdio>
#include <string>

#include "cli/DefaultsConfigurator.hpp"
#include "storage/StorageConstants.hpp"

#include "gflags/gflags.h"

using std::fprintf;

namespace quickstep {

DEFINE_bool(initialize_db, false, "If true, initialize a database.");

static bool ValidateNumWorkers(const char *flagname, int value) {
  if (value > 0) {
    return true;
  }

  // Detect the hardware concurrency level.
  const std::size_t num_hw_threads =
      DefaultsConfigurator::GetNumHardwareThreads();

  // Use the command-line value if that was supplied, else use the value
  // that we computed above, provided it did return a valid value.
  // TODO(jmp): May need to change this at some point to keep one thread
  //            available for the OS if the hardware concurrency level is high.
  FLAGS_num_workers = num_hw_threads != 0 ? num_hw_threads : 1;

  return FLAGS_num_workers > 0;
}
DEFINE_int32(num_workers, 0, "Number of worker threads. If this value is "
             "specified and is greater than 0, then this user-supplied value is "
             "used. Else (i.e. the default case), we examine the reported "
             "hardware concurrency level, and use that.");
static const volatile bool num_workers_dummy
    = gflags::RegisterFlagValidator(&FLAGS_num_workers, &ValidateNumWorkers);

static bool ValidateStoragePath(const char *flagname,
                                const std::string &value) {
  if (!value.empty() && value.back() != kPathSeparator) {
    FLAGS_storage_path.push_back(kPathSeparator);
  }

  return true;
}
DEFINE_string(storage_path, kDefaultStoragePath,
              "Filesystem path to store the Quickstep database.");
static const volatile bool storage_path_dummy
    = gflags::RegisterFlagValidator(&FLAGS_storage_path, &ValidateStoragePath);

}  // namespace quickstep
