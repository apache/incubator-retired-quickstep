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

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/SortMergeRunOperator.hpp"
#include "relational_operators/SortMergeRunOperatorHelpers.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/BasicColumnStoreValueAccessor.hpp"
#include "storage/CompressedColumnStoreValueAccessor.hpp"
#include "storage/CompressedPackedRowStoreValueAccessor.hpp"
#include "storage/CountedReference.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/InsertDestination.pb.h"
#include "storage/PackedRowStoreValueAccessor.hpp"
#include "storage/SplitRowStoreValueAccessor.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "types/IntType.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/PtrVector.hpp"
#include "utility/SortConfiguration.hpp"
#include "utility/SortConfiguration.pb.h"

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

#include "tmb/id_typedefs.h"
#include "tmb/tagged_message.h"

using std::move;
using std::unique_ptr;
using std::vector;

namespace quickstep {

namespace {

constexpr const std::size_t kOpIndex = 0;

// Helper struct for test tuple that will that will be inserted and sorted.
class TestTuple {
 public:
  static const int kCol1Bits;
  static const int kCol2Bits;
  static const int kCol3Bits;

  // Intialize tuple based on seed number.
  explicit TestTuple(int num)
      : tid_(num),
        col1_(Bits(num, 3, kCol1Bits)),
        col2_(Bits(num, 6, kCol2Bits)),
        col3_(Bits(num, 1, kCol3Bits)) {}

  int tid_;  // Tuple-ID
  int col1_;  // Col-1
  int col2_;  // Col-2
  int col3_;  // Col-3

  static inline int Bits(int value, int offset, int length) {
    return (value >> offset) & (0xffff >> (16 - length));
  }
};

const int TestTuple::kCol1Bits = 5;
const int TestTuple::kCol2Bits = 2;
const int TestTuple::kCol3Bits = 3;

// To simplify testing nullable attributes, we create another meta tuple from
// column values from TestTuple struct. Column 1, 2, 3 are same as TestTuple.
// Columns 4, 5, 6 are same as 1, 2, 3 respectively when non-zero, and null
// otherwise. That is, i^th and (i + 3)^th columns are same when the value is
// non-zero. When the value of i^th column is zero, (i + 3)^th column is NULL.
// null_c{i} captures if column i was NULL in the tuple.
struct TestTupleAttrs {
  int c1, c2, c3;
  bool null_c4, null_c5, null_c6;
};

// Convert from TestTuple to TestTupleAttrs.
struct TestTupleAttrs TupleToTupleAttr(const Tuple &tuple) {
  auto attr = tuple.getAttributeValueVector();
  struct TestTupleAttrs out {};
  out.c1 = attr[0].getLiteral<int>();
  out.c2 = attr[1].getLiteral<int>();
  out.c3 = attr[2].getLiteral<int>();
  out.null_c4 = attr[3].isNull();
  out.null_c5 = attr[4].isNull();
  out.null_c6 = attr[5].isNull();
  return out;
}

}  // namespace

namespace merge_run_operator {

// Test Run class.
class RunTest : public ::testing::Test {
 public:
  static const tuple_id kNumTuples;
  static const tuple_id kNumTuplesPerBlock;

 protected:
  static const relation_id kTableId = 100;
  static const relation_id kResultTableId = kTableId + 1;
  static const char kTableName[];
  static const char kStoragePath[];

  virtual void SetUp() {
    // Initialize the TMB, register this thread as sender and receiver for
    // appropriate types of messages.
    bus_.Initialize();

    foreman_client_id_ = bus_.Connect();
    bus_.RegisterClientAsReceiver(foreman_client_id_, kCatalogRelationNewBlockMessage);
    bus_.RegisterClientAsReceiver(foreman_client_id_, kDataPipelineMessage);

    const tmb::client_id worker_thread_client_id = bus_.Connect();
    bus_.RegisterClientAsSender(worker_thread_client_id, kCatalogRelationNewBlockMessage);
    bus_.RegisterClientAsSender(worker_thread_client_id, kDataPipelineMessage);

    thread_id_map_ = ClientIDMap::Instance();
    // Usually the worker thread makes the following call. In this test setup,
    // we don't have a worker thread hence we have to explicitly make the call.
    thread_id_map_->addValue(worker_thread_client_id);

    storage_manager_.reset(new StorageManager(kStoragePath));
    table_.reset(new CatalogRelation(nullptr, kTableName, kTableId));
    const Type &int_type = IntType::InstanceNonNullable();
    table_->addAttribute(new CatalogAttribute(table_.get(), "col-1", int_type));
    table_->addAttribute(new CatalogAttribute(table_.get(), "col-2", int_type));
    table_->addAttribute(new CatalogAttribute(table_.get(), "col-3", int_type));
    col1_ = table_->getAttributeByName("col-1")->getID();
    col2_ = table_->getAttributeByName("col-2")->getID();
    col3_ = table_->getAttributeByName("col-3")->getID();
    table_->setDefaultStorageBlockLayout(StorageBlockLayout::GenerateDefaultLayout(*table_, false));

    insert_destination_.reset(
        new BlockPoolInsertDestination(*table_,
                                       nullptr,
                                       storage_manager_.get(),
                                       kOpIndex,
                                       foreman_client_id_,
                                       &bus_));
  }

  virtual void TearDown() {
    // Usually the worker thread makes the following call. In this test setup,
    // we don't have a worker thread hence we have to explicitly make the call.
    thread_id_map_->removeValue();

    // Drop blocks from relations and InsertDestination.
    const vector<block_id> tmp_blocks = insert_destination_->getTouchedBlocks();
    for (const block_id block : tmp_blocks) {
      storage_manager_->deleteBlockOrBlobFile(block);
    }

    const vector<block_id> blocks = table_->getBlocksSnapshot();
    for (const block_id block : blocks) {
      storage_manager_->deleteBlockOrBlobFile(block);
    }
  }

  // Helper method to insert test tuples.
  Tuple *createTuple(tuple_id id) {
    std::vector<TypedValue> values;
    values.emplace_back(static_cast<int>(id));
    values.emplace_back(static_cast<int>(id));
    values.emplace_back(static_cast<int>(id));
    return new Tuple(std::move(values));
  }

  std::unique_ptr<StorageManager> storage_manager_;
  std::unique_ptr<CatalogRelation> table_;
  std::unique_ptr<InsertDestination> insert_destination_;
  attribute_id col1_;
  attribute_id col2_;
  attribute_id col3_;
  merge_run_operator::Run run_;

  MessageBusImpl bus_;
  tmb::client_id foreman_client_id_;

  // This map is needed for InsertDestination and some operators that send
  // messages to Foreman directly. To know the reason behind the design of this
  // map, see the note in InsertDestination.hpp.
  ClientIDMap *thread_id_map_;
};

const char RunTest::kTableName[] = "table";
const char RunTest::kStoragePath[] = "./sort_merge_run_operator_test_data";
const tuple_id RunTest::kNumTuples = 100;
const tuple_id RunTest::kNumTuplesPerBlock = 10;

// Test creating an empty run.
TEST_F(RunTest, RunCreatorEmptyRun) {
  {
    RunCreator run_creator(&run_, insert_destination_.get());
    // Need run_creator to destruct to finalize the output run.
  }
  EXPECT_EQ(0u, run_.size());
}

// Test creating a run with few tuples.
TEST_F(RunTest, RunCreatorFewTuples) {
  tuple_id max_tuples = 10;

  {
    RunCreator run_creator(&run_, insert_destination_.get());
    std::unique_ptr<Tuple> tuple;
    for (tuple_id i = 0; i < max_tuples; ++i) {
      tuple.reset(createTuple(i));
      run_creator.appendTuple(*tuple);
    }
    EXPECT_EQ(1u, run_.size());
    // Need run_creator to destruct to finalize the output run.
  }


  BlockReference block(storage_manager_->getBlock(run_.back(), *table_));
  std::unique_ptr<ValueAccessor> accessor(
      block->getTupleStorageSubBlock().createValueAccessor());

  tuple_id num_tuples = 0;
  InvokeOnValueAccessorNotAdapter(
      accessor.get(),
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    while (accessor->next()) {
      const tuple_id current_num_tuples = num_tuples;
      EXPECT_EQ(static_cast<int>(current_num_tuples),
                accessor->getTypedValue(col1_).template getLiteral<int>());
      ++num_tuples;
    }
  });
  EXPECT_EQ(max_tuples, num_tuples);
}

// Test creating a run with multiple blocks.
TEST_F(RunTest, RunCreatorMultipleBlocks) {
  std::size_t max_blocks = 3;
  tuple_id max_tuples = 0;

  {
    RunCreator run_creator(&run_, insert_destination_.get());
    std::unique_ptr<Tuple> tuple;
    std::size_t blocks = 0;
    while (blocks < max_blocks) {
      tuple.reset(createTuple(max_tuples));
      if (run_creator.appendTuple(*tuple)) {
        ++blocks;
      }
      ++max_tuples;
    }
    EXPECT_EQ(max_blocks + 1, run_.size());
    // Need run_creator to destruct to finalize the output run.
  }

  tuple_id num_tuples = 0;
  for (block_id bid : run_) {
    BlockReference block(storage_manager_->getBlock(bid, *table_));
    std::unique_ptr<ValueAccessor> accessor(
        block->getTupleStorageSubBlock().createValueAccessor());

    InvokeOnValueAccessorNotAdapter(
        accessor.get(),
        [&](auto *accessor) -> void {  // NOLINT(build/c++11)
      while (accessor->next()) {
        const tuple_id current_num_tuples = num_tuples;
        EXPECT_EQ(static_cast<int>(current_num_tuples),
                  accessor->getTypedValue(col1_).template getLiteral<int>());
        ++num_tuples;
      }
    });  // NOLINT(whitespace/parens)
  }
  EXPECT_EQ(max_tuples, num_tuples);
}

// Test run iterator to see if tuples are accessed in the same order as stored.
TEST_F(RunTest, IterateTuples) {
  std::unique_ptr<Tuple> tuple;
  MutableBlockReference storage_block;
  for (tuple_id i = 0; i < kNumTuples; i += kNumTuplesPerBlock) {
    // Create block
    block_id block_id = storage_manager_->createBlock(*table_, table_->getDefaultStorageBlockLayout());
    storage_block = storage_manager_->getBlockMutable(block_id, *table_);
    table_->addBlock(block_id);
    run_.push_back(block_id);

    // Insert tuples
    tuple_id block_bound = i + kNumTuplesPerBlock < kNumTuples
                               ? i + kNumTuplesPerBlock
                               : kNumTuples;
    for (tuple_id tid = i; tid < block_bound; ++tid) {
      tuple.reset(createTuple(tid));
      EXPECT_TRUE(storage_block->insertTupleInBatch(*tuple));
    }
    storage_block->rebuild();

    // Create empty block
    block_id = storage_manager_->createBlock(*table_, table_->getDefaultStorageBlockLayout());
    storage_block = storage_manager_->getBlockMutable(block_id, *table_);
    table_->addBlock(block_id);
    run_.push_back(block_id);
    storage_block->rebuild();
  }

  BlockReference block(storage_manager_->getBlock(run_.front(), *table_));
  std::unique_ptr<ValueAccessor> dummy_accessor(
      block->getTupleStorageSubBlock().createValueAccessor());
  tuple_id num_tuples = 0;
  InvokeOnValueAccessorNotAdapter(
      dummy_accessor.get(),
      [&](auto *dummy_accessor) -> void {  // NOLINT(build/c++11)
    RunIterator<
        typename std::remove_reference<decltype(*dummy_accessor)>::type>
        run_iterator(run_, storage_manager_.get(), *table_);
    while (run_iterator.next()) {
      const tuple_id current_num_tuples = num_tuples;
      EXPECT_EQ(static_cast<int>(current_num_tuples),
                run_iterator.getValueAccessor()
                    ->getTypedValue(col1_).template getLiteral<int>());
      ++num_tuples;
    }
  });
  EXPECT_EQ(kNumTuples, num_tuples);
}

// Test RunMerger class.
class RunMergerTest : public ::testing::Test {
 public:
  static const std::size_t kNumTuplesPerBlock;
  static const tuple_id kNumBlocksPerRun;
  static const std::size_t kNumRuns;
  static const tuple_id kNumTuples;
  static const std::size_t kTopK;

