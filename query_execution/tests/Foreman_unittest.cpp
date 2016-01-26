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
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/Foreman.hpp"
#include "query_execution/ForemanMessage.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "query_execution/WorkerMessage.hpp"
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

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
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

  void execute(QueryContext *query_context,
               CatalogDatabase *catalog_database,
               StorageManager *storage_manager) override {
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

  // The methods below are used to check whether Foreman calls the Relational
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
  // in Foreman::markOperatorFinished.
  void setOutputRelationID(const relation_id rel_id) {
    output_relation_id_ = rel_id;
  }

  // Override methods from the base class.
  bool getAllWorkOrders(WorkOrdersContainer *container) override {
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


class ForemanTest : public ::testing::Test {
 protected:
  // Class ForemanTest is the friend of class Foreman. Each TEST_F behaves
  // as a separate class, so we can't access Foreman's private members in
  // TEST_F.
  virtual void SetUp() {
    query_plan_.reset(new QueryPlan());

    bus_.Initialize();
    foreman_.reset(new Foreman(&bus_));

    // This thread acts both as Foreman as well as Worker. Foreman connects to
    // the bus in its constructor.
    worker_client_id_ = bus_.Connect();

    // Register as sender and receiver for relevant types of messages.
    bus_.RegisterClientAsSender(worker_client_id_, kWorkOrderCompleteMessage);
    bus_.RegisterClientAsSender(worker_client_id_, kRebuildWorkOrderCompleteMessage);
    bus_.RegisterClientAsSender(worker_client_id_, kDataPipelineMessage);
    bus_.RegisterClientAsReceiver(worker_client_id_, kWorkOrderMessage);
    bus_.RegisterClientAsReceiver(worker_client_id_, kRebuildWorkOrderMessage);
    bus_.RegisterClientAsReceiver(worker_client_id_, kPoisonMessage);

    // Cache foreman's address.
    foreman_address_.AddRecipient(foreman_->getBusClientID());

    std::vector<client_id> worker_client_ids;
    worker_client_ids.push_back(worker_client_id_);

    std::vector<numa_node_id> numa_nodes;
    numa_nodes.push_back(static_cast<numa_node_id>(-1));

    workers_.reset(new WorkerDirectory(1, worker_client_ids, numa_nodes));
    foreman_->setWorkerDirectory(workers_.get());
  }

  inline const int getNumWorkOrdersInExecution(const QueryPlan::DAGNodeIndex index) const {
    return foreman_->queued_workorders_per_op_[index];
  }

  inline const int getNumOperatorsFinished() const {
    return foreman_->num_operators_finished_;
  }

  inline bool getOperatorFinishedStatus(const QueryPlan::DAGNodeIndex index) const {
    return foreman_->execution_finished_[index];
  }

  inline bool popWorkOrderIfAvailable(MockWorkOrder **workorder) {
    AnnotatedMessage msg;
    if (bus_.ReceiveIfAvailable(worker_client_id_, &msg)) {
      WorkerMessage message(*static_cast<const WorkerMessage*>(msg.tagged_message.message()));
      *workorder = static_cast<MockWorkOrder*>(message.getWorkOrder());
      return true;
    }
    return false;
  }

  inline bool popRebuildWorkOrderIfAvailable(MockWorkOrder **workorder) {
    return popWorkOrderIfAvailable(workorder);
  }

  inline bool placeDataPipelineMessage(const QueryPlan::DAGNodeIndex source_operator_index) {
    VLOG(3) << "Place DataPipeline message for Op[" << source_operator_index << "]";
    ForemanMessage foreman_message(ForemanMessage::DataPipelineMessage(source_operator_index, 0, 0));
    foreman_->processMessage(foreman_message);
    return foreman_->checkQueryExecutionFinished();
  }

  inline bool placeWorkOrderCompleteMessage(const QueryPlan::DAGNodeIndex index) {
    VLOG(3) << "Place WorkOrderComplete message for Op[" << index << "]";
    ForemanMessage foreman_message(ForemanMessage::WorkOrderCompletionMessage(index, 0));
    foreman_->processMessage(foreman_message);
    return foreman_->checkQueryExecutionFinished();
  }

  inline bool placeRebuildWorkOrderCompleteMessage(const QueryPlan::DAGNodeIndex index) {
    VLOG(3) << "Place RebuildWorkOrderComplete message for Op[" << index << "]";
    ForemanMessage foreman_message(ForemanMessage::RebuildCompletionMessage(index, 0));
    foreman_->processMessage(foreman_message);
    return foreman_->checkQueryExecutionFinished();
  }

  inline bool placeOutputBlockMessage(const QueryPlan::DAGNodeIndex index) {
    VLOG(3) << "Place OutputBlock message for Op[" << index << "]";
    ForemanMessage foreman_message(
    ForemanMessage::DataPipelineMessage(index, BlockIdUtil::GetBlockId(1 /* domain */, 1), 0));
    foreman_->processMessage(foreman_message);
    return foreman_->checkQueryExecutionFinished();
  }

  inline bool startForeman() {
    foreman_->initialize();
    return foreman_->checkQueryExecutionFinished();
  }

  inline int getWorkerInputQueueSize() {
    return bus_.CountQueuedMessagesForClient(worker_client_id_);
  }

  unique_ptr<QueryPlan> query_plan_;

  unique_ptr<Foreman> foreman_;
  MessageBusImpl bus_;

  Address foreman_address_;
  MessageStyle single_receiver_style_;

  client_id worker_client_id_;

  unique_ptr<WorkerDirectory> workers_;
};

TEST_F(ForemanTest, SingleNodeDAGNoWorkOrdersTest) {
  // This test creates a DAG of a single node. No workorders are generated.
  query_plan_->addRelationalOperator(new MockOperator(false, false));
  foreman_->setQueryPlan(query_plan_->getQueryPlanDAGMutable());

  const MockOperator &op = static_cast<const MockOperator&>(query_plan_->getQueryPlanDAG().getNodePayload(0));

  EXPECT_EQ(0, getWorkerInputQueueSize());

  // Foreman exits after initialize, since no workorders are generated.
  EXPECT_TRUE(startForeman());

  // op doesn't have any dependencies.
  EXPECT_TRUE(op.getBlockingDependenciesMet());

  // No workorder is generated. No response is received.
  EXPECT_EQ(0, getWorkerInputQueueSize());

  // We expect one call for op's getAllWorkOrders().
  EXPECT_EQ(1, op.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(0, op.getNumCalls(MockOperator::kFeedInputBlock));
  EXPECT_EQ(0, op.getNumCalls(MockOperator::kFeedInputBlocks));
}

TEST_F(ForemanTest, SingleNodeDAGStaticWorkOrdersTest) {
  // This test creates a DAG of a single node. Static workorders are generated.
  const QueryPlan::DAGNodeIndex id = query_plan_->addRelationalOperator(new MockOperator(true, false, 1));
  foreman_->setQueryPlan(query_plan_->getQueryPlanDAGMutable());

  const MockOperator &op = static_cast<const MockOperator&>(query_plan_->getQueryPlanDAG().getNodePayload(id));

  EXPECT_EQ(0, getWorkerInputQueueSize());
  EXPECT_FALSE(startForeman());

  // op doesn't have any dependencies.
  EXPECT_TRUE(op.getBlockingDependenciesMet());

  // We expect one call for op's getAllWorkOrders().
  EXPECT_EQ(1, op.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(0, op.getNumCalls(MockOperator::kFeedInputBlock));
  EXPECT_EQ(0, op.getNumCalls(MockOperator::kFeedInputBlocks));

  // One workorder is generated.
  EXPECT_EQ(1, getWorkerInputQueueSize());
  EXPECT_EQ(1, op.getNumWorkOrders());

  // Worker receives a WorkOrder.
  MockWorkOrder *wu_op;
  ASSERT_TRUE(popWorkOrderIfAvailable(&wu_op));
  EXPECT_EQ(id, wu_op->getOpIndex());

  // Passing all NULL parameters to a MockWorkOrder for testing purposes.
  // Note that this is not OK for real WorkOrders.
  wu_op->execute(nullptr, nullptr, nullptr);
  delete wu_op;

  EXPECT_EQ(1, getNumWorkOrdersInExecution(id));
  EXPECT_EQ(0, getNumOperatorsFinished());

  // Send a message to Foreman upon workorder completion.
  // Last event processed by Foreman.
  EXPECT_TRUE(placeWorkOrderCompleteMessage(id));

  EXPECT_EQ(0, getNumWorkOrdersInExecution(id));
  EXPECT_EQ(1, getNumOperatorsFinished());
  EXPECT_TRUE(getOperatorFinishedStatus(id));
}

TEST_F(ForemanTest, SingleNodeDAGDynamicWorkOrdersTest) {
  // This test creates a DAG of a single node. WorkOrders are generated
  // dynamically as pending work orders complete execution, i.e.,
  // getAllWorkOrders() is called multiple times.  getAllWorkOrders() will be
  // called 5 times and 3 work orders will be returned, i.e., 1st 3 calls to
  // getAllWorkOrders() insert 1 WU and return false, and the next will insert no
  // WU and return true.

  // TODO(shoban): This test can not be more robust than this because of fixed
  // scaffolding of mocking. If we use gMock, we can do much better.
  const QueryPlan::DAGNodeIndex id = query_plan_->addRelationalOperator(new MockOperator(true, false, 4, 3));
  foreman_->setQueryPlan(query_plan_->getQueryPlanDAGMutable());

  const MockOperator &op = static_cast<const MockOperator&>(query_plan_->getQueryPlanDAG().getNodePayload(id));

  EXPECT_EQ(0, getWorkerInputQueueSize());
  EXPECT_FALSE(startForeman());

  // op doesn't have any dependencies.
  EXPECT_TRUE(op.getBlockingDependenciesMet());

  for (int i = 0; i < 3; i++) {
    // We expect one call for op's getAllWorkOrders().
    EXPECT_EQ(i + 1, op.getNumCalls(MockOperator::kGetAllWorkOrders));

    // One workorder is generated.
    EXPECT_EQ(1, getWorkerInputQueueSize());
    EXPECT_EQ(i + 1, op.getNumWorkOrders());

    // Worker receives a WorkOrder.
    MockWorkOrder *wu_op;
    ASSERT_TRUE(popWorkOrderIfAvailable(&wu_op));
    EXPECT_EQ(id, wu_op->getOpIndex());

    // Passing all NULL parameters to a MockWorkOrder for testing purposes.
    // Note that this is not OK for real WorkOrders.
    wu_op->execute(nullptr, nullptr, nullptr);
    delete wu_op;

    EXPECT_EQ(1, getNumWorkOrdersInExecution(id));
    EXPECT_EQ(0, getNumOperatorsFinished());

    if (i < 2) {
      // Send a message to Foreman upon workorder completion.
      EXPECT_FALSE(placeWorkOrderCompleteMessage(id));
    } else {
      // Send a message to Foreman upon workorder completion.
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

TEST_F(ForemanTest, TwoNodesDAGBlockingLinkTest) {
  // We use two nodes in the DAG with a blocking link between them.
  // There is no streaming of data involved in this test.
  const QueryPlan::DAGNodeIndex id1 = query_plan_->addRelationalOperator(new MockOperator(true, false));
  const QueryPlan::DAGNodeIndex id2 = query_plan_->addRelationalOperator(new MockOperator(true, false));

  // Create a blocking link.
  query_plan_->addDirectDependency(id2, id1, true);

  static_cast<MockOperator*>(query_plan_->getQueryPlanDAGMutable()->getNodePayloadMutable(id1))
      ->setOutputRelationID(0xdead);

  const MockOperator &op1 = static_cast<const MockOperator&>(query_plan_->getQueryPlanDAG().getNodePayload(id1));
  const MockOperator &op2 = static_cast<const MockOperator&>(query_plan_->getQueryPlanDAG().getNodePayload(id2));

  foreman_->setQueryPlan(query_plan_->getQueryPlanDAGMutable());

  // Make sure queues are empty initially.
  EXPECT_EQ(0, getWorkerInputQueueSize());

  EXPECT_FALSE(startForeman());

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
  EXPECT_EQ(1, getWorkerInputQueueSize());
  EXPECT_EQ(1, op1.getNumWorkOrders());
  EXPECT_EQ(0, op2.getNumWorkOrders());

  // Worker receives a WorkOrder.
  MockWorkOrder *wu_op1;
  ASSERT_TRUE(popWorkOrderIfAvailable(&wu_op1));
  // This workorder's source should be op1.
  EXPECT_EQ(id1, wu_op1->getOpIndex());

  // Passing all NULL parameters to a MockWorkOrder for testing purposes.
  // Note that this is not OK for real WorkOrders.
  wu_op1->execute(nullptr, nullptr, nullptr);
  delete wu_op1;

  EXPECT_EQ(0, getWorkerInputQueueSize());
  // Foreman hasn't yet got workorder completion response for the workorder.
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
  EXPECT_EQ(1, getNumWorkOrdersInExecution(id2));

  // op1 is op2's blocking dependency.
  EXPECT_TRUE(op2.getBlockingDependenciesMet());

  EXPECT_EQ(1, op1.getNumCalls(MockOperator::kGetAllWorkOrders));
  // op2 should get first call of getAllWorkOrders() when op1 is over.
  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kGetAllWorkOrders));

  EXPECT_EQ(1, op2.getNumWorkOrders());

  // Send a message to Foreman upon workorder (generated by op2) completion.

  // Note that the worker hasn't yet popped the workorder. Usually this won't
  // happen as workers pop workorders first, execute and then send the response.
  EXPECT_TRUE(placeWorkOrderCompleteMessage(id2));

  // WorkOrder yet to be popped by the worker.
  EXPECT_EQ(1, getWorkerInputQueueSize());

  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kGetAllWorkOrders));

  EXPECT_EQ(0, getNumWorkOrdersInExecution(id1));
  EXPECT_EQ(0, getNumWorkOrdersInExecution(id2));

  EXPECT_EQ(2, getNumOperatorsFinished());
  EXPECT_TRUE(getOperatorFinishedStatus(id1));
  EXPECT_TRUE(getOperatorFinishedStatus(id2));

  MockWorkOrder *wu_op2;
  ASSERT_TRUE(popWorkOrderIfAvailable(&wu_op2));
  // The workorder should have come from op2.
  EXPECT_EQ(id2, wu_op2->getOpIndex());

  // Passing all NULL parameters to a MockWorkOrder for testing purposes.
  // Note that this is not OK for real WorkOrders.
  wu_op2->execute(nullptr, nullptr, nullptr);
  delete wu_op2;

  EXPECT_EQ(0, getWorkerInputQueueSize());

  // Expect no additional calls to getAllWorkOrders.
  EXPECT_EQ(1, op1.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kGetAllWorkOrders));

  EXPECT_EQ(0, getWorkerInputQueueSize());
}

TEST_F(ForemanTest, TwoNodesDAGPipeLinkTest) {
  // We use two nodes in the DAG with a non-blocking link between them.
  // We stream output of op1 to op2. Sequeuce of events is as follows:
  // 1. op1 creates a workorder.
  // 2. We send a "block full" (from op1) to Foreman.
  // 3. op2 creates a workorder because of step 2.
  const QueryPlan::DAGNodeIndex id1 = query_plan_->addRelationalOperator(new MockOperator(true, false, 1));
  const QueryPlan::DAGNodeIndex id2 = query_plan_->addRelationalOperator(new MockOperator(true, true, 3));

  // Create a non-blocking link.
  query_plan_->addDirectDependency(id2, id1, false);

  static_cast<MockOperator*>(query_plan_->getQueryPlanDAGMutable()->getNodePayloadMutable(id1))
      ->setOutputRelationID(0xdead);

  const MockOperator &op1 = static_cast<const MockOperator&>(query_plan_->getQueryPlanDAG().getNodePayload(id1));
  const MockOperator &op2 = static_cast<const MockOperator&>(query_plan_->getQueryPlanDAG().getNodePayload(id2));

  foreman_->setQueryPlan(query_plan_->getQueryPlanDAGMutable());

  // Make sure queues are empty initially.
  EXPECT_EQ(0, getWorkerInputQueueSize());

  startForeman();

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

  // There should be one workorder sent to the worker so far.
  EXPECT_EQ(1, getWorkerInputQueueSize());

  // Worker receives a WorkOrder.
  MockWorkOrder *wu_op1;
  ASSERT_TRUE(popWorkOrderIfAvailable(&wu_op1));
  // This workorder's source be op1.
  EXPECT_EQ(id1, wu_op1->getOpIndex());

  // Passing all NULL parameters to a MockWorkOrder for testing purposes.
  // Note that this is not OK for real WorkOrders.
  wu_op1->execute(nullptr, nullptr, nullptr);
  delete wu_op1;

  // Send a message to Foreman upon block getting full (output of op1).
  EXPECT_FALSE(placeOutputBlockMessage(id1));

  // op1 is not finished yet because the response of workorder completion hasn't
  // been received yet by the Foreman.
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
  EXPECT_EQ(1, getWorkerInputQueueSize());

  // Pop a workorder from Foreman's output queue.
  MockWorkOrder *wu_op2;
  ASSERT_TRUE(popWorkOrderIfAvailable(&wu_op2));
  // The workorder should have been generated by op2.
  EXPECT_EQ(id2, wu_op2->getOpIndex());
  wu_op2->execute(nullptr, nullptr, nullptr);
  delete wu_op2;

  // Place a message of a workorder completion of op2 on Foreman's input queue.
  EXPECT_FALSE(placeWorkOrderCompleteMessage(id2));

  EXPECT_TRUE(getOperatorFinishedStatus(id1));

  EXPECT_EQ(1, getNumWorkOrdersInExecution(id2));
  EXPECT_FALSE(getOperatorFinishedStatus(id2));

  EXPECT_EQ(1, getWorkerInputQueueSize());

  // Pop a workorder from Foreman's output queue.
  MockWorkOrder *wu1_op2;
  ASSERT_TRUE(popWorkOrderIfAvailable(&wu1_op2));
  // The workorder should have been generated by op2.
  EXPECT_EQ(id2, wu1_op2->getOpIndex());

  // Passing all NULL parameters to a MockWorkOrder for testing purposes.
  // Note that this is not OK for real WorkOrders.
  wu1_op2->execute(nullptr, nullptr, nullptr);
  delete wu1_op2;

  // Send a message to Foreman upon workorder (generated by op2) completion.
  EXPECT_TRUE(placeWorkOrderCompleteMessage(id2));

  EXPECT_TRUE(getOperatorFinishedStatus(id1));
  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kDoneFeedingInputBlocks));

  EXPECT_EQ(0, getNumWorkOrdersInExecution(id2));
  EXPECT_TRUE(getOperatorFinishedStatus(id2));

  EXPECT_EQ(0, getWorkerInputQueueSize());
}

