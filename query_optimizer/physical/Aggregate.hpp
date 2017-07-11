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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_AGGREGATE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_AGGREGATE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class Aggregate;
typedef std::shared_ptr<const Aggregate> AggregatePtr;

struct PartitionSchemeHeader;

/**
 * @brief Aggregate operator that computes aggregate expressions for each combination
 *        of the values of grouping expressions.
 */
class Aggregate : public Physical {
 public:
  PhysicalType getPhysicalType() const override { return PhysicalType::kAggregate; }

  std::string getName() const override { return "Aggregate"; }

  /**
   * @return The input physical node.
   */
  const PhysicalPtr input() const { return input_; }

  /**
   * @return Grouping expressions.
   */
  inline const std::vector<expressions::NamedExpressionPtr>& grouping_expressions() const {
    return grouping_expressions_;
  }

  /**
   * @return Aggregate expressions.
   */
  const std::vector<expressions::AliasPtr>& aggregate_expressions() const {
    return aggregate_expressions_;
  }

  /**
   * @return Filter predicate applied before aggregation.
   */
  const expressions::PredicatePtr& filter_predicate() const {
    return filter_predicate_;
  }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(new_children[0], grouping_expressions_, aggregate_expressions_, filter_predicate_,
                  cloneOutputPartitionSchemeHeader());
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override;

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override;

  PhysicalPtr copyWithNewOutputPartitionSchemeHeader(
      PartitionSchemeHeader *partition_scheme_header) const override {
    return Create(input_, grouping_expressions_, aggregate_expressions_, filter_predicate_,
                  partition_scheme_header);
  }

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    // The project expressions for an Aggregate cannot be changed.
    return false;
  }

  /**
   * @brief Creates an Aggregate physical node.
   *
   * @param input The input node.
   * @param aggregate_expressions The aggregate expressions.
   * @param grouping_expressions The grouping expressions.
   * @param filter_predicate The filtering predicate applied before aggregation. Can be NULL.
   * @param partition_scheme_header The optional output partition scheme header.
   *
   * @return An immutable Aggregate node.
   */
  static AggregatePtr Create(
      PhysicalPtr input,
      const std::vector<expressions::NamedExpressionPtr> &grouping_expressions,
      const std::vector<expressions::AliasPtr> &aggregate_expressions,
      const expressions::PredicatePtr &filter_predicate,
      PartitionSchemeHeader *partition_scheme_header = nullptr) {
    return AggregatePtr(
        new Aggregate(input, grouping_expressions, aggregate_expressions, filter_predicate, partition_scheme_header));
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
  Aggregate(
      const PhysicalPtr &input,
      const std::vector<expressions::NamedExpressionPtr> &grouping_expressions,
      const std::vector<expressions::AliasPtr> &aggregate_expressions,
      const expressions::PredicatePtr &filter_predicate,
      PartitionSchemeHeader *partition_scheme_header)
      : Physical(partition_scheme_header),
        input_(input),
        grouping_expressions_(grouping_expressions),
        aggregate_expressions_(aggregate_expressions),
        filter_predicate_(filter_predicate) {
    addChild(input_);
  }

  const PhysicalPtr input_;
  const std::vector<expressions::NamedExpressionPtr> grouping_expressions_;
  const std::vector<expressions::AliasPtr> aggregate_expressions_;
  const expressions::PredicatePtr filter_predicate_;

  DISALLOW_COPY_AND_ASSIGN(Aggregate);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_AGGREGATE_HPP_ */
