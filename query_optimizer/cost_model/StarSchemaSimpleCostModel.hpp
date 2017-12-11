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

#include "catalog/CatalogTypedefs.hpp"
#include "query_optimizer/cost_model/CostModel.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/CrossReferenceCoalesceAggregate.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
#include "query_optimizer/physical/FilterJoin.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/Sort.hpp"
#include "query_optimizer/physical/TableGenerator.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "query_optimizer/physical/TransitiveClosure.hpp"
#include "query_optimizer/physical/UnionAll.hpp"
#include "query_optimizer/physical/WindowAggregate.hpp"
#include "types/TypedValue.hpp"
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

  /**
   * @brief Check whether a set of attributes are unique (i.e. have distinct
   *        values) for a relation.
   *
   * @param physical_plan The physical plan that corresponds to a relation.
   * @param attributes The set of attributes to be checked. Note that each
   *        attribute in this set must be an output attribute of the physical
   *        plan.
   * @return True if it is guaranteed that the attributes are unique; false
   *         otherwise.
   */
  bool impliesUniqueAttributes(
      const physical::PhysicalPtr &physical_plan,
      const std::vector<expressions::AttributeReferencePtr> &attributes);

  /**
   * @brief For a physical plan attribute, find its correponding catalog attribute's
   *        MIN statistic. Returns Null value if there is no corresponding catalog
   *        attribute for the physical plan attribute.
   *
   * @param physical_plan The physical plan.
   * @param attribute The attribute. Must be an output attribute of the given
   *        physical plan.
   * @param is_exact_stat If this pointer is not null, its pointed content will
   *        be modified by this method to indicate whether the returned statistic
   *        is EXACT for the stored relation (i.e. not outdated or estimated).
   * @return The MIN statistic for the attribute.
   */
  TypedValue findMinValueStat(
      const physical::PhysicalPtr &physical_plan,
      const expressions::AttributeReferencePtr &attribute,
      bool *is_exact_stat = nullptr) {
    return findCatalogRelationStat(
        physical_plan, attribute->id(), StatType::kMin, is_exact_stat);
  }

  /**
   * @brief For a physical plan attribute, find its correponding catalog attribute's
   *        MAX statistic. Returns Null value if there is no corresponding catalog
   *        attribute for the physical plan attribute.
   *
   * @param physical_plan The physical plan.
   * @param attribute The attribute. Must be an output attribute of the given
   *        physical plan.
   * @param is_exact_stat If this pointer is not null, its pointed content will
   *        be modified by this method to indicate whether the returned statistic
   *        is EXACT for the stored relation (i.e. not not outdated or estimated).
   * @return The MAX statistic for the attribute.
   */
  TypedValue findMaxValueStat(
      const physical::PhysicalPtr &physical_plan,
      const expressions::AttributeReferencePtr &attribute,
      bool *is_exact_stat = nullptr) {
    return findCatalogRelationStat(
        physical_plan, attribute->id(), StatType::kMax, is_exact_stat);
  }

  /**
   * @brief Checks whether an aggregate node can be efficiently evaluated with
   *        the collision-free aggregation fast path.
   *
   * @param aggregate The physical aggregate node to be checked.
   * @param exact_num_groups If collision-free aggregation is applicable, the
   *        pointed content of this pointer will be set as the maximum possible
   *        number of groups that the collision-free hash table need to hold.
   * @return A bool value indicating whether collision-free aggregation can be
   *         used to evaluate \p aggregate.
   */
  bool canUseCollisionFreeAggregation(const physical::AggregatePtr &aggregate,
                                      std::size_t *max_num_groups);

  /**
   * @brief Checks whether an aggregate node can be efficiently evaluated with
   *        the two-phase compact key aggregation fast path.
   *
   * @param aggregate The physical aggregate node to be checked.
   * @param estimated_num_groups The estimated number of groups for the aggregate.
   * @return A bool value indicating whether two-phase compact key aggregation
   *         can be used to evaluate \p aggregate.
   */
  bool canUseTwoPhaseCompactKeyAggregation(const physical::AggregatePtr &aggregate,
                                           const std::size_t estimated_num_groups);

  bool canUseCompactKeySeparateChainingAggregation(const physical::AggregatePtr &aggregate);

 private:
  std::size_t estimateCardinalityForAggregate(
      const physical::AggregatePtr &physical_plan);

  std::size_t estimateCardinalityForCrossReferenceCoalesceAggregate(
      const physical::CrossReferenceCoalesceAggregatePtr &physical_plan);

  std::size_t estimateCardinalityForFilterJoin(
      const physical::FilterJoinPtr &physical_plan);

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

  std::size_t estimateCardinalityForTransitiveClosure(
      const physical::TransitiveClosurePtr &physical_plan);

  std::size_t estimateCardinalityForWindowAggregate(
      const physical::WindowAggregatePtr &physical_plan);

  std::size_t estimateCardinalityForUnionAll(
      const physical::UnionAllPtr &physical_plan);

  double estimateSelectivityForPredicate(
      const expressions::PredicatePtr &filter_predicate,
      const physical::PhysicalPtr &physical_plan);

  const std::vector<physical::PhysicalPtr> &shared_subplans_;

  // Get the number of distinct values of an attribute in the table reference.
  // If the stat is not avaiable, simply returns the table's cardinality.
  std::size_t getNumDistinctValues(const expressions::ExprId attribute_id,
                                   const physical::TableReferencePtr &table_reference);

  enum class StatType {
    kMax = 0,
    kMin
  };

  // For a physical plan attribute, find its correponding catalog attribute's
  // min/max statistics. Returns Null value if there is no corresponding catalog
  // attribute for the physical plan attribute (e.g. the attribute is the result
  // of an expression).
  TypedValue findCatalogRelationStat(
      const physical::PhysicalPtr &physical_plan,
      const expressions::ExprId expr_id,
      const StatType stat_type,
      bool *is_exact_stat);

  // For a table reference attribute, find its correponding catalog attribute.
  attribute_id findCatalogRelationAttributeId(
      const physical::TableReferencePtr &table_reference,
      const expressions::ExprId expr_id);

  DISALLOW_COPY_AND_ASSIGN(StarSchemaSimpleCostModel);
};

/** @} */

}  // namespace cost
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUERY_OPTIMIZER_COST_MODEL_STAR_SCHEMA_SIMPLE_COST_MODEL_HPP_ */
