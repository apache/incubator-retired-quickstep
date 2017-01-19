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

#include <cstdint>
#include <cstddef>
#include <ostream>
#include <string>

#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "query_optimizer/QueryPlan.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep { class QueryContext; }
namespace quickstep { class StorageManager; }
namespace tmb { class MessageBus; }

namespace quickstep {

class WorkOrderProtosContainer;

class MockWorkOrder : public WorkOrder {
 public:
  explicit MockWorkOrder(const int op_index)
      : WorkOrder(0), op_index_(op_index) {}

  void execute() override {
    VLOG(3) << "WorkOrder[" << op_index_ << "] executing.";
  }

  inline QueryPlan::DAGNodeIndex getOpIndex() const {
    return op_index_;
  }

 private:
  const QueryPlan::DAGNodeIndex op_index_;

  DISALLOW_COPY_AND_ASSIGN(MockWorkOrder);
};

class MockOperator: public RelationalOperator {
 public:
  enum function_name {
    kFeedInputBlock = 0,
    kDoneFeedingInputBlocks,
    kGetAllWorkOrders
  };

  MockOperator(const bool produce_workorders,
               const bool has_streaming_input,
               const int max_getworkorder_iters = 1,
               const int max_workorders = INT_MAX)
      : RelationalOperator(0 /* Query Id */),
        produce_workorders_(produce_workorders),
        has_streaming_input_(has_streaming_input),
        max_workorders_(max_workorders),
        max_getworkorder_iters_(max_getworkorder_iters),
        num_calls_get_workorders_(0),
        num_workorders_generated_(0),
        num_calls_feedblock_(0),
        num_calls_donefeedingblocks_(0) {
  }

  std::string getName() const override {
    return "MockOperator";
  }

#define MOCK_OP_LOG(x) VLOG(x) << "Op[" << op_index_ << "]: " << __func__ << ": "

  // The methods below are used to check whether QueryManager calls the Relational
  // operator, how many times it calls a particular method etc.
  inline int getNumWorkOrders() const {
    return num_workorders_generated_;
  }

  inline int getNumCalls(const function_name fname) const {
    switch (fname) {
      case kFeedInputBlock:
        return num_calls_feedblock_;
      case kDoneFeedingInputBlocks:
        return num_calls_donefeedingblocks_;
      case kGetAllWorkOrders:
        return num_calls_get_workorders_;
      default:
        return -1;
    }
  }

  inline bool getBlockingDependenciesMet() const {
    MOCK_OP_LOG(3) << "met.";
    return blocking_dependencies_met_;
  }

  void setInsertDestinationID(const QueryContext::insert_destination_id insert_destination_index) {
    insert_destination_index_ = insert_destination_index;
  }

  // Mock to trigger doneFeedingInputBlocks for the dependent operators
  // in QueryManager::markOperatorFinished.
  void setOutputRelationID(const relation_id rel_id) {
    output_relation_id_ = rel_id;
  }

  // Override methods from the base class.
  bool getAllWorkOrders(
      WorkOrdersContainer *container,
      QueryContext *query_context,
      StorageManager *storage_manager,
      const tmb::client_id foreman_client_id,
      tmb::MessageBus *bus) override {
    ++num_calls_get_workorders_;
    if (produce_workorders_) {
      if (has_streaming_input_) {
        if (num_calls_feedblock_ > 0 && (num_workorders_generated_ < max_workorders_)) {
          MOCK_OP_LOG(3) << "[stream] generate WorkOrder";
          container->addNormalWorkOrder(new MockWorkOrder(op_index_), op_index_);
          ++num_workorders_generated_;
        }
      } else {
        if (blocking_dependencies_met_ && (num_workorders_generated_ < max_workorders_)) {
          MOCK_OP_LOG(3) << "[static] generate WorkOrder";
          container->addNormalWorkOrder(new MockWorkOrder(op_index_), op_index_);
          ++num_workorders_generated_;
        }
      }
    }
    MOCK_OP_LOG(3) << "count(" << num_calls_get_workorders_ << ") "
                   << "return(" << (num_calls_get_workorders_ == max_getworkorder_iters_) << ")";
    return num_calls_get_workorders_ == max_getworkorder_iters_;
  }

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override {
    return true;
  }

  void feedInputBlock(const block_id input_block_id, const relation_id input_relation_id,
                      const partition_id part_id) override {
    ++num_calls_feedblock_;
    MOCK_OP_LOG(3) << "count(" << num_calls_feedblock_ << ")";
  }

  void doneFeedingInputBlocks(const relation_id rel_id) override {
    ++num_calls_donefeedingblocks_;
    MOCK_OP_LOG(3) << "count(" << num_calls_donefeedingblocks_ << ")";
  }

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return insert_destination_index_;
  }

  const relation_id getOutputRelationID() const override {
    return output_relation_id_;
  }

 private:
  const bool produce_workorders_;
  const bool has_streaming_input_;
  const int max_workorders_;
  const int max_getworkorder_iters_;

  int num_calls_get_workorders_;
  int num_workorders_generated_;
  int num_calls_feedblock_;
  int num_calls_donefeedingblocks_;

  QueryContext::insert_destination_id insert_destination_index_ = QueryContext::kInvalidInsertDestinationId;

  relation_id output_relation_id_ = -1;

#undef MOCK_OP_LOG

  DISALLOW_COPY_AND_ASSIGN(MockOperator);
};

}  // namespace quickstep