 protected:
  static const relation_id kTableId = 100;
  static const relation_id kResultTableId = kTableId + 1;
  static const char kTableName[];
  static const char kStoragePath[];

  virtual void SetUp() {
    // Initialize the TMB, register this thread as sender and receiver for
    // appropriate types of messages.
    bus_.Initialize();
    foreman_client_id_ = bus_.Connect();
    bus_.RegisterClientAsReceiver(foreman_client_id_, kDataPipelineMessage);
    bus_.RegisterClientAsReceiver(foreman_client_id_, kCatalogRelationNewBlockMessage);

    const tmb::client_id worker_thread_client_id = bus_.Connect();
    bus_.RegisterClientAsSender(worker_thread_client_id, kCatalogRelationNewBlockMessage);
    bus_.RegisterClientAsSender(worker_thread_client_id, kDataPipelineMessage);

    thread_id_map_ = ClientIDMap::Instance();
    // Usually the worker thread makes the following call. In this test setup,
    // we don't have a worker thread hence we have to explicitly make the call.
    thread_id_map_->addValue(worker_thread_client_id);

    storage_manager_.reset(new StorageManager(kStoragePath));

    // Initialize table attributes.
    table_.reset(new CatalogRelation(nullptr, kTableName, kTableId));
    const Type &int_type = IntType::InstanceNonNullable();
    const Type &null_int_type = IntType::InstanceNullable();
    table_->addAttribute(new CatalogAttribute(table_.get(), "col-1", int_type));
    table_->addAttribute(new CatalogAttribute(table_.get(), "col-2", int_type));
    table_->addAttribute(new CatalogAttribute(table_.get(), "col-3", int_type));
    table_->addAttribute(new CatalogAttribute(table_.get(), "null-col-1", null_int_type));
    table_->addAttribute(new CatalogAttribute(table_.get(), "null-col-2", null_int_type));
    table_->addAttribute(new CatalogAttribute(table_.get(), "null-col-3", null_int_type));
    table_->addAttribute(new CatalogAttribute(table_.get(), "tid", null_int_type));
    col1_ = table_->getAttributeByName("col-1")->getID();
    col2_ = table_->getAttributeByName("col-2")->getID();
    col3_ = table_->getAttributeByName("col-3")->getID();
    null_col1_ = table_->getAttributeByName("null-col-1")->getID();
    null_col2_ = table_->getAttributeByName("null-col-2")->getID();
    null_col3_ = table_->getAttributeByName("null-col-3")->getID();
    tid_col_ = table_->getAttributeByName("tid")->getID();
    table_->setDefaultStorageBlockLayout(StorageBlockLayout::GenerateDefaultLayout(*table_, false));

    insert_destination_.reset(
        new BlockPoolInsertDestination(*table_,
                                       nullptr,
                                       storage_manager_.get(),
                                       kOpIndex,
                                       foreman_client_id_,
                                       &bus_));
  }

  virtual void TearDown() {
    // Usually the worker thread makes the following call. In this test setup,
    // we don't have a worker thread hence we have to explicitly make the call.
    thread_id_map_->removeValue();

    // Drop blocks from relations and InsertDestination.
    const vector<block_id> tmp_blocks = insert_destination_->getTouchedBlocks();
    for (const block_id block : tmp_blocks) {
      storage_manager_->deleteBlockOrBlobFile(block);
    }

    const vector<block_id> blocks = table_->getBlocksSnapshot();
    for (const block_id block : blocks) {
      storage_manager_->deleteBlockOrBlobFile(block);
    }
  }

  // Helper method to create test tuples.
  Tuple *createTuple(tuple_id id) {
    std::vector<TypedValue> values;
    TestTuple tuple(id);
    values.emplace_back(static_cast<int>(tuple.col1_));
    values.emplace_back(static_cast<int>(tuple.col2_));
    values.emplace_back(static_cast<int>(tuple.col3_));
    if (!tuple.col1_) {
      values.emplace_back(kInt);
    } else {
      values.emplace_back(static_cast<int>(tuple.col1_));
    }
    if (!tuple.col2_) {
      values.emplace_back(kInt);
    } else {
      values.emplace_back(static_cast<int>(tuple.col2_));
    }
    if (!tuple.col3_) {
      values.emplace_back(kInt);
    } else {
      values.emplace_back(static_cast<int>(tuple.col3_));
    }
    values.emplace_back(static_cast<int>(tuple.tid_));
    return new Tuple(std::move(values));
  }

  // Method to runs which are sorted based on supplied Tuple comparator.
  template <typename ComparatorT>
  void createRuns(const ComparatorT &comparator, bool maintain_top_k = false) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, (1 << 8) - 1);
    std::unique_ptr<Tuple> tuple;
    std::vector<TestTuple> top_k_tuples;
    MutableBlockReference storage_block;

    auto sort_comparator =
        [this, comparator](const TestTuple &l, const TestTuple &r) -> bool {
          std::unique_ptr<Tuple> left(createTuple(l.tid_)),
              right(createTuple(r.tid_));
          return comparator(*left, *right);
        };

    input_runs_.clear();
    for (std::size_t run_id = 0; run_id < kNumRuns; ++run_id) {
      std::vector<TestTuple> tuples;
      input_runs_.emplace_back();
      for (std::size_t i = 0; i < kNumBlocksPerRun * kNumTuplesPerBlock; ++i) {
        tuples.emplace_back(dist(gen));
      }

      // Sort tuples before creating the run.
      std::sort(tuples.begin(), tuples.end(), sort_comparator);

      // Maintain top-K to test top-K output after merge.
      if (maintain_top_k) {
        top_k_tuples.insert(top_k_tuples.end(),
                            tuples.begin(),
                            tuples.begin() + std::min(tuples.size(), kTopK));
        std::partial_sort(
            top_k_tuples.begin(),
            top_k_tuples.begin() + std::min(top_k_tuples.size(), kTopK),
            top_k_tuples.end(),
            sort_comparator);
        if (top_k_tuples.size() > kTopK) {
          top_k_tuples.erase(top_k_tuples.begin() + kTopK, top_k_tuples.end());
        }
      }

      // Insert sorted tuples in run.
      for (std::size_t id = 0; id < kNumBlocksPerRun; ++id) {
        std::string run_name = "run[" + std::to_string(run_id) + "]";
        // Create block.
        block_id block_id = storage_manager_->createBlock(*table_, table_->getDefaultStorageBlockLayout());
        storage_block = storage_manager_->getBlockMutable(block_id, *table_);
        table_->addBlock(block_id);
        input_runs_.back().push_back(block_id);
        for (std::size_t tid = 0; tid < kNumTuplesPerBlock; ++tid) {
          // Insert tuples.
          tuple.reset(createTuple(tuples[id * kNumTuplesPerBlock + tid].tid_));
          printTuple(run_name.c_str(), *tuple);
          EXPECT_TRUE(storage_block->insertTupleInBatch(*tuple));
        }
        storage_block->rebuild();
      }
    }

    if (maintain_top_k) {
      // Last expected ORDER BY key of the sort.
      last_expected_tuple_.reset(createTuple(top_k_tuples.back().tid_));
    }

