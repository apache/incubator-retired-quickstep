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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_SELECT_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_SELECT_OPERATOR_HPP_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"

#ifdef QUICKSTEP_HAVE_LIBNUMA
#include "catalog/NUMAPlacementScheme.hpp"
#endif

#include "catalog/PartitionScheme.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"
#include "utility/lip_filter/LIPFilterAdaptiveProber.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogRelationSchema;
class InsertDestination;
class LIPFilterDeployment;
class Predicate;
class Scalar;
class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

namespace serialization { class WorkOrder; }

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which performs simple selection over a relation.
 **/
class SelectOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor for selection with arbitrary expressions in projection
   *        list.
   *
   * @param query_id The ID of the query to which this operator belongs.
   * @param input_relation The relation to perform selection over.
   * @param has_repartition Whether this operator does repartition.
   * @param output_relation The output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert the selection results.
   * @param predicate_index The index of selection predicate in QueryContext.
   *        All tuples matching pred will be selected (or kInvalidPredicateId to
   *        select all tuples).
   * @param selection_index The group index of Scalars in QueryContext, which
   *        will be evaluated to project input tuples.
   * @param input_relation_is_stored If input_relation is a stored relation and
   *        is fully available to the operator before it can start generating
   *        workorders.
   **/
  SelectOperator(
      const std::size_t query_id,
      const CatalogRelation &input_relation,
      const bool has_repartition,
      const CatalogRelation &output_relation,
      const QueryContext::insert_destination_id output_destination_index,
      const QueryContext::predicate_id predicate_index,
      const QueryContext::scalar_group_id selection_index,
      const bool input_relation_is_stored)
      : RelationalOperator(query_id, input_relation.getNumPartitions(), has_repartition,
                           output_relation.getNumPartitions()),
        input_relation_(input_relation),
        output_relation_(output_relation),
        output_destination_index_(output_destination_index),
        predicate_index_(predicate_index),
        selection_index_(selection_index),
        input_relation_block_ids_(num_partitions_),
        num_workorders_generated_(num_partitions_),
        simple_projection_(false),
        input_relation_is_stored_(input_relation_is_stored),
        started_(false) {
    DCHECK(has_repartition || num_partitions_ == output_num_partitions_);
#ifdef QUICKSTEP_HAVE_LIBNUMA
    placement_scheme_ = input_relation.getNUMAPlacementSchemePtr();
#endif
    if (input_relation_is_stored) {
      if (input_relation.hasPartitionScheme()) {
        const PartitionScheme &part_scheme = *input_relation.getPartitionScheme();

        for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
          input_relation_block_ids_[part_id] = part_scheme.getBlocksInPartition(part_id);
        }
      } else {
        DCHECK_EQ(1u, num_partitions_);
        input_relation_block_ids_[0] = input_relation.getBlocksSnapshot();
      }
    }
  }

  /**
   * @brief Constructor for selection with simple projection of attributes.
   *
   * @note selection_index_ is invalid, and will not be used for projection.
   *
   * @param query_id The ID of the query to which this operator belongs.
   * @param input_relation The relation to perform selection over.
   * @param has_repartition Whether this operator does repartition.
   * @param output_relation The output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert the selection results.
   * @param selection A projection list of attribute IDs. The operator takes
   *        ownership of selection.
   * @param predicate_index The index of selection predicate in QueryContext.
   *        All tuples matching pred will be selected (or kInvalidPredicateId to
   *        select all tuples).
   * @param input_relation_is_stored If input_relation is a stored relation and
   *        is fully available to the operator before it can start generating
   *        workorders.
   **/
  SelectOperator(
      const std::size_t query_id,
      const CatalogRelation &input_relation,
      const bool has_repartition,
      const CatalogRelation &output_relation,
      const QueryContext::insert_destination_id output_destination_index,
      const QueryContext::predicate_id predicate_index,
      std::vector<attribute_id> &&selection,
      const bool input_relation_is_stored)
      : RelationalOperator(query_id, input_relation.getNumPartitions(), has_repartition,
                           output_relation.getNumPartitions()),
        input_relation_(input_relation),
        output_relation_(output_relation),
        output_destination_index_(output_destination_index),
        predicate_index_(predicate_index),
        selection_index_(QueryContext::kInvalidScalarGroupId),
        simple_selection_(std::move(selection)),
        input_relation_block_ids_(num_partitions_),
        num_workorders_generated_(num_partitions_),
        simple_projection_(true),
        input_relation_is_stored_(input_relation_is_stored),
        started_(false) {
    DCHECK(has_repartition || num_partitions_ == output_num_partitions_);
#ifdef QUICKSTEP_HAVE_LIBNUMA
    placement_scheme_ = input_relation.getNUMAPlacementSchemePtr();
#endif
    if (input_relation_is_stored) {
      if (input_relation.hasPartitionScheme()) {
        const PartitionScheme &part_scheme = *input_relation.getPartitionScheme();

        for (std::size_t part_id = 0; part_id < num_partitions_; ++part_id) {
          input_relation_block_ids_[part_id] = part_scheme.getBlocksInPartition(part_id);
        }
      } else {
        DCHECK_EQ(1u, num_partitions_);
        input_relation_block_ids_[0] = input_relation.getBlocksSnapshot();
      }
    }
  }

  ~SelectOperator() override {}

  OperatorType getOperatorType() const override {
    return kSelect;
  }

  std::string getName() const override {
    return "SelectOperator";
  }

  const CatalogRelation& input_relation() const {
    return input_relation_;
  }

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

  void feedInputBlock(const block_id input_block_id,
                      const relation_id input_relation_id,
                      const partition_id part_id) override {
    if (input_relation_id == input_relation_.getID()) {
      input_relation_block_ids_[part_id].push_back(input_block_id);
    }
  }

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return output_destination_index_;
  }

  const relation_id getOutputRelationID() const override {
    return output_relation_.getID();
  }

 private:
  /**
   * @brief Create Work Order proto.
   *
   * @param part_id The partition id.
   * @param block The block id used in the Work Order.
   **/
  serialization::WorkOrder* createWorkOrderProto(const partition_id part_id, const block_id block);

  const CatalogRelation &input_relation_;
  const CatalogRelation &output_relation_;
  const QueryContext::insert_destination_id output_destination_index_;
  const QueryContext::predicate_id predicate_index_;

  const QueryContext::scalar_group_id selection_index_;
  const std::vector<attribute_id> simple_selection_;

  // A vector of vectors V where V[i] indicates the list of block IDs of the
  // input relation that belong to the partition i.
  std::vector<std::vector<block_id>> input_relation_block_ids_;
  // A single workorder is generated for each block in each partition of input relation.
  std::vector<std::size_t> num_workorders_generated_;

  const bool simple_projection_;
  const bool input_relation_is_stored_;
