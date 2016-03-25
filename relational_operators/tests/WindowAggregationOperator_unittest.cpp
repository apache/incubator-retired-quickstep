/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <functional>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregateFunctionFactory.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/scalar/ScalarBinaryExpression.hpp"
#include "expressions/scalar/ScalarLiteral.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WindowAggregationOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/InsertDestination.pb.h"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "types/DoubleType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/NumericSuperType.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "tmb/id_typedefs.h"
#include "tmb/tagged_message.h"

using std::unique_ptr;

namespace quickstep {

namespace {
constexpr int kOpIndex = 0;
}  // namespace

class Type;

class WindowAggregationOperatorTest : public ::testing::Test {
 public:
  static const std::int32_t kNumWindows = 10;
  static const std::int32_t kWindowDuration = 10;
  static const std::int32_t kWindowRange = kWindowDuration * kNumWindows;
  static const std::int32_t kAgeDuration = 10000;

 protected:
  static const char kStoragePath[];
  static const char kDatabaseName[];
  static const char kTableName[];
  static const relation_id kTableId = 100;
  static const relation_id kResultTableId = kTableId + 1;
  static const tuple_id kNumTuplesPerBlock = 100;
  static const tuple_id kNumBlocks = 5;

  virtual void SetUp() {
    bus_.Initialize();
    client_id_ = bus_.Connect();
    bus_.RegisterClientAsSender(client_id_, kDataPipelineMessage);
    bus_.RegisterClientAsReceiver(client_id_, kDataPipelineMessage);
    thread_id_map_ = ClientIDMap::Instance();
    // Usually the worker thread makes the following call. In this test setup,
    // we don't have a worker thread hence we have to explicitly make the call.
    thread_id_map_->addValue(client_id_);

    storage_manager_.reset(new StorageManager(kStoragePath));

    // Create a database.
    db_.reset(new CatalogDatabase(nullptr, kDatabaseName));

    // Create a table, owned by db_.
    table_ = new CatalogRelation(nullptr, kTableName, kTableId);
    db_->addRelation(table_);

    // Add attributes.
    const Type &long_type = LongType::InstanceNonNullable();
    const Type &int_type = IntType::InstanceNonNullable();
    table_->addAttribute(new CatalogAttribute(table_, "Window", int_type));
    table_->addAttribute(new CatalogAttribute(table_, "IntType-0", int_type));
    table_->addAttribute(new CatalogAttribute(table_, "IntType-1", int_type));
    table_->addAttribute(new CatalogAttribute(table_, "LongType-0", long_type));
    table_->addAttribute(new CatalogAttribute(table_, "LongType-1", long_type));

    std::unique_ptr<StorageBlockLayout> layout(
        StorageBlockLayout::GenerateDefaultLayout(*table_, table_->isVariableLength()));

    // Insert tuples to table.
    std::unique_ptr<Tuple> tuple;
    MutableBlockReference storage_block;
    for (tuple_id i = 0; i < kNumBlocks; ++i) {
      // Create block
      block_id block_id = storage_manager_->createBlock(*table_, *layout);
      storage_block = storage_manager_->getBlockMutable(block_id, *table_);
      table_->addBlock(block_id);

      // Insert tuples
      for (tuple_id tid = i * kNumTuplesPerBlock;
           tid < (i + 1) * kNumTuplesPerBlock;
           ++tid) {
        tuple.reset(createTuple(*table_, tid));
        EXPECT_TRUE(storage_block->insertTupleInBatch(*tuple));
      }
      storage_block->rebuild();
    }
  }

  virtual void TearDown() {
    // Usually the worker thread makes the following call. In this test setup,
    // we don't have a worker thread hence we have to explicitly make the call.
    thread_id_map_->removeValue();
  }

  Tuple* createTuple(const CatalogRelation &relation, const std::int64_t val) {
    std::vector<TypedValue> attributes;
    std::int32_t window_id = val % kNumWindows;
    attributes.push_back(TypedValue(static_cast<IntType::cpptype>(
        (window_id * kWindowDuration) + (val % kWindowDuration))));
    attributes.push_back(TypedValue(static_cast<IntType::cpptype>(val)));
    attributes.push_back(TypedValue(static_cast<IntType::cpptype>(val)));
    attributes.push_back(TypedValue(static_cast<LongType::cpptype>(val)));
    attributes.push_back(TypedValue(static_cast<LongType::cpptype>(val)));
    return new Tuple(std::move(attributes));
  }

