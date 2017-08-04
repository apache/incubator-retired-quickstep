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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_TABLE_EXPORT_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_TABLE_EXPORT_OPERATOR_HPP_

#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "threading/SpinMutex.hpp"
#include "utility/BulkIoConfiguration.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogRelationSchema;
class StorageManager;
class ValueAccessor;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

namespace serialization { class WorkOrder; }

/** \addtogroup RelationalOperators
 *  @{
 */

class TableExportOperator : public RelationalOperator {
 public:
  /**
   * @brief Feedback message to Foreman when a TableExportToStringWorkOrder has
   *        completed writing a block to the string buffer.
   */
  enum FeedbackMessageType : WorkOrder::FeedbackMessageType {
      kBlockOutputMessage,
  };

  /**
   * @brief Constructor.
   *
   * @param query_id The ID of the query to which this operator belongs.
   * @param input_relation The relation to export.
   * @param input_relation_is_stored If input_relation is a stored relation and
   *        is fully available to the operator before it can start generating
   *        workorders.
   * @param file_name The name of the file to export the relation to.
   * @param options The options that specify the detailed format of the output
   *        file.
   */
  TableExportOperator(const std::size_t query_id,
                      const CatalogRelation &input_relation,
                      const bool input_relation_is_stored,
                      const std::string &file_name,
                      const BulkIoConfigurationPtr &options)
      : RelationalOperator(query_id),
        input_relation_(input_relation),
        input_relation_is_stored_(input_relation_is_stored),
        file_name_(file_name),
        options_(options),
        input_relation_block_ids_(input_relation_is_stored
                                      ? input_relation.getBlocksSnapshot()
                                      : std::vector<block_id>()),
        num_workorders_generated_(0),
        started_(false),
        num_blocks_written_(0),
        file_(nullptr) {}

  ~TableExportOperator() override;

  OperatorType getOperatorType() const override {
    return kTableExport;
  }

  std::string getName() const override {
    return "TableExportOperator";
  }

  /**
   * @return The relation to export.
   */
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
      SpinMutexLock lock(block_ids_mutex_);
      input_relation_block_ids_.emplace_back(input_block_id);
    }
  }

  void receiveFeedbackMessage(const WorkOrder::FeedbackMessage &msg) override;

 private:
  // Buffer for storing a block's exported string.
  struct BlockBuffer {
    explicit BlockBuffer(std::string *buffer_in)
        : done(false),
          buffer(buffer_in) {}
    bool done;
    std::unique_ptr<std::string> buffer;
  };

  const CatalogRelation &input_relation_;
  const bool input_relation_is_stored_;
  const std::string file_name_;
  const BulkIoConfigurationPtr options_;

  std::vector<block_id> input_relation_block_ids_;
  std::size_t num_workorders_generated_;
  alignas(kCacheLineBytes) SpinMutex block_ids_mutex_;

  bool started_;

  std::size_t num_blocks_written_;
  std::unordered_map<block_id, BlockBuffer> output_buffers_;
  alignas(kCacheLineBytes) SpinMutex output_buffers_mutex_;

  FILE *file_;

  DISALLOW_COPY_AND_ASSIGN(TableExportOperator);
};

class TableExportToStringWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_id The ID of the query to which this work order belongs.
   * @param input_relation The relation to export.
   * @param input_block_id The block id.
   * @param format The output file format.
   * @param print_header Whether to write table headers.
   * @param field_terminator The character that separates attribute values in
   *        each output row.
   * @param escape_strings Whether to encode special characters as escape
   *        sequences. NOTE: This options is for TEXT format only and is ignored
   *        otherwiae.
   * @param quote_character The quote character. NOTE: This options is for CSV
   *        format only and is ignored otherwiae.
   * @param null_string The string that represents a null value.
   * @param output_buffer The string buffer for writing the output to.
   * @param operator_index TableExportOperator index to send feedback messages
   *        to.
   * @param scheduler_client_id The TMB client ID of the scheduler thread.
   * @param storage_manager The StorageManager to use.
   * @param bus TMB to send the feedback message on.
   */
  TableExportToStringWorkOrder(const std::size_t query_id,
                               const CatalogRelationSchema &input_relation,
                               const block_id input_block_id,
                               const BulkIoFormat format,
                               const bool print_header,
                               const char field_terminator,
                               const bool escape_strings,
                               const char quote_character,
                               const std::string null_string,
                               std::string *output_buffer,
                               const std::size_t operator_index,
                               const tmb::client_id scheduler_client_id,
                               StorageManager *storage_manager,
                               MessageBus *bus)
      : WorkOrder(query_id),
        input_relation_(input_relation),
        input_block_id_(input_block_id),
        format_(format),
        print_header_(print_header),
        field_terminator_(field_terminator),
        escape_strings_(escape_strings),
        quote_character_(quote_character),
        null_string_(null_string),
        operator_index_(operator_index),
        scheduler_client_id_(scheduler_client_id),
        storage_manager_(storage_manager),
        bus_(bus),
        output_buffer_(output_buffer) {
  }

  ~TableExportToStringWorkOrder() override {}

  void execute() override;

 private:
  inline std::string quoteCSVField(std::string &&field) const;  // NOLINT(whitespace/operators)
  inline std::string escapeTextField(std::string &&field) const;  // NOLINT(whitespace/operators)

  // Helper method for writing each entry from a table row to the output string
  // buffer, with proper transformations (separated by delimiter character,
  // escape special characters, add quotes, etc.).
  template <std::string (TableExportToStringWorkOrder::*transform)(std::string&&) const,  // NOLINT
            typename Container, typename Functor>
  inline void writeEachToString(const Container &container,
                                std::string *output,
                                const Functor &functor) const;

  // Write all the rows from a value accessor to the output string buffer.
  template <std::string (TableExportToStringWorkOrder::*transform)(std::string&&) const>  // NOLINT
  void writeToString(ValueAccessor *accessor, std::string *output) const;

  const CatalogRelationSchema &input_relation_;
  const block_id input_block_id_;

  const BulkIoFormat format_;
  const bool print_header_;
  const char field_terminator_;
  const bool escape_strings_;
  const char quote_character_;
  const std::string null_string_;

  const std::size_t operator_index_;
  const tmb::client_id scheduler_client_id_;
  StorageManager *storage_manager_;
  MessageBus *bus_;

  std::string *output_buffer_;

  DISALLOW_COPY_AND_ASSIGN(TableExportToStringWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_TABLE_EXPORT_OPERATOR_HPP_
