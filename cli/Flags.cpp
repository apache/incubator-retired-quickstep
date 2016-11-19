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

#include <string>

#include "storage/StorageConstants.hpp"

#include "gflags/gflags.h"

namespace quickstep {

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
