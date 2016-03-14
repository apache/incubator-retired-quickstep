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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_NESTED_LOOPS_JOIN_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_NESTED_LOOPS_JOIN_OPERATOR_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
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
class TupleStorageSubBlock;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

namespace serialization { class WorkOrder; }

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which performs a simple (SLOW) nested-loops join on two
 *        relations.
 **/
class NestedLoopsJoinOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param left_input_relation The first relation in the join (order is not
   *        actually important).
   * @param right_input_relation The second relation in the join (order is not
   *        actually important).
   * @param output_relation The output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert the join results.
   * @param join_predicate_index The index of join predicate in QueryContext to
   *        evaluate for each pair of tuples in the input relations.
   *        (cannot be kInvalidPredicateId).
   * @param selection_index The group index of Scalars in QueryContext,
   *        corresponding to the attributes of the relation referred by
   *        output_relation_id. Each Scalar is evaluated for the joined tuples,
   *        and the resulting value is inserted into the join result.
   * @param left_relation_is_stored If left_input_relation is a stored relation.
   * @param right_relation_is_stored If right_input_relation is a stored
   *                                 relation.
   **/
  NestedLoopsJoinOperator(const CatalogRelation &left_input_relation,
                          const CatalogRelation &right_input_relation,
                          const CatalogRelation &output_relation,
                          const QueryContext::insert_destination_id output_destination_index,
                          const QueryContext::predicate_id join_predicate_index,
                          const QueryContext::scalar_group_id selection_index,
                          bool left_relation_is_stored,
                          bool right_relation_is_stored)
      : left_input_relation_(left_input_relation),
        right_input_relation_(right_input_relation),
        output_relation_(output_relation),
        output_destination_index_(output_destination_index),
        join_predicate_index_(join_predicate_index),
        selection_index_(selection_index),
        left_relation_is_stored_(left_relation_is_stored),
        right_relation_is_stored_(right_relation_is_stored),
        left_relation_block_ids_(left_relation_is_stored ? left_input_relation.getBlocksSnapshot()
                                                         : std::vector<block_id>()),
        right_relation_block_ids_(right_relation_is_stored ? right_input_relation.getBlocksSnapshot()
                                                           : std::vector<block_id>()),
        num_left_workorders_generated_(0),
        num_right_workorders_generated_(0),
        done_feeding_left_relation_(false),
        done_feeding_right_relation_(false),
        all_workorders_generated_(false) {
    DCHECK_NE(join_predicate_index_, QueryContext::kInvalidPredicateId);
  }

  ~NestedLoopsJoinOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id foreman_client_id,
                        const tmb::client_id agent_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

  void doneFeedingInputBlocks(const relation_id rel_id) override {
    if (rel_id == left_input_relation_.getID()) {
      done_feeding_left_relation_ = true;
    } else if (rel_id == right_input_relation_.getID()) {
      done_feeding_right_relation_ = true;
    } else {
      FATAL_ERROR("Wrong relation ID in doneFeedingInputBlocks method.");
    }
  }

  void feedInputBlock(const block_id input_block_id, const relation_id input_relation_id) override;

  void feedInputBlocks(const relation_id rel_id, std::vector<block_id> *partially_filled_blocks) override;

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return output_destination_index_;
  }

  const relation_id getOutputRelationID() const override {
    return output_relation_.getID();
  }

 private:
  /**
   * @brief Pairs block IDs from left and right relation block IDs and generates
   *        NestedLoopsJoinWorkOrders and pushes them to the WorkOrdersContainer
   *        when both relations are not stored relations.
   *
   * @param container A pointer to the WorkOrdersContainer to store the
   *                  resulting WorkOrders.
   * @param query_context The QueryContext that stores query execution states.
   * @param storage_manager The StorageManager to use.
   * @param left_min The starting index in left_relation_block_ids_ from where
   *                 we begin generating NestedLoopsJoinWorkOrders.
   * @param left_max The index in left_relation_block_ids_ until which we
   *                 generate NestedLoopsJoinWorkOrders (excluding left_max).
   * @param right_min The starting index in right_relation_block_ids_ from where
   *                  we begin generating NestedLoopsJoinWorkOrders.
   * @param right_max The index in right_relation_block_ids_ until which we
   *                  generate NestedLoopsJoinWorkOrders. (excluding right_max).
   *
   * @return The number of workorders generated during the execution of this
   *         function.
   **/
  std::size_t getAllWorkOrdersHelperBothNotStored(WorkOrdersContainer *container,
                                                  QueryContext *query_context,
                                                  StorageManager *storage_manager,
                                                  std::vector<block_id>::size_type left_min,
                                                  std::vector<block_id>::size_type left_max,
                                                  std::vector<block_id>::size_type right_min,
                                                  std::vector<block_id>::size_type right_max);

  /**
   * @brief Pairs block IDs from left and right relation block IDs and generates
   *        NestedLoopsJoinWorkOrders and pushes them to the WorkOrdersContainer
   *        when only one relation is a stored relation.
   *
   * @param container A pointer to the WorkOrdersContainer to store the
   *                  resulting WorkOrders.
   * @param query_context The QueryContext that stores query execution states.
   * @param storage_manager The StorageManager to use.
   *
   * @return Whether all work orders have been generated.
   **/
  bool getAllWorkOrdersHelperOneStored(WorkOrdersContainer *container,
                                       QueryContext *query_context,
                                       StorageManager *storage_manager);

  /**
   * @brief Pairs block IDs from left and right relation block IDs and generates
   *        NestedLoopsJoinWorkOrder protos and pushes them to the
   *        WorkOrderProtosContainer when both relations are not stored
   *        relations.
   *
   * @param container A pointer to the WorkOrderProtosContainer to store the
   *                  resulting WorkOrder protos.
   * @param left_min The starting index in left_relation_block_ids_ from where
   *                 we begin generating NestedLoopsJoinWorkOrders.
   * @param left_max The index in left_relation_block_ids_ until which we
   *                 generate NestedLoopsJoinWorkOrders (excluding left_max).
   * @param right_min The starting index in right_relation_block_ids_ from where
   *                  we begin generating NestedLoopsJoinWorkOrders.
   * @param right_max The index in right_relation_block_ids_ until which we
   *                  generate NestedLoopsJoinWorkOrders. (excluding right_max).
   *
   * @return The number of workorder protos generated during the execution of this
   *         function.
   **/
  std::size_t getAllWorkOrderProtosHelperBothNotStored(WorkOrderProtosContainer *container,
                                                       const std::vector<block_id>::size_type left_min,
                                                       const std::vector<block_id>::size_type left_max,
                                                       const std::vector<block_id>::size_type right_min,
                                                       const std::vector<block_id>::size_type right_max);

  /**
   * @brief Pairs block IDs from left and right relation block IDs and generates
   *        NestedLoopsJoinWorkOrder protos and pushes them to the
   *        WorkOrderProtosContainer when only one relation is a stored relation.
   *
   * @param container A pointer to the WorkOrderProtosContainer to store the
   *                  resulting WorkOrder protos.
   *
   * @return Whether all work orders have been generated.
   **/
  bool getAllWorkOrderProtosHelperOneStored(WorkOrderProtosContainer *container);

  /**
   * @brief Create Work Order proto.
   *
   * @param block The block id used in the Work Order.
   **/
  serialization::WorkOrder* createWorkOrderProto(const block_id left_block,
                                                 const block_id right_block);

  const CatalogRelation &left_input_relation_;
  const CatalogRelation &right_input_relation_;

  const CatalogRelation &output_relation_;
  const QueryContext::insert_destination_id output_destination_index_;

  const QueryContext::predicate_id join_predicate_index_;
  const QueryContext::scalar_group_id selection_index_;

  const bool left_relation_is_stored_;
  const bool right_relation_is_stored_;

  std::vector<block_id> left_relation_block_ids_;
  std::vector<block_id> right_relation_block_ids_;

  // At a given point of time, we have paired num_left_workorders_generated
  // number of blocks from the left relation with num_right_workorders_generated
  // number of blocks from the right relation.
  std::vector<block_id>::size_type num_left_workorders_generated_;
  std::vector<block_id>::size_type num_right_workorders_generated_;

  bool done_feeding_left_relation_;
  bool done_feeding_right_relation_;

  // Applicable only when both the relations are stored relations.
  bool all_workorders_generated_;

  DISALLOW_COPY_AND_ASSIGN(NestedLoopsJoinOperator);
};