    // Insert empty run.
    input_runs_.emplace_back();
  }

  // Debug print method.
  void printTuple(const char *prefix, const Tuple &tuple) {
    if (VLOG_IS_ON(2)) {
      std::ostringstream out;
      out << prefix << ' ';
      CatalogRelation::const_iterator attr_it = table_->begin();
      Tuple::const_iterator value_it = tuple.begin();
      for (; attr_it != table_->end(); ++attr_it, ++value_it) {
        if (value_it->isNull()) {
          out << "NULL";
        } else {
          out << attr_it->getType().printValueToString(*value_it);
        }
        out << '|';
      }
      out << '\n';
      VLOG(2) << out.str();
    }
  }

  // Merge runs.
  void mergeRuns(const std::vector<attribute_id> &attrs,
                 vector<bool> &&ordering,
                 vector<bool> &&null_ordering,
                 const std::size_t top_k = 0) {
    PtrVector<Scalar> order_by;
    for (const attribute_id attr : attrs) {
      order_by.push_back(new ScalarAttribute(*table_->getAttributeById(attr)));
    }
    SortConfiguration sort_config(order_by, move(ordering), move(null_ordering));
    RunMerger merge(sort_config,
                    std::move(input_runs_),
                    top_k,
                    *table_,
                    insert_destination_.get(),
                    0,
                    storage_manager_.get());
    merge.doMerge();
    output_run_ = std::move(*merge.getOutputRunMutable());
    EXPECT_GT(output_run_.size(), 0u);
  }

  // Check if the merged run is sorted.
  template <typename ComparatorT>
  void checkOutputRun(const ComparatorT comparator,
                      const tuple_id expected_num_tuples = kNumTuples) {
    BlockReference block(
        storage_manager_->getBlock(output_run_.front(), *table_));
    std::unique_ptr<ValueAccessor> dummy_accessor(
        block->getTupleStorageSubBlock().createValueAccessor());
    tuple_id num_tuples = 0;
    std::unique_ptr<Tuple> prev, current;
    InvokeOnValueAccessorNotAdapter(
        dummy_accessor.get(),
        [&](auto *dummy_accessor) -> void {  // NOLINT(build/c++11)
      RunIterator<
          typename std::remove_reference<decltype(*dummy_accessor)>::type>
          iterator(output_run_, storage_manager_.get(), *table_);
      while (iterator.next()) {
        prev = std::move(current);
        current.reset(iterator.getValueAccessor()->getTuple());
        ++num_tuples;
        if (num_tuples > 1) {
          EXPECT_FALSE(comparator(*current, *prev));
        }
        std::unique_ptr<Tuple> tuple(iterator.getValueAccessor()->getTuple());
        this->printTuple(">", *tuple);
      }
    });
    EXPECT_EQ(expected_num_tuples, num_tuples);
    last_actual_tuple_ = std::move(current);
  }

  // Check attribute value of last expected tuple and last output tuple.
  void checkLastTupleAttrValueIsEqual(const attribute_id id) {
    printTuple("Expected:", *last_expected_tuple_);
    printTuple("Actual:  ", *last_actual_tuple_);
    EXPECT_EQ(last_expected_tuple_->getAttributeValue(id).isNull(),
              last_actual_tuple_->getAttributeValue(id).isNull());
    if (!last_expected_tuple_->getAttributeValue(id).isNull() &&
        !last_actual_tuple_->getAttributeValue(id).isNull()) {
      EXPECT_TRUE(last_expected_tuple_->getAttributeValue(id).fastEqualCheck(
          last_actual_tuple_->getAttributeValue(id)))
          << "Expected: "
          << last_expected_tuple_->getAttributeValue(id).getLiteral<int>()
          << "; Actual: "
          << last_actual_tuple_->getAttributeValue(id).getLiteral<int>();
    }
  }

  std::unique_ptr<StorageManager> storage_manager_;
  std::unique_ptr<CatalogRelation> table_;
  std::unique_ptr<InsertDestination> insert_destination_;
  attribute_id col1_;
  attribute_id col2_;
  attribute_id col3_;
  attribute_id null_col1_;
  attribute_id null_col2_;
  attribute_id null_col3_;
  attribute_id tid_col_;
  merge_run_operator::Run output_run_;
  std::vector<merge_run_operator::Run> input_runs_;
  std::unique_ptr<Tuple> last_actual_tuple_;
  std::unique_ptr<Tuple> last_expected_tuple_;  // This is only populated for top-k tests.

  MessageBusImpl bus_;
  tmb::client_id foreman_client_id_;

  // This map is needed for InsertDestination and some operators that send
  // messages to Foreman directly. To know the reason behind the design of this
  // map, see the note in InsertDestination.hpp.
  ClientIDMap *thread_id_map_;
};

const char RunMergerTest::kTableName[] = "table";
const char RunMergerTest::kStoragePath[] = "./sort_merge_run_operator_test_data";
const std::size_t RunMergerTest::kNumTuplesPerBlock = 10;
const tuple_id RunMergerTest::kNumBlocksPerRun = 10;
const std::size_t RunMergerTest::kNumRuns = 10;
const tuple_id RunMergerTest::kNumTuples =
    RunMergerTest::kNumTuplesPerBlock * RunMergerTest::kNumBlocksPerRun * RunMergerTest::kNumRuns;
const std::size_t RunMergerTest::kTopK = 5;

namespace {

// Comparator for col-1 ASC.
auto kCol1AscComparator = [](const Tuple &left, const Tuple &right) -> bool {
  TestTupleAttrs l = TupleToTupleAttr(left);
  TestTupleAttrs r = TupleToTupleAttr(right);
  if (l.c1 < r.c1) {
    return true;
  }
  return false;
};

// Comparator for col-1 DESC.
auto kCol1DescComparator = [](const Tuple &left, const Tuple &right) -> bool {
  TestTupleAttrs l = TupleToTupleAttr(left);
  TestTupleAttrs r = TupleToTupleAttr(right);
  if (l.c1 > r.c1) {
    return true;
  }
  return false;
};

// Comparator for null-col-1 ASC NULLS LAST.
auto kNullLastCol1AscComparator =
    [](const Tuple &left, const Tuple &right) -> bool {
      TestTupleAttrs l = TupleToTupleAttr(left);
      TestTupleAttrs r = TupleToTupleAttr(right);
      l.c1 = l.null_c4 ? std::numeric_limits<int>::max() : l.c1;
      r.c1 = r.null_c4 ? std::numeric_limits<int>::max() : r.c1;
      if (l.c1 < r.c1) {
        return true;
      }
      return false;
    };

// Comparator for null-col-1 ASC NULLS FIRST.
auto kNullFirstCol1AscComparator =
    [](const Tuple &left, const Tuple &right) -> bool {
      TestTupleAttrs l = TupleToTupleAttr(left);
      TestTupleAttrs r = TupleToTupleAttr(right);
      l.c1 = l.null_c4 ? std::numeric_limits<int>::min() : l.c1;
      r.c1 = r.null_c4 ? std::numeric_limits<int>::min() : r.c1;
      if (l.c1 < r.c1) {
        return true;
      }
      return false;
    };

// Comparator for null-col-1 DESC NULLS LAST.
auto kNullLastCol1DescComparator =
    [](const Tuple &left, const Tuple &right) -> bool {
      TestTupleAttrs l = TupleToTupleAttr(left);
      TestTupleAttrs r = TupleToTupleAttr(right);
      l.c1 = l.null_c4 ? std::numeric_limits<int>::min() : l.c1;
      r.c1 = r.null_c4 ? std::numeric_limits<int>::min() : r.c1;
      if (l.c1 > r.c1) {
        return true;
      }
      return false;
    };

// Comparator for null-col-1 DESC NULLS FIRST.
auto kNullFirstCol1DescComparator =
    [](const Tuple &left, const Tuple &right) -> bool {
      TestTupleAttrs l = TupleToTupleAttr(left);
      TestTupleAttrs r = TupleToTupleAttr(right);
      l.c1 = l.null_c4 ? std::numeric_limits<int>::max() : l.c1;
      r.c1 = r.null_c4 ? std::numeric_limits<int>::max() : r.c1;
      if (l.c1 > r.c1) {
        return true;
      }
      return false;
    };

// Comparator for col-1 ASC, col-2 ASC, col-3 ASC.
auto kCol1Col2Col3AscComparator =
    [](const Tuple &left, const Tuple &right) -> bool {
      TestTupleAttrs l = TupleToTupleAttr(left);
      TestTupleAttrs r = TupleToTupleAttr(right);
      if (l.c1 < r.c1) {
        return true;
      } else if (l.c1 == r.c1) {
        if (l.c2 < r.c2) {
          return true;
        } else if (l.c2 == r.c2) {
          if (l.c3 < r.c3) {
            return true;
          }
        }
      }
      return false;
    };

// Comparator for col-1 DESC, col-2 DESC, col-3 DESC.
auto kCol1Col2Col3DescComparator =
    [](const Tuple &left, const Tuple &right) -> bool {
      TestTupleAttrs l = TupleToTupleAttr(left);
      TestTupleAttrs r = TupleToTupleAttr(right);
      if (l.c1 > r.c1) {
        return true;
      } else if (l.c1 == r.c1) {
        if (l.c2 > r.c2) {
          return true;
        } else if (l.c2 == r.c2) {
          if (l.c3 > r.c3) {
            return true;
          }
        }
      }
      return false;
    };

// Comparator for col-1 ASC NULLS LAST, col-2 ASC NULLS LAST, col-3 ASC NULLS
// LAST.
auto kNullLastCol1Col2Col3AscComparator =
    [](const Tuple &left, const Tuple &right) -> bool {
      TestTupleAttrs l = TupleToTupleAttr(left);
      TestTupleAttrs r = TupleToTupleAttr(right);
      l.c1 = l.null_c4 ? std::numeric_limits<int>::max() : l.c1;
      l.c2 = l.null_c5 ? std::numeric_limits<int>::max() : l.c2;
      l.c3 = l.null_c6 ? std::numeric_limits<int>::max() : l.c3;
      r.c1 = r.null_c4 ? std::numeric_limits<int>::max() : r.c1;
      r.c2 = r.null_c5 ? std::numeric_limits<int>::max() : r.c2;
      r.c3 = r.null_c6 ? std::numeric_limits<int>::max() : r.c3;
      if (l.c1 < r.c1) {
        return true;
      } else if (l.c1 == r.c1) {
        if (l.c2 < r.c2) {
          return true;
        } else if (l.c2 == r.c2) {
          if (l.c3 < r.c3) {
            return true;
          }
        }
      }
      return false;
    };

// Comparator for col-1 DESC NULLS LAST, col-2 DESC NULLS LAST, col-3 DESC NULLS
// LAST.
auto kNullLastCol1Col2Col3DescComparator =
    [](const Tuple &left, const Tuple &right) -> bool {
      TestTupleAttrs l = TupleToTupleAttr(left);
      TestTupleAttrs r = TupleToTupleAttr(right);
      l.c1 = l.null_c4 ? std::numeric_limits<int>::min() : l.c1;
      l.c2 = l.null_c5 ? std::numeric_limits<int>::min() : l.c2;
      l.c3 = l.null_c6 ? std::numeric_limits<int>::min() : l.c3;
      r.c1 = r.null_c4 ? std::numeric_limits<int>::min() : r.c1;
      r.c2 = r.null_c5 ? std::numeric_limits<int>::min() : r.c2;
      r.c3 = r.null_c6 ? std::numeric_limits<int>::min() : r.c3;
      if (l.c1 > r.c1) {
        return true;
      } else if (l.c1 == r.c1) {
        if (l.c2 > r.c2) {
          return true;
        } else if (l.c2 == r.c2) {
          if (l.c3 > r.c3) {
            return true;
          }
        }
      }
      return false;
    };

// Comparator for col-1 ASC NULLS FIRST, col-2 ASC NULLS FIRST, col-3 ASC NULLS
// FIRST.
auto kNullFirstCol1Col2Col3AscComparator =
    [](const Tuple &left, const Tuple &right) -> bool {
      TestTupleAttrs l = TupleToTupleAttr(left);
      TestTupleAttrs r = TupleToTupleAttr(right);
      l.c1 = l.null_c4 ? std::numeric_limits<int>::min() : l.c1;
      l.c2 = l.null_c5 ? std::numeric_limits<int>::min() : l.c2;
      l.c3 = l.null_c6 ? std::numeric_limits<int>::min() : l.c3;
      r.c1 = r.null_c4 ? std::numeric_limits<int>::min() : r.c1;
      r.c2 = r.null_c5 ? std::numeric_limits<int>::min() : r.c2;
      r.c3 = r.null_c6 ? std::numeric_limits<int>::min() : r.c3;
      if (l.c1 < r.c1) {
        return true;
      } else if (l.c1 == r.c1) {
        if (l.c2 < r.c2) {
          return true;
        } else if (l.c2 == r.c2) {
          if (l.c3 < r.c3) {
            return true;
          }
        }
      }
      return false;
    };

// Comparator for col-1 DESC NULLS FIRST, col-2 DESC NULLS FIRST, col-3 DESC
// NULLS FIRST.
auto kNullFirstCol1Col2Col3DescComparator =
    [](const Tuple &left, const Tuple &right) -> bool {
      TestTupleAttrs l = TupleToTupleAttr(left);
      TestTupleAttrs r = TupleToTupleAttr(right);
      l.c1 = l.null_c4 ? std::numeric_limits<int>::max() : l.c1;
      l.c2 = l.null_c5 ? std::numeric_limits<int>::max() : l.c2;
      l.c3 = l.null_c6 ? std::numeric_limits<int>::max() : l.c3;
      r.c1 = r.null_c4 ? std::numeric_limits<int>::max() : r.c1;
      r.c2 = r.null_c5 ? std::numeric_limits<int>::max() : r.c2;
      r.c3 = r.null_c6 ? std::numeric_limits<int>::max() : r.c3;
      if (l.c1 > r.c1) {
        return true;
      } else if (l.c1 == r.c1) {
        if (l.c2 > r.c2) {
          return true;
        } else if (l.c2 == r.c2) {
          if (l.c3 > r.c3) {
            return true;
          }
        }
      }
      return false;
    };

// Comparator for col-1 ASC NULLS FIRST, col-2 DESC NULLS LAST, col-3 ASC NULLS
// LAST.
auto kMixedNullFLLCol1Col2Col3Comparator =
    [](const Tuple &left, const Tuple &right) -> bool {
      TestTupleAttrs l = TupleToTupleAttr(left);
      TestTupleAttrs r = TupleToTupleAttr(right);
      l.c1 = l.null_c4 ? std::numeric_limits<int>::min() : l.c1;
      l.c2 = l.null_c5 ? std::numeric_limits<int>::min() : l.c2;
      l.c3 = l.null_c6 ? std::numeric_limits<int>::max() : l.c3;
      r.c1 = r.null_c4 ? std::numeric_limits<int>::min() : r.c1;
      r.c2 = r.null_c5 ? std::numeric_limits<int>::min() : r.c2;
      r.c3 = r.null_c6 ? std::numeric_limits<int>::max() : r.c3;
      if (l.c1 < r.c1) {
        return true;
      } else if (l.c1 == r.c1) {
        if (l.c2 > r.c2) {
          return true;
        } else if (l.c2 == r.c2) {
          if (l.c3 < r.c3) {
            return true;
          }
        }
      }
      return false;
    };

}  // namespace

