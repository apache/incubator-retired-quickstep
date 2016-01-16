/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_FINALIZE_AGGREGATION_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_FINALIZE_AGGREGATION_OPERATOR_HPP_

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which finalizes aggregation and writes output tuples.
 */
class FinalizeAggregationOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor for finalizing aggregation state and writing output
   * tuples.  The actual aggregation is computed by the AggregationOperator.
   *
   * @param aggr_state_index The index of the AggregationState in QueryContext.
   * @param output_relation_id The id of the output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert aggregation results.
   */
  FinalizeAggregationOperator(const QueryContext::aggregation_state_id aggr_state_index,
                              const relation_id output_relation_id,
                              const QueryContext::insert_destination_id output_destination_index)
      : aggr_state_index_(aggr_state_index),
        output_relation_id_(output_relation_id),
        output_destination_index_(output_destination_index),
        started_(false) {}

  ~FinalizeAggregationOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container) override;

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return output_destination_index_;
  }

  const relation_id getOutputRelationID() const override {
    return output_relation_id_;
  }

 private:
  const QueryContext::aggregation_state_id aggr_state_index_;
  const relation_id output_relation_id_;
  const QueryContext::insert_destination_id output_destination_index_;
  bool started_;

  DISALLOW_COPY_AND_ASSIGN(FinalizeAggregationOperator);
};

/**
 * @brief A WorkOrder produced by FinalizeAggregationOperator.
 **/
class FinalizeAggregationWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param aggr_state_index The index of the AggregationState in QueryContext.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert aggregation results.
   */
  FinalizeAggregationWorkOrder(
      const QueryContext::aggregation_state_id aggr_state_index,
      const QueryContext::insert_destination_id output_destination_index)
      : aggr_state_index_(aggr_state_index),
        output_destination_index_(output_destination_index) {}

  ~FinalizeAggregationWorkOrder() override {}

  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) override;

 private:
  const QueryContext::aggregation_state_id aggr_state_index_;
  const QueryContext::insert_destination_id output_destination_index_;

  DISALLOW_COPY_AND_ASSIGN(FinalizeAggregationWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_FINALIZE_AGGREGATION_OPERATOR_HPP_