  void setupTest(const std::string &stem,
                 const AggregationID agg_type,
                 const Type &result_type) {
    // Setup results table, owned by db_.
    const Type &int_type = IntType::InstanceNonNullable();
    result_table_ = new CatalogRelation(nullptr, "result_table", kResultTableId);
    db_->addRelation(result_table_);

    std::vector<const AggregateFunction*> agg_functions;
    agg_functions.emplace_back(&AggregateFunctionFactory::Get(agg_type));
    agg_functions.emplace_back(&AggregateFunctionFactory::Get(agg_type));

    std::vector<std::vector<std::unique_ptr<const Scalar>>> agg_arguments(2);
    agg_arguments[0].emplace_back(
        new ScalarAttribute(*table_->getAttributeByName(stem + "-0")));
    agg_arguments[1].emplace_back(new ScalarBinaryExpression(
        BinaryOperationFactory::GetBinaryOperation(
            BinaryOperationID::kMultiply),
        new ScalarAttribute(*table_->getAttributeByName(stem + "-1")),
        new ScalarLiteral(TypedValue(static_cast<int>(1)),
                          IntType::InstanceNonNullable())));

    result_table_->addAttribute(
        new CatalogAttribute(result_table_, "Window", int_type));
    result_table_->addAttribute(
        new CatalogAttribute(result_table_, "result-0", result_type));
    result_table_->addAttribute(
        new CatalogAttribute(result_table_, "result-1", result_type));

    const relation_id output_relation_id = result_table_->getID();

    serialization::QueryContext query_context_proto;

    // Setup the InsertDestination proto in the query context proto.
    const QueryContext::insert_destination_id insert_destination_index =
        query_context_proto.insert_destinations_size();
    serialization::InsertDestination *insert_destination_proto =
        query_context_proto.add_insert_destinations();

    insert_destination_proto->set_insert_destination_type(
        serialization::InsertDestinationType::BLOCK_POOL);
    insert_destination_proto->set_relation_id(output_relation_id);
    insert_destination_proto->set_relational_op_index(kOpIndex);

    // Set up the QueryContext.
   query_context_.reset(new QueryContext(
        query_context_proto, db_.get(), storage_manager_.get(), client_id_, &bus_));

    // Create Operators.
    std::vector<std::unique_ptr<const Scalar>> grouping;
    op_.reset(
        new WindowAggregationOperator(*table_,
                                      false /* relation is stored. */,
                                      agg_functions,
                                      std::move(agg_arguments),
                                      std::move(grouping),
                                      *table_->getAttributeByName("Window"),
                                      TypedValue(kWindowDuration),
                                      kAgeDuration,
                                      insert_destination_index,
                                      output_relation_id,
                                      serialization::HashTableImplType::LINEAR_OPEN_ADDRESSING,
                                      storage_manager_.get()));

    // Note: We treat these two operators as different query plan DAGs. The
    // index for each operator should be set, so that the WorkOrdersContainer
    // class' checks about operator index are successful.
    op_->setOperatorIndex(kOpIndex);
  }

  void execute() {

   const std::size_t op_index = 0;

 WorkOrdersContainer op_container(1, 0);
    op_->getAllWorkOrders(&op_container, query_context_.get(), storage_manager_.get(),
    		client_id_ /* foreman_client_id */,
            nullptr /* TMB */);

 
    while (op_container.hasNormalWorkOrder(op_index)) {
      WorkOrder *wu = op_container.getNormalWorkOrder(op_index);
      wu->execute();
      delete wu;
    }

  }



