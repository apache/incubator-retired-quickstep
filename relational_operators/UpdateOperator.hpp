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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_UPDATE_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_UPDATE_OPERATOR_HPP_

#include <cstddef>
#include <memory>
#include <unordered_map>
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
class InsertDestination;
class Predicate;
class Scalar;
class StorageManager;
class WorkOrderProtosContainer;
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
   *
   * @warning The constructed InsertDestination should belong to relation, but
   *          must NOT contain any pre-existing blocks.
   **/
  UpdateOperator(const CatalogRelation &relation,
                 const QueryContext::insert_destination_id relocation_destination_index,
                 const QueryContext::predicate_id predicate_index,
                 const QueryContext::update_group_id update_group_index)
      : relation_(relation),
        relocation_destination_index_(relocation_destination_index),
        predicate_index_(predicate_index),
        update_group_index_(update_group_index),
        input_blocks_(relation.getBlocksSnapshot()),
        started_(false) {}

  ~UpdateOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

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
   * @param relation The relation to perform the UPDATE over.
   * @param predicate All tuples matching \c predicate will be updated (or NULL
   *        to update all tuples).
   * @param assignments The assignments (the map of attribute_ids to Scalars)
   *        which should be evaluated to get the new value for the corresponding
   *        attribute.
   * @param input_block_id The block id.
   * @param relocation_destination The InsertDestination to relocate tuples
   *        which can not be updated in-place.
   * @param storage_manager The StorageManager to use.
   * @param update_operator_index The index of the Update Operator in the query
   *        plan DAG.
   * @param scheduler_client_id The TMB client ID of the scheduler thread.
   * @param bus A pointer to the TMB.
   **/
  UpdateWorkOrder(const CatalogRelationSchema &relation,
                  const block_id input_block_id,
                  const Predicate *predicate,
                  const std::unordered_map<attribute_id, std::unique_ptr<const Scalar>> &assignments,
                  InsertDestination *relocation_destination,
                  StorageManager *storage_manager,
                  const std::size_t update_operator_index,
                  const tmb::client_id scheduler_client_id,
                  MessageBus *bus)
      : relation_(relation),
        input_block_id_(input_block_id),
        predicate_(predicate),
        assignments_(assignments),
        relocation_destination_(DCHECK_NOTNULL(relocation_destination)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)),
        update_operator_index_(update_operator_index),
        scheduler_client_id_(scheduler_client_id),
        bus_(DCHECK_NOTNULL(bus)) {}

  ~UpdateWorkOrder() override {}

  void execute() override;

 private:
  const CatalogRelationSchema &relation_;
  const block_id input_block_id_;
  const Predicate *predicate_;
  const std::unordered_map<attribute_id, std::unique_ptr<const Scalar>> &assignments_;

  InsertDestination *relocation_destination_;
  StorageManager *storage_manager_;

  const std::size_t update_operator_index_;
  const tmb::client_id scheduler_client_id_;
  MessageBus *bus_;

  DISALLOW_COPY_AND_ASSIGN(UpdateWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_UPDATE_OPERATOR_HPP_
