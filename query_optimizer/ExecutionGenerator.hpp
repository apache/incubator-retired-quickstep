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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXECUTION_GENERATOR_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXECUTION_GENERATOR_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_optimizer/QueryHandle.hpp"
#include "query_optimizer/QueryPlan.hpp"
#include "query_optimizer/cost_model/CostModel.hpp"
#include "query_optimizer/cost_model/SimpleCostModel.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/CopyFrom.hpp"
#include "query_optimizer/physical/CreateTable.hpp"
#include "query_optimizer/physical/DeleteTuples.hpp"
#include "query_optimizer/physical/DropTable.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/InsertTuple.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/SharedSubplanReference.hpp"
#include "query_optimizer/physical/Sort.hpp"
#include "query_optimizer/physical/TableGenerator.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "query_optimizer/physical/UpdateTable.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogAttribute;
class CatalogRelation;
class Predicate;

namespace serialization {
class InsertDestination;
}  // namespace serialization

namespace optimizer {

class OptimizerContext;

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief Generates an execution plan from a physical plan.
 */
class ExecutionGenerator {
 public:
  /**
   * @brief Constructor. Does not take ownership of \p query_handle.
   *
   * @param optimizer_context Optimizer context.
   * @param query_handle The pointer to the output query handle.
   */
  ExecutionGenerator(OptimizerContext *optimizer_context,
                     QueryHandle *query_handle)
      : optimizer_context_(optimizer_context),
        query_handle_(query_handle),
        execution_plan_(query_handle->getQueryPlanMutable()),
        query_context_proto_(query_handle->getQueryContextProtoMutable()) {
    setupCostModel();
  }

  /**
   * @brief Destructor.
   */
  ~ExecutionGenerator() {}

  /**
   * @brief Generates an execution plan.
   *        The physical_plan must be rooted by a TopLevelPlan.
   *
   * @param physical_plan The physical plan from which
   *        the execution plan is created.
   */
  void generatePlan(const physical::PhysicalPtr &physical_plan);

 private:
  /**
   * @brief A CatalogRelation and the index of its producer operator.
   *        If the relation is a base relation, \p execution_operator_index
   *        is kInvalidOperatorIndex.
   */
  struct CatalogRelationInfo {
    CatalogRelationInfo(const QueryPlan::DAGNodeIndex producer_operator_index_in,
                        const CatalogRelation *relation_in)
        : producer_operator_index(producer_operator_index_in),
          relation(relation_in) {}

    /**
     * @return True if the relation is a stored relation (i.e. not a temporary relation
     *         created by a relational operator).
     */
    bool isStoredRelation() const {
      return producer_operator_index == kInvalidOperatorIndex;
    }

    const QueryPlan::DAGNodeIndex producer_operator_index;
    const CatalogRelation *relation;

    /**
     * @brief Represents an invalid node index.
     */
    static constexpr QueryPlan::DAGNodeIndex kInvalidOperatorIndex = static_cast<QueryPlan::DAGNodeIndex>(-1);

    // Copyable.
  };

  /**
   * @brief Internal implementation of generating an execution plan
   *        for a general physical plan.
   *
   * @param physical_plan The physical plan from which the execution
   *                      plan is created.
   */
  void generatePlanInternal(const physical::PhysicalPtr &physical_plan);

  /**
   * @brief Sets up the cost model.
   */
  void setupCostModel() {
    cost_model_.reset(new cost::SimpleCostModel());
  }

  /**
   * @brief Finds the CatalogRelationInfo from <physical_to_output_relation_map_>
   *        by the physical node \p physical. Returns NULL if not found.
   *
   * @param physical The physical node.
   * @return A mutable pointer to the catalog relation output by the physical node.
   */
  const CatalogRelationInfo* findRelationInfoOutputByPhysical(
      const physical::PhysicalPtr &physical) const {
    std::unordered_map<physical::PhysicalPtr, CatalogRelationInfo>::const_iterator it =
        physical_to_output_relation_map_.find(physical);
    if (it == physical_to_output_relation_map_.end()) {
      return nullptr;
    }
    return &it->second;
  }

  /**
   * @brief Creates and returns a temporary relation as the output of
   *        \p physical in \p catalog_relation_output, and a serialized insert
   *        destination in \p insert_destination_output.
   *
   * @param physical The input physical node.
   * @param catalog_relation_output The output catalog relation.
   * @param insert_destination_proto The output insert destination in proto
   *        form.
   */
  void createTemporaryCatalogRelation(
      const physical::PhysicalPtr &physical,
      const CatalogRelation **catalog_relation_output,
      serialization::InsertDestination *insert_destination_proto);

  /**
   * @brief Returns a new distinct relation name.
   *
   * @return A new distinct relation name.
   */
  std::string getNewRelationName();

  /**
   * @brief Sets up the info of the CatalogRelation represented by TableReference.
   *        TableReference is not converted to any operator.
   *
   * @param physical_plan The TableReference node.
   */
  void convertTableReference(const physical::TableReferencePtr &physical_plan);

  /**
   * @brief Attempt to convert a group of scalar projection expressions into a
   *        simple vector of attribute IDs.
   *
   * @param project_expressions_group_index The group index of scalar projection
   *        expressions in QueryContext proto.
   * @param attributes A vector of attribute_ids that will be filled in with
   *        the attributes to project if scalars constitute a pure projection.
   * @return true if all the scalar expressions in the group were attributes and
   *         conversion was successful, false otherwise.
   **/
  bool convertSimpleProjection(const QueryContext::scalar_group_id project_expressions_group_index,
                               std::vector<attribute_id> *attributes) const;