TEST_F(RunMergerTest, 1Column_NonNull_Asc) {
  std::vector<attribute_id> attrs{col1_};
  std::vector<bool> ordering{kSortAscending};
  std::vector<bool> null_ordering{kSortNullLast};

  createRuns(kCol1AscComparator);
  mergeRuns(attrs, move(ordering), move(null_ordering));
  checkOutputRun(kCol1AscComparator);
}

TEST_F(RunMergerTest, 1Column_NonNull_Asc_TopK) {
  std::vector<attribute_id> attrs{col1_};
  std::vector<bool> ordering{kSortAscending};
  std::vector<bool> null_ordering{kSortNullLast};

  createRuns(kCol1AscComparator, true);
  mergeRuns(attrs, move(ordering), move(null_ordering), RunMergerTest::kTopK);
  checkOutputRun(kCol1AscComparator, RunMergerTest::kTopK);
  checkLastTupleAttrValueIsEqual(col1_);
}

TEST_F(RunMergerTest, 1Column_NonNull_Desc) {
  std::vector<attribute_id> attrs{col1_};
  std::vector<bool> ordering{kSortDescending};
  std::vector<bool> null_ordering{kSortNullLast};

  createRuns(kCol1DescComparator);
  mergeRuns(attrs, move(ordering), move(null_ordering));
  checkOutputRun(kCol1DescComparator);
}

TEST_F(RunMergerTest, 1Column_NonNull_Desc_TopK) {
  std::vector<attribute_id> attrs{col1_};
  std::vector<bool> ordering{kSortDescending};
  std::vector<bool> null_ordering{kSortNullLast};

  createRuns(kCol1DescComparator, true);
  mergeRuns(attrs, move(ordering), move(null_ordering), RunMergerTest::kTopK);
  checkOutputRun(kCol1DescComparator, RunMergerTest::kTopK);
  checkLastTupleAttrValueIsEqual(col1_);
}

TEST_F(RunMergerTest, 1Column_NullLast_Asc) {
  std::vector<attribute_id> attrs{null_col1_};
  std::vector<bool> ordering{kSortAscending};
  std::vector<bool> null_ordering{kSortNullLast};

  createRuns(kNullLastCol1AscComparator);
  mergeRuns(attrs, move(ordering), move(null_ordering));
  checkOutputRun(kNullLastCol1AscComparator);
}

TEST_F(RunMergerTest, 1Column_NullLast_Asc_TopK) {
  std::vector<attribute_id> attrs{null_col1_};
  std::vector<bool> ordering{kSortAscending};
  std::vector<bool> null_ordering{kSortNullLast};

  createRuns(kNullLastCol1AscComparator, true);
  mergeRuns(attrs, move(ordering), move(null_ordering), RunMergerTest::kTopK);
  checkOutputRun(kNullLastCol1AscComparator, RunMergerTest::kTopK);
  checkLastTupleAttrValueIsEqual(null_col1_);
}

TEST_F(RunMergerTest, 1Column_NullFirst_Asc) {
  std::vector<attribute_id> attrs{null_col1_};
  std::vector<bool> ordering{kSortAscending};
  std::vector<bool> null_ordering{kSortNullFirst};

  createRuns(kNullFirstCol1AscComparator);
  mergeRuns(attrs, move(ordering), move(null_ordering));
  checkOutputRun(kNullFirstCol1AscComparator);
}

TEST_F(RunMergerTest, 1Column_NullFirst_Asc_TopK) {
  std::vector<attribute_id> attrs{null_col1_};
  std::vector<bool> ordering{kSortAscending};
  std::vector<bool> null_ordering{kSortNullFirst};

  createRuns(kNullFirstCol1AscComparator, true);
  mergeRuns(attrs, move(ordering), move(null_ordering), RunMergerTest::kTopK);
  checkOutputRun(kNullFirstCol1AscComparator, RunMergerTest::kTopK);
  checkLastTupleAttrValueIsEqual(null_col1_);
}

TEST_F(RunMergerTest, 1Column_NullLast_Desc) {
  std::vector<attribute_id> attrs{null_col1_};
  std::vector<bool> ordering{kSortDescending};
  std::vector<bool> null_ordering{kSortNullLast};

  createRuns(kNullLastCol1DescComparator);
  mergeRuns(attrs, move(ordering), move(null_ordering));
  checkOutputRun(kNullLastCol1DescComparator);
}

TEST_F(RunMergerTest, 1Column_NullLast_Desc_TopK) {
  std::vector<attribute_id> attrs{null_col1_};
  std::vector<bool> ordering{kSortDescending};
  std::vector<bool> null_ordering{kSortNullLast};

  createRuns(kNullLastCol1DescComparator, true);
  mergeRuns(attrs, move(ordering), move(null_ordering), RunMergerTest::kTopK);
  checkOutputRun(kNullLastCol1DescComparator, RunMergerTest::kTopK);
  checkLastTupleAttrValueIsEqual(null_col1_);
}

TEST_F(RunMergerTest, 1Column_NullFirst_Desc) {
  std::vector<attribute_id> attrs{null_col1_};
  std::vector<bool> ordering{kSortDescending};
  std::vector<bool> null_ordering{kSortNullFirst};

  createRuns(kNullFirstCol1DescComparator);
  mergeRuns(attrs, move(ordering), move(null_ordering));
  checkOutputRun(kNullFirstCol1DescComparator);
}

TEST_F(RunMergerTest, 1Column_NullFirst_Desc_TopK) {
  std::vector<attribute_id> attrs{null_col1_};
  std::vector<bool> ordering{kSortDescending};
  std::vector<bool> null_ordering{kSortNullFirst};

  createRuns(kNullFirstCol1DescComparator, true);
  mergeRuns(attrs, move(ordering), move(null_ordering), RunMergerTest::kTopK);
  checkOutputRun(kNullFirstCol1DescComparator, RunMergerTest::kTopK);
  checkLastTupleAttrValueIsEqual(null_col1_);
}

TEST_F(RunMergerTest, 3Column_NonNull_Asc) {
  std::vector<attribute_id> attrs{col1_, col2_, col3_};
  std::vector<bool> ordering{kSortAscending, kSortAscending, kSortAscending};
  std::vector<bool> null_ordering{kSortNullLast, kSortNullLast, kSortNullLast};

  createRuns(kCol1Col2Col3AscComparator);
  mergeRuns(attrs, move(ordering), move(null_ordering));
  checkOutputRun(kCol1Col2Col3AscComparator);
}

