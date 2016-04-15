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

#include <climits>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionState.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryManager.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "query_execution/WorkerMessage.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "query_optimizer/QueryPlan.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/InsertDestination.pb.h"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "utility/DAG.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"
#include "gtest/gtest.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

using std::move;
using std::unique_ptr;
using std::vector;

using tmb::client_id;

namespace quickstep {

class MockWorkOrder : public WorkOrder {
 public:
  explicit MockWorkOrder(const int op_index)
      : op_index_(op_index) {}

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
    kFeedInputBlocks,
    kDoneFeedingInputBlocks,
    kGetAllWorkOrders
  };

  MockOperator(const bool produce_workorders,
               const bool has_streaming_input,
               const int max_getworkorder_iters = 1,
               const int max_workorders = INT_MAX)
      : produce_workorders_(produce_workorders),
        has_streaming_input_(has_streaming_input),
        max_workorders_(max_workorders),
        max_getworkorder_iters_(max_getworkorder_iters),
        num_calls_get_workorders_(0),
        num_workorders_generated_(0),
        num_calls_feedblock_(0),
        num_calls_feedblocks_(0),
        num_calls_donefeedingblocks_(0) {
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
      case kFeedInputBlocks:
        return num_calls_feedblocks_;
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
        if ((num_calls_feedblock_ > 0 || num_calls_feedblocks_ > 0) && (num_workorders_generated_ < max_workorders_)) {
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

  void feedInputBlock(const block_id input_block_id,
                      const relation_id input_relation_id) override {
    ++num_calls_feedblock_;
    MOCK_OP_LOG(3) << "count(" << num_calls_feedblock_ << ")";
  }

  void feedInputBlocks(const relation_id rel_id,
                       std::vector<block_id> *partially_filled_blocks) override {
    ++num_calls_feedblocks_;
    MOCK_OP_LOG(3) << "count(" << num_calls_feedblocks_ << ")";
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
  int num_calls_feedblocks_;
  int num_calls_donefeedingblocks_;

  QueryContext::insert_destination_id insert_destination_index_ = QueryContext::kInvalidInsertDestinationId;

  relation_id output_relation_id_ = -1;

#undef MOCK_OP_LOG

  DISALLOW_COPY_AND_ASSIGN(MockOperator);
};


class QueryManagerTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    db_.reset(new CatalogDatabase(nullptr /* catalog */, "database"));
    storage_manager_.reset(new StorageManager("./"));
    bus_.Initialize();
    query_handle_.reset(new QueryHandle(0));
    query_plan_ = query_handle_->getQueryPlanMutable();
  }

  inline void constructQueryManager() {
    query_manager_.reset(new QueryManager(
        0, 1, query_handle_.get(), db_.get(), storage_manager_.get(), &bus_));
  }

  inline const int getNumWorkOrdersInExecution(const QueryPlan::DAGNodeIndex index) const {
    return query_manager_->getQueryExecutionState().getNumQueuedWorkOrders(index);
  }

  inline const int getNumOperatorsFinished() const {
    return query_manager_->getQueryExecutionState().getNumOperatorsFinished();
  }

  inline bool getOperatorFinishedStatus(const QueryPlan::DAGNodeIndex index) const {
    return query_manager_->getQueryExecutionState().hasExecutionFinished(index);
  }

  inline bool placeDataPipelineMessage(const QueryPlan::DAGNodeIndex source_operator_index) {
    VLOG(3) << "Place DataPipeline message for Op[" << source_operator_index << "]";
    serialization::DataPipelineMessage proto;
    proto.set_operator_index(source_operator_index);

    proto.set_block_id(0);  // dummy block ID
    proto.set_relation_id(0);  // dummy relation ID.

    // NOTE(zuyu): Using the heap memory to serialize proto as a c-like string.
    const std::size_t proto_length = proto.ByteSize();
    char *proto_bytes = static_cast<char*>(std::malloc(proto_length));
    CHECK(proto.SerializeToArray(proto_bytes, proto_length));

    tmb::TaggedMessage tagged_message(static_cast<const void *>(proto_bytes),
                                      proto_length,
                                      kDataPipelineMessage);
    std::free(proto_bytes);
    query_manager_->processMessage(tagged_message);
    return query_manager_->getQueryExecutionState().hasQueryExecutionFinished();
  }

  inline bool placeWorkOrderCompleteMessage(const QueryPlan::DAGNodeIndex index) {
    VLOG(3) << "Place WorkOrderComplete message for Op[" << index << "]";
    TaggedMessage tagged_message;
    serialization::WorkOrderCompletionMessage proto;
    proto.set_operator_index(index);
    proto.set_worker_thread_index(1);  // dummy worker ID.

    // NOTE(zuyu): Using the heap memory to serialize proto as a c-like string.
    const size_t proto_length = proto.ByteSize();
    char *proto_bytes = static_cast<char*>(std::malloc(proto_length));
    CHECK(proto.SerializeToArray(proto_bytes, proto_length));

    TaggedMessage message(static_cast<const void*>(proto_bytes),
                          proto_length,
                          kWorkOrderCompleteMessage);
    std::free(proto_bytes);
    query_manager_->processMessage(message);

    return query_manager_->getQueryExecutionState().hasQueryExecutionFinished();
  }

  inline bool placeRebuildWorkOrderCompleteMessage(const QueryPlan::DAGNodeIndex index) {
    VLOG(3) << "Place RebuildWorkOrderComplete message for Op[" << index << "]";
    // foreman_->processRebuildWorkOrderCompleteMessage(index, 0 /* worker id */);
    serialization::WorkOrderCompletionMessage proto;
    proto.set_operator_index(index);
    proto.set_worker_thread_index(1);  // dummy worker thread ID.

    // NOTE(zuyu): Using the heap memory to serialize proto as a c-like string.
    const size_t proto_length = proto.ByteSize();
    char *proto_bytes = static_cast<char*>(std::malloc(proto_length));
    CHECK(proto.SerializeToArray(proto_bytes, proto_length));

    TaggedMessage message(static_cast<const void*>(proto_bytes),
                          proto_length,
                          kRebuildWorkOrderCompleteMessage);

    std::free(proto_bytes);
    query_manager_->processMessage(message);

    return query_manager_->getQueryExecutionState().hasQueryExecutionFinished();
  }

  inline bool placeOutputBlockMessage(const QueryPlan::DAGNodeIndex index) {
    VLOG(3) << "Place OutputBlock message for Op[" << index << "]";
    serialization::DataPipelineMessage proto;
    proto.set_operator_index(index);

    proto.set_block_id(0);  // dummy block ID
    proto.set_relation_id(0);  // dummy relation ID.

    // NOTE(zuyu): Using the heap memory to serialize proto as a c-like string.
    const std::size_t proto_length = proto.ByteSize();
    char *proto_bytes = static_cast<char*>(std::malloc(proto_length));
    CHECK(proto.SerializeToArray(proto_bytes, proto_length));

    tmb::TaggedMessage tagged_message(static_cast<const void *>(proto_bytes),
                                      proto_length,
                                      kDataPipelineMessage);
    std::free(proto_bytes);
    query_manager_->processMessage(tagged_message);
    return query_manager_->getQueryExecutionState().hasQueryExecutionFinished();
  }

  unique_ptr<CatalogDatabase> db_;
  unique_ptr<StorageManager> storage_manager_;

  QueryPlan *query_plan_;
  unique_ptr<QueryHandle> query_handle_;
  unique_ptr<QueryManager> query_manager_;

  // unique_ptr<Foreman> foreman_;
  MessageBusImpl bus_;

  client_id worker_client_id_;

  unique_ptr<WorkerDirectory> workers_;
};

TEST_F(QueryManagerTest, SingleNodeDAGNoWorkOrdersTest) {
  // This test creates a DAG of a single node. No workorders are generated.
  query_plan_->addRelationalOperator(new MockOperator(false, false));
  // foreman_->setQueryPlan(query_plan_->getQueryPlanDAGMutable());

  const MockOperator &op = static_cast<const MockOperator &>(
      query_plan_->getQueryPlanDAG().getNodePayload(0));

  constructQueryManager();

  // op doesn't have any dependencies.
  EXPECT_TRUE(op.getBlockingDependenciesMet());

  // We expect one call for op's getAllWorkOrders().
  EXPECT_EQ(1, op.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(0, op.getNumCalls(MockOperator::kFeedInputBlock));
  EXPECT_EQ(0, op.getNumCalls(MockOperator::kFeedInputBlocks));
}

TEST_F(QueryManagerTest, SingleNodeDAGStaticWorkOrdersTest) {
  // This test creates a DAG of a single node. Static workorders are generated.
  const QueryPlan::DAGNodeIndex id =
      query_plan_->addRelationalOperator(new MockOperator(true, false, 1));
  // foreman_->setQueryPlan(query_plan_->getQueryPlanDAGMutable());

  const MockOperator &op = static_cast<const MockOperator &>(
      query_plan_->getQueryPlanDAG().getNodePayload(id));

  constructQueryManager();

  // op doesn't have any dependencies.
  EXPECT_TRUE(op.getBlockingDependenciesMet());

  // We expect one call for op's getAllWorkOrders().
  EXPECT_EQ(1, op.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(0, op.getNumCalls(MockOperator::kFeedInputBlock));
  EXPECT_EQ(0, op.getNumCalls(MockOperator::kFeedInputBlocks));

  // One workorder is generated.
  EXPECT_EQ(1, op.getNumWorkOrders());

  unique_ptr<WorkerMessage> worker_message;
  worker_message.reset(query_manager_->getNextWorkerMessage(0, -1));
  EXPECT_TRUE(worker_message != nullptr);

  EXPECT_EQ(WorkerMessage::WorkerMessageType::kWorkOrder,
            worker_message->getType());
  EXPECT_EQ(0u, worker_message->getRelationalOpIndex());

  delete worker_message->getWorkOrder();

  EXPECT_EQ(1, getNumWorkOrdersInExecution(id));
  EXPECT_EQ(0, getNumOperatorsFinished());

  // Send a message to QueryManager upon workorder completion.
  // Last event processed by QueryManager.
  EXPECT_TRUE(placeWorkOrderCompleteMessage(id));

  EXPECT_EQ(0, getNumWorkOrdersInExecution(id));
  EXPECT_EQ(1, getNumOperatorsFinished());
  EXPECT_TRUE(getOperatorFinishedStatus(id));
}

TEST_F(QueryManagerTest, SingleNodeDAGDynamicWorkOrdersTest) {
  // This test creates a DAG of a single node. WorkOrders are generated
  // dynamically as pending work orders complete execution, i.e.,
  // getAllWorkOrders() is called multiple times.  getAllWorkOrders() will be
  // called 5 times and 3 work orders will be returned, i.e., 1st 3 calls to
  // getAllWorkOrders() insert 1 WorkOrder and return false, and the next will
  // insert no WorkOrder and return true.

  // TODO(shoban): This test can not be more robust than this because of fixed
  // scaffolding of mocking. If we use gMock, we can do much better.
  const QueryPlan::DAGNodeIndex id =
      query_plan_->addRelationalOperator(new MockOperator(true, false, 4, 3));
  // foreman_->setQueryPlan(query_plan_->getQueryPlanDAGMutable());

  const MockOperator &op = static_cast<const MockOperator &>(
      query_plan_->getQueryPlanDAG().getNodePayload(id));

  constructQueryManager();

  // op doesn't have any dependencies.
  EXPECT_TRUE(op.getBlockingDependenciesMet());

  for (int i = 0; i < 3; ++i) {
    // We expect one call for op's getAllWorkOrders().
    EXPECT_EQ(i + 1, op.getNumCalls(MockOperator::kGetAllWorkOrders));

    // One workorder is generated.
    // EXPECT_EQ(1, getWorkerInputQueueSize());
    EXPECT_EQ(i + 1, op.getNumWorkOrders());

    unique_ptr<WorkerMessage> worker_message;
    worker_message.reset(query_manager_->getNextWorkerMessage(id, -1));

    EXPECT_TRUE(worker_message != nullptr);
    EXPECT_EQ(WorkerMessage::WorkerMessageType::kWorkOrder,
              worker_message->getType());
    EXPECT_EQ(id, worker_message->getRelationalOpIndex());

    delete worker_message->getWorkOrder();

    EXPECT_EQ(1, getNumWorkOrdersInExecution(id));
    EXPECT_EQ(0, getNumOperatorsFinished());

    if (i < 2) {
      // Send a message to QueryManager upon workorder completion.
      EXPECT_FALSE(placeWorkOrderCompleteMessage(id));
    } else {
      // Send a message to QueryManager upon workorder completion.
      // Last event.
      EXPECT_TRUE(placeWorkOrderCompleteMessage(id));
    }
  }

  EXPECT_EQ(0, getNumWorkOrdersInExecution(id));

  EXPECT_EQ(1, getNumOperatorsFinished());
  EXPECT_TRUE(getOperatorFinishedStatus(id));

  // We place this check in the end, since it's true throughout the test.
  EXPECT_EQ(0, op.getNumCalls(MockOperator::kFeedInputBlock));
  EXPECT_EQ(0, op.getNumCalls(MockOperator::kFeedInputBlocks));
}

TEST_F(QueryManagerTest, TwoNodesDAGBlockingLinkTest) {
  // We use two nodes in the DAG with a blocking link between them.
  // There is no streaming of data involved in this test.
  const QueryPlan::DAGNodeIndex id1 =
      query_plan_->addRelationalOperator(new MockOperator(true, false));
  const QueryPlan::DAGNodeIndex id2 =
      query_plan_->addRelationalOperator(new MockOperator(true, false));

  // Create a blocking link.
  query_plan_->addDirectDependency(id2, id1, true);

  static_cast<MockOperator *>(
      query_plan_->getQueryPlanDAGMutable()->getNodePayloadMutable(id1))
          ->setOutputRelationID(0xdead);

  const MockOperator &op1 = static_cast<const MockOperator &>(
      query_plan_->getQueryPlanDAG().getNodePayload(id1));
  const MockOperator &op2 = static_cast<const MockOperator &>(
      query_plan_->getQueryPlanDAG().getNodePayload(id2));

  constructQueryManager();

  // op1 doesn't have any dependencies
  EXPECT_TRUE(op1.getBlockingDependenciesMet());

  // Only op1 should receive a call to getAllWorkOrders initially.
  EXPECT_EQ(1, op1.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(0, op1.getNumCalls(MockOperator::kFeedInputBlock));
  EXPECT_EQ(0, op1.getNumCalls(MockOperator::kFeedInputBlocks));

  EXPECT_EQ(0, op2.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(0, op2.getNumCalls(MockOperator::kFeedInputBlock));
  EXPECT_EQ(0, op2.getNumCalls(MockOperator::kFeedInputBlocks));

  // Only op1 should produce a workorder.
  EXPECT_EQ(1, op1.getNumWorkOrders());
  EXPECT_EQ(0, op2.getNumWorkOrders());

  // Foreman hasn't yet got workorder completion response for the workorder.
  unique_ptr<WorkerMessage> worker_message;
  worker_message.reset(query_manager_->getNextWorkerMessage(id1, -1));

  EXPECT_TRUE(worker_message != nullptr);
  EXPECT_EQ(WorkerMessage::WorkerMessageType::kWorkOrder,
            worker_message->getType());
  EXPECT_EQ(id1, worker_message->getRelationalOpIndex());

  delete worker_message->getWorkOrder();

  EXPECT_EQ(1, getNumWorkOrdersInExecution(id1));
  EXPECT_EQ(0, getNumWorkOrdersInExecution(id2));
  EXPECT_EQ(0, getNumOperatorsFinished());

  // Send a message to Foreman upon workorder (generated by op1) completion.
  EXPECT_FALSE(placeWorkOrderCompleteMessage(id1));

  EXPECT_EQ(0, getNumWorkOrdersInExecution(id1));
  // op1 is over now, op2 still to go.
  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kDoneFeedingInputBlocks));
  EXPECT_EQ(1, getNumOperatorsFinished());

  EXPECT_TRUE(getOperatorFinishedStatus(id1));
  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kDoneFeedingInputBlocks));
  EXPECT_FALSE(getOperatorFinishedStatus(id2));

  worker_message.reset(query_manager_->getNextWorkerMessage(id2, -1));
  EXPECT_TRUE(worker_message != nullptr);
  EXPECT_EQ(WorkerMessage::WorkerMessageType::kWorkOrder,
            worker_message->getType());
  EXPECT_EQ(id2, worker_message->getRelationalOpIndex());

  delete worker_message->getWorkOrder();

  EXPECT_EQ(1, getNumWorkOrdersInExecution(id2));

  // op1 is op2's blocking dependency.
  EXPECT_TRUE(op2.getBlockingDependenciesMet());

  EXPECT_EQ(1, op1.getNumCalls(MockOperator::kGetAllWorkOrders));
  // op2 should get first call of getAllWorkOrders() when op1 is over.
  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kGetAllWorkOrders));

  EXPECT_EQ(1, op2.getNumWorkOrders());

  // Send a message to QueryManager upon workorder (generated by op2) completion.
  // Note that the worker hasn't yet popped the workorder. Usually this won't
  // happen as workers pop workorders first, execute and then send the response.
  EXPECT_TRUE(placeWorkOrderCompleteMessage(id2));

  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kGetAllWorkOrders));

  EXPECT_EQ(0, getNumWorkOrdersInExecution(id1));
  EXPECT_EQ(0, getNumWorkOrdersInExecution(id2));

  EXPECT_EQ(2, getNumOperatorsFinished());
  EXPECT_TRUE(getOperatorFinishedStatus(id1));
  EXPECT_TRUE(getOperatorFinishedStatus(id2));

  // Expect no additional calls to getAllWorkOrders.
  EXPECT_EQ(1, op1.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kGetAllWorkOrders));
}

