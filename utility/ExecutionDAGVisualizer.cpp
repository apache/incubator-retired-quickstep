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

#include "utility/ExecutionDAGVisualizer.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogRelationSchema.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_optimizer/QueryPlan.hpp"
#include "relational_operators/AggregationOperator.hpp"
#include "relational_operators/BuildHashOperator.hpp"
#include "relational_operators/HashJoinOperator.hpp"
#include "relational_operators/SelectOperator.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

namespace quickstep {

ExecutionDAGVisualizer::ExecutionDAGVisualizer(const QueryPlan &plan) {
  // Do not display these relational operators in the graph.
  std::set<std::string> no_display_op_names =
      { "DestroyHashOperator", "DropTableOperator" };

  const auto &dag = plan.getQueryPlanDAG();
  num_nodes_ = dag.size();

  // Collect DAG vertices info.
  std::vector<bool> display_ops(num_nodes_, false);
  for (std::size_t node_index = 0; node_index < num_nodes_; ++node_index) {
    const auto &node = dag.getNodePayload(node_index);
    const std::string relop_name = node.getName();
    if (no_display_op_names.find(relop_name) == no_display_op_names.end()) {
      display_ops[node_index] = true;
      NodeInfo &node_info = nodes_[node_index];
      node_info.id = node_index;
      node_info.labels.emplace_back(
          "[" + std::to_string(node.getOperatorIndex()) + "] " + relop_name);

      std::vector<std::pair<std::string, const CatalogRelationSchema*>> input_relations;
      if (relop_name == "AggregationOperator") {
        const AggregationOperator &aggregation_op =
            static_cast<const AggregationOperator&>(node);
        input_relations.emplace_back("input", &aggregation_op.input_relation());
      } else if (relop_name == "BuildHashOperator") {
        const BuildHashOperator &build_hash_op =
            static_cast<const BuildHashOperator&>(node);
        input_relations.emplace_back("input", &build_hash_op.input_relation());
      } else if (relop_name == "HashJoinOperator") {
        const HashJoinOperator &hash_join_op =
            static_cast<const HashJoinOperator&>(node);
        input_relations.emplace_back("probe side", &hash_join_op.probe_relation());
      } else if (relop_name == "SelectOperator") {
        const SelectOperator &select_op =
            static_cast<const SelectOperator&>(node);
        input_relations.emplace_back("input", &select_op.input_relation());
      }
      for (const auto &rel_pair : input_relations) {
        if (!rel_pair.second->isTemporary()) {
          node_info.labels.emplace_back(
              rel_pair.first + " stored relation [" +
              rel_pair.second->getName() + "]");
        }
      }
    }
  }

  // Collect DAG edges info.
  for (std::size_t node_index = 0; node_index < num_nodes_; ++node_index) {
    if (display_ops[node_index]) {
      for (const auto &link : dag.getDependents(node_index)) {
        if (display_ops[link.first]) {
          edges_.emplace_back();
          edges_.back().src_node_id = node_index;
          edges_.back().dst_node_id = link.first;
          edges_.back().is_pipeline_breaker = link.second;
        }
      }
    }
  }
}

void ExecutionDAGVisualizer::bindProfilingStats(
  const std::vector<WorkOrderTimeEntry> &execution_time_records) {
  std::vector<std::size_t> time_start(num_nodes_, std::numeric_limits<std::size_t>::max());
  std::vector<std::size_t> time_end(num_nodes_, 0);
  std::vector<std::size_t> time_elapsed(num_nodes_, 0);
  std::size_t overall_start_time = std::numeric_limits<std::size_t>::max();
  std::size_t overall_end_time = 0;
  for (const auto &entry : execution_time_records) {
    const std::size_t relop_index = entry.operator_id;
    DCHECK_LT(relop_index, num_nodes_);

    const std::size_t workorder_start_time = entry.start_time;
    const std::size_t workorder_end_time = entry.end_time;
    overall_start_time = std::min(overall_start_time, workorder_start_time);
    overall_end_time = std::max(overall_end_time, workorder_end_time);

    time_start[relop_index] =
        std::min(time_start[relop_index], workorder_start_time);
    time_end[relop_index] =
        std::max(time_end[relop_index], workorder_end_time);
    time_elapsed[relop_index] += (workorder_end_time - workorder_start_time);
  }

  double total_time_elapsed = 0;
  for (std::size_t i = 0; i < time_elapsed.size(); ++i) {
    total_time_elapsed += time_elapsed[i];
  }
  std::vector<double> time_percentage(num_nodes_, 0);
  std::vector<double> span_percentage(num_nodes_, 0);
  double overall_span = overall_end_time - overall_start_time;
  double max_percentage = 0;
  for (std::size_t i = 0; i < time_elapsed.size(); ++i) {
    time_percentage[i] = time_elapsed[i] / total_time_elapsed * 100;
    span_percentage[i] = (time_end[i] - time_start[i]) / overall_span * 100;
    max_percentage = std::max(max_percentage, time_percentage[i] + span_percentage[i]);
  }

  for (std::size_t node_index = 0; node_index < num_nodes_; ++node_index) {
    if (nodes_.find(node_index) != nodes_.end()) {
      const std::size_t relop_start_time = time_start[node_index];
      const std::size_t relop_end_time = time_end[node_index];
      const std::size_t relop_elapsed_time = time_elapsed[node_index];
      NodeInfo &node_info = nodes_[node_index];

      const double hue =
          (time_percentage[node_index] + span_percentage[node_index]) / max_percentage;
      node_info.color = std::to_string(hue) + " " + std::to_string(hue) + " 1.0";

      if (overall_start_time == 0) {
        node_info.labels.emplace_back(
            "span: " +
            std::to_string((relop_end_time - relop_start_time) / 1000) + "ms");
      } else {
        node_info.labels.emplace_back(
            "span: [" +
            std::to_string((relop_start_time - overall_start_time) / 1000) + "ms, " +
            std::to_string((relop_end_time - overall_start_time) / 1000) + "ms] (" +
            FormatDigits(span_percentage[node_index], 2) + "%)");
      }

      node_info.labels.emplace_back(
          "total: " +
          std::to_string(relop_elapsed_time / 1000) + "ms (" +
          FormatDigits(time_percentage[node_index], 2) + "%)");

      const double concurrency =
          static_cast<double>(relop_elapsed_time) / (relop_end_time - relop_start_time);
      node_info.labels.emplace_back(
          "effective concurrency: " + FormatDigits(concurrency, 2));
    }
  }
}

std::string ExecutionDAGVisualizer::toDOT() {
  // Format output graph
  std::ostringstream graph_oss;
  graph_oss << "digraph g {\n";
  graph_oss << "  rankdir=BT\n";
  graph_oss << "  node [penwidth=2]\n";
  graph_oss << "  edge [fontsize=16 fontcolor=gray penwidth=2]\n\n";

  // Format nodes
  for (const auto &node_pair : nodes_) {
    const NodeInfo &node_info = node_pair.second;
    graph_oss << "  " << node_info.id << " [ ";
    if (!node_info.labels.empty()) {
      graph_oss << "label=\""
                << EscapeSpecialChars(JoinToString(node_info.labels, "&#10;"))
                << "\" ";
    }
    if (!node_info.color.empty()) {
      graph_oss << "style=filled fillcolor=\"" << node_info.color << "\" ";
    }
    graph_oss << "]\n";
  }
  graph_oss << "\n";

  // Format edges
  for (const EdgeInfo &edge_info : edges_) {
    graph_oss << "  " << edge_info.src_node_id << " -> "
              << edge_info.dst_node_id << " [ ";
    if (edge_info.is_pipeline_breaker) {
      graph_oss << "style=dashed ";
    }
    if (!edge_info.labels.empty()) {
      graph_oss << "label=\""
                << EscapeSpecialChars(JoinToString(edge_info.labels, "&#10;"))
                << "\" ";
    }
    graph_oss << "]\n";
  }
  graph_oss << "}\n";

  return graph_oss.str();
}

std::string ExecutionDAGVisualizer::FormatDigits(const double value,
                                                 const int num_digits) {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(num_digits) << value;
  return oss.str();
}

}  // namespace quickstep