TEST_F(RunMergerTest, 3Column_NonNull_Asc_TopK) {
  std::vector<attribute_id> attrs{col1_, col2_, col3_};
  std::vector<bool> ordering{kSortAscending, kSortAscending, kSortAscending};
  std::vector<bool> null_ordering{kSortNullLast, kSortNullLast, kSortNullLast};

  createRuns(kCol1Col2Col3AscComparator, true);
  mergeRuns(attrs, move(ordering), move(null_ordering), RunMergerTest::kTopK);
  checkOutputRun(kCol1Col2Col3AscComparator, RunMergerTest::kTopK);
  checkLastTupleAttrValueIsEqual(col1_);
  checkLastTupleAttrValueIsEqual(col2_);
  checkLastTupleAttrValueIsEqual(col3_);
}

TEST_F(RunMergerTest, 3Column_NonNull_Desc) {
  std::vector<attribute_id> attrs{col1_, col2_, col3_};
  std::vector<bool> ordering{kSortDescending, kSortDescending, kSortDescending};
  std::vector<bool> null_ordering{kSortNullLast, kSortNullLast, kSortNullLast};

  createRuns(kCol1Col2Col3DescComparator);
  mergeRuns(attrs, move(ordering), move(null_ordering));
  checkOutputRun(kCol1Col2Col3DescComparator);
}

TEST_F(RunMergerTest, 3Column_NonNull_Desc_TopK) {
  std::vector<attribute_id> attrs{col1_, col2_, col3_};
  std::vector<bool> ordering{kSortDescending, kSortDescending, kSortDescending};
  std::vector<bool> null_ordering{kSortNullLast, kSortNullLast, kSortNullLast};

  createRuns(kCol1Col2Col3DescComparator, true);
  mergeRuns(attrs, move(ordering), move(null_ordering), RunMergerTest::kTopK);
  checkOutputRun(kCol1Col2Col3DescComparator, RunMergerTest::kTopK);
  checkLastTupleAttrValueIsEqual(col1_);
  checkLastTupleAttrValueIsEqual(col2_);
  checkLastTupleAttrValueIsEqual(col3_);
}

TEST_F(RunMergerTest, 3Column_NullLast_Asc) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortAscending, kSortAscending, kSortAscending};
  std::vector<bool> null_ordering{kSortNullLast, kSortNullLast, kSortNullLast};

  createRuns(kNullLastCol1Col2Col3AscComparator);
  mergeRuns(attrs, move(ordering), move(null_ordering));
  checkOutputRun(kNullLastCol1Col2Col3AscComparator);
}

TEST_F(RunMergerTest, 3Column_NullLast_Asc_TopK) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortAscending, kSortAscending, kSortAscending};
  std::vector<bool> null_ordering{kSortNullLast, kSortNullLast, kSortNullLast};

  createRuns(kNullLastCol1Col2Col3AscComparator, true);
  mergeRuns(attrs, move(ordering), move(null_ordering), RunMergerTest::kTopK);
  checkOutputRun(kNullLastCol1Col2Col3AscComparator, RunMergerTest::kTopK);
  checkLastTupleAttrValueIsEqual(null_col1_);
  checkLastTupleAttrValueIsEqual(null_col2_);
  checkLastTupleAttrValueIsEqual(null_col3_);
}

TEST_F(RunMergerTest, 3Column_NullLast_Desc) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortDescending, kSortDescending, kSortDescending};
  std::vector<bool> null_ordering{kSortNullLast, kSortNullLast, kSortNullLast};

  createRuns(kNullLastCol1Col2Col3DescComparator);
  mergeRuns(attrs, move(ordering), move(null_ordering));
  checkOutputRun(kNullLastCol1Col2Col3DescComparator);
}

TEST_F(RunMergerTest, 3Column_NullLast_Desc_TopK) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortDescending, kSortDescending, kSortDescending};
  std::vector<bool> null_ordering{kSortNullLast, kSortNullLast, kSortNullLast};

  createRuns(kNullLastCol1Col2Col3DescComparator, true);
  mergeRuns(attrs, move(ordering), move(null_ordering), RunMergerTest::kTopK);
  checkOutputRun(kNullLastCol1Col2Col3DescComparator, RunMergerTest::kTopK);
  checkLastTupleAttrValueIsEqual(null_col1_);
  checkLastTupleAttrValueIsEqual(null_col2_);
  checkLastTupleAttrValueIsEqual(null_col3_);
}

TEST_F(RunMergerTest, 3Column_NullFirst_Asc) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortAscending, kSortAscending, kSortAscending};
  std::vector<bool> null_ordering{kSortNullFirst, kSortNullFirst, kSortNullFirst};

  createRuns(kNullFirstCol1Col2Col3AscComparator);
  mergeRuns(attrs, move(ordering), move(null_ordering));
  checkOutputRun(kNullFirstCol1Col2Col3AscComparator);
}

TEST_F(RunMergerTest, 3Column_NullFirst_Asc_TopK) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortAscending, kSortAscending, kSortAscending};
  std::vector<bool> null_ordering{kSortNullFirst, kSortNullFirst, kSortNullFirst};

  createRuns(kNullFirstCol1Col2Col3AscComparator, true);
  mergeRuns(attrs, move(ordering), move(null_ordering), RunMergerTest::kTopK);
  checkOutputRun(kNullFirstCol1Col2Col3AscComparator, RunMergerTest::kTopK);
  checkLastTupleAttrValueIsEqual(null_col1_);
  checkLastTupleAttrValueIsEqual(null_col2_);
  checkLastTupleAttrValueIsEqual(null_col3_);
}

TEST_F(RunMergerTest, 3Column_NullFirst_Desc) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortDescending, kSortDescending, kSortDescending};
  std::vector<bool> null_ordering{kSortNullFirst, kSortNullFirst, kSortNullFirst};

  createRuns(kNullFirstCol1Col2Col3DescComparator);
  mergeRuns(attrs, move(ordering), move(null_ordering));
  checkOutputRun(kNullFirstCol1Col2Col3DescComparator);
}

TEST_F(RunMergerTest, 3Column_NullFirst_Desc_TopK) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortDescending, kSortDescending, kSortDescending};
  std::vector<bool> null_ordering{kSortNullFirst, kSortNullFirst, kSortNullFirst};

  createRuns(kNullFirstCol1Col2Col3DescComparator, true);
  mergeRuns(attrs, move(ordering), move(null_ordering), RunMergerTest::kTopK);
  checkOutputRun(kNullFirstCol1Col2Col3DescComparator, RunMergerTest::kTopK);
  checkLastTupleAttrValueIsEqual(null_col1_);
  checkLastTupleAttrValueIsEqual(null_col2_);
  checkLastTupleAttrValueIsEqual(null_col3_);
}

TEST_F(RunMergerTest, 3Column_MixedNullOrdering_MixedOrdering) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortAscending, kSortDescending, kSortAscending};
  std::vector<bool> null_ordering{kSortNullFirst, kSortNullLast, kSortNullLast};

  createRuns(kMixedNullFLLCol1Col2Col3Comparator);
  mergeRuns(attrs, move(ordering), move(null_ordering));
  checkOutputRun(kMixedNullFLLCol1Col2Col3Comparator);
}

TEST_F(RunMergerTest, 3Column_MixedNullOrdering_MixedOrdering_TopK) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortAscending, kSortDescending, kSortAscending};
  std::vector<bool> null_ordering{kSortNullFirst, kSortNullLast, kSortNullLast};

  createRuns(kMixedNullFLLCol1Col2Col3Comparator, true);
  mergeRuns(attrs, move(ordering), move(null_ordering), RunMergerTest::kTopK);
  checkOutputRun(kMixedNullFLLCol1Col2Col3Comparator, RunMergerTest::kTopK);
  checkLastTupleAttrValueIsEqual(null_col1_);
  checkLastTupleAttrValueIsEqual(null_col2_);
  checkLastTupleAttrValueIsEqual(null_col3_);
}

}  // namespace merge_run_operator

// Test SortMergeRunOperator class. Since RunMerger is tested for various
// sort configurations, we only test if merge trees are computed and executed
// correctly keeping the sort configuration the same.
class SortMergeRunOperatorTest : public ::testing::Test {
 protected:
  static const relation_id kTableId = 100;
  static const relation_id kResultTableId = kTableId + 1;
  static const relation_id kRunTableId = kTableId + 2;
  static const char kTableName[];
  static const char kResultTableName[];
  static const char kRunTableName[];
  static const char kStoragePath[];
  static const char kDatabaseName[];