/**
 * @brief A WorkOrder produced by NestedLoopsJoinOperator
 **/
class NestedLoopsJoinWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param left_input_relation The first relation in the join (order is not
   *        actually important).
   * @param right_input_relation The second relation in the join (order is not
   *        actually important).
   * @param left_block_id The block id of the first relation.
   * @param right_block_id The block id of the second relation.
   * @param join_predicate The join predicate to evaluate for each pair of
   *        tuples in the input relations. (cannot be NULL).
   * @param selection A list of Scalars corresponding to the relation attributes
   *        in \c output_destination. Each Scalar is evaluated for the joined
   *        tuples, and the resulting value is inserted into the join result.
   * @param output_destination The InsertDestination to insert the join results.
   * @param storage_manager The StorageManager to use.
   **/
  NestedLoopsJoinWorkOrder(const CatalogRelationSchema &left_input_relation,
                           const CatalogRelationSchema &right_input_relation,
                           const block_id left_block_id,
                           const block_id right_block_id,
                           const Predicate *join_predicate,
                           const std::vector<std::unique_ptr<const Scalar>> &selection,
                           InsertDestination *output_destination,
                           StorageManager *storage_manager)
      : left_input_relation_(left_input_relation),
        right_input_relation_(right_input_relation),
        left_block_id_(left_block_id),
        right_block_id_(right_block_id),
        join_predicate_(DCHECK_NOTNULL(join_predicate)),
        selection_(selection),
        output_destination_(DCHECK_NOTNULL(output_destination)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)) {}

  ~NestedLoopsJoinWorkOrder() override {}

  /**
   * @exception TupleTooLargeForBlock A tuple produced by this join was too
   *            large to insert into an empty block provided by
   *            output_destination_. Join may be partially complete (with
   *            some tuples inserted into the destination) when this exception
   *            is thrown, causing potential inconsistency.
   **/
  void execute() override;

 private:
  template <bool LEFT_PACKED, bool RIGHT_PACKED>
  void executeHelper(const TupleStorageSubBlock &left_store,
                     const TupleStorageSubBlock &right_store);

  const CatalogRelationSchema &left_input_relation_, &right_input_relation_;
  const block_id left_block_id_, right_block_id_;
  const Predicate *join_predicate_;
  const std::vector<std::unique_ptr<const Scalar>> &selection_;

  InsertDestination *output_destination_;
  StorageManager *storage_manager_;

  DISALLOW_COPY_AND_ASSIGN(NestedLoopsJoinWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_NESTED_LOOPS_JOIN_OPERATOR_HPP_
