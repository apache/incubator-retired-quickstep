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

#ifndef QUERY_OPTIMIZER_COST_MODEL_STAR_SCHEMA_SIMPLE_COST_MODEL_HPP_
#define QUERY_OPTIMIZER_COST_MODEL_STAR_SCHEMA_SIMPLE_COST_MODEL_HPP_

#include <cstddef>
#include <vector>

#include "query_optimizer/cost_model/CostModel.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/Sort.hpp"
#include "query_optimizer/physical/TableGenerator.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "query_optimizer/physical/WindowAggregate.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace cost {

/** \addtogroup CostModel
 *  @{
 */

/**
 * @brief A simple cost model for hash join planning.
 */
class StarSchemaSimpleCostModel : public CostModel {
 public:
  /**
   * @brief Constructor.
   */
  explicit StarSchemaSimpleCostModel(const std::vector<physical::PhysicalPtr> &shared_subplans)
      : shared_subplans_(shared_subplans) {}

  /**
   * @brief Estimate the cardinality of a physical plan.
   *
   * @param physical_plan The physical plan.
   * @return The estimated cardinality.
   */
  std::size_t estimateCardinality(
      const physical::PhysicalPtr &physical_plan) override;

  /**
   * @brief Estimate the number of groups in an aggregation.
   *
   * @param aggregate The physical plan of the aggregation.
   * @return The estimated number of groups.
   */
  std::size_t estimateNumGroupsForAggregate(
      const physical::AggregatePtr &aggregate) override;

  /**
   * @brief Estimate the number of distinct values of an attribute in a relation.
   * 
   * @param attribute_id The expression id of the target attribute.
   * @param physical_plan The physical plan of the attribute's relation.
   * @return The estimated number of distinct values for the attribute.
   */
  std::size_t estimateNumDistinctValues(const expressions::ExprId attribute_id,
                                        const physical::PhysicalPtr &physical_plan);

  /**
   * @brief Estimate the "selectivity" of a physical plan under the assumption
   *        that it acts as a filtered dimension table in a hash join.
   *
   * @param phyiscal_plan The physical plan.
   * @return The estimated selectivity.
   */
  double estimateSelectivity(const physical::PhysicalPtr &physical_plan);

  /**
   * @brief Estimate the filter predicate's selectivity if it is present in
   *        the input plan's root node.
   *
   * @param physical_plan The input physical plan.
   * @return The estimated selectivity of the filter predicate if physical_plan
   *         has such a filter predicate; 1.0 otherwise.
   */
  double estimateSelectivityForFilterPredicate(
      const physical::PhysicalPtr &physical_plan);

 private:
  std::size_t estimateCardinalityForAggregate(
      const physical::AggregatePtr &physical_plan);

  std::size_t estimateCardinalityForHashJoin(
      const physical::HashJoinPtr &physical_plan);

  std::size_t estimateCardinalityForNestedLoopsJoin(
      const physical::NestedLoopsJoinPtr &physical_plan);

  std::size_t estimateCardinalityForSelection(
      const physical::SelectionPtr &physical_plan);

  std::size_t estimateCardinalityForSort(
      const physical::SortPtr &physical_plan);

  std::size_t estimateCardinalityForTableGenerator(
      const physical::TableGeneratorPtr &physical_plan);

  std::size_t estimateCardinalityForTableReference(
      const physical::TableReferencePtr &physical_plan);

  std::size_t estimateCardinalityForTopLevelPlan(
      const physical::TopLevelPlanPtr &physical_plan);

  std::size_t estimateCardinalityForWindowAggregate(
      const physical::WindowAggregatePtr &physical_plan);

  double estimateSelectivityForPredicate(
      const expressions::PredicatePtr &filter_predicate,
      const physical::PhysicalPtr &physical_plan);

  const std::vector<physical::PhysicalPtr> &shared_subplans_;

  // Get the number of distinct values of an attribute in the table reference.
  // If the stat is not avaiable, simply returns the table's cardinality.
  std::size_t getNumDistinctValues(const expressions::ExprId attribute_id,
                                   const physical::TableReferencePtr &table_reference);


  DISALLOW_COPY_AND_ASSIGN(StarSchemaSimpleCostModel);
};

/** @} */

}  // namespace cost
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUERY_OPTIMIZER_COST_MODEL_STAR_SCHEMA_SIMPLE_COST_MODEL_HPP_ */