TEST_F(QueryManagerTest, TwoNodesDAGPipeLinkTest) {
  // We use two nodes in the DAG with a non-blocking link between them.
  // We stream output of op1 to op2. Sequeuce of events is as follows:
  // 1. op1 creates a workorder.
  // 2. We send a "block full" (from op1) to QueryManager.
  // 3. op2 creates a workorder because of step 2.
  const QueryPlan::DAGNodeIndex id1 =
      query_plan_->addRelationalOperator(new MockOperator(true, false, 1));
  const QueryPlan::DAGNodeIndex id2 =
      query_plan_->addRelationalOperator(new MockOperator(true, true, 3));

  // Create a non-blocking link.
  query_plan_->addDirectDependency(id2, id1, false);

  static_cast<MockOperator *>(
      query_plan_->getQueryPlanDAGMutable()->getNodePayloadMutable(id1))
      ->setOutputRelationID(0xdead);

  const MockOperator &op1 = static_cast<const MockOperator &>(
      query_plan_->getQueryPlanDAG().getNodePayload(id1));
  const MockOperator &op2 = static_cast<const MockOperator &>(
      query_plan_->getQueryPlanDAG().getNodePayload(id2));

  constructQueryManager();

  // As none of the operators have a blocking link, blocking dependencies should
  // be met.
  EXPECT_TRUE(op1.getBlockingDependenciesMet());
  EXPECT_TRUE(op2.getBlockingDependenciesMet());

  EXPECT_EQ(1, op1.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(1, op1.getNumWorkOrders());
  EXPECT_EQ(0, op1.getNumCalls(MockOperator::kFeedInputBlock));
  EXPECT_EQ(0, op1.getNumCalls(MockOperator::kFeedInputBlocks));

  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kGetAllWorkOrders));
  // op2 will generate workorder only after receiving a streaming input.
  EXPECT_EQ(0, op2.getNumWorkOrders());
  EXPECT_EQ(0, op2.getNumCalls(MockOperator::kFeedInputBlock));
  EXPECT_EQ(0, op2.getNumCalls(MockOperator::kFeedInputBlocks));

  unique_ptr<WorkerMessage> worker_message;
  worker_message.reset(query_manager_->getNextWorkerMessage(id1, -1));

  EXPECT_TRUE(worker_message != nullptr);
  EXPECT_EQ(WorkerMessage::WorkerMessageType::kWorkOrder,
            worker_message->getType());
  EXPECT_EQ(id1, worker_message->getRelationalOpIndex());

  delete worker_message->getWorkOrder();

  // Send a message to QueryManager upon block getting full (output of op1).
  EXPECT_FALSE(placeOutputBlockMessage(id1));

  // op1 is not finished yet because the response of workorder completion hasn't
  // been received yet by the QueryManager.
  EXPECT_FALSE(getOperatorFinishedStatus(id1));
  EXPECT_FALSE(getOperatorFinishedStatus(id2));

  // No additional call to op1's getAllWorkOrders.
  EXPECT_EQ(1, op1.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(0, op1.getNumCalls(MockOperator::kFeedInputBlock));
  EXPECT_EQ(0, op1.getNumCalls(MockOperator::kFeedInputBlocks));

  // Output from op1 should be fed to op2.
  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kFeedInputBlock));
  EXPECT_EQ(0, op2.getNumCalls(MockOperator::kFeedInputBlocks));

  // A call to op2's getAllWorkOrders because of the streamed input.
  EXPECT_EQ(2, op2.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(1, op2.getNumWorkOrders());

  // Place a message of a workorder completion of op1 on Foreman's input queue.
  EXPECT_FALSE(placeWorkOrderCompleteMessage(id1));

  EXPECT_EQ(0, getNumWorkOrdersInExecution(id1));
  EXPECT_TRUE(getOperatorFinishedStatus(id1));
  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kDoneFeedingInputBlocks));

  // An additional call to op2's getAllWorkOrders because of completion of op1.
  EXPECT_EQ(3, op2.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(2, op2.getNumWorkOrders());

  worker_message.reset(query_manager_->getNextWorkerMessage(id2, -1));

  EXPECT_TRUE(worker_message != nullptr);
  EXPECT_EQ(WorkerMessage::WorkerMessageType::kWorkOrder,
            worker_message->getType());
  EXPECT_EQ(id2, worker_message->getRelationalOpIndex());

  delete worker_message->getWorkOrder();

  // Place a message of a workorder completion of op2 on Foreman's input queue.
  EXPECT_FALSE(placeWorkOrderCompleteMessage(id2));

  EXPECT_TRUE(getOperatorFinishedStatus(id1));

  worker_message.reset(query_manager_->getNextWorkerMessage(id2, -1));

  EXPECT_TRUE(worker_message != nullptr);
  EXPECT_EQ(WorkerMessage::WorkerMessageType::kWorkOrder,
            worker_message->getType());
  EXPECT_EQ(id2, worker_message->getRelationalOpIndex());

  delete worker_message->getWorkOrder();

  EXPECT_EQ(1, getNumWorkOrdersInExecution(id2));
  EXPECT_FALSE(getOperatorFinishedStatus(id2));

  // Send a message to Foreman upon workorder (generated by op2) completion.
  EXPECT_TRUE(placeWorkOrderCompleteMessage(id2));

  EXPECT_TRUE(getOperatorFinishedStatus(id1));
  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kDoneFeedingInputBlocks));

  EXPECT_EQ(0, getNumWorkOrdersInExecution(id2));
  EXPECT_TRUE(getOperatorFinishedStatus(id2));
}

