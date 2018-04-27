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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_DELETE_TUPLES_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_DELETE_TUPLES_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogRelation;

namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class DeleteTuples;
typedef std::shared_ptr<const DeleteTuples> DeleteTuplesPtr;

/**
 * @brief Deletes tuples on which a predicate <predicate_> is evaluated to true.
 */
class DeleteTuples : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kDeleteTuples; }

  std::string getName() const override { return "DeleteTuples"; }

  /**
   * @return The logical node that produces the relation where the DeleteTuples
   *         is applied.
   */
  const LogicalPtr& input() const { return input_; }

  /**
   * @return The predicate used to determine whether a tuple should be deleted.
   */
  const expressions::PredicatePtr& predicate() const { return predicate_; }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(new_children[0], predicate_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return {};
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  /**
   * @brief Creates a DeleteTuples logical node that represents an operation to
   *        delete tuples on which \p predicate is evaluated to true from the
   *        relation output by \p input.
   *
   * @param input The logical node that produces the relation where the
   *              DeleteTuples is applied.
   * @param predicate The predicate used to determine whether a tuple needs to
   *                  be deleted.
   * @return An immutable DeleteTuples node.
   */
  static DeleteTuplesPtr Create(const LogicalPtr &input,
                                const expressions::PredicatePtr &predicate) {
    return DeleteTuplesPtr(new DeleteTuples(input, predicate));
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
  DeleteTuples(const LogicalPtr &input,
               const expressions::PredicatePtr &predicate)
      : input_(input), predicate_(predicate) {
    addChild(input_);
  }

  const LogicalPtr input_;
  const expressions::PredicatePtr predicate_;

  DISALLOW_COPY_AND_ASSIGN(DeleteTuples);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_DELETE_TUPLES_HPP_ */
