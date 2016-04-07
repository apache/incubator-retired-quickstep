/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

class CatalogRelationSchema;
class Predicate;
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
   **/
  DeleteOperator(const CatalogRelation &relation,
                 const QueryContext::predicate_id predicate_index,
                 const bool relation_is_stored)
     :  relation_(relation),
        predicate_index_(predicate_index),
        relation_is_stored_(relation_is_stored),
        started_(false),
        relation_block_ids_(relation_is_stored ? relation.getBlocksSnapshot()
                                               : std::vector<block_id>()),
        num_workorders_generated_(0) {}

  ~DeleteOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id foreman_client_id,
                        tmb::MessageBus *bus) override;

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
   * @param input_relation The relation to perform the DELETE over.
   * @param input_block_id The block Id.
   * @param predicate All tuples matching \c predicate will be deleted (If
   *        NULL, then all tuples will be deleted).
   * @param storage_manager The StorageManager to use.
   * @param delete_operator_index The index of the Delete Operator in the query
   *        plan DAG.
   * @param foreman_client_id The TMB client ID of the Foreman thread.
   * @param bus A pointer to the TMB.
   **/
  DeleteWorkOrder(const CatalogRelationSchema &input_relation,
                  const block_id input_block_id,
                  const Predicate *predicate,
                  StorageManager *storage_manager,
                  const std::size_t delete_operator_index,
                  const tmb::client_id foreman_client_id,
                  MessageBus *bus)
      : input_relation_(input_relation),
        input_block_id_(input_block_id),
        predicate_(predicate),
        storage_manager_(DCHECK_NOTNULL(storage_manager)),
        delete_operator_index_(delete_operator_index),
        foreman_client_id_(foreman_client_id),
        bus_(DCHECK_NOTNULL(bus)) {}

  ~DeleteWorkOrder() override {}

  void execute() override;

 private:
  const CatalogRelationSchema &input_relation_;
  const block_id input_block_id_;
  const Predicate *predicate_;

  StorageManager *storage_manager_;

  const std::size_t delete_operator_index_;
  const tmb::client_id foreman_client_id_;
  MessageBus *bus_;

  DISALLOW_COPY_AND_ASSIGN(DeleteWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_DELETE_OPERATOR_HPP_
