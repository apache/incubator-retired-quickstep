// Copyright (c) 2014-2015, Quickstep Technologies LLC.
// Copyright (c) 2015, Pivotal Software, Inc.
// All Rights Reserved.

#include "tmbbench/affinity.h"

#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>

namespace tmbbench {

bool ParseAffinityString(const std::string &affinity_string,
                         std::vector<std::vector<int>> *affinities) {
  std::size_t pos = 0;
  while (pos < affinity_string.size()) {
    char *endptr = nullptr;
    int affinity = std::strtol(affinity_string.c_str() + pos, &endptr, 10);
    if ((endptr > affinity_string.c_str() + pos)
        && ((*endptr == ',') || (*endptr == '\0'))) {
      affinities->push_back(std::vector<int>(1, affinity));
      pos = endptr - affinity_string.c_str() + 1;
    } else {
      return false;
    }
  }
  return true;
}

bool ParseGlobalAffinityString(const std::string &affinity_string,
                               std::vector<int> *affinities) {
  std::size_t pos = 0;
  while (pos < affinity_string.size()) {
    char *endptr = nullptr;
    int affinity = std::strtol(affinity_string.c_str() + pos, &endptr, 10);
    if ((endptr > affinity_string.c_str() + pos)
        && ((*endptr == ',') || (*endptr == '\0'))) {
      affinities->push_back(affinity);
      pos = endptr - affinity_string.c_str() + 1;
    } else {
      return false;
    }
  }
  return true;
}

}  // namespace tmbbench
