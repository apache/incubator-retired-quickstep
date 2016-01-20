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

// This small program generates an initial catalog.pb.bin with an empty
// "default" database.

#include <cstdio>
#include <fstream>

#include "catalog/Catalog.hpp"
#include "catalog/Catalog.pb.h"
#include "catalog/CatalogDatabase.hpp"

int main() {
  std::ofstream catalog_file("catalog.pb.bin");
  if (!catalog_file.good()) {
    std::fprintf(stderr, "ERROR: Unable to open catalog.pb.bin for writing.\n");
    return 1;
  }

  quickstep::Catalog catalog;
  catalog.addDatabase(new quickstep::CatalogDatabase(nullptr, "default"));

  if (!catalog.getProto().SerializeToOstream(&catalog_file)) {
    std::fprintf(stderr, "ERROR: Unable to serialize catalog proto to file catalog.pb.bin\n");
    return 1;
  }

  catalog_file.close();
  return 0;
}
