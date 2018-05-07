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
#include <cstddef>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_optimizer/QueryPlan.hpp"
#include "relational_operators/AggregationOperator.hpp"
#include "relational_operators/BuildHashOperator.hpp"
#include "relational_operators/BuildLIPFilterOperator.hpp"
#include "relational_operators/HashJoinOperator.hpp"
#include "relational_operators/NestedLoopsJoinOperator.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/SelectOperator.hpp"
#include "relational_operators/SortRunGenerationOperator.hpp"
#include "relational_operators/UnionAllOperator.hpp"
#include "utility/DAG.hpp"
#include "utility/StringUtil.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

using std::to_string;

namespace quickstep {

DEFINE_bool(visualize_execution_dag_partition_info, false,
            "If true, display the operator partition info in the visualized "
            "execution plan DAG. Valid iff 'visualize_execution_dag' turns on.");

ExecutionDAGVisualizer::ExecutionDAGVisualizer(const QueryPlan &plan) {
  using ROEnumType =
      typename std::underlying_type<RelationalOperator::OperatorType>::type;

  // Do not display these relational operators in the graph.
  const std::unordered_set<ROEnumType> no_display_op_types =
      { RelationalOperator::kDestroyAggregationState,
        RelationalOperator::kDestroyHash,
        RelationalOperator::kDropTable };

  const auto &dag = plan.getQueryPlanDAG();
  num_nodes_ = dag.size();

  // Collect DAG vertices info.
  std::vector<bool> display_ops(num_nodes_, false);
  for (std::size_t node_index = 0; node_index < num_nodes_; ++node_index) {
    const auto &node = dag.getNodePayload(node_index);
    const RelationalOperator::OperatorType node_type = node.getOperatorType();
    if (no_display_op_types.find(node_type) != no_display_op_types.end()) {
      continue;
    }

    display_ops[node_index] = true;
    NodeInfo &node_info = nodes_[node_index];
    node_info.id = node_index;
    node_info.labels.emplace_back(
        "[" + std::to_string(node_index) + "] " + node.getName());

    const CatalogRelationSchema *input_relation = nullptr;
    std::string input_relation_info;
    switch (node_type) {
      case RelationalOperator::kAggregation: {
        const AggregationOperator &aggregation_op =
            static_cast<const AggregationOperator&>(node);
        input_relation = &aggregation_op.input_relation();
        input_relation_info = "input";
        break;
      }
      case RelationalOperator::kBuildHash: {
        const BuildHashOperator &build_hash_op =
            static_cast<const BuildHashOperator&>(node);
        input_relation = &build_hash_op.input_relation();
        input_relation_info = "input";
        break;
      }
      case RelationalOperator::kBuildLIPFilter: {
        const BuildLIPFilterOperator &build_lip_filter_op =
            static_cast<const BuildLIPFilterOperator&>(node);
        input_relation = &build_lip_filter_op.input_relation();
        input_relation_info = "input";
        break;
      }
      case RelationalOperator::kInnerJoin:
      case RelationalOperator::kLeftAntiJoin:
      case RelationalOperator::kLeftOuterJoin:
      case RelationalOperator::kLeftSemiJoin: {
        const HashJoinOperator &hash_join_op =
            static_cast<const HashJoinOperator&>(node);
        input_relation = &hash_join_op.probe_relation();
        input_relation_info = "probe side";
        break;
      }
      case RelationalOperator::kNestedLoopsJoin: {
        const NestedLoopsJoinOperator &nlj_op =
            static_cast<const NestedLoopsJoinOperator&>(node);

        const CatalogRelation &left_input_relation = nlj_op.left_input_relation();
        if (!left_input_relation.isTemporary()) {
          node_info.labels.emplace_back(
              "left input stored relation [" + left_input_relation.getName() + "]");
        }

        const CatalogRelation &right_input_relation = nlj_op.right_input_relation();
        if (!right_input_relation.isTemporary()) {
          node_info.labels.emplace_back(
              "right input stored relation [" + right_input_relation.getName() + "]");
        }
        break;
      }
      case RelationalOperator::kSelect: {
        const SelectOperator &select_op =
            static_cast<const SelectOperator&>(node);
        input_relation = &select_op.input_relation();
        input_relation_info = "input";
        break;
      }
      case RelationalOperator::kSortRunGeneration: {
        const SortRunGenerationOperator &sort_op =
            static_cast<const SortRunGenerationOperator&>(node);
        input_relation = &sort_op.input_relation();
        input_relation_info = "input";
        break;
      }
      case RelationalOperator::kUnionAll: {
        const UnionAllOperator &union_all_op = static_cast<const UnionAllOperator&>(node);

        std::string input_stored_relation_names;
        std::size_t num_input_stored_relations = 0;
        for (const auto &input_relation : union_all_op.input_relations()) {
          if (input_relation->isTemporary()) {
            continue;
          }
          ++num_input_stored_relations;

          if (!input_stored_relation_names.empty()) {
            input_stored_relation_names += ", ";
          }
          input_stored_relation_names += input_relation->getName();
        }

        if (!input_stored_relation_names.empty()) {
          node_info.labels.emplace_back(
              std::string("input stored relation") +
              (num_input_stored_relations > 1 ? "s" : "") +
              " [" + input_stored_relation_names + "]");
        }
        break;
      }
      default:
        break;
    }

    if (input_relation && !input_relation->isTemporary()) {
      node_info.labels.emplace_back(
          input_relation_info + " stored relation [" + input_relation->getName() + "]");
    }

    if (FLAGS_visualize_execution_dag_partition_info) {
      node_info.labels.emplace_back(
          "input #partitions = " + to_string(node.getNumPartitions()));
      node_info.labels.emplace_back(
          std::string("repartition = ") + (node.hasRepartition() ? "true" : "false"));
      node_info.labels.emplace_back(
          "output #partitions = " + to_string(node.getOutputNumPartitions()));
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
  std::unordered_map<std::size_t, std::size_t> workorders_count;
  std::unordered_map<std::size_t, float> mean_time_per_workorder;
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

    if (workorders_count.find(relop_index) == workorders_count.end()) {
      workorders_count[relop_index] = 0;
    }
    ++workorders_count[relop_index];
    if (mean_time_per_workorder.find(relop_index) ==
        mean_time_per_workorder.end()) {
      mean_time_per_workorder[relop_index] = 0;
    }
    mean_time_per_workorder[relop_index] += workorder_end_time - workorder_start_time;
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

      const auto cit = workorders_count.find(node_index);
      if (cit != workorders_count.end()) {
        const std::size_t workorders_count_for_node = cit->second;
        mean_time_per_workorder[node_index] /=
            (1000 * static_cast<float>(workorders_count_for_node));

        node_info.labels.emplace_back(std::to_string(workorders_count_for_node) + " work orders");
        node_info.labels.emplace_back(
            "Mean work order execution time: " +
            FormatDigits(mean_time_per_workorder[node_index], 2) + " ms");
      }
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
