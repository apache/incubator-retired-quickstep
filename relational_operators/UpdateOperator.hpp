/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_UPDATE_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_UPDATE_OPERATOR_HPP_

#include <cstddef>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogDatabase;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @note We assume that UpdateOperator won't have dependencies in the query plan
 *       DAG. Thus it will have all the input that it needs before execution.
 *       If at all in the future, if UpdateOperator has dependencies, we can
 *       follow similar arrangement as in SelectOperator.
 **/
class UpdateOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor
   *
   * @param relation The relation to perform the UPDATE over.
   * @param relocation_destination_index The index of the InsertDestination in
   *        the QueryContext to relocate tuples which can not be updated
   *        in-place.
   * @param predicate_index The index of predicate in QueryContext. All tuples
   *        matching pred will be updated (or kInvalidPredicateId to update all
   *        tuples).
   * @param update_group_index The index of a update group (the map of
   *        attribute_ids to Scalars) which should be evaluated to get the new
   *        value for the corresponding attribute.
   * @param foreman_client_id The TMB client ID of the Foreman thread.
   * @param bus A pointer to the TMB.
   *
   * @warning The constructed InsertDestination should belong to relation, but
   *          must NOT contain any pre-existing blocks.
   **/
  UpdateOperator(const CatalogRelation &relation,
                 const QueryContext::insert_destination_id relocation_destination_index,
                 const QueryContext::predicate_id predicate_index,
                 const QueryContext::update_group_id update_group_index,
                 const tmb::client_id foreman_client_id,
                 tmb::MessageBus *bus)
      : relation_(relation),
        relocation_destination_index_(relocation_destination_index),
        predicate_index_(predicate_index),
        update_group_index_(update_group_index),
        input_blocks_(relation.getBlocksSnapshot()),
        foreman_client_id_(foreman_client_id),
        bus_(bus),
        started_(false) {}

  ~UpdateOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container) override;

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return relocation_destination_index_;
  }

  const relation_id getOutputRelationID() const override {
    return relation_.getID();
  }

 private:
  const CatalogRelation &relation_;
  const QueryContext::insert_destination_id relocation_destination_index_;
  const QueryContext::predicate_id predicate_index_;
  const QueryContext::update_group_id update_group_index_;

  const std::vector<block_id> input_blocks_;

  const tmb::client_id foreman_client_id_;
  // TODO(zuyu): Remove 'bus_' once WorkOrder serialization is done.
  tmb::MessageBus *bus_;

  bool started_;

  DISALLOW_COPY_AND_ASSIGN(UpdateOperator);
};

/**
 * @brief A WorkOrder produced by UpdateOperator.
 **/
class UpdateWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor
   *
   * @param rel_id The id of the relation to perform the UPDATE over.
   * @param relocation_destination_index The index of the InsertDestination in
   *        the QueryContext to relocate tuples which can not be updated
   *        in-place.
   * @param predicate_index The index of predicate in QueryContext. All tuples
   *        matching pred will be updated (or kInvalidPredicateId to update all
   *        tuples).
   * @param update_group_index The index of a update group (the map of
   *        attribute_ids to Scalars) which should be evaluated to get the new
   *        value for the corresponding attribute.
   * @param input_block_id The block id.
   * @param update_operator_index The index of the Update Operator in the query
   *        plan DAG.
   * @param foreman_input_queue The input queue in Foreman.
   * @param bus A pointer to the TMB.
   **/
  UpdateWorkOrder(const relation_id rel_id,
                  const QueryContext::insert_destination_id relocation_destination_index,
                  const QueryContext::predicate_id predicate_index,
                  const QueryContext::update_group_id update_group_index,
                  const block_id input_block_id,
                  const std::size_t update_operator_index,
                  const tmb::client_id foreman_client_id,
                  MessageBus *bus)
      : rel_id_(rel_id),
        relocation_destination_index_(relocation_destination_index),
        predicate_index_(predicate_index),
        update_group_index_(update_group_index),
        input_block_id_(input_block_id),
        update_operator_index_(update_operator_index),
        foreman_client_id_(foreman_client_id),
        bus_(bus) {}

  ~UpdateWorkOrder() override {}

  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) override;

 private:
  const relation_id rel_id_;
  const QueryContext::insert_destination_id relocation_destination_index_;
  const QueryContext::predicate_id predicate_index_;
  const QueryContext::update_group_id update_group_index_;

  const block_id input_block_id_;

  const std::size_t update_operator_index_;
  const tmb::client_id foreman_client_id_;
  MessageBus *bus_;

  DISALLOW_COPY_AND_ASSIGN(UpdateWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_UPDATE_OPERATOR_HPP_
