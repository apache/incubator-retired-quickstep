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

#include "query_execution/DAGAnalyzer.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <queue>
#include <utility>
#include <vector>

#include "utility/DAG.hpp"

#include "glog/logging.h"

namespace quickstep {

void DAGAnalyzer::findPipelines() {
  // Key = node ID, value = whether the node has been visited or not.
  std::unordered_map<std::size_t, bool> visited_nodes;
  // Pair 1st element: Operator ID
  // Pair 2nd element: The index of the pipeline in pipelines_ vector to which
  // the 1st element belongs.
  std::queue<std::pair<std::size_t, std::size_t>> to_be_checked_nodes;
  // Traverse the DAG once. Find leaf nodes and create a pipeline with one node.
  for (std::size_t node_id = 0; node_id < query_plan_dag_->size(); ++node_id) {
    if (query_plan_dag_->getDependencies(node_id).empty()) {
      pipelines_.push_back(std::unique_ptr<Pipeline>(new Pipeline(node_id)));
      to_be_checked_nodes.emplace(node_id, pipelines_.size() - 1);
      // No check needed here, as we are visiting these nodes for the first time.
      visited_nodes[node_id] = true;
    }
  }
  while (!to_be_checked_nodes.empty()) {
    auto queue_front = to_be_checked_nodes.front();
    to_be_checked_nodes.pop();
    const std::size_t pipeline_id = queue_front.second;
    const std::size_t operator_id = queue_front.first;
    auto dependents = query_plan_dag_->getDependents(operator_id);
    for (auto dependent_pair : dependents) {
      const std::size_t dependent_id = dependent_pair.first;
      const bool dependent_in_new_pipeline =
          (query_plan_dag_->getDependencies(dependent_id).size() > 1u) ||
          (dependent_pair.second);
      if (dependent_in_new_pipeline) {
        // Start a new pipeline.
        if (visited_nodes.find(dependent_id) == visited_nodes.end()) {
          pipelines_.push_back(std::unique_ptr<Pipeline>(new Pipeline(dependent_id)));
          to_be_checked_nodes.emplace(dependent_id, pipelines_.size() - 1);
          visited_nodes[dependent_id] = true;
        }
      } else {
        // This means that pipelining is enabled on this link. Add this
        // dependent to the current pipeline being processed.
        if (visited_nodes.find(dependent_id) == visited_nodes.end()) {
          pipelines_[pipeline_id]->addOperatorToPipeline(dependent_id);
          to_be_checked_nodes.emplace(dependent_id, pipeline_id);
          visited_nodes[dependent_id] = true;
        }
      }
    }
  }
  // Assuming that we have a connected DAG, make sure that all nodes belong to
  // some pipeline exactly once.
  DCHECK_EQ(query_plan_dag_->size(), getTotalNodes());
}

const std::size_t DAGAnalyzer::getTotalNodes() {
  std::size_t total = 0;
  for (std::size_t i = 0; i < pipelines_.size(); ++i) {
    total += pipelines_[i]->size();
  }
  return total;
}

}  // namespace quickstep
