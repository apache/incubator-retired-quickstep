//   Copyright 2014-2015 Quickstep Technologies LLC.
// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
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