  /**
   * @brief Converts a Selection to a Select operator.
   *
   * @param physical_plan The Selection to be converted.
   */
  void convertSelection(const physical::SelectionPtr &physical_plan);

  /**
   * @brief Adds a map entry in <physical_to_execution_map_> from the
   *        SharedSubplanReference to the execution info of the
   *        referenced subplan.
   *
   * @param physical_plan The SharedSubplanReference to be converted.
   */
  void convertSharedSubplanReference(const physical::SharedSubplanReferencePtr &physical_plan);

  /**
   * @brief Converts a HashJoin to BuildHash, HashJoin and
   *        DestroyHash operators.
   *
   * @param physical_plan The HashJoin to be converted.
   */
  void convertHashJoin(const physical::HashJoinPtr &physical_plan);

  /**
   * @brief Converts a NestedLoopsJoin to a NestedLoopsJoin operator.
   *
   * @param physical_plan The NestedLoopsJoin to be converted.
   */
  void convertNestedLoopsJoin(const physical::NestedLoopsJoinPtr &physical_plan);

  /**
   * @brief Converts a CopyFrom to a TextScan and a SaveBlocks.
   *
   * @param physical_plan The CopyFrom to be converted.
   */
  void convertCopyFrom(const physical::CopyFromPtr &physical_plan);

  /**
   * @brief Converts a CreateTable to a CreateTable operator.
   *
   * @param physical_plan The CreateTable to be converted.
   */
  void convertCreateTable(const physical::CreateTablePtr &physical_plan);

  /**
   * @brief If there is a selection predicate in the DeleteTuples
   *        and the predicate value is not statically true,
   *        DeleteTuples is converted to a Delete and a SaveBlocks;
   *        if there is not a selection predicate or the predicate value
   *        not statically true, it is converted to a DropTableOperator;
   *        otherwise, no operator is created.
   *
   * @param physical_plan The DeleteTuples to be converted.
   */
  void convertDeleteTuples(const physical::DeleteTuplesPtr &physical_plan);

  /**
   * @brief Converts a DropTable to a DropTable operator.
   *
   * @param physical_plan The DropTable to be converted.
   */
  void convertDropTable(const physical::DropTablePtr &physical_plan);

  /**
   * @brief Converts an InsertTuple to an Insert and a SaveBlocks.
   *
   * @param physical_plan The InsertTuple to be converted.
   */
  void convertInsertTuple(const physical::InsertTuplePtr &physical_plan);

  /**
   * @brief Converts an UpdateTable to an Update and a SaveBlocks.
   *
   * @param physical_plan The UpdateTable to be converted.
   */
  void convertUpdateTable(const physical::UpdateTablePtr &physical_plan);

  /**
   * @brief Converts a physical Aggregate to an Aggregation operator
   *
   * @param physical_plan The Aggregate to be converted.
   */
  void convertAggregate(const physical::AggregatePtr &physical_plan);

  /**
   * @brief Converts a physical Sort to SortRunGeneration and SortMergeRun.
   *
   * @param physical_plan The Sort to be converted.
   */
  void convertSort(const physical::SortPtr &physical_plan);

  /**
   * @brief Converts a physical TableGenerator to a TableGeneratorOperator.
   *
   * @param physical_plan The TableGenerator to be converted.
   */
  void convertTableGenerator(const physical::TableGeneratorPtr &physical_plan);

  /**
   * @brief Converts a list of NamedExpressions in the optimizer expression
   *        system to scalars proto in QueryContext proto.
   *
   * @param named_expressions The list of NamedExpressions to be converted.
   * @param scalar_group_proto The corresponding scalars proto in QueryContext
   *        proto.
   */
  void convertNamedExpressions(
      const std::vector<expressions::NamedExpressionPtr> &named_expressions,
      serialization::QueryContext::ScalarGroup *scalar_group_proto);

  /**
   * @brief Converts a Predicate in the optimizer expression system to a
   *        Predicate in the execution expression system. The caller should
   *        takes ownership of the returned pointer.
   *
   * @param optimizer_predicate The Predicate to be converted.
   * @return The corresponding Predicate in the execution expression system.
   */
  Predicate* convertPredicate(const expressions::PredicatePtr &optimizer_predicate) const;

  /**
   * @brief Drops all temporary relations created by the generator
   *        from the database. Called to avoid side effects
   *        when some exception or error occurs.
   */
  void dropAllTemporaryRelations();

  OptimizerContext *optimizer_context_;
  QueryHandle *query_handle_;
  QueryPlan *execution_plan_;  // A part of QueryHandle.
  serialization::QueryContext *query_context_proto_;  // A part of QueryHandle.

  /**
   * @brief Used to generate distinct relation names for temporary relations.
   */
  relation_id rel_id_ = 0;

  /**
   * @brief For each entry <expr_id, catalog_attribute>,
   *        the NamedExpression with the expr_id is to be replaced with
   *        the catalog_attribute when an expression in the optimizer
   *        system is converted to one in the execution system.
   */
  std::unordered_map<expressions::ExprId, const CatalogAttribute *> attribute_substitution_map_;

  /**
   * @brief Map from a physical node to its output relation info.
   */
  std::unordered_map<physical::PhysicalPtr, CatalogRelationInfo> physical_to_output_relation_map_;

  /**
   * @brief All temporary relations created during query processing.
   */
  std::vector<CatalogRelationInfo> temporary_relation_info_vec_;

  /**
   * @brief The cost model to use for creating the execution plan.
   */
  std::unique_ptr<cost::CostModel> cost_model_;

  physical::TopLevelPlanPtr top_level_physical_plan_;

  DISALLOW_COPY_AND_ASSIGN(ExecutionGenerator);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXECUTION_GENERATOR_HPP_ */
