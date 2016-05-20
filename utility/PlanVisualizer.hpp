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

#ifndef QUICKSTEP_UTILITY_PLAN_VISUALIZER_HPP_
#define QUICKSTEP_UTILITY_PLAN_VISUALIZER_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief A query plan visualizer that converts a physical plan into a graph in
 *        DOT format. Note that DOT is a plain text graph description language.
 *
 * @note This utility tool can be further extended to be more generic.
 */
class PlanVisualizer {
 public:
  PlanVisualizer()
      : id_counter_(0) {}

  ~PlanVisualizer() {}

  /**
   * @brief Visualize the query plan into a graph in DOT format (DOT is a plain
   *        text graph description language).
   *
   * @return The visualized query plan graph in DOT format.
   */
  std::string visualize(const optimizer::physical::PhysicalPtr &input);

 private:
  /**
   * @brief Information of a graph node.
   */
  struct NodeInfo {
    int id;
    std::vector<std::string> labels;
    std::string color;
  };

  /**
   * @brief Information of a graph edge.
   */
  struct EdgeInfo {
    int src_node_id;
    int dst_node_id;
    std::vector<std::string> labels;
  };

  void visit(const optimizer::physical::PhysicalPtr &input);

  int id_counter_;
  std::unordered_map<optimizer::physical::PhysicalPtr, int> node_id_map_;
  std::unordered_map<std::string, std::string> color_map_;

  std::vector<NodeInfo> nodes_;
  std::vector<EdgeInfo> edges_;

  std::unique_ptr<optimizer::cost::StarSchemaSimpleCostModel> cost_model_;

  DISALLOW_COPY_AND_ASSIGN(PlanVisualizer);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_UTILITY_PLAN_VISUALIZER_HPP_ */