TEST_F(ForemanTest, TwoNodesDAGPartiallyFilledBlocksTest) {
  // In this test, we create a 2-node DAG with a non-blocking link between them.
  // There is no streaming of data from op1 to op2 during the execution of op1.
  // op1 produces a partially filled block at the end of its execution which is
  // rebuilt and then fed to op2.
  const QueryPlan::DAGNodeIndex id1 = query_plan_->addRelationalOperator(new MockOperator(true, false, 1));
  const QueryPlan::DAGNodeIndex id2 = query_plan_->addRelationalOperator(new MockOperator(true, true, 3, 1));

  // Create a non-blocking link.
  query_plan_->addDirectDependency(id2, id1, false);

  unique_ptr<CatalogDatabase> db(new CatalogDatabase(nullptr /* catalog */, "database"));

  // Create a relation, owned by db.
  CatalogRelation *relation = new CatalogRelation(nullptr /* catalog_database */, "test_relation");
  const relation_id output_relation_id = db->addRelation(relation);

  // Setup the InsertDestination proto in the query context proto.
  serialization::QueryContext query_context_proto;

  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto.insert_destinations_size();
  serialization::InsertDestination *insert_destination_proto = query_context_proto.add_insert_destinations();

  insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
  insert_destination_proto->set_relation_id(output_relation_id);
  insert_destination_proto->set_need_to_add_blocks_from_relation(false);
  insert_destination_proto->set_relational_op_index(id1);
  insert_destination_proto->set_foreman_client_id(foreman_->getBusClientID());

  MockOperator *op1_mutable =
      static_cast<MockOperator*>(query_plan_->getQueryPlanDAGMutable()->getNodePayloadMutable(id1));
  op1_mutable->setInsertDestinationID(insert_destination_index);
  op1_mutable->setOutputRelationID(output_relation_id);

  const MockOperator &op1 = static_cast<const MockOperator&>(query_plan_->getQueryPlanDAG().getNodePayload(id1));
  const MockOperator &op2 = static_cast<const MockOperator&>(query_plan_->getQueryPlanDAG().getNodePayload(id2));

  // Set up the QueryContext.
  unique_ptr<StorageManager> storage_manager(new StorageManager("./"));
  unique_ptr<QueryContext> query_context(
      new QueryContext(query_context_proto, db.get(), storage_manager.get(), &bus_));

  // NOTE(zuyu): An operator generally has no ideas about partially filled blocks, but Foreman does.
  // Mock to add partially filled blocks in the InsertDestination.
  InsertDestination *insert_destination = query_context->getInsertDestination(insert_destination_index);
  DCHECK(insert_destination != nullptr);
  MutableBlockReference block_ref;
  static_cast<BlockPoolInsertDestination*>(insert_destination)->available_block_refs_.push_back(move(block_ref));

  foreman_->setQueryPlan(query_plan_->getQueryPlanDAGMutable());
  foreman_->setQueryContext(query_context.get());

  // Make sure queues are empty initially.
  EXPECT_EQ(0, getWorkerInputQueueSize());

  startForeman();

  // There's no blocking dependency in the DAG.
  EXPECT_TRUE(op1.getBlockingDependenciesMet());
  EXPECT_TRUE(op2.getBlockingDependenciesMet());

  EXPECT_EQ(1, op1.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(1, op1.getNumWorkOrders());

  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(0, op2.getNumWorkOrders());

  // Worker receives a WorkOrder.
  MockWorkOrder *wu_op1;
  ASSERT_TRUE(popWorkOrderIfAvailable(&wu_op1));
  // The workorder should have been generated by op1.
  EXPECT_EQ(id1, wu_op1->getOpIndex());

  // Passing all NULL parameters to a MockWorkOrder for testing purposes.
  // Note that this is not OK for real WorkOrders.
  wu_op1->execute(nullptr, nullptr, nullptr);
  delete wu_op1;

  EXPECT_EQ(0, getWorkerInputQueueSize());

  // Send a message to Foreman upon workorder (generated by op1) completion.
  EXPECT_FALSE(placeWorkOrderCompleteMessage(id1));

  EXPECT_EQ(0, getNumWorkOrdersInExecution(id1));

  // op1 generates a rebuild workorder. The block is rebuilt and streamed
  // to Foreman.
  EXPECT_FALSE(placeDataPipelineMessage(id1));

  // Based on the streamed input, op2's getAllWorkOrders should produce a
  // workorder.
  EXPECT_EQ(3, op2.getNumCalls(MockOperator::kGetAllWorkOrders));
  EXPECT_EQ(1, op2.getNumWorkOrders());

  // Worker receives a rebuild WorkOrder.
  MockWorkOrder *rebuild_op1;
  ASSERT_TRUE(popRebuildWorkOrderIfAvailable(&rebuild_op1));
  // We skip the check for relation ID of the rebuild WorkOrder, as the partially
  // filled block reference is a mock reference with garbage contents.
  delete rebuild_op1;

  EXPECT_FALSE(placeRebuildWorkOrderCompleteMessage(id1));

  EXPECT_TRUE(getOperatorFinishedStatus(id1));
  EXPECT_EQ(1, op2.getNumCalls(MockOperator::kDoneFeedingInputBlocks));
  EXPECT_FALSE(getOperatorFinishedStatus(id2));
  EXPECT_EQ(1, getNumWorkOrdersInExecution(id2));

  EXPECT_EQ(1, getWorkerInputQueueSize());

  // Worker receives a WorkOrder.
  MockWorkOrder *wu_op2;
  ASSERT_TRUE(popWorkOrderIfAvailable(&wu_op2));
  // The workorder should have been generated by op2.
  EXPECT_EQ(id2, wu_op2->getOpIndex());

  // Passing all NULL parameters to a MockWorkOrder for testing purposes.
  // Note that this is not OK for real WorkOrders.
  wu_op2->execute(nullptr, nullptr, nullptr);
  delete wu_op2;

  EXPECT_EQ(0, getWorkerInputQueueSize());

  // Send a message to Foreman upon workorder (generated by op2) completion.
  EXPECT_TRUE(placeWorkOrderCompleteMessage(id2));

  EXPECT_EQ(0, getNumWorkOrdersInExecution(id2));

  EXPECT_TRUE(getOperatorFinishedStatus(id2));

  EXPECT_EQ(0, getWorkerInputQueueSize());
}

TEST_F(ForemanTest, MultipleNodesNoOutputTest) {
  // When an operator produces workorders but no output, the Foreman should
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

  foreman_->setQueryPlan(query_plan_->getQueryPlanDAGMutable());

  // Make sure queues are empty initially.
  EXPECT_EQ(0, getWorkerInputQueueSize());

  startForeman();

  // operators[0] should have produced a workorder by now.
  EXPECT_EQ(1, operators[0]->getNumWorkOrders());
  EXPECT_EQ(1, getWorkerInputQueueSize());

  EXPECT_EQ(1, getNumWorkOrdersInExecution(ids[0]));
  EXPECT_FALSE(getOperatorFinishedStatus(ids[0]));

  for (QueryPlan::DAGNodeIndex i = 0; i < kNumNodes; ++i) {
    EXPECT_EQ(1, operators[ids[i]]->getNumCalls(MockOperator::kGetAllWorkOrders));
  }

  // Worker receives a WorkOrder.
  MockWorkOrder *wu_op1;
  ASSERT_TRUE(popWorkOrderIfAvailable(&wu_op1));
  // The workorder should have been generated by operators[0].
  EXPECT_EQ(ids[0], wu_op1->getOpIndex());

  // Passing all NULL parameters to a MockWorkOrder for testing purposes.
  // Note that this is not OK for real WorkOrders.
  wu_op1->execute(nullptr, nullptr, nullptr);
  delete wu_op1;

  // Send a message to Foreman upon workorder (generated by operators[0])
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

TEST_F(ForemanTest, OutOfOrderWorkOrderCompletionTest) {
  // Consider two operators, both generate one workorder each. The dependent's
  // workorder finishes before dependency's workorder.
  const QueryPlan::DAGNodeIndex id1 = query_plan_->addRelationalOperator(new MockOperator(true, false, 1));
  const QueryPlan::DAGNodeIndex id2 = query_plan_->addRelationalOperator(new MockOperator(true, true, 2, 1));

  // Create a non-blocking link.
  query_plan_->addDirectDependency(id2, id1, false);

  foreman_->setQueryPlan(query_plan_->getQueryPlanDAGMutable());
  // There should be two workorders on Worker's private queue, for this test.
  foreman_->setMaxMessagesPerWorker(2);

  // Make sure queues are empty initially.
  EXPECT_EQ(0, getWorkerInputQueueSize());

  startForeman();

  // Expect one workorder produced by op1.
  EXPECT_EQ(1, getWorkerInputQueueSize());

  // Pop a workorder from Foreman's output queue.
  MockWorkOrder *wu_op1;
  ASSERT_TRUE(popWorkOrderIfAvailable(&wu_op1));
  // This workorder's source be op1.
  EXPECT_EQ(id1, wu_op1->getOpIndex());

  // Passing all NULL parameters to a MockWorkOrder for testing purposes.
  // Note that this is not OK for real WorkOrders.
  wu_op1->execute(nullptr, nullptr, nullptr);
  delete wu_op1;

  // Send a message to Foreman upon a block (output of op1) getting full.
  EXPECT_FALSE(placeOutputBlockMessage(id1));

  // op1 is not finished yet because the response of workorder completion hasn't
  // been received yet.
  EXPECT_FALSE(getOperatorFinishedStatus(id1));
  EXPECT_FALSE(getOperatorFinishedStatus(id2));

  // Expect one workorder produced by op2.
  EXPECT_EQ(1, getWorkerInputQueueSize());

  // Worker receives a WorkOrder.
  MockWorkOrder *wu_op2;
  ASSERT_TRUE(popWorkOrderIfAvailable(&wu_op2));
  // This workorder's source should be op2.
  EXPECT_EQ(id2, wu_op2->getOpIndex());

  // Passing all NULL parameters to a MockWorkOrder for testing purposes.
  // Note that this is not OK for real WorkOrders.
  wu_op2->execute(nullptr, nullptr, nullptr);
  delete wu_op2;

  // As mentioned earlier, op2 finishes before op1.
  EXPECT_FALSE(placeWorkOrderCompleteMessage(id2));

  EXPECT_EQ(0, getWorkerInputQueueSize());

  // op1's workorder execution is over.
  EXPECT_TRUE(placeWorkOrderCompleteMessage(id1));

  EXPECT_TRUE(getOperatorFinishedStatus(id1));
  EXPECT_TRUE(getOperatorFinishedStatus(id2));

  EXPECT_EQ(0, getWorkerInputQueueSize());
}

}  // namespace quickstep
