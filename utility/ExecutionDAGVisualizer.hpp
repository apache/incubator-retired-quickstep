/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#ifndef QUICKSTEP_UTILITY_EXECUTION_DAG_VISUALIZER_HPP_
#define QUICKSTEP_UTILITY_EXECUTION_DAG_VISUALIZER_HPP_

#include <cstddef>
#include <map>
#include <string>
#include <vector>

#include "utility/Macros.hpp"

namespace quickstep {

class QueryPlan;
struct WorkOrderTimeEntry;

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief A visualizer that converts an execution plan DAG into a graph in
 *        DOT format. Note that DOT is a plain text graph description language.
 *
 * @note This utility tool can be further extended to be more generic.
 */
class ExecutionDAGVisualizer {
 public:
  /**
   * @brief Constructor
   *
   * @param plan The execution plan to be visualized.
   */
  explicit ExecutionDAGVisualizer(const QueryPlan &plan);

  /**
   * @brief Destructor
   */
  ~ExecutionDAGVisualizer() {}

  /**
   * @brief Summarize the execution timing stats and bind the stats to the
   *        corresponding relational operators in the execution plan.
   *
   * @param execution_time_records The profiled timing records of execution.
   */
  void bindProfilingStats(
      const std::vector<WorkOrderTimeEntry> &execution_time_records);

  /**
   * @brief Get the string represenation of the visualized execution plan
   *        in DOT format (DOT is a plain text graph description language).
   *
   * @return The execution plan graph in DOT format.
   */
  std::string toDOT();

 private:
  /**
   * @brief Format a float value to string representation with the specified
   *        number of decimal digits.
   */
  static std::string FormatDigits(const double value,
                                  const int num_digits);

  /**
   * @brief Information of a graph node.
   */
  struct NodeInfo {
    std::size_t id;
    std::vector<std::string> labels;
    std::string color;
  };

  /**
   * @brief Information of a graph edge.
   */
  struct EdgeInfo {
    std::size_t src_node_id;
    std::size_t dst_node_id;
    std::vector<std::string> labels;
    bool is_pipeline_breaker;
  };

  std::size_t num_nodes_;
  std::map<std::size_t, NodeInfo> nodes_;
  std::vector<EdgeInfo> edges_;

  DISALLOW_COPY_AND_ASSIGN(ExecutionDAGVisualizer);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_UTILITY_EXECUTION_DAG_VISUALIZER_HPP_ */
