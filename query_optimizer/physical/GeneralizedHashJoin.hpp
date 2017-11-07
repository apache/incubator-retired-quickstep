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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_GENERALIZED_HASHJOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_GENERALIZED_HASHJOIN_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/BinaryJoin.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class GeneralizedHashJoin;
typedef std::shared_ptr<const GeneralizedHashJoin> GeneralizedHashJoinPtr;

struct PartitionSchemeHeader;

/**
 * @brief Physical generalized hash join node.
 */
class GeneralizedHashJoin : public HashJoin {
 public:

  PhysicalType getPhysicalType() const override { return PhysicalType::kGeneralizedHashJoin; }

  /**
   * @return The middle operand.
   */
  const PhysicalPtr& middle() const { return middle_; }

  /**
   * @brief Join attributes in the left logical 'second_left_'.
   */
  const std::vector<expressions::AttributeReferencePtr>& second_left_join_attributes() const {
    return second_left_join_attributes_;
  }

  /**
   * @brief Join attributes in the right logical 'second_right_'.
   */
  const std::vector<expressions::AttributeReferencePtr>& second_right_join_attributes() const {
    return second_right_join_attributes_;
  }

  const expressions::PredicatePtr& second_residual_predicate() const {
    return second_residual_predicate_;
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(children().size(), new_children.size());
    return Create(new_children[0],
                  new_children[1],
                  new_children[2],
                  left_join_attributes_,
                  right_join_attributes_,
                  second_left_join_attributes_,
                  second_right_join_attributes_,
                  residual_predicate_,
                  second_residual_predicate_,
                  build_predicate_,
                  project_expressions(),
                  join_type_,
                  has_repartition_,
                  cloneOutputPartitionSchemeHeader());
  }

  PhysicalPtr copyWithNewOutputPartitionSchemeHeader(
      PartitionSchemeHeader *partition_scheme_header,
      const bool has_repartition = true) const override {
    return Create(left(), right(), middle(), left_join_attributes_, right_join_attributes_,
                  second_left_join_attributes_, second_right_join_attributes_, residual_predicate_, 
                  second_residual_predicate_, build_predicate_, project_expressions(), 
                  join_type_, has_repartition, partition_scheme_header);
  }

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override;

  /**
   * @brief Creates a physical GeneralizedHashJoin. The left/right operand does not correspond to
   *        probe/build operand.
   *
   * @param left The left operand.
   * @param middle The middle operand.
   * @param right The right operand.
   * @param left_join_attributes The join attributes in the 'left'.
   * @param right_join_attributes The join attributes in the 'right'.
   * @param second_left_join_attributes The second set of attributes to apply to the left side.
   * @param second_right_join_attributes The second set of attributes to apply to the right side.
   * @param residual_predicate Optional filtering predicate evaluated after join.
   * @param second_residual_predicate Optional filtering predicate evaulated after second join.
   * @param build_predicate Optional select predicate for a hash join fuse.
   * @param project_expressions The project expressions.
   * @param Join type of this hash join.
   * @param has_repartition Whether this node has repartition.
   * @param partition_scheme_header The optional output partition scheme header.
   *
   * @return An immutable physical HashJoin.
   */
  static GeneralizedHashJoinPtr Create(
      const PhysicalPtr &left,
      const PhysicalPtr &right,
      const PhysicalPtr &middle,
      const std::vector<expressions::AttributeReferencePtr> &left_join_attributes,
      const std::vector<expressions::AttributeReferencePtr> &right_join_attributes,
      const std::vector<expressions::AttributeReferencePtr> &second_left_join_attributes,
      const std::vector<expressions::AttributeReferencePtr> &second_right_join_attributes,
      const expressions::PredicatePtr &residual_predicate,
      const expressions::PredicatePtr &second_residual_predicate,
      const expressions::PredicatePtr &build_predicate,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions,
      const JoinType join_type,
      const bool has_repartition = false,
      PartitionSchemeHeader *partition_scheme_header = nullptr) {
    return GeneralizedHashJoinPtr(
        new GeneralizedHashJoin(left,
                     right,
                     middle,
                     left_join_attributes,
                     right_join_attributes,
                     second_left_join_attributes,
                     second_right_join_attributes,
                     residual_predicate,
                     second_residual_predicate,
                     build_predicate,
                     project_expressions,
                     join_type,
                     has_repartition,
                     partition_scheme_header));
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  GeneralizedHashJoin(
      const PhysicalPtr &left,
      const PhysicalPtr &right,
      const PhysicalPtr &middle,
      const std::vector<expressions::AttributeReferencePtr> &left_join_attributes,
      const std::vector<expressions::AttributeReferencePtr> &right_join_attributes,
      const std::vector<expressions::AttributeReferencePtr> &second_left_join_attributes,
      const std::vector<expressions::AttributeReferencePtr> &second_right_join_attributes,
      const expressions::PredicatePtr &residual_predicate,
      const expressions::PredicatePtr &second_residual_predicate,
      const expressions::PredicatePtr &build_predicate,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions,
      const JoinType join_type,
      const bool has_repartition,
      PartitionSchemeHeader *partition_scheme_header)
      : HashJoin(left, right, left_join_attributes,
                 right_join_attributes, residual_predicate, build_predicate, project_expressions,
                 join_type, has_repartition, partition_scheme_header),
        middle_(middle),
        second_left_join_attributes_(second_left_join_attributes),
        second_right_join_attributes_(second_right_join_attributes),
        second_residual_predicate_(second_residual_predicate) {
          addChild(middle_);
  }

  PhysicalPtr middle_;
  std::vector<expressions::AttributeReferencePtr> second_left_join_attributes_;
  std::vector<expressions::AttributeReferencePtr> second_right_join_attributes_;
  expressions::PredicatePtr second_residual_predicate_;

  DISALLOW_COPY_AND_ASSIGN(GeneralizedHashJoin);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif // QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_GENERALIZED_HASHJOIN_HPP_