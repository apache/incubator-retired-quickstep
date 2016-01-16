// Copyright (c) 2014-2015, Quickstep Technologies LLC.
// Copyright (c) 2015, Pivotal Software, Inc.
// All Rights Reserved.

#ifndef TMBBENCH_AFFINITY_H_
#define TMBBENCH_AFFINITY_H_

#include <string>
#include <vector>

namespace tmbbench {

bool ParseAffinityString(const std::string &affinity_string,
                         std::vector<std::vector<int>> *affinities);

bool ParseGlobalAffinityString(const std::string &affinity_string,
                               std::vector<int> *affinities);

}  // namespace tmbbench

#endif  // TMBBENCH_AFFINITY_H_
