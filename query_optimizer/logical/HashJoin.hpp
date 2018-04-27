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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_HASH_JOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_HASH_JOIN_HPP_

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/logical/BinaryJoin.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Cast.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class HashJoin;
typedef std::shared_ptr<const HashJoin> HashJoinPtr;

/**
 * @brief Hash join operator. It takes two relations and a single equality
 *        predicate between a pair of attributes from each input relation as input,
 *        scans one relation and probes the hash table on the other relation.
 */
class HashJoin : public BinaryJoin {
 public:
  enum class JoinType {
    kInnerJoin = 0,
    kLeftSemiJoin,
    kLeftAntiJoin,
    kLeftOuterJoin
  };

  LogicalType getLogicalType() const override { return LogicalType::kHashJoin; }

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
   * @return Join type of this hash join.
   */
  JoinType join_type() const {
    return join_type_;
  }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override {
    DCHECK_EQ(new_children.size(), children().size());
    return HashJoin::Create(new_children[0],
                            new_children[1],
                            left_join_attributes_,
                            right_join_attributes_,
                            residual_predicate_,
                            join_type_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    if (join_type_ != JoinType::kLeftOuterJoin) {
      return BinaryJoin::getOutputAttributes();
    }

    // For left outer join, all the output attributes from the right relation
    // must be nullable.
    std::vector<expressions::AttributeReferencePtr> output_attributes =
        left()->getOutputAttributes();
    const std::vector<expressions::AttributeReferencePtr> right_nullable_output_attributes =
        GetNullableAttributeVector(right()->getOutputAttributes());
    output_attributes.insert(output_attributes.end(),
                             right_nullable_output_attributes.begin(),
                             right_nullable_output_attributes.end());
    return output_attributes;
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    std::vector<expressions::AttributeReferencePtr> referenced_attributes =
        left_join_attributes_;
    referenced_attributes.insert(referenced_attributes.end(),
                                 right_join_attributes_.begin(),
                                 right_join_attributes_.end());
    if (residual_predicate_ != nullptr) {
      const std::vector<expressions::AttributeReferencePtr> referenced_attributes_in_residual =
          residual_predicate_->getReferencedAttributes();
      referenced_attributes.insert(referenced_attributes.end(),
                                   referenced_attributes_in_residual.begin(),
                                   referenced_attributes_in_residual.end());
    }
    return referenced_attributes;
  }

  /**
   * @brief Creates a logical HashJoin operator that combines two relations
   *        produced by the two input operator using join predicate. The node
   *        does not specify which one of the two relations is the build or
   *        the probe relation. The join predicate is the conjunction of
   *        equality predicates for pairs in left_join_attributes and
   *        right_join_attributes.
   *
   * @param left The left input operator.
   * @param right The right input operator.
   * @param left_join_attributes The join attributes in the 'left'.
   * @param right_join_attributes The join attributes in the 'right'.
   * @param Join type of this hash join.
   * @return An immutable HashJoin.
   */
  static HashJoinPtr Create(const LogicalPtr &left,
                            const LogicalPtr &right,
                            const std::vector<expressions::AttributeReferencePtr> &left_join_attributes,
                            const std::vector<expressions::AttributeReferencePtr> &right_join_attributes,
                            const expressions::PredicatePtr &residual_predicate,
                            const JoinType join_type) {
    return HashJoinPtr(
        new HashJoin(left,
                     right,
                     left_join_attributes,
                     right_join_attributes,
                     residual_predicate,
                     join_type));
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override {
    BinaryJoin::getFieldStringItems(inline_field_names,
                                    inline_field_values,
                                    non_container_child_field_names,
                                    non_container_child_fields,
                                    container_child_field_names,
                                    container_child_fields);

    if (residual_predicate_ != nullptr) {
      non_container_child_field_names->push_back("residual_predicate");
      non_container_child_fields->push_back(residual_predicate_);
    }

    container_child_field_names->push_back("left_join_attributes");
    container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(left_join_attributes_));
    container_child_field_names->push_back("right_join_attributes");
    container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(right_join_attributes_));
  }

 private:
  HashJoin(const LogicalPtr &left,
           const LogicalPtr &right,
           const std::vector<expressions::AttributeReferencePtr> &left_join_attributes,
           const std::vector<expressions::AttributeReferencePtr> &right_join_attributes,
           const expressions::PredicatePtr &residual_predicate,
           const JoinType join_type)
      : BinaryJoin(left, right),
        left_join_attributes_(left_join_attributes),
        right_join_attributes_(right_join_attributes),
        residual_predicate_(residual_predicate),
        join_type_(join_type) {
    DCHECK_EQ(left_join_attributes.size(), right_join_attributes.size());
    DCHECK(!left_join_attributes.empty() || residual_predicate != nullptr);

    if (residual_predicate_ != nullptr) {
      addInputExpression(residual_predicate_);
    }
  }

  const std::vector<expressions::AttributeReferencePtr> left_join_attributes_;
  const std::vector<expressions::AttributeReferencePtr> right_join_attributes_;
  const expressions::PredicatePtr residual_predicate_;
  const JoinType join_type_;

  DISALLOW_COPY_AND_ASSIGN(HashJoin);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_HASH_JOIN_HPP_ */