#ifdef QUICKSTEP_HAVE_LIBNUMA
  const NUMAPlacementScheme *placement_scheme_;
#endif
  bool started_;

  DISALLOW_COPY_AND_ASSIGN(SelectOperator);
};

/**
 * @brief A WorkOrder produced by SelectOperator.
 **/
class SelectWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @note Reference parameter selection is NOT owned by this class and must
   *       remain valid until after execute() is called.
   *
   * @param query_id The ID of the query to which this WorkOrder belongs.
   * @param input_relation The relation to perform selection over.
   * @param part_id The partition id.
   * @param input_block_id The block id.
   * @param predicate All tuples matching \c predicate will be selected (or NULL
   *        to select all tuples).
   * @param simple_projection Whether the Select is simple.
   * @param simple_selection The list of attribute ids, used if \c
   *        simple_projection is true.
   * @param selection A list of Scalars which will be evaluated to project
   *        input tuples, used if \c simple_projection is false.
   * @param output_destination The InsertDestination to insert the selection
   *        results.
   * @param storage_manager The StorageManager to use.
   * @param lip_filter_adaptive_prober The attached LIP filter prober.
   **/
  SelectWorkOrder(const std::size_t query_id,
                  const CatalogRelationSchema &input_relation,
                  const partition_id part_id,
                  const block_id input_block_id,
                  const Predicate *predicate,
                  const bool simple_projection,
                  const std::vector<attribute_id> &simple_selection,
                  const std::vector<std::unique_ptr<const Scalar>> *selection,
                  InsertDestination *output_destination,
                  StorageManager *storage_manager,
                  LIPFilterAdaptiveProber *lip_filter_adaptive_prober,
                  const numa_node_id numa_node = 0)
      : WorkOrder(query_id, part_id),
        input_relation_(input_relation),
        input_block_id_(input_block_id),
        predicate_(predicate),
        simple_projection_(simple_projection),
        simple_selection_(simple_selection),
        selection_(selection),
        output_destination_(DCHECK_NOTNULL(output_destination)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)),
        lip_filter_adaptive_prober_(lip_filter_adaptive_prober) {
    preferred_numa_nodes_.push_back(numa_node);
  }

  /**
   * @brief Constructor for the distributed version.
   *
   * @note Reference parameter selection is NOT owned by this class and must
   *       remain valid until after execute() is called.
   *
   * @param query_id The ID of the query to which this WorkOrder belongs.
   * @param input_relation The relation to perform selection over.
   * @param part_id The partition id.
   * @param input_block_id The block id.
   * @param predicate All tuples matching \c predicate will be selected (or NULL
   *        to select all tuples).
   * @param simple_projection Whether the Select is simple.
   * @param simple_selection The list of attribute ids, used if \c
   *        simple_projection is true.
   * @param selection A list of Scalars which will be evaluated to project
   *        input tuples, used if \c simple_projection is false.
   * @param output_destination The InsertDestination to insert the selection
   *        results.
   * @param storage_manager The StorageManager to use.
   * @param lip_filter_adaptive_prober The attached LIP filter prober.
   **/
  SelectWorkOrder(const std::size_t query_id,
                  const CatalogRelationSchema &input_relation,
                  const partition_id part_id,
                  const block_id input_block_id,
                  const Predicate *predicate,
                  const bool simple_projection,
                  std::vector<attribute_id> &&simple_selection,
                  const std::vector<std::unique_ptr<const Scalar>> *selection,
                  InsertDestination *output_destination,
                  StorageManager *storage_manager,
                  LIPFilterAdaptiveProber *lip_filter_adaptive_prober,
                  const numa_node_id numa_node = 0)
      : WorkOrder(query_id, part_id),
        input_relation_(input_relation),
        input_block_id_(input_block_id),
        predicate_(predicate),
        simple_projection_(simple_projection),
        simple_selection_(std::move(simple_selection)),
        selection_(selection),
        output_destination_(DCHECK_NOTNULL(output_destination)),
        storage_manager_(DCHECK_NOTNULL(storage_manager)),
        lip_filter_adaptive_prober_(lip_filter_adaptive_prober) {
    preferred_numa_nodes_.push_back(numa_node);
  }

  ~SelectWorkOrder() override {}

  /**
   * @exception TupleTooLargeForBlock A tuple produced by this selection was
   *            too large to insert into an empty block provided by
   *            output_destination_index_ in query_context. Select may be
   *            partially complete (with some tuples inserted into the
   *            destination) when this exception is thrown, causing potential
   *            inconsistency.
   **/
  void execute() override;

 private:
  const CatalogRelationSchema &input_relation_;
  const block_id input_block_id_;
  const Predicate *predicate_;

  const bool simple_projection_;
  const std::vector<attribute_id> simple_selection_;
  const std::vector<std::unique_ptr<const Scalar>> *selection_;

  InsertDestination *output_destination_;
  StorageManager *storage_manager_;

  std::unique_ptr<LIPFilterAdaptiveProber> lip_filter_adaptive_prober_;

  DISALLOW_COPY_AND_ASSIGN(SelectWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_SELECT_OPERATOR_HPP_
