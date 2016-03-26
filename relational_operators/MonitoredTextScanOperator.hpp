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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_MONITORED_TEXT_SCAN_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_MONITORED_TEXT_SCAN_OPERATOR_HPP_

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which reads tuples from a text file and inserts them into
 *        a relation.
 **/
class MonitoredTextScanOperator : public RelationalOperator {
 public:
  enum FeedbackMessageType : WorkOrder::FeedbackMessageType {
    kTerminateMonitoring,
  };

  /**
   * @brief Constructor
   *
   * @param file_pattern The glob-like file pattern of the sources to load. The
   *        pattern could include * (wildcard for multiple chars) and ?
   *        (wildcard for single char). It defaults to single file load, if a
   *        file is specified.
   * @param Time period of monitoring for new files to add.
   * @param field_terminator The string which separates attribute values in
   *        the text file.
   * @param process_escape_sequences Whether to decode escape sequences in the
   *        text file.
   * @param output_relation_id The id of the output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert tuples.
   **/
  MonitoredTextScanOperator(
      const std::string &file_pattern,
      const std::int32_t monitor_time_period_secs,
      const char field_terminator,
      const bool process_escape_sequences,
      const relation_id output_relation_id,
      const QueryContext::insert_destination_id output_destination_index)
      : file_pattern_(file_pattern),
        monitor_time_period_secs_(monitor_time_period_secs),
        field_terminator_(field_terminator),
        process_escape_sequences_(process_escape_sequences),
        output_relation_id_(output_relation_id),
        output_destination_index_(output_destination_index) {}

  ~MonitoredTextScanOperator() override {}

 // bool getAllWorkOrders(WorkOrdersContainer *container) override;
 bool getAllWorkOrders(WorkOrdersContainer *container, 
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id foreman_client_id,
                        tmb::MessageBus *bus) override;

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return output_destination_index_;
  }

  const relation_id getOutputRelationID() const override {
    return output_relation_id_;
  }

  void receiveFeedbackMessage(const WorkOrder::FeedbackMessage &msg) override {
    if (msg.type() == kTerminateMonitoring) {
      done_monitoring_ = true;
    }
  }

  std::pair<std::chrono::milliseconds, bool> registerTimeWorkOrderRequest()
      override {
    return std::make_pair(
        std::chrono::milliseconds(monitor_time_period_secs_ * 1000), true);
  }

 private:
  const std::string file_pattern_;
  const std::int32_t monitor_time_period_secs_;
  const char field_terminator_;
  const bool process_escape_sequences_;
  std::unordered_set<std::string> added_files_;
  bool done_monitoring_ = false;

  const relation_id output_relation_id_;
  const QueryContext::insert_destination_id output_destination_index_;

  DISALLOW_COPY_AND_ASSIGN(MonitoredTextScanOperator);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_MONITORED_TEXT_SCAN_OPERATOR_HPP_
