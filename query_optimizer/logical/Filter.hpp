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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_FILTER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_FILTER_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/LogicalAnd.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
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

class Filter;
typedef std::shared_ptr<const Filter> FilterPtr;

/**
 * @brief Applies a filter to the input tuples and outputs a tuple only if the
 *        filter evaluates to true.
 */
class Filter : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kFilter; }

  std::string getName() const override { return "Filter"; }

  /**
   * @return The filter predicate.
   */
  const expressions::PredicatePtr& filter_predicate() const {
    return filter_predicate_;
  }

  /**
   * @return The input operator.
   */
  const LogicalPtr& input() const { return children()[0]; }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override {
    DCHECK_EQ(new_children.size(), children().size());
    return Filter::Create(new_children[0], filter_predicate_);
  }

  LogicalPtr copyWithNewInputExpressions(
      const std::vector<expressions::ExpressionPtr> &input_expressions) const override;

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return input()->getOutputAttributes();
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return filter_predicate_->getReferencedAttributes();
  }

  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *
          container_child_fields) const override;

  /**
   * @brief Creates a Filter that applies \p filter_predicate to the tuples
   *        produced by \p input.
   * @note If the input is a Filter, it will be pulled up and merged into the
   *       new Filter.
   *
   * @param input The input operator.
   * @param filter_predicate The filter predicate.
   * @return An immutable Filter.
   */
  static FilterPtr Create(const LogicalPtr &input,
                          const expressions::PredicatePtr &filter_predicate) {
    return FilterPtr(new Filter(input, filter_predicate));
  }

  /**
   * @brief Construct a (conjunctive) predicate on \p filter_predicates and
   *        creates a Filter that applies the predicates to the tuples produced
   *        by \p input.
   * @note If the input is a Filter, it will be pulled up and merged into the
   *       new Filter.
   *
   * @param input The input operator.
   * @param filter_predicates The filter predicates to be applied to the input.
   * @return An immutable Filter.
   */
  static FilterPtr Create(
      const LogicalPtr &input,
      const std::vector<expressions::PredicatePtr> &filter_predicates) {
    if (filter_predicates.size() > 1u) {
      return Create(input, expressions::LogicalAnd::Create(filter_predicates));
    } else {
      return Create(input, filter_predicates[0]);
    }
  }

 private:
  Filter(const LogicalPtr &input,
         const expressions::PredicatePtr &filter_predicate);

  expressions::PredicatePtr filter_predicate_;

  DISALLOW_COPY_AND_ASSIGN(Filter);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_FILTER_HPP_ */
