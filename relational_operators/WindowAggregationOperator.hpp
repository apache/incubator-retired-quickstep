/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_WINDOW_AGGREGATION_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_WINDOW_AGGREGATION_OPERATOR_HPP_

#include <cstdint>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class AggregateFunction;
class CatalogAttribute;
class CatalogDatabase;
class CatalogRelation;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief Struct to store the window aggregation state.
 */
struct WindowAggregationState {
  /**
   * @brief Constructor.
   * @param win_attr Windowing attribute.
   * @param grp_by The grouping expressions.
   * @param win_dur Window duraion/size.
   * @param age_dur Aging duration.
   */
  WindowAggregationState(const CatalogAttribute &win_attr,
                         std::vector<std::unique_ptr<const Scalar>> &&grp_by,
                         const TypedValue &win_dur,
                         std::int32_t age_dur)
      : window_attribute(win_attr),
        window_duration(win_dur),
        age_duration(age_dur),
        bucket_key(std::move(grp_by)) {}

  // Windowing specification.
  const CatalogAttribute &window_attribute;
  const TypedValue window_duration;
  std::int32_t age_duration;

  // Hash bucket key expression
  std::vector<std::unique_ptr<const Scalar>> bucket_key;
  // Reverse bucket operation.
  std::unique_ptr<UncheckedBinaryOperator> rev_bucket_key;

  // Window aggregation handle state.
  std::vector<std::unique_ptr<AggregationHandle>> handles;
  std::vector<std::vector<std::unique_ptr<const Scalar>>> arguments;

  // Window aggregation state.
  std::vector<std::unique_ptr<AggregationStateHashTableBase>> hashtables;
};

/**
 * @brief An operator which performs window aggregation over a relation.
 **/
class WindowAggregationOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor for aggregating with arbitrary expressions in projection
   *        list.
   *
   * @param input_relation The relation to perform aggregation over.
   * @param input_relation_is_stored If input_relation is a stored relation and
   *        is fully available to the operator before it can start generating
   *        workorders.
   * @param aggregate_functions The aggregation functions.
   * @param aggregate_argument The arguments to aggregations.
   * @param group_by The grouping expressions.
   * @param window_attribute Windowing attribute.
   * @param window_duration Window duration/size.
   * @param age_duration Aging duration.
   * @param output_dest_id The output insert destination id.
   * @param output_rel_id The output relation id.
   * @param hash_table_impl_type Hashtable implementation to use.
   * @param storage_manager The storage manager to use.
   **/
  WindowAggregationOperator(
      const CatalogRelation &input_relation,
      bool input_relation_is_stored,
      const std::vector<const AggregateFunction *> &aggregate_functions,
      std::vector<std::vector<std::unique_ptr<const Scalar>>>
          &&aggregate_arguments,
      std::vector<std::unique_ptr<const Scalar>> &&group_by,
      const CatalogAttribute &window_attribute,
      const TypedValue &window_duration,
      std::int32_t age_duration,
      const QueryContext::insert_destination_id output_dest_id,
      const relation_id output_rel_id,
      const serialization::HashTableImplType hash_table_impl_type,
      StorageManager *storage_manager);

  ~WindowAggregationOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container, 
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id foreman_client_id,
                        tmb::MessageBus *bus) override;

  void feedInputBlock(const block_id input_block_id, const relation_id input_relation_id) override {
    input_relation_block_ids_.push_back(input_block_id);
  }

  void feedInputBlocks(const relation_id rel_id, std::vector<block_id> *partially_filled_blocks) override {
    input_relation_block_ids_.insert(input_relation_block_ids_.end(),
                                     partially_filled_blocks->begin(),
                                     partially_filled_blocks->end());
  }

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return output_dest_id_;
  }

  const relation_id getOutputRelationID() const override {
    return output_rel_id_;
  }

 private:
  const CatalogRelation &input_relation_;
  const bool input_relation_is_stored_;
  std::vector<block_id> input_relation_block_ids_;
  std::vector<block_id>::size_type num_workorders_generated_ = 0;
  std::vector<block_id>::size_type input_write_threshold_;
  const QueryContext::insert_destination_id output_dest_id_;
  const relation_id output_rel_id_;
  bool started_ = false;
  WindowAggregationState state_;

  DISALLOW_COPY_AND_ASSIGN(WindowAggregationOperator);
};

/**
 * @brief A WorkOrder produced by WindowAggregationOperator.
 **/
class WindowAggregationWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Write mode.
   */
  enum WriteMode {
    kAllBuckets,
    kChangedBuckets,
    kNoBuckets,
  };

  /**
   * @brief Constructor
   *
   * @param input_block_id The input block id.
   * @param input_rel_id The input relation.
   * @param output_dest_id The insert destination to the output.
   * @param state The window aggregation state.
   * @param write_mode The write mode of the work order.
   **/
  WindowAggregationWorkOrder(const block_id input_block_id,
				  const relation_id input_rel_id,
                             const QueryContext::insert_destination_id output_dest_id,
                             WindowAggregationState *state,
			     StorageManager *storage_manager,
			     QueryContext *query_context,
		             WriteMode write_mode = kChangedBuckets			
				)
      : input_block_id_(input_block_id),
	 input_rel_id_(input_rel_id),
        output_dest_id_(output_dest_id),
        state_(state),
	storage_manager(DCHECK_NOTNULL(storage_manager)),
        query_context(DCHECK_NOTNULL(query_context)),
	write_mode_(write_mode)
        {}

  ~WindowAggregationWorkOrder() override {}

  void execute() override;

 private:
  const block_id input_block_id_;
const relation_id input_rel_id_;
  const QueryContext::insert_destination_id output_dest_id_;
  WindowAggregationState *state_;
  StorageManager *storage_manager;
   QueryContext *query_context; 
 const WriteMode write_mode_;		 
  DISALLOW_COPY_AND_ASSIGN(WindowAggregationWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_WINDOW_AGGREGATION_OPERATOR_HPP_