  virtual void SetUp() {
    // Initialize the TMB, register this thread as sender and receiver for
    // appropriate types of messages.
    bus_.Initialize();

    const tmb::client_id worker_thread_client_id = bus_.Connect();
    bus_.RegisterClientAsSender(worker_thread_client_id, kCatalogRelationNewBlockMessage);
    bus_.RegisterClientAsSender(worker_thread_client_id, kDataPipelineMessage);
    bus_.RegisterClientAsSender(worker_thread_client_id, kWorkOrderFeedbackMessage);

    thread_id_map_ = ClientIDMap::Instance();
    // Usually the worker thread makes the following call. In this test setup,
    // we don't have a worker thread hence we have to explicitly make the call.
    thread_id_map_->addValue(worker_thread_client_id);

    foreman_client_id_ = bus_.Connect();
    bus_.RegisterClientAsReceiver(foreman_client_id_, kCatalogRelationNewBlockMessage);
    bus_.RegisterClientAsReceiver(foreman_client_id_, kDataPipelineMessage);
    bus_.RegisterClientAsReceiver(foreman_client_id_, kWorkOrderFeedbackMessage);

    storage_manager_.reset(new StorageManager(kStoragePath));

    // Create a database.
    db_.reset(new CatalogDatabase(nullptr, kDatabaseName));

    // Create input_table_, owned by db_.
    input_table_ = createTable(kTableName, kTableId);
    db_->addRelation(input_table_);

    col1_ = input_table_->getAttributeByName("col-1")->getID();
    col2_ = input_table_->getAttributeByName("col-2")->getID();
    col3_ = input_table_->getAttributeByName("col-3")->getID();
    null_col1_ = input_table_->getAttributeByName("null-col-1")->getID();
    null_col2_ = input_table_->getAttributeByName("null-col-2")->getID();
    null_col3_ = input_table_->getAttributeByName("null-col-3")->getID();
    tid_col_ = input_table_->getAttributeByName("tid")->getID();

    // Create result_table_, owned by db_.
    result_table_ = createTable(kResultTableName, kResultTableId);
    const relation_id result_table_id = db_->addRelation(result_table_);

    ASSERT_EQ(col1_, result_table_->getAttributeByName("col-1")->getID());
    ASSERT_EQ(col2_, result_table_->getAttributeByName("col-2")->getID());
    ASSERT_EQ(col3_, result_table_->getAttributeByName("col-3")->getID());
    ASSERT_EQ(null_col1_, result_table_->getAttributeByName("null-col-1")->getID());
    ASSERT_EQ(null_col2_, result_table_->getAttributeByName("null-col-2")->getID());
    ASSERT_EQ(null_col3_, result_table_->getAttributeByName("null-col-3")->getID());
    ASSERT_EQ(tid_col_, result_table_->getAttributeByName("tid")->getID());

    // Setup the InsertDestination proto in the query context proto.
    insert_destination_index_ = query_context_proto_.insert_destinations_size();
    serialization::InsertDestination *insert_destination_proto = query_context_proto_.add_insert_destinations();

    insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
    insert_destination_proto->set_relation_id(result_table_id);
    insert_destination_proto->set_relational_op_index(kOpIndex);

    // Create run_table_, owned by db_.
    run_table_ = createTable(kRunTableName, kRunTableId);
    const relation_id run_table_id = db_->addRelation(run_table_);

    ASSERT_EQ(col1_, run_table_->getAttributeByName("col-1")->getID());
    ASSERT_EQ(col2_, run_table_->getAttributeByName("col-2")->getID());
    ASSERT_EQ(col3_, run_table_->getAttributeByName("col-3")->getID());
    ASSERT_EQ(null_col1_, run_table_->getAttributeByName("null-col-1")->getID());
    ASSERT_EQ(null_col2_, run_table_->getAttributeByName("null-col-2")->getID());
    ASSERT_EQ(null_col3_, run_table_->getAttributeByName("null-col-3")->getID());
    ASSERT_EQ(tid_col_, run_table_->getAttributeByName("tid")->getID());

    run_destination_index_ = query_context_proto_.insert_destinations_size();
    insert_destination_proto = query_context_proto_.add_insert_destinations();

    insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
    insert_destination_proto->set_relation_id(run_table_id);
    insert_destination_proto->set_relational_op_index(kOpIndex);

    // Set up the QueryContext.
    query_context_.reset(new QueryContext(query_context_proto_,
                                          *db_,
                                          storage_manager_.get(),
                                          foreman_client_id_,
                                          &bus_));
  }

  virtual void TearDown() {
    // Usually the worker thread makes the following call. In this test setup,
    // we don't have a worker thread hence we have to explicitly make the call.
    thread_id_map_->removeValue();

    // Drop blocks from relations.
    const vector<block_id> input_blocks = input_table_->getBlocksSnapshot();
    for (const block_id block : input_blocks) {
      storage_manager_->deleteBlockOrBlobFile(block);
    }

    const vector<block_id> result_blocks = result_table_->getBlocksSnapshot();
    for (const block_id block : result_blocks) {
      storage_manager_->deleteBlockOrBlobFile(block);
    }

    const vector<block_id> run_blocks = run_table_->getBlocksSnapshot();
    for (const block_id block : run_blocks) {
      storage_manager_->deleteBlockOrBlobFile(block);
    }
  }

  CatalogRelation *createTable(const char *name, const relation_id rel_id) {
    CatalogRelation *table = new CatalogRelation(nullptr, name, rel_id);
    const Type &int_type = IntType::InstanceNonNullable();
    const Type &null_int_type = IntType::InstanceNullable();
    table->addAttribute(new CatalogAttribute(table, "col-1", int_type));
    table->addAttribute(new CatalogAttribute(table, "col-2", int_type));
    table->addAttribute(new CatalogAttribute(table, "col-3", int_type));
    table->addAttribute(new CatalogAttribute(table, "null-col-1", null_int_type));
    table->addAttribute(new CatalogAttribute(table, "null-col-2", null_int_type));
    table->addAttribute(new CatalogAttribute(table, "null-col-3", null_int_type));
    table->addAttribute(new CatalogAttribute(table, "tid", int_type));
    table->setDefaultStorageBlockLayout(StorageBlockLayout::GenerateDefaultLayout(*table, false));
    return table;
  }

  // Helper method to create tuple.
  Tuple *createTuple(tuple_id id) {
    std::vector<TypedValue> values;
    TestTuple tuple(id);
    values.emplace_back(static_cast<int>(tuple.col1_));
    values.emplace_back(static_cast<int>(tuple.col2_));
    values.emplace_back(static_cast<int>(tuple.col3_));
    if (!tuple.col1_) {
      values.emplace_back(kInt);
    } else {
      values.emplace_back(static_cast<int>(tuple.col1_));
    }
    if (!tuple.col2_) {
      values.emplace_back(kInt);
    } else {
      values.emplace_back(static_cast<int>(tuple.col2_));
    }
    if (!tuple.col3_) {
      values.emplace_back(kInt);
    } else {
      values.emplace_back(static_cast<int>(tuple.col3_));
    }
    values.emplace_back(static_cast<int>(tuple.tid_));
    return new Tuple(std::move(values));
  }