  void checkResult(std::function<
      void(int, const TypedValue &, const TypedValue &, size_t)> check_fn) {
    InsertDestination *insert_destination =
        query_context_->getInsertDestination(op_->getInsertDestinationID());
    DCHECK(insert_destination);
    std::vector<std::size_t> num_block_per_window(kNumWindows, 1);

    const std::vector<block_id> result =
        insert_destination->getTouchedBlocks();
    std::int32_t total_tuples = 0;
    for (size_t bid = 0; bid < result.size(); ++bid) {
      BlockReference block = storage_manager_->getBlock(
          result[bid], insert_destination->getRelation());
      const TupleStorageSubBlock &sub_block = block->getTupleStorageSubBlock();
      ASSERT_TRUE(sub_block.isPacked());
      for (tuple_id tid = 0; tid < sub_block.numTuples(); ++tid) {
        const TypedValue window_col = sub_block.getAttributeValueTyped(
            tid, result_table_->getAttributeByName("Window")->getID());
        const TypedValue actual0 = sub_block.getAttributeValueTyped(
            tid, result_table_->getAttributeByName("result-0")->getID());
        const TypedValue actual1 = sub_block.getAttributeValueTyped(
            tid, result_table_->getAttributeByName("result-1")->getID());
        int window = window_col.getLiteral<IntType::cpptype>() / kWindowDuration;
        check_fn(
            window,
            actual0,
            actual1,
            num_block_per_window[window] * kNumTuplesPerBlock / kNumWindows);
#if 0
        LOG(INFO) << ">>>> " << window_col.getLiteral<IntType::cpptype>() << ' '
                  << actual0.getLiteral<LongType::cpptype>() << ' '
                  << actual1.getLiteral<LongType::cpptype>();
#endif
        ++num_block_per_window[window];
      }
      total_tuples += sub_block.numTuples();
    }
    EXPECT_EQ((std::int32_t)kNumWindows * kNumBlocks, (std::int32_t)total_tuples);
  }

  template <class FinalDataType>
  void testWindowAggregationOperator(
      const std::string &stem,
      const AggregationID agg_type,
      std::function<void(int, const TypedValue &, const TypedValue &, size_t)>
          check_fn) {
    setupTest(stem, agg_type, FinalDataType::InstanceNullable());
    execute();
    checkResult(check_fn);
  }

  MessageBusImpl bus_;
  ClientIDMap *thread_id_map_;
  tmb::client_id client_id_;
  std::unique_ptr<QueryContext> query_context_;
  std::unique_ptr<StorageManager> storage_manager_;

  std::unique_ptr<CatalogDatabase> db_;
  // The following two CatalogRelations are owned by db_.
  CatalogRelation *table_, *result_table_;

  std::unique_ptr<WindowAggregationOperator> op_;
};

const char WindowAggregationOperatorTest::kDatabaseName[] = "database";
const char WindowAggregationOperatorTest::kTableName[] = "table";
const char WindowAggregationOperatorTest::kStoragePath[] = "./test_data";

namespace {

// Computes the sum of arthemetic series: a, a + d, a + 2*d, ..., a + (n-1)*d
std::int64_t ArithemeticSum(int a, int d, int n) {
  return n * (2 * a + (n - 1) * d) / 2;
}

template <class T>
void CheckLiteral(T expected, TypedValue actual) {
  EXPECT_EQ(expected, actual.getLiteral<T>());
}

template <class T>
void Window_SumCheckIntegral(int window_id,
                             const TypedValue &value1,
                             const TypedValue &value2,
                             std::size_t num_repeats) {
  std::int64_t sum =
      ArithemeticSum(window_id,
                     WindowAggregationOperatorTest::kNumWindows,
                     num_repeats);
  EXPECT_EQ(sum, value1.getLiteral<T>());
  EXPECT_EQ(sum, value2.getLiteral<T>());
}

}  // namespace

TEST_F(WindowAggregationOperatorTest, Sum_IntType) {
  testWindowAggregationOperator<LongType>(
      "IntType",
      AggregationID::kSum,
      Window_SumCheckIntegral<LongType::cpptype>);
}

TEST_F(WindowAggregationOperatorTest, Sum_LongType) {
  testWindowAggregationOperator<LongType>(
      "LongType",
      AggregationID::kSum,
      Window_SumCheckIntegral<LongType::cpptype>);
}

namespace {

void Window_AvgCheckIntegral(int window_id,
                             const TypedValue &value1,
                             const TypedValue &value2,
                             std::size_t num_repeats) {
  double avg =
      ArithemeticSum(
          window_id, WindowAggregationOperatorTest::kNumWindows, num_repeats) /
      static_cast<double>(num_repeats);
  EXPECT_NEAR(avg, value1.getLiteral<DoubleType::cpptype>(), 1e-5 * avg);
  EXPECT_NEAR(avg, value2.getLiteral<DoubleType::cpptype>(), 1e-5 * avg);
}

}  // namespace

TEST_F(WindowAggregationOperatorTest, Avg_IntType) {
  testWindowAggregationOperator<DoubleType>(
      "IntType",
      AggregationID::kAvg,
      Window_AvgCheckIntegral);
}

TEST_F(WindowAggregationOperatorTest, Avg_LongType) {
  testWindowAggregationOperator<DoubleType>(
      "LongType",
      AggregationID::kAvg,
      Window_AvgCheckIntegral);
}

}  // namespace quickstep
