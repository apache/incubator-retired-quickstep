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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_DESTROY_HASH_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_DESTROY_HASH_OPERATOR_HPP_

#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"

#ifdef QUICKSTEP_HAVE_LIBNUMA
#include "catalog/NUMAPlacementScheme.hpp"
#endif

#include "catalog/PartitionSchemeHeader.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb {
class MessageBus;
}

namespace quickstep {

class CatalogRelationSchema;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which destroys a shared hash table.
 **/
class DestroyHashOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param hash_table_group_index The group index of the JoinHashTable in QueryContext.
   **/
  explicit DestroyHashOperator(const CatalogRelation &input_relation,
                               const QueryContext::join_hash_table_group_id hash_table_group_index)
      : input_relation_(input_relation), hash_table_group_index_(hash_table_group_index), work_generated_(false) {}

  ~DestroyHashOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

 private:
  const CatalogRelation &input_relation_;
  const QueryContext::join_hash_table_group_id hash_table_group_index_;
  bool work_generated_;

  DISALLOW_COPY_AND_ASSIGN(DestroyHashOperator);
};

/**
 * @brief A WorkOrder produced by DestroyHashOperator.
 **/
class DestroyHashWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param hash_table_group_index The group index of the JoinHashTable in QueryContext.
   * @param query_context The QueryContext to use.
   **/
  DestroyHashWorkOrder(const CatalogRelation &input_relation,
                       const QueryContext::join_hash_table_group_id hash_table_group_index,
                       QueryContext *query_context,
                       const numa_node_id numa_node = 0)
      : input_relation_(input_relation),
        hash_table_group_index_(hash_table_group_index),
        query_context_(DCHECK_NOTNULL(query_context)) {
    preferred_numa_nodes_.push_back(numa_node);
  }

  ~DestroyHashWorkOrder() override {}

  void execute() override;

 private:
  const CatalogRelation &input_relation_;
  const QueryContext::join_hash_table_group_id hash_table_group_index_;
  QueryContext *query_context_;

  DISALLOW_COPY_AND_ASSIGN(DestroyHashWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_DESTROY_HASH_OPERATOR_HPP_
