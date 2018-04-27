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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_HASHJOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_HASHJOIN_HPP_

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

class HashJoin;
typedef std::shared_ptr<const HashJoin> HashJoinPtr;

struct PartitionSchemeHeader;

/**
 * @brief Physical hash join node.
 */
class HashJoin : public BinaryJoin {
 public:
  enum class JoinType {
    kInnerJoin = 0,
    kLeftSemiJoin,
    kLeftAntiJoin,
    kLeftOuterJoin
  };

  PhysicalType getPhysicalType() const override { return PhysicalType::kHashJoin; }

  std::string getName() const override {
    switch (join_type_) {
      case JoinType::kInnerJoin:
        return "HashJoin";
      case JoinType::kLeftSemiJoin:
        return "HashLeftSemiJoin";
      case JoinType::kLeftAntiJoin:
        return "HashLeftAntiJoin";
      case JoinType::kLeftOuterJoin:
        return "HashLeftOuterJoin";
      default:
        LOG(FATAL) << "Invalid JoinType: "
                   << static_cast<typename std::underlying_type<JoinType>::type>(join_type_);
    }
  }

  /**
   * @brief Join attributes in the left logical 'left_'.
   */
  const std::vector<expressions::AttributeReferencePtr>& left_join_attributes() const {
    return left_join_attributes_;
  }

  /**
   * @brief Join attributes in the right logical 'right_'.
   */
  const std::vector<expressions::AttributeReferencePtr>& right_join_attributes() const {
    return right_join_attributes_;
  }

  /**
   * @brief The filtering predicate evaluated after join.
   */
  const expressions::PredicatePtr& residual_predicate() const {
    return residual_predicate_;
  }

  /**
   * @ brief The select predicate for a hash join fuse.
   */
  const expressions::PredicatePtr& build_predicate() const {
    return build_predicate_;
  }

  /**
   * @return Join type of this hash join.
   */
  JoinType join_type() const {
    return join_type_;
  }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(children().size(), new_children.size());
    return Create(new_children[0],
                  new_children[1],
                  left_join_attributes_,
                  right_join_attributes_,
                  residual_predicate_,
                  build_predicate_,
                  project_expressions(),
                  join_type_,
                  has_repartition_,
                  cloneOutputPartitionSchemeHeader());
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  PhysicalPtr copyWithNewOutputPartitionSchemeHeader(
      PartitionSchemeHeader *partition_scheme_header,
      const bool has_repartition = true) const override {
    return Create(left(), right(), left_join_attributes_, right_join_attributes_,
                  residual_predicate_, build_predicate_, project_expressions(), join_type_,
                  has_repartition, partition_scheme_header);
  }

  PhysicalPtr copyWithNewProjectExpressions(
      const std::vector<expressions::NamedExpressionPtr> &output_expressions) const override;

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override;

  /**
   * @brief Creates a physical HashJoin. The left/right operand does not correspond to
   *        probe/build operand.
   *
   * @param left The left operand.
   * @param right The right operand.
   * @param left_join_attributes The join attributes in the 'left'.
   * @param right_join_attributes The join attributes in the 'right'.
   * @param residual_predicate Optional filtering predicate evaluated after join.
   * @param build_predicate Optional select predicate for a hash join fuse.
   * @param project_expressions The project expressions.
   * @param Join type of this hash join.
   * @param has_repartition Whether this node has repartition.
   * @param partition_scheme_header The optional output partition scheme header.
   *
   * @return An immutable physical HashJoin.
   */
  static HashJoinPtr Create(
      const PhysicalPtr &left,
      const PhysicalPtr &right,
      const std::vector<expressions::AttributeReferencePtr> &left_join_attributes,
      const std::vector<expressions::AttributeReferencePtr> &right_join_attributes,
      const expressions::PredicatePtr &residual_predicate,
      const expressions::PredicatePtr &build_predicate,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions,
      const JoinType join_type,
      const bool has_repartition = false,
      PartitionSchemeHeader *partition_scheme_header = nullptr) {
    return HashJoinPtr(
        new HashJoin(left,
                     right,
                     left_join_attributes,
                     right_join_attributes,
                     residual_predicate,
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
  HashJoin(
      const PhysicalPtr &left,
      const PhysicalPtr &right,
      const std::vector<expressions::AttributeReferencePtr> &left_join_attributes,
      const std::vector<expressions::AttributeReferencePtr> &right_join_attributes,
      const expressions::PredicatePtr &residual_predicate,
      const expressions::PredicatePtr &build_predicate,
      const std::vector<expressions::NamedExpressionPtr> &project_expressions,
      const JoinType join_type,
      const bool has_repartition,
      PartitionSchemeHeader *partition_scheme_header)
      : BinaryJoin(left, right, project_expressions, has_repartition, partition_scheme_header),
        left_join_attributes_(left_join_attributes),
        right_join_attributes_(right_join_attributes),
        residual_predicate_(residual_predicate),
        build_predicate_(build_predicate),
        join_type_(join_type) {
  }

  const std::vector<expressions::AttributeReferencePtr> left_join_attributes_;
  const std::vector<expressions::AttributeReferencePtr> right_join_attributes_;
  const expressions::PredicatePtr residual_predicate_;
  const expressions::PredicatePtr build_predicate_;
  const JoinType join_type_;

  DISALLOW_COPY_AND_ASSIGN(HashJoin);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_HASHJOIN_HPP_ */