TEST_F(QueryManagerTest, TwoNodesDAGPartiallyFilledBlocksTest) {
  // In this test, we create a 2-node DAG with a non-blocking link between them.
  // There is no streaming of data from op1 to op2 during the execution of op1.
  // op1 produces a partially filled block at the end of its execution which is
  // rebuilt and then fed to op2.
  const QueryPlan::DAGNodeIndex id1 =
      query_plan_->addRelationalOperator(new MockOperator(true, false, 1));
  const QueryPlan::DAGNodeIndex id2 =
      query_plan_->addRelationalOperator(new MockOperator(true, true, 3, 1));

  // Create a non-blocking link.
  query_plan_->addDirectDependency(id2, id1, false);

  // Create a relation, owned by db_.*/
  CatalogRelation *relation =
      new CatalogRelation(nullptr /* catalog_database */, "test_relation");
  const relation_id output_relation_id = db_->addRelation(relation);

  // Setup the InsertDestination proto in the query context proto.
  serialization::QueryContext *query_context_proto =
      query_handle_->getQueryContextProtoMutable();

  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto->insert_destinations_size();
  serialization::InsertDestination *insert_destination_proto =
      query_context_proto->add_insert_destinations();

  insert_destination_proto->set_insert_destination_type(
      serialization::InsertDestinationType::BLOCK_POOL);
  insert_destination_proto->set_relation_id(output_relation_id);
  insert_destination_proto->set_relational_op_index(id1);

  MockOperator *op1_mutable = static_cast<MockOperator *>(
      query_plan_->getQueryPlanDAGMutable()->getNodePayloadMutable(id1));
  op1_mutable->setInsertDestinationID(insert_destination_index);
  op1_mutable->setOutputRelationID(output_relation_id);

  const MockOperator &op1 = static_cast<const MockOperator &>(
      query_plan_->getQueryPlanDAG().getNodePayload(id1));
  const MockOperator &op2 = static_cast<const MockOperator &>(
      query_plan_->getQueryPlanDAG().getNodePayload(id2));

  constructQueryManager();

  // NOTE(zuyu): An operator generally has no ideas about partially filled
  // blocks, but InsertDestination in QueryContext does.
  // Mock to add partially filled blocks in the InsertDestination.
  InsertDestination *insert_destination =
      query_manager_->getQueryContextMutable()->getInsertDestination(
          insert_destination_index);
  DCHECK(insert_destination != nullptr);
  MutableBlockReference block_ref;
  static_cast<BlockPoolInsertDestination *>(insert_destination)
      ->available_block_refs_.push_back(move(block_ref));

  // There's no blocking dependency in the DAG.
  EXPECT_TRUE(op1.getBlockingDependenciesMet());
  EXPECT_TRUE(op2.getBlockingDependenciesMet());

  EXPECT_EQ(1, op1.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(1, op1.getNumWorkOrders());

  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(0, op2.getNumWorkOrders());

  unique_ptr<WorkerMessage> worker_message;
  worker_message.reset(query_manager_->getNextWorkerMessage(id1, -1));

  EXPECT_TRUE(worker_message != nullptr);
  EXPECT_EQ(WorkerMessage::WorkerMessageType::kWorkOrder,
            worker_message->getType());
  EXPECT_EQ(id1, worker_message->getRelationalOpIndex());

  delete worker_message->getWorkOrder();

  // Send a message to QueryManager upon workorder (generated by op1) completion.
  EXPECT_FALSE(placeWorkOrderCompleteMessage(id1));

  EXPECT_EQ(0, getNumWorkOrdersInExecution(id1));

  worker_message.reset(query_manager_->getNextWorkerMessage(id1, -1));
  EXPECT_TRUE(worker_message != nullptr);
  EXPECT_EQ(WorkerMessage::WorkerMessageType::kRebuildWorkOrder,
            worker_message->getType());

  EXPECT_EQ(id1, worker_message->getRelationalOpIndex());

  delete worker_message->getWorkOrder();

  // op1 generates a rebuild workorder. The block is rebuilt and streamed
  // to Foreman.
  EXPECT_FALSE(placeDataPipelineMessage(id1));

  EXPECT_FALSE(placeRebuildWorkOrderCompleteMessage(id1));
  // Based on the streamed input, op2's getAllWorkOrders should produce a
  // workorder.
  EXPECT_EQ(3, op2.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(1, op2.getNumWorkOrders());

  worker_message.reset(query_manager_->getNextWorkerMessage(id2, -1));

  EXPECT_TRUE(worker_message != nullptr);
  EXPECT_EQ(WorkerMessage::WorkerMessageType::kWorkOrder,
            worker_message->getType());

  EXPECT_EQ(id2, worker_message->getRelationalOpIndex());

  delete worker_message->getWorkOrder();

  EXPECT_TRUE(getOperatorFinishedStatus(id1));
  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kDoneFeedingInputBlocks));
  EXPECT_FALSE(getOperatorFinishedStatus(id2));
  EXPECT_EQ(1, getNumWorkOrdersInExecution(id2));

  // Send a message to QueryManager upon workorder (generated by op2) completion.
  EXPECT_TRUE(placeWorkOrderCompleteMessage(id2));

  EXPECT_EQ(0, getNumWorkOrdersInExecution(id2));

  EXPECT_TRUE(getOperatorFinishedStatus(id2));
}