  // Create (sorted) blocks for input to SortMergeRunOperator.
  template <typename ComparatorT>
  void createBlocks(const std::size_t num_tuples_per_block,
                    const std::size_t num_blocks,
                    const ComparatorT &comparator,
                    const std::size_t top_k = 0) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, (1 << 8) - 1);
    std::unique_ptr<Tuple> tuple;
    std::vector<TestTuple> top_k_tuples;
    MutableBlockReference storage_block;

    auto sort_comparator =
        [this, comparator](const TestTuple &l, const TestTuple &r) -> bool {
          std::unique_ptr<Tuple> left(createTuple(l.tid_)),
              right(createTuple(r.tid_));
          return comparator(*left, *right);
        };

    expect_num_tuples_ = 0;
    for (std::size_t bid = 0; bid < num_blocks; ++bid) {
      // Generate tuples.
      std::vector<TestTuple> tuples;
      for (std::size_t i = 0; i < num_tuples_per_block; ++i) {
        tuples.emplace_back(dist(gen));
      }

      // Sort tuples before creating the run.
      std::sort(tuples.begin(), tuples.end(), sort_comparator);

      // Maintain top-k.
      if (top_k > 0) {
        top_k_tuples.insert(top_k_tuples.end(),
                            tuples.begin(),
                            tuples.begin() + std::min(tuples.size(), top_k));
        std::partial_sort(
            top_k_tuples.begin(),
            top_k_tuples.begin() + std::min(top_k, top_k_tuples.size()),
            top_k_tuples.end(),
            sort_comparator);
        if (top_k_tuples.size() > top_k) {
          top_k_tuples.erase(top_k_tuples.begin() + top_k, top_k_tuples.end());
        }
      }

      // Create block.
      block_id block_id = storage_manager_->createBlock(*input_table_, input_table_->getDefaultStorageBlockLayout());
      storage_block = storage_manager_->getBlockMutable(block_id, *input_table_);
      input_table_->addBlock(block_id);

      // Insert sorted tuples.
      for (std::size_t i = 0; i < num_tuples_per_block; ++i) {
        tuple.reset(createTuple(tuples[i].tid_));
        EXPECT_TRUE(storage_block->insertTupleInBatch(*tuple));
      }
      storage_block->rebuild();
      expect_num_tuples_ += num_tuples_per_block;
    }

    if (top_k > 0) {
      expect_num_tuples_ = top_k;
      last_expected_tuple_.reset(createTuple(top_k_tuples.back().tid_));
    }
  }

  // Debug print method.
  void printTuple(const char *prefix, const Tuple &tuple) {
    if (VLOG_IS_ON(4)) {
      std::ostringstream out;
      out << prefix << ' ';
      CatalogRelation::const_iterator attr_it = input_table_->begin();
      Tuple::const_iterator value_it = tuple.begin();
      for (; attr_it != input_table_->end(); ++attr_it, ++value_it) {
        if (value_it->isNull()) {
          out << "NULL";
        } else {
          out << attr_it->getType().printValueToString(*value_it);
        }
        out << '|';
      }
      out << '\n';
      VLOG(4) << out.str();
    }
  }

  void processMessages(const std::size_t num_expected_feedback_messages = 0) {
    AnnotatedMessage msg;
    std::size_t num_receieved_feedback_messages = 0;
    do {
      if (bus_.ReceiveIfAvailable(foreman_client_id_, &msg)) {
        const TaggedMessage &tagged_message = msg.tagged_message;
        switch (tagged_message.message_type()) {
          case kWorkOrderFeedbackMessage: {
            // Dispatch feedback messages to SortMergeRunOperator.
            WorkOrder::FeedbackMessage feedback_msg(
                const_cast<void *>(tagged_message.message()),
                tagged_message.message_bytes());
            EXPECT_EQ(kOpIndex, feedback_msg.header().rel_op_index);
            merge_op_->receiveFeedbackMessage(feedback_msg);
            ++num_receieved_feedback_messages;
            break;
          }
          case kCatalogRelationNewBlockMessage: {
            serialization::CatalogRelationNewBlockMessage proto;
            CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

            CatalogRelation *relation = db_->getRelationByIdMutable(proto.relation_id());
            relation->addBlock(proto.block_id());
            break;
          }
          default:
            // It is safe to discard other kinds of messages (e.g. pipeline) in
            // this funtion.
            break;
        }
      }
    } while (num_receieved_feedback_messages < num_expected_feedback_messages);
  }

  void executeOperatorUntilDone() {
    bool done;
    WorkOrdersContainer container(kOpIndex + 1, 0);
    do {
      done = merge_op_->getAllWorkOrders(&container,
                                         query_context_.get(),
                                         storage_manager_.get(),
                                         foreman_client_id_,
                                         &bus_);
      while (container.hasNormalWorkOrder(kOpIndex)) {
        std::unique_ptr<WorkOrder> order(container.getNormalWorkOrder(kOpIndex));
        order->execute();
        processMessages(1);
      }
    } while (!done);
  }

  bool executeOperator() {
    bool done = false;
    WorkOrdersContainer container(kOpIndex + 1, 0);
    bool executed;
    do {
      if (!done) {
        // Find work orders to execute, if not done already.
        done = merge_op_->getAllWorkOrders(&container,
                                           query_context_.get(),
                                           storage_manager_.get(),
                                           foreman_client_id_,
                                           &bus_);
      }

      executed = false;
      if (container.hasNormalWorkOrder(kOpIndex)) {
        std::unique_ptr<WorkOrder> order(container.getNormalWorkOrder(kOpIndex));
        order->execute();
        processMessages(1);
        executed = true;
      }
    } while (container.hasNormalWorkOrder(kOpIndex) || executed);
    return done;
  }

  // Send doneFeedingInputBlocks callback.
  void feedingDone(RelationalOperator *op) {
    SortMergeRunOperator *merge_op = static_cast<SortMergeRunOperator *>(op);
    merge_op->doneFeedingInputBlocks(input_table_->getID());
  }

  QueryContext::sort_config_id createSortConfigProto(const std::vector<attribute_id> &attrs,
                                                     const std::vector<bool> &ordering,
                                                     const std::vector<bool> &null_ordering) {
    // Setup the SortConfiguration proto.
    DCHECK_EQ(attrs.size(), ordering.size());
    DCHECK_EQ(attrs.size(), null_ordering.size());

    const QueryContext::sort_config_id sort_config_index = query_context_proto_.sort_configs_size();
    serialization::SortConfiguration *sort_config_proto = query_context_proto_.add_sort_configs();

    for (std::size_t i = 0; i < attrs.size(); ++i) {
      serialization::SortConfiguration::OrderBy *order_by_proto = sort_config_proto->add_order_by_list();

      unique_ptr<Scalar> scalar(
          new ScalarAttribute(*input_table_->getAttributeById(attrs[i])));
      order_by_proto->mutable_expression()->CopyFrom(scalar->getProto());
      order_by_proto->set_is_ascending(ordering[i]);
      order_by_proto->set_null_first(null_ordering[i]);
    }

    return sort_config_index;
  }

  // Execute in pipeline breaking mode.
  void executeNotPipelined(const std::vector<attribute_id> &attrs,
                           const std::vector<bool> &ordering,
                           const std::vector<bool> &null_ordering,
                           const std::uint32_t merge_factor,
                           const std::size_t top_k = 0) {
    const QueryContext::sort_config_id sort_config_index = createSortConfigProto(attrs, ordering, null_ordering);

    merge_op_.reset(new SortMergeRunOperator(*input_table_,
                                             *result_table_,
                                             insert_destination_index_,
                                             *run_table_,
                                             run_destination_index_,
                                             sort_config_index,
                                             merge_factor,
                                             top_k,
                                             true));
    merge_op_->setOperatorIndex(kOpIndex);

    // Set up the QueryContext.
    query_context_.reset(new QueryContext(query_context_proto_,
                                          *db_,
                                          storage_manager_.get(),
                                          foreman_client_id_,
                                          &bus_));

    executeOperatorUntilDone();
  }

  void feedBlocks(const std::size_t num_blocks, std::vector<block_id> *blocks) {
    std::size_t count = std::min(blocks->size(), num_blocks);
    std::vector<block_id> to_feed(blocks->begin() + blocks->size() - count,
                                  blocks->end());

    // Feed blocks.
    DVLOG(1) << "Feeding " << to_feed.size() << " blocks.";
    merge_op_->feedInputBlocks(input_table_->getID(), &to_feed);

    // Remove fed blocks.
    blocks->erase(blocks->begin() + blocks->size() - count, blocks->end());
  }

  // Execute in pipelined mode.
  void executePipelined(const std::vector<attribute_id> &attrs,
                        const std::vector<bool> &ordering,
                        const std::vector<bool> &null_ordering,
                        const std::uint32_t merge_factor,
                        const std::size_t num_blocks_to_feed_per_iteration,
                        const std::size_t top_k = 0) {
    const QueryContext::sort_config_id sort_config_index = createSortConfigProto(attrs, ordering, null_ordering);

    merge_op_.reset(new SortMergeRunOperator(*input_table_,
                                             *result_table_,
                                             insert_destination_index_,
                                             *run_table_,
                                             run_destination_index_,
                                             sort_config_index,
                                             merge_factor,
                                             top_k,
                                             false));
    merge_op_->setOperatorIndex(kOpIndex);

    // Set up the QueryContext.
    query_context_.reset(new QueryContext(query_context_proto_,
                                          *db_,
                                          storage_manager_.get(),
                                          foreman_client_id_,
                                          &bus_));

    std::vector<block_id> blocks = input_table_->getBlocksSnapshot();
    while (!blocks.empty()) {
      feedBlocks(num_blocks_to_feed_per_iteration, &blocks);
      EXPECT_FALSE(executeOperator());

      if (blocks.empty()) {
        merge_op_->doneFeedingInputBlocks(input_table_->getID());
      }

      // Expect the operator to finish execution with 'done' response when block
      // feedDone() is signalled.
      EXPECT_EQ(blocks.empty(), executeOperator());
    }
  }

  // Check if the merged run is sorted.
  template <typename ComparatorT>
  void checkOutputRun(const ComparatorT comparator) {
    std::vector<block_id> sorted_blocks = result_table_->getBlocksSnapshot();
    std::unique_ptr<Tuple> prev, current;
    std::unique_ptr<ValueAccessor> accessor;

    ASSERT_GT(sorted_blocks.size(), 0u);
    tuple_id num_tuples = 0;
    for (const block_id &id : sorted_blocks) {
      BlockReference block(storage_manager_->getBlock(id, *result_table_));
      accessor.reset(block->getTupleStorageSubBlock().createValueAccessor());
      InvokeOnValueAccessorNotAdapter(
          accessor.get(),
          [&](auto *accessor) -> void {  // NOLINT(build/c++11)
        while (accessor->next()) {
          prev = std::move(current);
          current.reset(accessor->getTuple());
          ++num_tuples;
          if (prev) {
            EXPECT_FALSE(comparator(*current, *prev));
          }
          this->printTuple(">", *current);
        }
      });
    }

    last_actual_tuple_ = std::move(current);
    EXPECT_LT(0, expect_num_tuples_);
    EXPECT_EQ(expect_num_tuples_, num_tuples);
  }

  void checkLastTupleAttrValueIsEqual(const attribute_id id) {
    printTuple("Expected:", *last_expected_tuple_);
    printTuple("Actual:  ", *last_actual_tuple_);
    EXPECT_EQ(last_expected_tuple_->getAttributeValue(id).isNull(),
              last_actual_tuple_->getAttributeValue(id).isNull());
    if (!last_expected_tuple_->getAttributeValue(id).isNull() &&
        !last_actual_tuple_->getAttributeValue(id).isNull()) {
      EXPECT_TRUE(last_expected_tuple_->getAttributeValue(id).fastEqualCheck(
          last_actual_tuple_->getAttributeValue(id)));
    }
  }

  std::unique_ptr<StorageManager> storage_manager_;

  serialization::QueryContext query_context_proto_;
  std::unique_ptr<QueryContext> query_context_;
  std::unique_ptr<CatalogDatabase> db_;
  CatalogRelation *input_table_, *result_table_, *run_table_;

  QueryContext::insert_destination_id insert_destination_index_, run_destination_index_;

  std::unique_ptr<RelationalOperator> merge_op_;

  attribute_id col1_;
  attribute_id col2_;
  attribute_id col3_;
  attribute_id null_col1_;
  attribute_id null_col2_;
  attribute_id null_col3_;
  attribute_id tid_col_;
  tuple_id expect_num_tuples_;
  std::unique_ptr<Tuple> last_expected_tuple_;
  std::unique_ptr<Tuple> last_actual_tuple_;

  MessageBusImpl bus_;
  tmb::client_id foreman_client_id_;

  // This map is needed for InsertDestination and some operators that send
  // messages to Foreman directly. To know the reason behind the design of this
  // map, see the note in InsertDestination.hpp.
  ClientIDMap *thread_id_map_;
};

const char SortMergeRunOperatorTest::kTableName[] = "table";
const char SortMergeRunOperatorTest::kResultTableName[] = "result-table";
const char SortMergeRunOperatorTest::kRunTableName[] = "run-table";
const char SortMergeRunOperatorTest::kStoragePath[] = "./sort_merge_run_operator_test_data";
const char SortMergeRunOperatorTest::kDatabaseName[] = "database";

namespace {

static const std::size_t kNumTuplesPerBlock = 100;

auto kDefaultSortComparator =
    [](const Tuple &left, const Tuple &right) -> bool {
      TestTupleAttrs l = TupleToTupleAttr(left);
      TestTupleAttrs r = TupleToTupleAttr(right);
      if (l.c1 < r.c1) {
        return true;
      }
      return false;
    };

std::vector<bool> kDefaultSortOrdering{kSortAscending};
std::vector<bool> kDefaultSortNullOrdering{kSortNullLast};

}  // namespace

