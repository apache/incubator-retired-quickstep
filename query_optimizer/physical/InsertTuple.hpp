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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_INSERT_TUPLE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_INSERT_TUPLE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/ScalarLiteral.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogRelation;

namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class InsertTuple;
typedef std::shared_ptr<const InsertTuple> InsertTuplePtr;

/**
 * @brief Inserts a tuple with <column_values_> into a relation
 *        <catalog_relation>.
 */
class InsertTuple : public Physical {
 public:
  PhysicalType getPhysicalType() const override {
    return PhysicalType::kInsertTuple;
  }

  std::string getName() const override { return "InsertTuple"; }

  /**
   * @return The input that produces the relation to insert the tuple to.
   */
  const PhysicalPtr& input() const { return input_; }

  /**
   * @return Column values to be used to compose a new tuple.
   */
  const std::vector<std::vector<expressions::ScalarLiteralPtr>>& column_values() const {
    return column_values_;
  }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(new_children[0], column_values_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return {};
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return {};
  }

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    return false;
  }

  /**
   * @brief Creates an InsertTuple physical node.
   *
   * @param input The input produces the relation to insert the tuple to.
   * @param column_values The column values of the tuple to be inserted.
   *                      The type checking is done in the Resolver.
   * @return An immutable InsertTuple node.
   */
  static InsertTuplePtr Create(
      const PhysicalPtr &input,
      const std::vector<std::vector<expressions::ScalarLiteralPtr>> &column_values) {
    return InsertTuplePtr(new InsertTuple(input, column_values));
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
  InsertTuple(const PhysicalPtr &input,
              const std::vector<std::vector<expressions::ScalarLiteralPtr>> &column_values)
      : input_(input), column_values_(column_values) {
    addChild(input_);
  }

  const PhysicalPtr input_;
  const std::vector<std::vector<expressions::ScalarLiteralPtr>> column_values_;

  DISALLOW_COPY_AND_ASSIGN(InsertTuple);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_INSERT_TUPLE_HPP_ */