TEST_F(QueryManagerTest, MultipleNodesNoOutputTest) {
  // When an operator produces workorders but no output, the QueryManager should
  // check the dependents of this operator to make progress.
  const QueryPlan::DAGNodeIndex kNumNodes = 5;
  std::vector<QueryPlan::DAGNodeIndex> ids;
  ids.reserve(kNumNodes);

  for (QueryPlan::DAGNodeIndex i = 0; i < kNumNodes; ++i) {
    if (i == 0) {
      ids[i] = query_plan_->addRelationalOperator(new MockOperator(true, false));
    } else {
      ids[i] = query_plan_->addRelationalOperator(new MockOperator(true, true));
    }
    VLOG(3) << ids[i];
  }

  /**
   * The DAG looks like this:
   *
   * op1 -> op2 -> op3 -> op4 -> op5
   *
   **/
  for (QueryPlan::DAGNodeIndex i = 0; i < kNumNodes - 1; ++i) {
    query_plan_->addDirectDependency(ids[i + 1], ids[i], false);
    static_cast<MockOperator*>(query_plan_->getQueryPlanDAGMutable()->getNodePayloadMutable(ids[i]))
        ->setOutputRelationID(0xdead);
  }

  std::vector<const MockOperator*> operators;
  for (QueryPlan::DAGNodeIndex i = 0; i < kNumNodes; ++i) {
    operators.push_back(static_cast<const MockOperator*>(&query_plan_->getQueryPlanDAG().getNodePayload(ids[i])));
  }

  constructQueryManager();

  // operators[0] should have produced a workorder by now.
  EXPECT_EQ(1, operators[0]->getNumWorkOrders());

  unique_ptr<WorkerMessage> worker_message;
  worker_message.reset(query_manager_->getNextWorkerMessage(ids[0], -1));

  EXPECT_TRUE(worker_message != nullptr);
  EXPECT_EQ(WorkerMessage::WorkerMessageType::kWorkOrder,
            worker_message->getType());

  EXPECT_EQ(ids[0], worker_message->getRelationalOpIndex());

  delete worker_message->getWorkOrder();

  EXPECT_EQ(1, getNumWorkOrdersInExecution(ids[0]));
  EXPECT_FALSE(getOperatorFinishedStatus(ids[0]));

  for (QueryPlan::DAGNodeIndex i = 0; i < kNumNodes; ++i) {
    EXPECT_EQ(1, operators[ids[i]]->getNumCalls(MockOperator::kGetAllWorkOrders));
  }

  // Send a message to QueryManager upon workorder (generated by operators[0])
  // completion.
  EXPECT_TRUE(placeWorkOrderCompleteMessage(ids[0]));

  for (QueryPlan::DAGNodeIndex i = 0; i < kNumNodes; ++i) {
    EXPECT_EQ(0, getNumWorkOrdersInExecution(ids[i]));
    EXPECT_TRUE(getOperatorFinishedStatus(ids[i]));
    if (i < kNumNodes - 1) {
      EXPECT_EQ(1, operators[i + 1]->getNumCalls(MockOperator::kDoneFeedingInputBlocks));
    }
  }
}

