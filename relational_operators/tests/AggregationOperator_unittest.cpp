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

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregateFunction.pb.h"
#include "expressions/aggregation/AggregateFunctionFactory.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/scalar/ScalarBinaryExpression.hpp"
#include "expressions/scalar/ScalarLiteral.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/AggregationOperator.hpp"
#include "relational_operators/FinalizeAggregationOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/AggregationOperationState.pb.h"
#include "storage/HashTable.pb.h"
#include "storage/InsertDestination.hpp"
#include "storage/InsertDestination.pb.h"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/PtrList.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

#include "tmb/id_typedefs.h"

using std::unique_ptr;

namespace quickstep {

namespace {
constexpr int kOpIndex = 0;
}  // namespace

class Type;

class AggregationOperatorTest : public ::testing::Test {
 public:
  static const tuple_id kNumTuples = 300;
  static const int kGroupByWidth = 20;
  static const int kGroupByRepeats = kNumTuples / kGroupByWidth;
  static const int kGroupBy1Size = 4;
  static const int kGroupBy2Size = kGroupByWidth / kGroupBy1Size;

 protected:
  static const char kStoragePath[];
  static const char kDatabaseName[];
  static const char kTableName[];
  static const relation_id kTableId = 100;
  static const relation_id kResultTableId = kTableId + 1;
  static const int kTuplesOffset = 20;
  static const tuple_id kNumTuplesPerBlock = 10;

  // Constants to help in readablity of test instantiations.
  static const bool kExpression = true;
  static const bool kAttribute = false;
  static const bool kWithPredicate = true;
  static const bool kWithoutPredicate = false;
  static const int kPlaceholder = 0xbeef;

  virtual void SetUp() {
    thread_id_map_ = ClientIDMap::Instance();

    bus_.Initialize();

    const tmb::client_id worker_thread_client_id = bus_.Connect();
    bus_.RegisterClientAsSender(worker_thread_client_id, kCatalogRelationNewBlockMessage);

    // Usually the worker thread makes the following call. In this test setup,
    // we don't have a worker thread hence we have to explicitly make the call.
    thread_id_map_->addValue(worker_thread_client_id);

    foreman_client_id_ = bus_.Connect();
    bus_.RegisterClientAsReceiver(foreman_client_id_, kCatalogRelationNewBlockMessage);

    storage_manager_.reset(new StorageManager(kStoragePath));

    // Create a database.
    db_.reset(new CatalogDatabase(nullptr, kDatabaseName));

    // Create a table, owned by db_.
    table_ = new CatalogRelation(nullptr, kTableName, kTableId);
    db_->addRelation(table_);

    // Add attributes.
    const Type &long_type = LongType::InstanceNonNullable();
    const Type &int_type = IntType::InstanceNonNullable();
    const Type &float_type = FloatType::InstanceNonNullable();
    const Type &double_type = DoubleType::InstanceNonNullable();
    table_->addAttribute(new CatalogAttribute(table_, "GroupBy-0", int_type));
    table_->addAttribute(new CatalogAttribute(table_, "GroupBy-1", int_type));
    table_->addAttribute(new CatalogAttribute(table_, "IntType-0", int_type));
    table_->addAttribute(new CatalogAttribute(table_, "IntType-1", int_type));
    table_->addAttribute(new CatalogAttribute(table_, "LongType-0", long_type));
    table_->addAttribute(new CatalogAttribute(table_, "LongType-1", long_type));
    table_->addAttribute(new CatalogAttribute(table_, "FloatType-0", float_type));
    table_->addAttribute(new CatalogAttribute(table_, "FloatType-1", float_type));
    table_->addAttribute(new CatalogAttribute(table_, "DoubleType-0", double_type));
    table_->addAttribute(new CatalogAttribute(table_, "DoubleType-1", double_type));

    std::unique_ptr<StorageBlockLayout> layout(
        StorageBlockLayout::GenerateDefaultLayout(*table_, table_->isVariableLength()));

    // Insert tuples to table.
    std::unique_ptr<Tuple> tuple;
    MutableBlockReference storage_block;
    for (tuple_id i = 0; i < kNumTuples; i += kNumTuplesPerBlock) {
      // Create block
      block_id block_id = storage_manager_->createBlock(*table_, *layout);
      storage_block = storage_manager_->getBlockMutable(block_id, *table_);
      table_->addBlock(block_id);

      // Insert tuples
      tuple_id block_bound = i + kNumTuplesPerBlock < kNumTuples ? i + kNumTuplesPerBlock : kNumTuples;
      for (tuple_id tid = i; tid < block_bound; ++tid) {
        tuple.reset(createTuple(*table_, tid));
        EXPECT_TRUE(storage_block->insertTupleInBatch(*tuple));
      }
      storage_block->rebuild();
    }
  }

  virtual void TearDown() {
    thread_id_map_->removeValue();
  }

  Tuple* createTuple(const CatalogRelation &relation, const std::int64_t val) {
    int group_by_id = val % kGroupByWidth;
    std::vector<TypedValue> attributes;
    attributes.push_back(TypedValue(static_cast<IntType::cpptype>(group_by_id % kGroupBy1Size)));
    attributes.push_back(TypedValue(static_cast<IntType::cpptype>(group_by_id / kGroupBy1Size)));
    attributes.push_back(TypedValue(static_cast<IntType::cpptype>(val)));
    attributes.push_back(TypedValue(static_cast<IntType::cpptype>(val)));
    attributes.push_back(TypedValue(static_cast<LongType::cpptype>(val)));
    attributes.push_back(TypedValue(static_cast<LongType::cpptype>(val)));
    attributes.push_back(TypedValue(static_cast<FloatType::cpptype>(0.1 * val)));
    attributes.push_back(TypedValue(static_cast<FloatType::cpptype>(0.1 * val)));
    attributes.push_back(TypedValue(static_cast<DoubleType::cpptype>(0.1 * val)));
    attributes.push_back(TypedValue(static_cast<DoubleType::cpptype>(0.1 * val)));
    return new Tuple(std::move(attributes));
  }

  PtrList<Scalar>* makeGroupByAttributes() {
    PtrList<Scalar> *attributes = new PtrList<Scalar>();
    attributes->push_back(new ScalarAttribute(*table_->getAttributeByName("GroupBy-0")));
    attributes->push_back(new ScalarAttribute(*table_->getAttributeByName("GroupBy-1")));
    return attributes;
  }

  Predicate* makeLessThanPredicate(int value) {
    Scalar *left = new ScalarAttribute(*table_->getAttributeByName("IntType-0"));
    Scalar *right = new ScalarLiteral(TypedValue(value), IntType::InstanceNonNullable());
    return new ComparisonPredicate(ComparisonFactory::GetComparison(ComparisonID::kLess), left, right);
  }

