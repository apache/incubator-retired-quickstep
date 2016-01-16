/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