TEST_F(QueryManagerTest, OutOfOrderWorkOrderCompletionTest) {
  // Consider two operators, both generate one workorder each. The dependent's
  // workorder finishes before dependency's workorder.
  const QueryPlan::DAGNodeIndex id1 = query_plan_->addRelationalOperator(new MockOperator(true, false, 1));
  const QueryPlan::DAGNodeIndex id2 = query_plan_->addRelationalOperator(new MockOperator(true, true, 2, 1));

  // Create a non-blocking link.
  query_plan_->addDirectDependency(id2, id1, false);

  constructQueryManager();

  unique_ptr<WorkerMessage> worker_message;
  worker_message.reset(query_manager_->getNextWorkerMessage(id1, -1));

  EXPECT_TRUE(worker_message != nullptr);
  EXPECT_EQ(WorkerMessage::WorkerMessageType::kWorkOrder,
            worker_message->getType());

  EXPECT_EQ(id1, worker_message->getRelationalOpIndex());

  delete worker_message->getWorkOrder();

  // Send a message to QueryManager upon a block (output of op1) getting full.
  EXPECT_FALSE(placeOutputBlockMessage(id1));

  // op1 is not finished yet because the response of workorder completion hasn't
  // been received yet.
  EXPECT_FALSE(getOperatorFinishedStatus(id1));
  EXPECT_FALSE(getOperatorFinishedStatus(id2));

  worker_message.reset(query_manager_->getNextWorkerMessage(id2, -1));
  EXPECT_TRUE(worker_message != nullptr);
  EXPECT_EQ(WorkerMessage::WorkerMessageType::kWorkOrder,
            worker_message->getType());

  EXPECT_EQ(id2, worker_message->getRelationalOpIndex());

  delete worker_message->getWorkOrder();

  // As mentioned earlier, op2 finishes before op1.
  EXPECT_FALSE(placeWorkOrderCompleteMessage(id2));

  // op1's workorder execution is over.
  EXPECT_TRUE(placeWorkOrderCompleteMessage(id1));

  EXPECT_TRUE(getOperatorFinishedStatus(id1));
  EXPECT_TRUE(getOperatorFinishedStatus(id2));
}

}  // namespace quickstep