  void setupTest(const std::string &stem,
                 const AggregationID agg_type,
                 const bool is_expression,
                 const bool with_predicate,
                 const Type &result_type,
                 const int predicate_value) {
    // Setup results table, owned by db_.
    result_table_ = new CatalogRelation(nullptr, "result_table", kResultTableId);
    db_->addRelation(result_table_);

    result_table_->addAttribute(new CatalogAttribute(result_table_, "result-0", result_type));
    result_table_->addAttribute(new CatalogAttribute(result_table_, "result-1", result_type));

    // Setup the aggregation state proto in the query context proto.
    serialization::QueryContext query_context_proto;
    const QueryContext::aggregation_state_id aggr_state_index = query_context_proto.aggregation_states_size();
    serialization::AggregationOperationState *aggr_state_proto = query_context_proto.add_aggregation_states();
    aggr_state_proto->set_relation_id(table_->getID());

    // Add an aggregate.
    serialization::Aggregate *aggr_proto = aggr_state_proto->add_aggregates();
    aggr_proto->mutable_function()->CopyFrom(AggregateFunctionFactory::Get(agg_type).getProto());
    aggr_proto->set_is_distinct(false);
    if (is_expression) {
      unique_ptr<ScalarBinaryExpression> exp(
          new ScalarBinaryExpression(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
                                     new ScalarAttribute(*table_->getAttributeByName(stem + "-0")),
                                     new ScalarAttribute(*table_->getAttributeByName(stem + "-1"))));
      aggr_proto->add_argument()->CopyFrom(exp->getProto());
    } else {
      unique_ptr<ScalarAttribute> attr(new ScalarAttribute(*table_->getAttributeByName(stem + "-0")));
      aggr_proto->add_argument()->CopyFrom(attr->getProto());
    }

    // Add another aggregate.
    aggr_proto = aggr_state_proto->add_aggregates();
    aggr_proto->mutable_function()->CopyFrom(AggregateFunctionFactory::Get(agg_type).getProto());
    aggr_proto->set_is_distinct(false);
    if (is_expression) {
      unique_ptr<ScalarBinaryExpression> exp(
          new ScalarBinaryExpression(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply),
                                     new ScalarAttribute(*table_->getAttributeByName(stem + "-0")),
                                     new ScalarAttribute(*table_->getAttributeByName(stem + "-1"))));
      aggr_proto->add_argument()->CopyFrom(exp->getProto());
    } else {
      unique_ptr<ScalarAttribute> attr(new ScalarAttribute(*table_->getAttributeByName(stem + "-1")));
      aggr_proto->add_argument()->CopyFrom(attr->getProto());
    }

    if (with_predicate) {
      unique_ptr<Predicate> predicate(makeLessThanPredicate(predicate_value));
      aggr_state_proto->mutable_predicate()->CopyFrom(predicate->getProto());
    }

    std::size_t estimated_entries = 0.1*table_->estimateTupleCardinality();
    aggr_state_proto->set_estimated_num_entries(estimated_entries);

    // Create Operators.
    op_.reset(new AggregationOperator(*table_, true, aggr_state_index));

    // Setup the InsertDestination proto in the query context proto.
    const QueryContext::insert_destination_id insert_destination_index =
        query_context_proto.insert_destinations_size();
    serialization::InsertDestination *insert_destination_proto = query_context_proto.add_insert_destinations();

    insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
    insert_destination_proto->set_relation_id(result_table_->getID());
    insert_destination_proto->set_relational_op_index(kOpIndex);

    finalize_op_.reset(
        new FinalizeAggregationOperator(aggr_state_index, *result_table_, insert_destination_index));

    // Set up the QueryContext.
    query_context_.reset(new QueryContext(query_context_proto,
                                          *db_,
                                          storage_manager_.get(),
                                          foreman_client_id_,
                                          &bus_));

    // Note: We treat these two operators as different query plan DAGs. The
    // index for each operator should be set, so that the WorkOrdersContainer
    // class' checks about operator index are successful.
    op_->setOperatorIndex(kOpIndex);
    finalize_op_->setOperatorIndex(kOpIndex);
  }

  void setupTestGroupBy(const std::string &stem,
                        const AggregationID agg_type,
                        const bool with_predicate,
                        const Type &result_type,
                        const int predicate_value) {
    // Setup results table, owned by db_.
    const Type &int_type = IntType::InstanceNonNullable();
    result_table_ = new CatalogRelation(nullptr, "result_table", kResultTableId);
    db_->addRelation(result_table_);

    result_table_->addAttribute(new CatalogAttribute(result_table_, "GroupBy-0", int_type));
    result_table_->addAttribute(new CatalogAttribute(result_table_, "GroupBy-1", int_type));
    result_table_->addAttribute(new CatalogAttribute(result_table_, "result-0", result_type));
    result_table_->addAttribute(new CatalogAttribute(result_table_, "result-1", result_type));

    // Setup the aggregation state proto in the query context proto.
    serialization::QueryContext query_context_proto;
    const QueryContext::aggregation_state_id aggr_state_index = query_context_proto.aggregation_states_size();
    serialization::AggregationOperationState *aggr_state_proto = query_context_proto.add_aggregation_states();
    aggr_state_proto->set_relation_id(table_->getID());

    // Add an aggregate.
    serialization::Aggregate *aggr_proto = aggr_state_proto->add_aggregates();
    aggr_proto->mutable_function()->CopyFrom(AggregateFunctionFactory::Get(agg_type).getProto());
    aggr_proto->set_is_distinct(false);

    unique_ptr<ScalarAttribute> attr(new ScalarAttribute(*table_->getAttributeByName(stem + "-0")));
    aggr_proto->add_argument()->CopyFrom(attr->getProto());

    // Add another aggregate.
    aggr_proto = aggr_state_proto->add_aggregates();
    aggr_proto->mutable_function()->CopyFrom(AggregateFunctionFactory::Get(agg_type).getProto());
    aggr_proto->set_is_distinct(false);
    attr.reset(new ScalarAttribute(*table_->getAttributeByName(stem + "-1")));
    aggr_proto->add_argument()->CopyFrom(attr->getProto());

    if (with_predicate) {
      unique_ptr<Predicate> predicate(makeLessThanPredicate(predicate_value));
      aggr_state_proto->mutable_predicate()->CopyFrom(predicate->getProto());
    }

    unique_ptr<PtrList<Scalar>> group_by_expressions(makeGroupByAttributes());
    for (const Scalar &group_by_expression : *group_by_expressions) {
      aggr_state_proto->add_group_by_expressions()->CopyFrom(group_by_expression.getProto());
    }

    std::size_t estimated_entries = 0.1 * table_->estimateTupleCardinality();
    aggr_state_proto->set_estimated_num_entries(estimated_entries);

    // Also need to set the HashTable implementation for GROUP BY.
    aggr_state_proto->set_hash_table_impl_type(
        serialization::HashTableImplType::LINEAR_OPEN_ADDRESSING);

    // Create Operators.
    op_.reset(new AggregationOperator(*table_, true, aggr_state_index));

    // Setup the InsertDestination proto in the query context proto.
    const QueryContext::insert_destination_id insert_destination_index =
        query_context_proto.insert_destinations_size();
    serialization::InsertDestination *insert_destination_proto = query_context_proto.add_insert_destinations();

    insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
    insert_destination_proto->set_relation_id(result_table_->getID());
    insert_destination_proto->set_relational_op_index(kOpIndex);

    finalize_op_.reset(
        new FinalizeAggregationOperator(aggr_state_index, *result_table_, insert_destination_index));

    // Set up the QueryContext.
    query_context_.reset(new QueryContext(query_context_proto,
                                          *db_,
                                          storage_manager_.get(),
                                          foreman_client_id_,
                                          &bus_));

    // Note: We treat these two operators as different query plan DAGs. The
    // index for each operator should be set, so that the WorkOrdersContainer
    // class' checks about operator index are successful.
    op_->setOperatorIndex(kOpIndex);
    finalize_op_->setOperatorIndex(kOpIndex);
  }

  void execute() {
    const std::size_t op_index = 0;
    WorkOrdersContainer op_container(1, 0);
    op_->getAllWorkOrders(&op_container,
                          query_context_.get(),
                          storage_manager_.get(),
                          foreman_client_id_,
                          &bus_);

    while (op_container.hasNormalWorkOrder(op_index)) {
      WorkOrder *work_order = op_container.getNormalWorkOrder(op_index);
      work_order->execute();
      delete work_order;
    }

    finalize_op_->informAllBlockingDependenciesMet();

    WorkOrdersContainer finalize_op_container(1, 0);
    const std::size_t finalize_op_index = 0;
    finalize_op_->getAllWorkOrders(&finalize_op_container,
                                   query_context_.get(),
                                   storage_manager_.get(),
                                   foreman_client_id_,
                                   &bus_);

    while (finalize_op_container.hasNormalWorkOrder(finalize_op_index)) {
      WorkOrder *work_order = finalize_op_container.getNormalWorkOrder(finalize_op_index);
      work_order->execute();
      delete work_order;
    }
  }

