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

#include "cli/DefaultsConfigurator.hpp"

#include "cli/CliConfig.h"  // For QUICKSTEP_OS_WINDOWS.

#ifdef QUICKSTEP_OS_WINDOWS
// TODO(jmp): If filesystem shows up in C++-17, we can switch to just using that.
#include <experimental/filesystem>
#else
#include <cstdlib>
#endif  // QUICKSTEP_OS_WINDOWS

#include <fstream>
#include <string>

#include "catalog/Catalog.hpp"
#include "catalog/Catalog.pb.h"
#include "catalog/CatalogDatabase.hpp"

#include "glog/logging.h"

using std::string;

namespace quickstep {

void DefaultsConfigurator::InitializeDefaultDatabase(const string &storage_path, const string &catalog_path) {
  // TODO(jmp): Refactor the code in this file!
  LOG(INFO) << "Initializing the database, creating a new catalog file and storage directory";

  // Create the directory
  // TODO(jmp): At some point, likely in C++-17, we will just have the
  //            filesystem path, and we can clean this up
#ifdef QUICKSTEP_OS_WINDOWS
  CHECK(std::experimental::filesystem::create_directories(storage_path))
      << "Failed when attempting to create the directory: " << storage_path
      << "\nCheck if the directory already exists. If so, delete it or move it before initializing";
#else
  {
    const string path_name = "mkdir " + storage_path;
    CHECK(!std::system(path_name.c_str()))
         << "Failed when attempting to create the directory: " << storage_path;
  }
#endif  // QUICKSTEP_OS_WINDOWS

  // Create the default catalog file.
  std::ofstream catalog_file(catalog_path.c_str());
  CHECK(catalog_file.good())
      << "ERROR: Unable to open " << catalog_path << " for writing.";

  Catalog catalog;
  catalog.addDatabase(new CatalogDatabase(nullptr, "default"));

  CHECK(catalog.getProto().SerializeToOstream(&catalog_file))
      << "ERROR: Unable to serialize catalog proto to file " << catalog_path;

  // Close the catalog file - it will be reopened below by the QueryProcessor.
  catalog_file.close();
}

}  // namespace quickstep