TEST_F(SortMergeRunOperatorTest, NotPipelined_SinglePass_SameRunsAsMergeFactor) {
  const std::size_t merge_factor = 8;
  const std::size_t num_blocks = merge_factor;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_SinglePass_SameRunsAsMergeFactor_TopK) {
  const std::size_t merge_factor = 8;
  const std::size_t num_blocks = merge_factor;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};
  std::size_t top_k = 20;

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator, top_k);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, top_k);
  checkOutputRun(kDefaultSortComparator);
  checkLastTupleAttrValueIsEqual(col1_);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_SinglePass_LessRunsThanMergeFactor) {
  const std::size_t merge_factor = 8;
  const std::size_t num_blocks = merge_factor / 2;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_SinglePass_OneRun) {
  const std::size_t merge_factor = 8;
  const std::size_t num_blocks = 1;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_TwoPass_CompleteMerges) {
  // Should cause:
  // [3, 3, 3]-way merges in first pass.
  // [3]-way merges in second pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_TwoPass_PartialFirstPass) {
  // Should cause:
  // [3, 3, 2]-way merges in first pass.
  // [3]-way merges in second pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor - 1;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_TwoPass_PartialBothPasses) {
  // Should cause:
  // [3, 2]-way merge in first pass.
  // 2-way merge in second pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor + 2;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_ThreePass_CompleteMerges) {
  // Should cause:
  // [3, 3, 3, 3, 3, 3, 3, 3, 3]-way merges in first pass.
  // [3, 3, 3]-way merges in second pass.
  // [3]-way merges in third pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor * merge_factor;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_ThreePass_CompleteMerges_TopK) {
  // Should cause:
  // [3, 3, 3, 3, 3, 3, 3, 3, 3]-way merges in first pass.
  // [3, 3, 3]-way merges in second pass.
  // [3]-way merges in third pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor * merge_factor;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};
  const std::size_t top_k = 33;

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator, top_k);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, top_k);
  checkOutputRun(kDefaultSortComparator);
  checkLastTupleAttrValueIsEqual(col1_);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_ThreePass_PartialMergesInFirstPass) {
  // Should cause:
  // [3, 3, 3, 3, 3, 3, 3, 3, 1]-way merges in first pass.
  // [3, 3, 3]-way merges in second pass.
  // [3]-way merges in third pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor * merge_factor - 2;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_ThreePass_PartialMergesInFirstSecondPass) {
  // Should cause:
  // [3, 3, 3, 3, 3, 3, 3, 2]-way merges in first pass.
  // [3, 3, 2]-way merges in second pass.
  // [3]-way merges in third pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor * merge_factor - 4;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_ThreePass_PartialMergesInAllPasses) {
  // Should cause:
  // [3, 3, 3, 3, 2]-way merges in first pass.
  // [3, 2]-way merges in second pass.
  // [2]-way merges in third pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = 2 * merge_factor * merge_factor - 4;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_ManyPasses_MergeFactor7) {
  const std::size_t merge_factor = 7;
  const std::size_t num_blocks = 509;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_ManyPasses_MergeFactor7_TopK) {
  const std::size_t merge_factor = 7;
  const std::size_t num_blocks = 509;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};
  const std::size_t top_k = 17;

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator, top_k);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, top_k);
  checkOutputRun(kDefaultSortComparator);
  checkLastTupleAttrValueIsEqual(col1_);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_ManyPasses_MergeFactor17) {
  const std::size_t merge_factor = 17;
  const std::size_t num_blocks = 509;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, NotPipelined_ManyPasses_MergeFactor17_TopK) {
  const std::size_t merge_factor = 17;
  const std::size_t num_blocks = 509;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};
  const std::size_t top_k = 53;

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator, top_k);
  executeNotPipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, top_k);
  checkOutputRun(kDefaultSortComparator);
  checkLastTupleAttrValueIsEqual(col1_);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_SinglePass_LessRunsThanMergeFactor_AllFeed) {
  const std::size_t merge_factor = 8;
  const std::size_t num_blocks = merge_factor / 2;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_SinglePass_LessRunsThanMergeFactor_SlowFeed) {
  const std::size_t merge_factor = 16;
  const std::size_t num_blocks = merge_factor - 1;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, 1);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_SinglePass_SameRunsAsMergeFactor_AllFeed) {
  const std::size_t merge_factor = 8;
  const std::size_t num_blocks = merge_factor;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_SinglePass_SameRunsAsMergeFactor_AllFeed_TopK) {
  const std::size_t merge_factor = 8;
  const std::size_t num_blocks = merge_factor;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};
  const std::size_t top_k = 13;

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator, top_k);
  executePipelined(
      kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks, top_k);
  checkOutputRun(kDefaultSortComparator);
  checkLastTupleAttrValueIsEqual(col1_);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_SinglePass_SameRunsAsMergeFactor_SlowFeed) {
  const std::size_t merge_factor = 8;
  const std::size_t num_blocks = merge_factor;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, 1);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_SinglePass_OneRun) {
  const std::size_t merge_factor = 8;
  const std::size_t num_blocks = 1;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_TwoPass_CompleteMerges_AllFeed) {
  // Should cause:
  // [3, 3, 3]-way merges in first pass.
  // [3]-way merges in second pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_TwoPass_CompleteMerges_SlowFeed) {
  // Should cause:
  // [3, 3, 3]-way merges in first pass.
  // [3]-way merges in second pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, 1);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_TwoPass_PartialFirstPass_AllFeed) {
  // Should cause:
  // [3, 3, 2]-way merges in first pass.
  // [3]-way merges in second pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor - 1;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_TwoPass_PartialFirstPass_SlowFeed) {
  // Should cause:
  // [3, 3, 2]-way merges in first pass.
  // [3]-way merges in second pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor - 1;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, 1);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_TwoPass_PartialBothPasses_AllFeed) {
  // Should cause:
  // [3, 2]-way merge in first pass.
  // 2-way merge in second pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor + 2;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_TwoPass_PartialBothPasses_SlowFeed) {
  // Should cause:
  // [3, 2]-way merge in first pass.
  // 2-way merge in second pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor + 2;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, 1);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ThreePass_CompleteMerges_AllFeed) {
  // Should cause:
  // [3, 3, 3, 3, 3, 3, 3, 3, 3]-way merges in first pass.
  // [3, 3, 3]-way merges in second pass.
  // [3]-way merges in third pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor * merge_factor;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ThreePass_CompleteMerges_SlowFeed) {
  // Should cause:
  // [3, 3, 3, 3, 3, 3, 3, 3, 3]-way merges in first pass.
  // [3, 3, 3]-way merges in second pass.
  // [3]-way merges in third pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor * merge_factor;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, 1);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ThreePass_CompleteMerges_SlowFeed_TopK) {
  // Should cause:
  // [3, 3, 3, 3, 3, 3, 3, 3, 3]-way merges in first pass.
  // [3, 3, 3]-way merges in second pass.
  // [3]-way merges in third pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor * merge_factor;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};
  const std::size_t top_k = 19;

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator, top_k);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, 1, top_k);
  checkOutputRun(kDefaultSortComparator);
  checkLastTupleAttrValueIsEqual(col1_);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ThreePass_PartialMergesInFirstPass_AllFeed) {
  // Should cause:
  // [3, 3, 3, 3, 3, 3, 3, 3, 1]-way merges in first pass.
  // [3, 3, 3]-way merges in second pass.
  // [3]-way merges in third pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor * merge_factor - 2;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ThreePass_PartialMergesInFirstPass_SlowFeed) {
  // Should cause:
  // [3, 3, 3, 3, 3, 3, 3, 3, 1]-way merges in first pass.
  // [3, 3, 3]-way merges in second pass.
  // [3]-way merges in third pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor * merge_factor - 2;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, 1);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ThreePass_PartialMergesInFirstSecondPass_AllFeed) {
  // Should cause:
  // [3, 3, 3, 3, 3, 3, 3, 2]-way merges in first pass.
  // [3, 3, 2]-way merges in second pass.
  // [3]-way merges in third pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor * merge_factor - 4;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ThreePass_PartialMergesInFirstSecondPass_SlowFeed) {
  // Should cause:
  // [3, 3, 3, 3, 3, 3, 3, 2]-way merges in first pass.
  // [3, 3, 2]-way merges in second pass.
  // [3]-way merges in third pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = merge_factor * merge_factor * merge_factor - 4;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, 1);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ThreePass_PartialMergesInAllPasses_AllFeed) {
  // Should cause:
  // [3, 3, 3, 3, 2]-way merges in first pass.
  // [3, 2]-way merges in second pass.
  // [2]-way merges in third pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = 2 * merge_factor * merge_factor - 4;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ThreePass_PartialMergesInAllPasses_SlowFeed) {
  // Should cause:
  // [3, 3, 3, 3, 2]-way merges in first pass.
  // [3, 2]-way merges in second pass.
  // [2]-way merges in third pass.
  const std::size_t merge_factor = 3;
  const std::size_t num_blocks = 2 * merge_factor * merge_factor - 4;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, 1);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ManyPasses_MergeFactor7_AllFeed) {
  const std::size_t merge_factor = 7;
  const std::size_t num_blocks = 509;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ManyPasses_MergeFactor7_AllFeed_TopK) {
  const std::size_t merge_factor = 7;
  const std::size_t num_blocks = 509;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};
  const std::size_t top_k = 78;

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator, top_k);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks,
      top_k);
  checkOutputRun(kDefaultSortComparator);
  checkLastTupleAttrValueIsEqual(col1_);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ManyPasses_MergeFactor7_SlowFeed) {
  const std::size_t merge_factor = 7;
  const std::size_t num_blocks = 509;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, 1);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ManyPasses_MergeFactor7_SlowFeed_TopK) {
  const std::size_t merge_factor = 7;
  const std::size_t num_blocks = 509;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};
  const std::size_t top_k = 37;

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator, top_k);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, 1, top_k);
  checkOutputRun(kDefaultSortComparator);
  checkLastTupleAttrValueIsEqual(col1_);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ManyPasses_MergeFactor17_AllFeed) {
  const std::size_t merge_factor = 17;
  const std::size_t num_blocks = 509;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ManyPasses_MergeFactor17_AllFeed_TopK) {
  const std::size_t merge_factor = 17;
  const std::size_t num_blocks = 509;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};
  const std::size_t top_k = 32;

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator, top_k);
  executePipelined(
      kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, num_blocks, top_k);
  checkOutputRun(kDefaultSortComparator);
  checkLastTupleAttrValueIsEqual(col1_);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ManyPasses_MergeFactor17_SlowFeed) {
  const std::size_t merge_factor = 17;
  const std::size_t num_blocks = 509;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, 1);
  checkOutputRun(kDefaultSortComparator);
}

TEST_F(SortMergeRunOperatorTest, Pipleined_ManyPasses_MergeFactor17_SlowFeed_TopK) {
  const std::size_t merge_factor = 17;
  const std::size_t num_blocks = 509;
  std::vector<attribute_id> kDefaultSortAttributes{col1_};
  const std::size_t top_k = 42;

  createBlocks(kNumTuplesPerBlock, num_blocks, kDefaultSortComparator, top_k);
  executePipelined(kDefaultSortAttributes, kDefaultSortOrdering, kDefaultSortNullOrdering, merge_factor, 1, top_k);
  checkOutputRun(kDefaultSortComparator);
  checkLastTupleAttrValueIsEqual(col1_);
}

}  // namespace quickstep

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  // Honor FLAGS_buffer_pool_slots in StorageManager.
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