  template <class T>
  void checkResult(T expected0, T expected1, std::function<void(T, TypedValue)> test) {
    DCHECK(query_context_);
    InsertDestination *insert_destination =
        query_context_->getInsertDestination(finalize_op_->getInsertDestinationID());
    DCHECK(insert_destination);

    const std::vector<block_id> result = insert_destination->getTouchedBlocks();
    ASSERT_EQ(1u, result.size());
    BlockReference block = storage_manager_->getBlock(result[0], insert_destination->getRelation());
    const TupleStorageSubBlock &sub_block = block->getTupleStorageSubBlock();
    ASSERT_EQ(1, sub_block.numTuples());
    ASSERT_TRUE(sub_block.hasTupleWithID(0));
    const TypedValue actual0 =
        sub_block.getAttributeValueTyped(0, result_table_->getAttributeByName("result-0")->getID());
    const TypedValue actual1 =
        sub_block.getAttributeValueTyped(0, result_table_->getAttributeByName("result-1")->getID());
    test(expected0, actual0);
    test(expected1, actual1);
  }

  template <class FinalDataType>
  void testAggregationOperator(const std::string &stem,
                               const AggregationID agg_type,
                               const bool is_expression,
                               const bool with_predicate,
                               const typename FinalDataType::cpptype expected0,
                               const typename FinalDataType::cpptype expected1,
                               std::function<void(typename FinalDataType::cpptype, TypedValue)> check_fn,
                               const int predicate_value = 30) {
    setupTest(stem, agg_type, is_expression, with_predicate, FinalDataType::InstanceNullable(), predicate_value);
    execute();
    checkResult<typename FinalDataType::cpptype>(expected0, expected1, check_fn);
  }

  void checkGroupByResult(std::function<void(int, const TypedValue &, const TypedValue &)> check_fn,
                          std::size_t num_tuples) {
    DCHECK(query_context_);
    InsertDestination *insert_destination =
        query_context_->getInsertDestination(finalize_op_->getInsertDestinationID());
    DCHECK(insert_destination);

    const std::vector<block_id> result = insert_destination->getTouchedBlocks();
    std::size_t total_tuples = 0;
    for (size_t bid = 0; bid < result.size(); ++bid) {
      BlockReference block = storage_manager_->getBlock(result[bid],
                                                        insert_destination->getRelation());
      const TupleStorageSubBlock &sub_block = block->getTupleStorageSubBlock();
      ASSERT_TRUE(sub_block.isPacked());
      for (tuple_id tid = 0; tid < sub_block.numTuples(); ++tid) {
        const TypedValue group_by_1 =
            sub_block.getAttributeValueTyped(tid, result_table_->getAttributeByName("GroupBy-0")->getID());
        const TypedValue group_by_2 =
            sub_block.getAttributeValueTyped(tid, result_table_->getAttributeByName("GroupBy-1")->getID());
        int group_by_id = group_by_1.getLiteral<int>() + (group_by_2.getLiteral<int>() * kGroupBy1Size);
        const TypedValue actual0 =
            sub_block.getAttributeValueTyped(tid, result_table_->getAttributeByName("result-0")->getID());
        const TypedValue actual1 =
            sub_block.getAttributeValueTyped(tid, result_table_->getAttributeByName("result-1")->getID());
        check_fn(group_by_id, actual0, actual1);
      }
      total_tuples += sub_block.numTuples();
    }
    EXPECT_EQ(num_tuples, total_tuples);
  }

  template <class FinalDataType>
  void testAggregationOperatorWithGroupBy(const std::string &stem,
                                          const AggregationID agg_type,
                                          const bool with_predicate,
                                          std::function<void(int, const TypedValue &, const TypedValue &)> check_fn,
                                          std::size_t num_tuples,
                                          const int predicate_value = kGroupByWidth * (kGroupByRepeats >> 1)) {
    setupTestGroupBy(stem, agg_type, with_predicate, FinalDataType::InstanceNullable(), predicate_value);
    execute();
    checkGroupByResult(check_fn, num_tuples);
  }

  // This map is needed for InsertDestination and some WorkOrders that send
  // messages to Foreman directly. To know the reason behind the design of this
  // map, see the note in InsertDestination.hpp.
  ClientIDMap *thread_id_map_;

  MessageBusImpl bus_;
  tmb::client_id foreman_client_id_;

  std::unique_ptr<QueryContext> query_context_;
  std::unique_ptr<StorageManager> storage_manager_;

  std::unique_ptr<CatalogDatabase> db_;
  // The following two CatalogRelations are owned by db_.
  CatalogRelation *table_, *result_table_;

  std::unique_ptr<AggregationOperator> op_;
  std::unique_ptr<FinalizeAggregationOperator> finalize_op_;
};

const char AggregationOperatorTest::kDatabaseName[] = "database";
const char AggregationOperatorTest::kTableName[] = "table";
const char AggregationOperatorTest::kStoragePath[] = "./test_data";

namespace {

// Summation 1, 2, ..., n
std::int64_t Summation(int n) {
  return (n + 1) * n / 2;
}

// Summation 1*1, 2*2, 3*3, ..., n*n
std::int64_t SummationSquares(int n) {
  return n * (n + 1) * (2 * n + 1) / 6;
}

template <class T>
void CheckLiteral(T expected, TypedValue actual) {
  EXPECT_EQ(expected, actual.getLiteral<T>());
}

template <class T>
void CheckNear(T expected, TypedValue actual) {
  EXPECT_NEAR(expected, actual.getLiteral<T>(), expected * 1e-5);
}

}  // namespace

