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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_UNION_ALL_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_UNION_ALL_OPERATOR_HPP_

#include <cstddef>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class InsertDestination;
class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which performs UNION ALL operation to relational tables.
 **/
class UnionAllOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_id The ID of the query to which this operator belongs.
   * @param input_relations The input relations to this UnionAll operator.
   * @param output_relation The output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert the results.
   * @param input_relations_are_stored A boolean vector indicating whether each
   *        input relation is a stored relation and is fully availabie to the
   *        operator before it can start generating workorders.
   * @param select_attribute_ids The attributes for all input relations.
   **/
  UnionAllOperator(const std::size_t query_id,
                   const std::vector<const CatalogRelation*> &input_relations,
                   const CatalogRelation &output_relation,
                   const QueryContext::insert_destination_id output_destination_index,
                   const std::vector<bool> &input_relations_are_stored,
                   const std::vector<std::vector<attribute_id>> &select_attribute_ids)
      : RelationalOperator(query_id),
        input_relations_(input_relations),
        input_relations_are_stored_(input_relations_are_stored),
        output_relation_(output_relation),
        output_destination_index_(output_destination_index),
        select_attribute_ids_(select_attribute_ids),
        stored_generated_(false) {
    // For every input relation, initialize the UnionAll data structure.
    for (std::size_t i = 0; i < input_relations.size(); i++) {
      relation_id_to_index_.emplace(input_relations[i]->getID(), i);
      if (input_relations_are_stored[i]) {
        input_relations_block_ids_.emplace_back(
            input_relations[i]->getBlocksSnapshot());
      } else {
        input_relations_block_ids_.emplace_back();
        still_feeding_.insert(i);
      }

      num_workorders_generated_.push_back(0);
    }
  }

  ~UnionAllOperator() override {}

  OperatorType getOperatorType() const override {
    return kUnionAll;
  }

  std::string getName() const override {
    return "UnionAll";
  }

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return output_destination_index_;
  }

  /**
   * @return The input relations to this UnionAll operator.
   */
  const std::vector<const CatalogRelation*>& input_relations() const {
    return input_relations_;
  }

  /**
   * @return The output relation to this UnionAll operator.
   */
  const CatalogRelation& output_relation() const {
    return output_relation_;
  }

  const relation_id getOutputRelationID() const override {
    return output_relation_.getID();
  }

  void feedInputBlock(const block_id input_block_id,
                      const relation_id input_relation_id,
                      const partition_id part_id) override;

  void doneFeedingInputBlocks(const relation_id rel_id) override;

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

 private:
  // Add work orders for a single relation.
  void addWorkOrdersSingleRelation(WorkOrdersContainer *container,
                                   QueryContext *query_context,
                                   StorageManager *storage_manager,
                                   InsertDestination *output_destination,
                                   const std::size_t relation_index);

  const std::vector<const CatalogRelation*> input_relations_;
  const std::vector<bool> input_relations_are_stored_;

  const CatalogRelation &output_relation_;
  const QueryContext::insert_destination_id output_destination_index_;

  const std::vector<std::vector<attribute_id>> select_attribute_ids_;

  // For input_relation_[i],
  // its block ids are in input_relations_block_ids_[i],
  // its number of generated work orders is in num_workorders_generated_[i].
  std::vector<std::vector<block_id>> input_relations_block_ids_;
  std::vector<std::size_t> num_workorders_generated_;

  // Relation indices that are not stored, and are still feeding.
  std::unordered_set<std::size_t> still_feeding_;

  // Map from relation_id to index in vector.
  std::unordered_map<relation_id, std::size_t> relation_id_to_index_;

  // If all the stored relations are generated.
  bool stored_generated_;

  DISALLOW_COPY_AND_ASSIGN(UnionAllOperator);
};


/**
 * @brief A work order produced by UnionAllOperator.
 */
class UnionAllWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_id The ID of the query to which this WorkOrder belongs.
   * @param input_relation The relation to perform UnionAll over.
   * @param input_block_id The block id.
   * @param select_attribute_id The ids of the attributes to select from
   *        the input relation.
   * @param output_destination The InsertDestination to insert the UnionAll
   *        result.
   * @param storage_manager The StorageManager to use.
   */
  UnionAllWorkOrder(const std::size_t query_id,
                    const CatalogRelationSchema *input_relation,
                    const block_id input_block_id,
                    const std::vector<attribute_id> &select_attribute_id,
                    InsertDestination *output_destination,
                    StorageManager *storage_manager)
      : WorkOrder(query_id),
        input_relation_(input_relation),
        input_block_id_(input_block_id),
        select_attribute_id_(select_attribute_id),
        output_destination_(output_destination),
        storage_manager_(storage_manager) {}

  ~UnionAllWorkOrder() override {}

  void execute() override;

 private:
  const CatalogRelationSchema *input_relation_;
  const block_id input_block_id_;
  const std::vector<attribute_id> select_attribute_id_;

  InsertDestination *output_destination_;
  StorageManager* storage_manager_;

  DISALLOW_COPY_AND_ASSIGN(UnionAllWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_UNION_ALL_OPERATOR_HPP_
