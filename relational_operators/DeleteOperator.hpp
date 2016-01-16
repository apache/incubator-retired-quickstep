/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_DELETE_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_DELETE_OPERATOR_HPP_

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

#include "glog/logging.h"

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
 * @brief An operator which deletes some tuples.
 **/
class DeleteOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param relation The relation to perform the DELETE over.
   * @param predicate_index The index of Predicate in QueryContext. All tuples
   *        matching pred will be deleted (If kInvalidPredicateId, then all
   *        tuples will be deleted).
   * @param relation_is_stored If relation is a stored relation and is fully
   *        available to the operator before it can start generating workorders.
   * @param foreman_client_id The TMB client ID of the Foreman thread.
   * @param bus A pointer to the TMB.
   **/
  DeleteOperator(const CatalogRelation &relation,
                 const QueryContext::predicate_id predicate_index,
                 const bool relation_is_stored,
                 const tmb::client_id foreman_client_id,
                 tmb::MessageBus *bus)
     :  relation_(relation),
        predicate_index_(predicate_index),
        relation_is_stored_(relation_is_stored),
        foreman_client_id_(foreman_client_id),
        bus_(bus),
        started_(false),
        relation_block_ids_(relation_is_stored ? relation.getBlocksSnapshot()
                                               : std::vector<block_id>()),
        num_workorders_generated_(0) {}

  ~DeleteOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container) override;

  const relation_id getOutputRelationID() const override {
    return relation_.getID();
  }

  void feedInputBlock(const block_id input_block_id, const relation_id input_relation_id) override {
    DCHECK(!relation_is_stored_);
    relation_block_ids_.push_back(input_block_id);
  }

  void feedInputBlocks(const relation_id rel_id, std::vector<block_id> *partially_filled_blocks) override {
    DCHECK(!relation_is_stored_);
    relation_block_ids_.insert(relation_block_ids_.end(),
                               partially_filled_blocks->begin(),
                               partially_filled_blocks->end());
  }

 private:
  const CatalogRelation &relation_;
  const QueryContext::predicate_id predicate_index_;

  const bool relation_is_stored_;

  const tmb::client_id foreman_client_id_;
  // TODO(zuyu): Remove 'bus_' once WorkOrder serialization is done.
  tmb::MessageBus *bus_;

  bool started_;

  std::vector<block_id> relation_block_ids_;
  std::vector<block_id>::size_type num_workorders_generated_;

  DISALLOW_COPY_AND_ASSIGN(DeleteOperator);
};

/**
 * @brief A WorkOrder produced by DeleteOperator.
 **/
class DeleteWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param rel_id The id of the relation to perform the DELETE over.
   * @param predicate_index The index of Predicate in QueryContext. All tuples
   *        matching pred will be deleted (If kInvalidPredicateId, then all
   *        tuples will be deleted).
   * @param input_block_id The block Id.
   * @param delete_operator_index The index of the Delete Operator in the query
   *        plan DAG.
   * @param foreman_client_id The TMB client ID of the Foreman thread.
   * @param bus A pointer to the TMB.
   **/
  DeleteWorkOrder(const relation_id rel_id,
                  const QueryContext::predicate_id predicate_index,
                  const block_id input_block_id,
                  const std::size_t delete_operator_index,
                  const tmb::client_id foreman_client_id,
                  MessageBus *bus)
      : rel_id_(rel_id),
        predicate_index_(predicate_index),
        input_block_id_(input_block_id),
        delete_operator_index_(delete_operator_index),
        foreman_client_id_(foreman_client_id),
        bus_(bus) {}

  ~DeleteWorkOrder() override {}

  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) override;

 private:
  const relation_id rel_id_;
  const QueryContext::predicate_id predicate_index_;
  const block_id input_block_id_;

  const std::size_t delete_operator_index_;
  const tmb::client_id foreman_client_id_;
  MessageBus *bus_;

  DISALLOW_COPY_AND_ASSIGN(DeleteWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_DELETE_OPERATOR_HPP_
