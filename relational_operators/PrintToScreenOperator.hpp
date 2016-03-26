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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_PRINT_TO_SCREEN_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_PRINT_TO_SCREEN_OPERATOR_HPP_

#include <atomic>
#include <cstddef>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class QueryContext;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief Print to screen operator for stored/pipelined printing.
 */
class PrintToScreenOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param input_relation The input relation to print.
   * @param input_is_stored The input relation is stored or pipelined.
   **/
  PrintToScreenOperator(const CatalogRelation &input_relation,
                        bool input_is_stored)
    : input_relation_(input_relation),
      input_is_stored_(input_is_stored),
      input_relation_blocks_(input_relation_.getBlocksSnapshot()) {}

  ~PrintToScreenOperator() override {}

  //bool getAllWorkOrders(WorkOrdersContainer *container) override;

  bool getAllWorkOrders(WorkOrdersContainer *container, 
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id foreman_client_id,
                        tmb::MessageBus *bus) override;
  
  void feedInputBlock(const block_id input_block_id,
                      const relation_id input_rel_id) override {
    input_relation_blocks_.push_back(input_block_id);
  }

  void feedInputBlocks(
      const relation_id input_rel_id,
      std::vector<block_id> *partially_filled_blocks) override {
    input_relation_blocks_.insert(input_relation_blocks_.end(),
                                  partially_filled_blocks->begin(),
                                  partially_filled_blocks->end());
  }

 private:
  const CatalogRelation &input_relation_;
  const bool input_is_stored_;
  std::vector<block_id> input_relation_blocks_;
  std::vector<block_id>::size_type num_workorders_generated_ = 0;
  std::atomic<std::size_t> num_workorders_completed_{0};
  bool done_printing_header_ = false;
  bool done_generating_ = false;

  DISALLOW_COPY_AND_ASSIGN(PrintToScreenOperator);
};

/**
 * @brief Work order to print entire relation or streamed blocks.
 **/
class PrintToScreenWorkOrder : public WorkOrder {
 public:
  enum Type {
    kPrintBlock,
    kPrintRelation,
  };

  /**
   * @brief Constructor to print entire relation.
   * @param input_relation Relation to print.
   **/
  explicit PrintToScreenWorkOrder(const CatalogRelation &input_relation)
      : type_(kPrintRelation), input_relation_(input_relation) {}

  /**
   * @brief Constructor to print the the streamed block.
   **/
  PrintToScreenWorkOrder(const CatalogRelation &input_relation,
                         block_id print_block,
			 StorageManager *storage_manager,
                         std::atomic<std::size_t> *num_workorders_completed)
      : type_(kPrintBlock),
        input_relation_(input_relation),
        input_block_(print_block),
	storage_manager_(DCHECK_NOTNULL(storage_manager)),
        num_workorders_completed_(num_workorders_completed) {}

  void execute() override;

 private:
  const Type type_;
  const CatalogRelation &input_relation_;
  const block_id input_block_ = 0;
  StorageManager *storage_manager_;
  std::atomic<std::size_t> *num_workorders_completed_;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_MONITORED_TEXT_SCAN_OPERATOR_HPP_