TEST_F(AggregationOperatorTest, ScalarAttribute_IntType_Sum_withoutPredicate) {
  // Sum of IntType is LongType.
  testAggregationOperator<LongType>("IntType",
                                    AggregationID::kSum,
                                    kAttribute,
                                    kWithoutPredicate,
                                    Summation(299),
                                    Summation(299),
                                    CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_LongType_Sum_withoutPredicate) {
  testAggregationOperator<LongType>("LongType",
                                    AggregationID::kSum,
                                    kAttribute,
                                    kWithoutPredicate,
                                    Summation(299),
                                    Summation(299),
                                    CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_FloatType_Sum_withoutPredicate) {
  // Sum of FloatType is DoubleType.
  testAggregationOperator<DoubleType>("FloatType",
                                      AggregationID::kSum,
                                      kAttribute,
                                      kWithoutPredicate,
                                      0.1 * Summation(299),
                                      0.1 * Summation(299),
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_DoubleType_Sum_withoutPredicate) {
  testAggregationOperator<DoubleType>("DoubleType",
                                      AggregationID::kSum,
                                      kAttribute,
                                      kWithoutPredicate,
                                      0.1 * Summation(299),
                                      0.1 * Summation(299),
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_IntType_Avg_withoutPredicate) {
  testAggregationOperator<DoubleType>("IntType",
                                      AggregationID::kAvg,
                                      kAttribute,
                                      kWithoutPredicate,
                                      Summation(299) / 300.0,
                                      Summation(299) / 300.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_LongType_Avg_withoutPredicate) {
  testAggregationOperator<DoubleType>("LongType",
                                      AggregationID::kAvg,
                                      kAttribute,
                                      kWithoutPredicate,
                                      Summation(299) / 300.0,
                                      Summation(299) / 300.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_FloatType_Avg_withoutPredicate) {
  testAggregationOperator<DoubleType>("FloatType",
                                      AggregationID::kAvg,
                                      kAttribute,
                                      kWithoutPredicate,
                                      0.1 * Summation(299) / 300.0,
                                      0.1 * Summation(299) / 300.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_DoubleType_Avg_withoutPredicate) {
  testAggregationOperator<DoubleType>("DoubleType",
                                      AggregationID::kAvg,
                                      kAttribute,
                                      kWithoutPredicate,
                                      0.1 * Summation(299) / 300.0,
                                      0.1 * Summation(299) / 300.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_IntType_Max_withoutPredicate) {
  testAggregationOperator<IntType>(
      "IntType", AggregationID::kMax, kAttribute, kWithoutPredicate, 299, 299, CheckLiteral<IntType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_LongType_Max_withoutPredicate) {
  testAggregationOperator<LongType>(
      "LongType", AggregationID::kMax, kAttribute, kWithoutPredicate, 299, 299, CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_FloatType_Max_withoutPredicate) {
  testAggregationOperator<FloatType>(
      "FloatType", AggregationID::kMax, kAttribute, kWithoutPredicate, 29.9, 29.9, CheckNear<FloatType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_DoubleType_Max_withoutPredicate) {
  testAggregationOperator<DoubleType>(
      "DoubleType", AggregationID::kMax, kAttribute, kWithoutPredicate, 29.9, 29.9, CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_IntType_Min_withoutPredicate) {
  testAggregationOperator<IntType>(
      "IntType", AggregationID::kMin, kAttribute, kWithoutPredicate, 0, 0, CheckLiteral<IntType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_LongType_Min_withoutPredicate) {
  testAggregationOperator<LongType>(
      "LongType", AggregationID::kMin, kAttribute, kWithoutPredicate, 0, 0, CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_FloatType_Min_withoutPredicate) {
  testAggregationOperator<FloatType>(
      "FloatType", AggregationID::kMin, kAttribute, kWithoutPredicate, 0, 0, CheckNear<FloatType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_DoubleType_Min_withoutPredicate) {
  testAggregationOperator<DoubleType>(
      "DoubleType", AggregationID::kMin, kAttribute, kWithoutPredicate, 0, 0, CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_Count_withoutPredicate) {
  testAggregationOperator<LongType>("DoubleType",
                                    AggregationID::kCount,
                                    kAttribute,
                                    kWithoutPredicate,
                                    300,
                                    300,
                                    CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_IntType_Sum_withoutPredicate) {
  // Sum of IntType is LongType.
  testAggregationOperator<LongType>("IntType",
                                    AggregationID::kSum,
                                    kExpression,
                                    kWithoutPredicate,
                                    2 * Summation(299),
                                    SummationSquares(299),
                                    CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_LongType_Sum_withoutPredicate) {
  testAggregationOperator<LongType>("LongType",
                                    AggregationID::kSum,
                                    kExpression,
                                    kWithoutPredicate,
                                    2 * Summation(299),
                                    SummationSquares(299),
                                    CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_FloatType_Sum_withoutPredicate) {
  // Sum of FloatType is DoubleType.
  testAggregationOperator<DoubleType>("FloatType",
                                      AggregationID::kSum,
                                      kExpression,
                                      kWithoutPredicate,
                                      2 * 0.1 * Summation(299),
                                      0.01 * SummationSquares(299),
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_DoubleType_Sum_withoutPredicate) {
  testAggregationOperator<DoubleType>("DoubleType",
                                      AggregationID::kSum,
                                      kExpression,
                                      kWithoutPredicate,
                                      2 * 0.1 * Summation(299),
                                      0.01 * SummationSquares(299),
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_IntType_Avg_withoutPredicate) {
  testAggregationOperator<DoubleType>("IntType",
                                      AggregationID::kAvg,
                                      kExpression,
                                      kWithoutPredicate,
                                      2 * Summation(299) / 300.0,
                                      SummationSquares(299) / 300.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_LongType_Avg_withoutPredicate) {
  testAggregationOperator<DoubleType>("LongType",
                                      AggregationID::kAvg,
                                      kExpression,
                                      kWithoutPredicate,
                                      2 * Summation(299) / 300.0,
                                      SummationSquares(299) / 300.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_FloatType_Avg_withoutPredicate) {
  testAggregationOperator<DoubleType>("FloatType",
                                      AggregationID::kAvg,
                                      kExpression,
                                      kWithoutPredicate,
                                      2 * 0.1 * Summation(299) / 300.0,
                                      0.01 * SummationSquares(299) / 300.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_DoubleType_Avg_withoutPredicate) {
  testAggregationOperator<DoubleType>("DoubleType",
                                      AggregationID::kAvg,
                                      kExpression,
                                      kWithoutPredicate,
                                      2 * 0.1 * Summation(299) / 300.0,
                                      0.01 * SummationSquares(299) / 300.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_IntType_Max_withoutPredicate) {
  testAggregationOperator<IntType>("IntType",
                                   AggregationID::kMax,
                                   kExpression,
                                   kWithoutPredicate,
                                   2 * 299,
                                   299 * 299,
                                   CheckLiteral<IntType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_LongType_Max_withoutPredicate) {
  testAggregationOperator<LongType>("LongType",
                                    AggregationID::kMax,
                                    kExpression,
                                    kWithoutPredicate,
                                    2 * 299,
                                    299 * 299,
                                    CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_FloatType_Max_withoutPredicate) {
  testAggregationOperator<FloatType>("FloatType",
                                     AggregationID::kMax,
                                     kExpression,
                                     kWithoutPredicate,
                                     2 * 29.9,
                                     29.9 * 29.9,
                                     CheckNear<FloatType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_DoubleType_Max_withoutPredicate) {
  testAggregationOperator<DoubleType>("DoubleType",
                                      AggregationID::kMax,
                                      kExpression,
                                      kWithoutPredicate,
                                      2 * 29.9,
                                      29.9 * 29.9,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_IntType_Min_withoutPredicate) {
  testAggregationOperator<IntType>(
      "IntType", AggregationID::kMin, kExpression, kWithoutPredicate, 0, 0, CheckLiteral<IntType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_LongType_Min_withoutPredicate) {
  testAggregationOperator<LongType>(
      "LongType", AggregationID::kMin, kExpression, kWithoutPredicate, 0, 0, CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_FloatType_Min_withoutPredicate) {
  testAggregationOperator<FloatType>(
      "FloatType", AggregationID::kMin, kExpression, kWithoutPredicate, 0, 0, CheckNear<FloatType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_DoubleType_Min_withoutPredicate) {
  testAggregationOperator<DoubleType>(
      "DoubleType", AggregationID::kMin, kExpression, kWithoutPredicate, 0, 0, CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_Count_withoutPredicate) {
  testAggregationOperator<LongType>("DoubleType",
                                    AggregationID::kCount,
                                    kExpression,
                                    kWithoutPredicate,
                                    300,
                                    300,
                                    CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_IntType_Sum_withPredicate) {
  // Sum of IntType is LongType.
  testAggregationOperator<LongType>("IntType",
                                    AggregationID::kSum,
                                    kAttribute,
                                    kWithPredicate,
                                    Summation(29),
                                    Summation(29),
                                    CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_LongType_Sum_withPredicate) {
  testAggregationOperator<LongType>("LongType",
                                    AggregationID::kSum,
                                    kAttribute,
                                    kWithPredicate,
                                    Summation(29),
                                    Summation(29),
                                    CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_FloatType_Sum_withPredicate) {
  // Sum of FloatType is DoubleType.
  testAggregationOperator<DoubleType>("FloatType",
                                      AggregationID::kSum,
                                      kAttribute,
                                      kWithPredicate,
                                      0.1 * Summation(29),
                                      0.1 * Summation(29),
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_DoubleType_Sum_withPredicate) {
  testAggregationOperator<DoubleType>("DoubleType",
                                      AggregationID::kSum,
                                      kAttribute,
                                      kWithPredicate,
                                      0.1 * Summation(29),
                                      0.1 * Summation(29),
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_IntType_Avg_withPredicate) {
  testAggregationOperator<DoubleType>("IntType",
                                      AggregationID::kAvg,
                                      kAttribute,
                                      kWithPredicate,
                                      Summation(29) / 30.0,
                                      Summation(29) / 30.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_LongType_Avg_withPredicate) {
  testAggregationOperator<DoubleType>("LongType",
                                      AggregationID::kAvg,
                                      kAttribute,
                                      kWithPredicate,
                                      Summation(29) / 30.0,
                                      Summation(29) / 30.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_FloatType_Avg_withPredicate) {
  testAggregationOperator<DoubleType>("FloatType",
                                      AggregationID::kAvg,
                                      kAttribute,
                                      kWithPredicate,
                                      0.1 * Summation(29) / 30.0,
                                      0.1 * Summation(29) / 30.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_DoubleType_Avg_withPredicate) {
  testAggregationOperator<DoubleType>("DoubleType",
                                      AggregationID::kAvg,
                                      kAttribute,
                                      kWithPredicate,
                                      0.1 * Summation(29) / 30.0,
                                      0.1 * Summation(29) / 30.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_IntType_Max_withPredicate) {
  testAggregationOperator<IntType>(
      "IntType", AggregationID::kMax, kAttribute, kWithPredicate, 29, 29, CheckLiteral<IntType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_LongType_Max_withPredicate) {
  testAggregationOperator<LongType>(
      "LongType", AggregationID::kMax, kAttribute, kWithPredicate, 29, 29, CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_FloatType_Max_withPredicate) {
  testAggregationOperator<FloatType>(
      "FloatType", AggregationID::kMax, kAttribute, kWithPredicate, 2.9, 2.9, CheckNear<FloatType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_DoubleType_Max_withPredicate) {
  testAggregationOperator<DoubleType>(
      "DoubleType", AggregationID::kMax, kAttribute, kWithPredicate, 2.9, 2.9, CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_IntType_Min_withPredicate) {
  testAggregationOperator<IntType>(
      "IntType", AggregationID::kMin, kAttribute, kWithPredicate, 0, 0, CheckLiteral<IntType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_LongType_Min_withPredicate) {
  testAggregationOperator<LongType>(
      "LongType", AggregationID::kMin, kAttribute, kWithPredicate, 0, 0, CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_FloatType_Min_withPredicate) {
  testAggregationOperator<FloatType>(
      "FloatType", AggregationID::kMin, kAttribute, kWithPredicate, 0, 0, CheckNear<FloatType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_DoubleType_Min_withPredicate) {
  testAggregationOperator<DoubleType>(
      "DoubleType", AggregationID::kMin, kAttribute, kWithPredicate, 0, 0, CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_Count_withPredicate) {
  testAggregationOperator<LongType>(
      "DoubleType", AggregationID::kCount, kAttribute, kWithPredicate, 30, 30, CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_IntType_Sum_withPredicate) {
  // Sum of IntType is LongType.
  testAggregationOperator<LongType>("IntType",
                                    AggregationID::kSum,
                                    kExpression,
                                    kWithPredicate,
                                    2 * Summation(29),
                                    SummationSquares(29),
                                    CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_LongType_Sum_withPredicate) {
  testAggregationOperator<LongType>("LongType",
                                    AggregationID::kSum,
                                    kExpression,
                                    kWithPredicate,
                                    2 * Summation(29),
                                    SummationSquares(29),
                                    CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_FloatType_Sum_withPredicate) {
  // Sum of FloatType is DoubleType.
  testAggregationOperator<DoubleType>("FloatType",
                                      AggregationID::kSum,
                                      kExpression,
                                      kWithPredicate,
                                      2 * 0.1 * Summation(29),
                                      0.01 * SummationSquares(29),
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_DoubleType_Sum_withPredicate) {
  testAggregationOperator<DoubleType>("DoubleType",
                                      AggregationID::kSum,
                                      kExpression,
                                      kWithPredicate,
                                      2 * 0.1 * Summation(29),
                                      0.01 * SummationSquares(29),
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_IntType_Avg_withPredicate) {
  testAggregationOperator<DoubleType>("IntType",
                                      AggregationID::kAvg,
                                      kExpression,
                                      kWithPredicate,
                                      2 * Summation(29) / 30.0,
                                      SummationSquares(29) / 30.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_LongType_Avg_withPredicate) {
  testAggregationOperator<DoubleType>("LongType",
                                      AggregationID::kAvg,
                                      kExpression,
                                      kWithPredicate,
                                      2 * Summation(29) / 30.0,
                                      SummationSquares(29) / 30.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_FloatType_Avg_withPredicate) {
  testAggregationOperator<DoubleType>("FloatType",
                                      AggregationID::kAvg,
                                      kExpression,
                                      kWithPredicate,
                                      2 * 0.1 * Summation(29) / 30.0,
                                      0.01 * SummationSquares(29) / 30.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_DoubleType_Avg_withPredicate) {
  testAggregationOperator<DoubleType>("DoubleType",
                                      AggregationID::kAvg,
                                      kExpression,
                                      kWithPredicate,
                                      2 * 0.1 * Summation(29) / 30.0,
                                      0.01 * SummationSquares(29) / 30.0,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_IntType_Max_withPredicate) {
  testAggregationOperator<IntType>("IntType",
                                   AggregationID::kMax,
                                   kExpression,
                                   kWithPredicate,
                                   2 * 29,
                                   29 * 29,
                                   CheckLiteral<IntType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_LongType_Max_withPredicate) {
  testAggregationOperator<LongType>("LongType",
                                    AggregationID::kMax,
                                    kExpression,
                                    kWithPredicate,
                                    2 * 29,
                                    29 * 29,
                                    CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_FloatType_Max_withPredicate) {
  testAggregationOperator<FloatType>("FloatType",
                                     AggregationID::kMax,
                                     kExpression,
                                     kWithPredicate,
                                     2 * 2.9,
                                     2.9 * 2.9,
                                     CheckNear<FloatType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_DoubleType_Max_withPredicate) {
  testAggregationOperator<DoubleType>("DoubleType",
                                      AggregationID::kMax,
                                      kExpression,
                                      kWithPredicate,
                                      2 * 2.9,
                                      2.9 * 2.9,
                                      CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_IntType_Min_withPredicate) {
  testAggregationOperator<IntType>(
      "IntType", AggregationID::kMin, kExpression, kWithPredicate, 0, 0, CheckLiteral<IntType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_LongType_Min_withPredicate) {
  testAggregationOperator<LongType>(
      "LongType", AggregationID::kMin, kExpression, kWithPredicate, 0, 0, CheckLiteral<LongType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_FloatType_Min_withPredicate) {
  testAggregationOperator<FloatType>(
      "FloatType", AggregationID::kMin, kExpression, kWithPredicate, 0, 0, CheckNear<FloatType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_DoubleType_Min_withPredicate) {
  testAggregationOperator<DoubleType>(
      "DoubleType", AggregationID::kMin, kExpression, kWithPredicate, 0, 0, CheckNear<DoubleType::cpptype>);
}

TEST_F(AggregationOperatorTest, ScalarExpression_Count_withPredicate) {
  testAggregationOperator<LongType>(
      "DoubleType", AggregationID::kCount, kExpression, kWithPredicate, 30, 30, CheckLiteral<LongType::cpptype>);
}

namespace {

template <class T>
void CheckNull(T dummy, TypedValue actual) {
  EXPECT_TRUE(actual.isNull());
}

}  // namespace

TEST_F(AggregationOperatorTest, ScalarAttribute_IntType_Sum_zeroRows) {
  // Sum of IntType is LongType.
  testAggregationOperator<LongType>("IntType",
                                    AggregationID::kSum,
                                    kAttribute,
                                    kWithPredicate,
                                    kPlaceholder,
                                    kPlaceholder,
                                    CheckNull<LongType::cpptype>,
                                    -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_LongType_Sum_zeroRows) {
  testAggregationOperator<LongType>("LongType",
                                    AggregationID::kSum,
                                    kAttribute,
                                    kWithPredicate,
                                    kPlaceholder,
                                    kPlaceholder,
                                    CheckNull<LongType::cpptype>,
                                    -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_FloatType_Sum_zeroRows) {
  // Sum of FloatType is DoubleType.
  testAggregationOperator<DoubleType>("FloatType",
                                      AggregationID::kSum,
                                      kAttribute,
                                      kWithPredicate,
                                      kPlaceholder,
                                      kPlaceholder,
                                      CheckNull<DoubleType::cpptype>,
                                      -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_DoubleType_Sum_zeroRows) {
  testAggregationOperator<DoubleType>("DoubleType",
                                      AggregationID::kSum,
                                      kAttribute,
                                      kWithPredicate,
                                      kPlaceholder,
                                      kPlaceholder,
                                      CheckNull<DoubleType::cpptype>,
                                      -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_IntType_Avg_zeroRows) {
  testAggregationOperator<DoubleType>("IntType",
                                      AggregationID::kAvg,
                                      kAttribute,
                                      kWithPredicate,
                                      kPlaceholder,
                                      kPlaceholder,
                                      CheckNull<DoubleType::cpptype>,
                                      -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_LongType_Avg_zeroRows) {
  testAggregationOperator<DoubleType>("LongType",
                                      AggregationID::kAvg,
                                      kAttribute,
                                      kWithPredicate,
                                      kPlaceholder,
                                      kPlaceholder,
                                      CheckNull<DoubleType::cpptype>,
                                      -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_FloatType_Avg_zeroRows) {
  testAggregationOperator<DoubleType>("FloatType",
                                      AggregationID::kAvg,
                                      kAttribute,
                                      kWithPredicate,
                                      kPlaceholder,
                                      kPlaceholder,
                                      CheckNull<DoubleType::cpptype>,
                                      -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_DoubleType_Avg_zeroRows) {
  testAggregationOperator<DoubleType>("DoubleType",
                                      AggregationID::kAvg,
                                      kAttribute,
                                      kWithPredicate,
                                      kPlaceholder,
                                      kPlaceholder,
                                      CheckNull<DoubleType::cpptype>,
                                      -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_IntType_Max_zeroRows) {
  testAggregationOperator<IntType>("IntType",
                                   AggregationID::kMax,
                                   kAttribute,
                                   kWithPredicate,
                                   kPlaceholder,
                                   kPlaceholder,
                                   CheckNull<IntType::cpptype>,
                                   -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_LongType_Max_zeroRows) {
  testAggregationOperator<LongType>("LongType",
                                    AggregationID::kMax,
                                    kAttribute,
                                    kWithPredicate,
                                    kPlaceholder,
                                    kPlaceholder,
                                    CheckNull<LongType::cpptype>,
                                    -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_FloatType_Max_zeroRows) {
  testAggregationOperator<FloatType>("FloatType",
                                     AggregationID::kMax,
                                     kAttribute,
                                     kWithPredicate,
                                     kPlaceholder,
                                     kPlaceholder,
                                     CheckNull<FloatType::cpptype>,
                                     -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_DoubleType_Max_zeroRows) {
  testAggregationOperator<DoubleType>("DoubleType",
                                      AggregationID::kMax,
                                      kAttribute,
                                      kWithPredicate,
                                      kPlaceholder,
                                      kPlaceholder,
                                      CheckNull<DoubleType::cpptype>,
                                      -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_IntType_Min_zeroRows) {
  testAggregationOperator<IntType>("IntType",
                                   AggregationID::kMin,
                                   kAttribute,
                                   kWithPredicate,
                                   kPlaceholder,
                                   kPlaceholder,
                                   CheckNull<IntType::cpptype>,
                                   -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_LongType_Min_zeroRows) {
  testAggregationOperator<LongType>("LongType",
                                    AggregationID::kMin,
                                    kAttribute,
                                    kWithPredicate,
                                    kPlaceholder,
                                    kPlaceholder,
                                    CheckNull<LongType::cpptype>,
                                    -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_FloatType_Min_zeroRows) {
  testAggregationOperator<FloatType>("FloatType",
                                     AggregationID::kMin,
                                     kAttribute,
                                     kWithPredicate,
                                     kPlaceholder,
                                     kPlaceholder,
                                     CheckNull<FloatType::cpptype>,
                                     -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_DoubleType_Min_zeroRows) {
  testAggregationOperator<DoubleType>("DoubleType",
                                      AggregationID::kMin,
                                      kAttribute,
                                      kWithPredicate,
                                      kPlaceholder,
                                      kPlaceholder,
                                      CheckNull<DoubleType::cpptype>,
                                      -1);
}

TEST_F(AggregationOperatorTest, ScalarAttribute_Count_zeroRows) {
  // Count of zero rows is 0 unlike other aggregate functions.
  testAggregationOperator<LongType>("DoubleType",
                                    AggregationID::kCount,
                                    kAttribute,
                                    kWithPredicate,
                                    0,
                                    0,
                                    CheckLiteral<LongType::cpptype>,
                                    -1);
}

namespace {

// Computes the sum of arthemetic series: a, a + d, a + 2*d, ..., a + (n-1)*d
std::int64_t ArthemeticSum(int a, int d, int n) {
  return n * (2 * a + (n - 1) * d) / 2;
}

template <class T, bool with_predicate>
void GroupBy_SumCheckIntegral(int group_by_id, const TypedValue &value1, const TypedValue &value2) {
  size_t num_repeats = AggregationOperatorTest::kGroupByRepeats;
  if (with_predicate) {
    num_repeats = AggregationOperatorTest::kGroupByRepeats >> 1;
  }
  std::int64_t sum =
      ArthemeticSum(group_by_id, AggregationOperatorTest::kGroupByWidth, num_repeats);
  EXPECT_EQ(sum, value1.getLiteral<T>());
  EXPECT_EQ(sum, value2.getLiteral<T>());
}

template <class T, bool with_predicate>
void GroupBy_SumCheckFloat(int group_by_id, const TypedValue &value1, const TypedValue &value2) {
  size_t num_repeats = AggregationOperatorTest::kGroupByRepeats;
  if (with_predicate) {
    num_repeats = AggregationOperatorTest::kGroupByRepeats >> 1;
  }
  double sum = 0.1 * ArthemeticSum(
                         group_by_id, AggregationOperatorTest::kGroupByWidth, num_repeats);
  EXPECT_NEAR(sum, value1.getLiteral<T>(), 1e-5 * sum);
  EXPECT_NEAR(sum, value2.getLiteral<T>(), 1e-5 * sum);
}

}  // namespace

TEST_F(AggregationOperatorTest, GroupBy_Sum_IntType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<LongType>("IntType",
                                               AggregationID::kSum,
                                               kWithoutPredicate,
                                               GroupBy_SumCheckIntegral<LongType::cpptype, kWithoutPredicate>,
                                               kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Sum_LongType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<LongType>("LongType",
                                               AggregationID::kSum,
                                               kWithoutPredicate,
                                               GroupBy_SumCheckIntegral<LongType::cpptype, kWithoutPredicate>,
                                               kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Sum_FloatType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>("FloatType",
                                                 AggregationID::kSum,
                                                 kWithoutPredicate,
                                                 GroupBy_SumCheckFloat<DoubleType::cpptype, kWithoutPredicate>,
                                                 kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Sum_DoubleType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>("DoubleType",
                                                 AggregationID::kSum,
                                                 kWithoutPredicate,
                                                 GroupBy_SumCheckFloat<DoubleType::cpptype, kWithoutPredicate>,
                                                 kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Sum_LongType_withPredicate) {
  testAggregationOperatorWithGroupBy<LongType>("LongType",
                                               AggregationID::kSum,
                                               kWithPredicate,
                                               GroupBy_SumCheckIntegral<LongType::cpptype, kWithPredicate>,
                                               kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Sum_IntType_withPredicate) {
  testAggregationOperatorWithGroupBy<LongType>("IntType",
                                               AggregationID::kSum,
                                               kWithPredicate,
                                               GroupBy_SumCheckIntegral<LongType::cpptype, kWithPredicate>,
                                               kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Sum_FloatType_withPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>("FloatType",
                                                 AggregationID::kSum,
                                                 kWithPredicate,
                                                 GroupBy_SumCheckFloat<DoubleType::cpptype, kWithPredicate>,
                                                 kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Sum_DoubleType_withPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>("DoubleType",
                                                 AggregationID::kSum,
                                                 kWithPredicate,
                                                 GroupBy_SumCheckFloat<DoubleType::cpptype, kWithPredicate>,
                                                 kGroupByWidth);
}

namespace {

template <bool with_predicate>
void GroupBy_AvgCheckIntegral(int group_by_id, const TypedValue &value1, const TypedValue &value2) {
  size_t num_repeats = AggregationOperatorTest::kGroupByRepeats;
  if (with_predicate) {
    num_repeats = AggregationOperatorTest::kGroupByRepeats >> 1;
  }
  double avg = ArthemeticSum(group_by_id, AggregationOperatorTest::kGroupByWidth, num_repeats) /
               static_cast<double>(num_repeats);
  EXPECT_NEAR(avg, value1.getLiteral<DoubleType::cpptype>(), 1e-5 * avg);
  EXPECT_NEAR(avg, value2.getLiteral<DoubleType::cpptype>(), 1e-5 * avg);
}

template <bool with_predicate>
void GroupBy_AvgCheckFloat(int group_by_id, const TypedValue &value1, const TypedValue &value2) {
  size_t num_repeats = AggregationOperatorTest::kGroupByRepeats;
  if (with_predicate) {
    num_repeats = AggregationOperatorTest::kGroupByRepeats >> 1;
  }
  double avg = 0.1 * ArthemeticSum(group_by_id, AggregationOperatorTest::kGroupByWidth, num_repeats) /
               static_cast<double>(num_repeats);
  EXPECT_NEAR(avg, value1.getLiteral<DoubleType::cpptype>(), 1e-5 * avg);
  EXPECT_NEAR(avg, value2.getLiteral<DoubleType::cpptype>(), 1e-5 * avg);
}

}  // namespace

TEST_F(AggregationOperatorTest, GroupBy_Avg_IntType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "IntType",
      AggregationID::kAvg,
      kWithoutPredicate,
      GroupBy_AvgCheckIntegral<kWithoutPredicate>,
      kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Avg_LongType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "LongType",
      AggregationID::kAvg,
      kWithoutPredicate,
      GroupBy_AvgCheckIntegral<kWithoutPredicate>,
      kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Avg_FloatType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "FloatType",
      AggregationID::kAvg,
      kWithoutPredicate,
      GroupBy_AvgCheckFloat<kWithoutPredicate>,
      kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Avg_DoubleType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "DoubleType",
      AggregationID::kAvg,
      kWithoutPredicate,
      GroupBy_AvgCheckFloat<kWithoutPredicate>,
      kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Avg_IntType_withPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "IntType",
      AggregationID::kAvg,
      kWithPredicate,
      GroupBy_AvgCheckIntegral<kWithPredicate>,
      kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Avg_LongType_withPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "LongType",
      AggregationID::kAvg,
      kWithPredicate,
      GroupBy_AvgCheckIntegral<kWithPredicate>,
      kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Avg_FloatType_withPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "FloatType",
      AggregationID::kAvg,
      kWithPredicate,
      GroupBy_AvgCheckFloat<kWithPredicate>,
      kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Avg_DoubleType_withPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "DoubleType",
      AggregationID::kAvg,
      kWithPredicate,
      GroupBy_AvgCheckFloat<kWithPredicate>,
      kGroupByWidth);
}

namespace {

template <class T, bool with_predicate>
void GroupBy_MaxCheckIntegral(int group_by_id, const TypedValue &value1, const TypedValue &value2) {
  size_t num_repeats = AggregationOperatorTest::kGroupByRepeats;
  if (with_predicate) {
    num_repeats = AggregationOperatorTest::kGroupByRepeats >> 1;
  }
  T max = (AggregationOperatorTest::kGroupByWidth * (num_repeats - 1)) + group_by_id;
  EXPECT_EQ(max, value1.getLiteral<T>());
  EXPECT_EQ(max, value2.getLiteral<T>());
}

template <bool with_predicate>
void GroupBy_MaxCheckFloat(int group_by_id, const TypedValue &value1, const TypedValue &value2) {
  size_t num_repeats = AggregationOperatorTest::kGroupByRepeats;
  if (with_predicate) {
    num_repeats = AggregationOperatorTest::kGroupByRepeats >> 1;
  }
  float max =
      0.1 * ((AggregationOperatorTest::kGroupByWidth * (num_repeats - 1)) + group_by_id);
  EXPECT_FLOAT_EQ(max, value1.getLiteral<float>());
  EXPECT_FLOAT_EQ(max, value2.getLiteral<float>());
}

template <bool with_predicate>
void GroupBy_MaxCheckDouble(int group_by_id, const TypedValue &value1, const TypedValue &value2) {
  size_t num_repeats = AggregationOperatorTest::kGroupByRepeats;
  if (with_predicate) {
    num_repeats = AggregationOperatorTest::kGroupByRepeats >> 1;
  }
  double max =
      0.1 * ((AggregationOperatorTest::kGroupByWidth * (num_repeats - 1)) + group_by_id);
  EXPECT_DOUBLE_EQ(max, value1.getLiteral<double>());
  EXPECT_DOUBLE_EQ(max, value2.getLiteral<double>());
}

}  // namespace

TEST_F(AggregationOperatorTest, GroupBy_Max_IntType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<IntType>("IntType",
                                              AggregationID::kMax,
                                              kWithoutPredicate,
                                              GroupBy_MaxCheckIntegral<IntType::cpptype, kWithoutPredicate>,
                                              kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Max_LongType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<LongType>("LongType",
                                               AggregationID::kMax,
                                               kWithoutPredicate,
                                               GroupBy_MaxCheckIntegral<LongType::cpptype, kWithoutPredicate>,
                                               kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Max_FloatType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<FloatType>(
      "FloatType", AggregationID::kMax, kWithoutPredicate, GroupBy_MaxCheckFloat<kWithoutPredicate>, kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Max_DoubleType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>("DoubleType",
                                                 AggregationID::kMax,
                                                 kWithoutPredicate,
                                                 GroupBy_MaxCheckDouble<kWithoutPredicate>,
                                                 kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Max_IntType_withPredicate) {
  testAggregationOperatorWithGroupBy<IntType>("IntType",
                                              AggregationID::kMax,
                                              kWithPredicate,
                                              GroupBy_MaxCheckIntegral<IntType::cpptype, kWithPredicate>,
                                              kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Max_LongType_withPredicate) {
  testAggregationOperatorWithGroupBy<LongType>("LongType",
                                               AggregationID::kMax,
                                               kWithPredicate,
                                               GroupBy_MaxCheckIntegral<LongType::cpptype, kWithPredicate>,
                                               kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Max_FloatType_withPredicate) {
  testAggregationOperatorWithGroupBy<FloatType>(
      "FloatType", AggregationID::kMax, kWithPredicate, GroupBy_MaxCheckFloat<kWithPredicate>, kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Max_DoubleType_withPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>("DoubleType",
                                                 AggregationID::kMax,
                                                 kWithPredicate,
                                                 GroupBy_MaxCheckDouble<kWithPredicate>,
                                                 kGroupByWidth);
}

namespace {

template <class T, bool with_predicate>
void GroupBy_MinCheckIntegral(int group_by_id, const TypedValue &value1, const TypedValue &value2) {
  T min = group_by_id;
  EXPECT_EQ(min, value1.getLiteral<T>());
  EXPECT_EQ(min, value2.getLiteral<T>());
}

template <bool with_predicate>
void GroupBy_MinCheckFloat(int group_by_id, const TypedValue &value1, const TypedValue &value2) {
  float min = 0.1 * group_by_id;
  EXPECT_FLOAT_EQ(min, value1.getLiteral<float>());
  EXPECT_FLOAT_EQ(min, value2.getLiteral<float>());
}

template <bool with_predicate>
void GroupBy_MinCheckDouble(int group_by_id, const TypedValue &value1, const TypedValue &value2) {
  double min = 0.1 * group_by_id;
  EXPECT_DOUBLE_EQ(min, value1.getLiteral<double>());
  EXPECT_DOUBLE_EQ(min, value2.getLiteral<double>());
}

}  // namespace

TEST_F(AggregationOperatorTest, GroupBy_Min_IntType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<IntType>("IntType",
                                              AggregationID::kMin,
                                              kWithoutPredicate,
                                              GroupBy_MinCheckIntegral<IntType::cpptype, kWithoutPredicate>,
                                              kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Min_LongType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<LongType>("LongType",
                                               AggregationID::kMin,
                                               kWithoutPredicate,
                                               GroupBy_MinCheckIntegral<LongType::cpptype, kWithoutPredicate>,
                                               kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Min_FloatType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<FloatType>(
      "FloatType", AggregationID::kMin, kWithoutPredicate, GroupBy_MinCheckFloat<kWithoutPredicate>, kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Min_DoubleType_withoutPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>("DoubleType",
                                                 AggregationID::kMin,
                                                 kWithoutPredicate,
                                                 GroupBy_MinCheckDouble<kWithoutPredicate>,
                                                 kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Min_IntType_withPredicate) {
  testAggregationOperatorWithGroupBy<IntType>("IntType",
                                              AggregationID::kMin,
                                              kWithPredicate,
                                              GroupBy_MinCheckIntegral<IntType::cpptype, kWithPredicate>,
                                              kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Min_LongType_withPredicate) {
  testAggregationOperatorWithGroupBy<LongType>("LongType",
                                               AggregationID::kMin,
                                               kWithPredicate,
                                               GroupBy_MinCheckIntegral<LongType::cpptype, kWithPredicate>,
                                               kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Min_FloatType_withPredicate) {
  testAggregationOperatorWithGroupBy<FloatType>(
      "FloatType", AggregationID::kMin, kWithPredicate, GroupBy_MinCheckFloat<kWithPredicate>, kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Min_DoubleType_withPredicate) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "DoubleType", AggregationID::kMin, kWithPredicate, GroupBy_MinCheckDouble<kWithPredicate>, kGroupByWidth);
}

namespace {

template <bool with_predicate>
void GroupBy_CountCheck(int group_by_id, const TypedValue &value1, const TypedValue &value2) {
  LongType::cpptype count = AggregationOperatorTest::kGroupByRepeats;
  if (with_predicate) {
    count = AggregationOperatorTest::kGroupByRepeats >> 1;
  }
  EXPECT_EQ(count, value1.getLiteral<LongType::cpptype>());
  EXPECT_EQ(count, value2.getLiteral<LongType::cpptype>());
}

}  // namespace

TEST_F(AggregationOperatorTest, GroupBy_Count_withoutPredicate) {
  testAggregationOperatorWithGroupBy<LongType>(
      "DoubleType", AggregationID::kCount, kWithoutPredicate, GroupBy_CountCheck<kWithoutPredicate>, kGroupByWidth);
}

TEST_F(AggregationOperatorTest, GroupBy_Count_withPredicate) {
  testAggregationOperatorWithGroupBy<LongType>(
      "DoubleType", AggregationID::kCount, kWithPredicate, GroupBy_CountCheck<kWithPredicate>, kGroupByWidth);
}

namespace {

void GroupBy_NoCall(int group_by_id, const TypedValue &value1, const TypedValue &value2) {
  EXPECT_TRUE(false);
}

TEST_F(AggregationOperatorTest, GroupBy_Sum_LongType_zeroRows) {
  testAggregationOperatorWithGroupBy<LongType>(
      "LongType", AggregationID::kSum, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Sum_IntType_zeroRows) {
  testAggregationOperatorWithGroupBy<LongType>(
      "IntType", AggregationID::kSum, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Sum_FloatType_zeroRows) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "FloatType", AggregationID::kSum, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Sum_DoubleType_zeroRows) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "DoubleType", AggregationID::kSum, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Avg_IntType_zeroRows) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "IntType", AggregationID::kAvg, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Avg_LongType_zeroRows) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "LongType", AggregationID::kAvg, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Avg_FloatType_zeroRows) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "FloatType", AggregationID::kAvg, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Avg_DoubleType_zeroRows) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "DoubleType", AggregationID::kAvg, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Max_IntType_zeroRows) {
  testAggregationOperatorWithGroupBy<IntType>(
      "IntType", AggregationID::kMax, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Max_LongType_zeroRows) {
  testAggregationOperatorWithGroupBy<LongType>(
      "LongType", AggregationID::kMax, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Max_FloatType_zeroRows) {
  testAggregationOperatorWithGroupBy<FloatType>(
      "FloatType", AggregationID::kMax, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Max_DoubleType_zeroRows) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "DoubleType", AggregationID::kMax, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Min_IntType_zeroRows) {
  testAggregationOperatorWithGroupBy<IntType>(
      "IntType", AggregationID::kMin, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Min_LongType_zeroRows) {
  testAggregationOperatorWithGroupBy<LongType>(
      "LongType", AggregationID::kMin, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Min_FloatType_zeroRows) {
  testAggregationOperatorWithGroupBy<FloatType>(
      "FloatType", AggregationID::kMin, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Min_DoubleType_zeroRows) {
  testAggregationOperatorWithGroupBy<DoubleType>(
      "DoubleType", AggregationID::kMin, kWithPredicate, GroupBy_NoCall, 0, -1);
}

TEST_F(AggregationOperatorTest, GroupBy_Count_zeroRows) {
  testAggregationOperatorWithGroupBy<LongType>(
      "DoubleType", AggregationID::kCount, kWithPredicate, GroupBy_NoCall, 0, -1);
}

}  // namespace
}  // namespace quickstep

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  // Honor FLAGS_buffer_pool_slots in StorageManager.
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
