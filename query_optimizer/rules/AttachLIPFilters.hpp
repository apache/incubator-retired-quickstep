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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_ATTACH_LIP_FILTERS_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_ATTACH_LIP_FILTERS_HPP_

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/physical/LIPFilterConfiguration.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Rule that applies to a physical plan to attach LIPFilters.
 */
class AttachLIPFilters : public Rule<physical::Physical> {
 public:
  /**
   * @brief Constructor.
   */
  AttachLIPFilters() {}

  ~AttachLIPFilters() override {}

  std::string getName() const override {
    return "AttachLIPFilters";
  }

  physical::PhysicalPtr apply(const physical::PhysicalPtr &input) override;

 private:
  /**
   * @brief Internal data structure for passing around LIPFilter information.
   */
  struct LIPFilterInfo {
    LIPFilterInfo(const expressions::AttributeReferencePtr &attribute_in,
                  const physical::PhysicalPtr &source_in,
                  const int depth_in,
                  const double estimated_selectivity_in,
                  const std::size_t estimated_cardinality_in,
                  const expressions::AttributeReferencePtr &source_attribute_in = nullptr)
        : attribute(attribute_in),
          source(source_in),
          depth(depth_in),
          estimated_selectivity(estimated_selectivity_in),
          estimated_cardinality(estimated_cardinality_in),
          source_attribute(
              source_attribute_in == nullptr
                  ? attribute_in
                  : source_attribute_in) {}

    static bool isBetterThan(const LIPFilterInfo &a, const LIPFilterInfo &b) {
      if (a.estimated_selectivity == b.estimated_selectivity) {
        return a.depth > b.depth;
      } else {
        return a.estimated_selectivity < b.estimated_selectivity;
      }
    }

    expressions::AttributeReferencePtr attribute;
    physical::PhysicalPtr source;
    int depth;
    double estimated_selectivity;
    std::size_t estimated_cardinality;
    expressions::AttributeReferencePtr source_attribute;
  };

  typedef std::shared_ptr<const LIPFilterInfo> LIPFilterInfoPtr;

  /**
   * @brief Functional list data structure for internal use.
   */
  struct NodeList {
    explicit NodeList(const physical::PhysicalPtr &node_in)
        : node(node_in),
          next(nullptr),
          depth(0) {}

    NodeList(const physical::PhysicalPtr &node_in,
             const NodeList *next_in,
             const int depth_in)
        : node(node_in),
          next(next_in),
          depth(depth_in) {}

    inline const NodeList *cdr() const {
      return next;
    }

    inline const NodeList cons(const physical::PhysicalPtr &new_node) const {
      return NodeList(new_node, this, depth+1);
    }

    const physical::PhysicalPtr node;
    const NodeList *next;
    const int depth;
  };

  void attachLIPFilters(const NodeList &path,
                        std::set<expressions::ExprId> *already_filtered_attributes);

  const std::vector<LIPFilterInfoPtr>& getBuildSideInfo(const NodeList &path);

  const std::vector<LIPFilterInfoPtr>& getProbeSideInfo(const NodeList &path);

  // TODO(jianqiao): refactor this method as it is a duplication of
  // InjectJoinFilters::findExactMinMaxValuesForAttributeHelper().
  bool findExactMinMaxValuesForAttributeHelper(
      const physical::PhysicalPtr &physical_plan,
      const expressions::AttributeReferencePtr &attribute,
      std::int64_t *min_cpp_value,
      std::int64_t *max_cpp_value) const;

  std::unique_ptr<cost::StarSchemaSimpleCostModel> cost_model_;
  std::map<physical::PhysicalPtr, std::vector<LIPFilterInfoPtr>> build_side_info_;
  std::map<physical::PhysicalPtr, std::vector<LIPFilterInfoPtr>> probe_side_info_;
  std::unique_ptr<physical::LIPFilterConfiguration> lip_filter_configuration_;

  DISALLOW_COPY_AND_ASSIGN(AttachLIPFilters);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_ATTACH_LIP_FILTERS_HPP_ */
