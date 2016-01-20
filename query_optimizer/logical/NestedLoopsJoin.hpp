/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_NESTED_LOOPS_JOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_NESTED_LOOPS_JOIN_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/PredicateLiteral.hpp"
#include "query_optimizer/logical/BinaryJoin.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class NestedLoopsJoin;
typedef std::shared_ptr<const NestedLoopsJoin> NestedLoopsJoinPtr;

/**
 * @brief Logical nested loops join operator that joins two relations by using
 *        two nested loops.
 */
class NestedLoopsJoin : public BinaryJoin {
 public:
  LogicalType getLogicalType() const override {
    return LogicalType::kNestedLoopsJoin;
  }

  std::string getName() const override { return "NestedLoopsJoin"; }

  /**
   * @return The join predicate.
   */
  const expressions::PredicatePtr& join_predicate() const {
    return join_predicate_;
  }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override {
    DCHECK_EQ(new_children.size(), children().size());
    return NestedLoopsJoin::Create(new_children[0],
                                   new_children[1],
                                   join_predicate_);
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return join_predicate_->getReferencedAttributes();
  }

  /**
   * @brief Creates a NestedLoopsJoin that joins \p left_operand and
   *        \p right_operand based on \p join_predicate.
   *
   * @param left_operand The left input operator.
   * @param right_operand The right input operator.
   * @param join_predicate The join predicate. Can be NULL for a cross-product join.
   * @return An immutable NestedLoopsJoin.
   */
  static NestedLoopsJoinPtr Create(
      const LogicalPtr &left_operand,
      const LogicalPtr &right_operand,
      const expressions::PredicatePtr &join_predicate) {
    if (join_predicate == nullptr) {
      return NestedLoopsJoinPtr(new NestedLoopsJoin(left_operand,
                                                    right_operand,
                                                    expressions::PredicateLiteral::Create(true)));
    } else {
      return NestedLoopsJoinPtr(new NestedLoopsJoin(left_operand, right_operand, join_predicate));
    }
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
    non_container_child_field_names->push_back("join_predicate");
    non_container_child_fields->push_back(join_predicate_);
  }

 private:
  NestedLoopsJoin(const LogicalPtr &left_operand,
                  const LogicalPtr &right_operand,
                  const expressions::PredicatePtr &join_predicate)
      : BinaryJoin(left_operand, right_operand),
        join_predicate_(join_predicate) {}

  expressions::PredicatePtr join_predicate_;

  DISALLOW_COPY_AND_ASSIGN(NestedLoopsJoin);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_NESTED_LOOPS_JOIN_HPP_ */
