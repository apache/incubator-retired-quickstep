/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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
#include <limits>
#include <memory>
#include <random>
#include <sstream>
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
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/SortRunGenerationOperator.hpp"
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
#include "utility/SortConfiguration.pb.h"

#include "glog/logging.h"

#include "gtest/gtest.h"

#include "tmb/id_typedefs.h"

using std::unique_ptr;
using std::vector;

namespace quickstep {

namespace {

constexpr int kOpIndex = 0;

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
  struct TestTupleAttrs out{};
  out.c1 = attr[0].getLiteral<int>();
  out.c2 = attr[1].getLiteral<int>();
  out.c3 = attr[2].getLiteral<int>();
  out.null_c4 = attr[3].isNull();
  out.null_c5 = attr[4].isNull();
  out.null_c6 = attr[5].isNull();
  return out;
}

}  // namespace

// Test SortRunGenerationOperator class.
class SortRunGenerationOperatorTest : public ::testing::Test {
 protected:
  static const relation_id kTableId = 100;
  static const relation_id kResultTableId = kTableId + 1;
  static const relation_id kRunTableId = kTableId + 2;
  static const relation_id kTempRunTableId = kTableId + 3;
  static const char kTableName[];
  static const char kResultTableName[];
  static const char kStoragePath[];

  virtual void SetUp() {
    // Initialize the TMB, register this thread as sender and receiver for
    // appropriate types of messages.
    bus_.Initialize();
    thread_client_id_ = bus_.Connect();
    bus_.RegisterClientAsSender(thread_client_id_, kDataPipelineMessage);

    thread_id_map_ = ClientIDMap::Instance();
    // Usually the worker thread makes the following call. In this test setup,
    // we don't have a worker thread hence we have to explicitly make the call.
    thread_id_map_->addValue(thread_client_id_);

    storage_manager_.reset(new StorageManager(kStoragePath));

    db_.reset(new CatalogDatabase(nullptr, "database"));

    // Create tables, owned by db_.
    input_table_ = createTable(kTableName, kTableId);
    db_->addRelation(input_table_);

    col1_ = input_table_->getAttributeByName("col-1")->getID();
    col2_ = input_table_->getAttributeByName("col-2")->getID();
    col3_ = input_table_->getAttributeByName("col-3")->getID();
    null_col1_ = input_table_->getAttributeByName("null-col-1")->getID();
    null_col2_ = input_table_->getAttributeByName("null-col-2")->getID();
    null_col3_ = input_table_->getAttributeByName("null-col-3")->getID();
    tid_col_ = input_table_->getAttributeByName("tid")->getID();

    result_table_ = createTable(kResultTableName, kResultTableId);
    db_->addRelation(result_table_);

    ASSERT_EQ(col1_, result_table_->getAttributeByName("col-1")->getID());
    ASSERT_EQ(col2_, result_table_->getAttributeByName("col-2")->getID());
    ASSERT_EQ(col3_, result_table_->getAttributeByName("col-3")->getID());
    ASSERT_EQ(null_col1_, result_table_->getAttributeByName("null-col-1")->getID());
    ASSERT_EQ(null_col2_, result_table_->getAttributeByName("null-col-2")->getID());
    ASSERT_EQ(null_col3_, result_table_->getAttributeByName("null-col-3")->getID());
    ASSERT_EQ(tid_col_, result_table_->getAttributeByName("tid")->getID());
  }

  virtual void TearDown() {
    // Usually the worker thread makes the following call. In this test setup,
    // we don't have a worker thread hence we have to explicitly make the call.
    thread_id_map_->removeValue();
  }

  // Helper method to create catalog relation.
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

  // Create blocks of unsorted tuples.
  void createBlocks(const std::size_t num_tuples_per_block, const std::size_t num_blocks) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, (1 << 8) - 1);
    std::unique_ptr<Tuple> tuple;
    MutableBlockReference storage_block;

    expect_num_tuples_ = 0;
    for (std::size_t bid = 0; bid < num_blocks; ++bid) {
      // Create block.
      block_id block_id = storage_manager_->createBlock(*input_table_, nullptr);
      storage_block = storage_manager_->getBlockMutable(block_id, *input_table_);
      input_table_->addBlock(block_id);

      // Insert tuples.
      for (std::size_t i = 0; i < num_tuples_per_block; ++i) {
        tuple.reset(createTuple(dist(gen)));
        EXPECT_TRUE(storage_block->insertTupleInBatch(*tuple));
      }
      storage_block->rebuild();
      expect_num_tuples_ += num_tuples_per_block;
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

  void executeOperator(RelationalOperator *op) {
    WorkOrdersContainer container(kOpIndex + 1, 0);
    op->getAllWorkOrders(&container);
    while (container.hasNormalWorkOrder(kOpIndex)) {
      std::unique_ptr<WorkOrder> order(container.getNormalWorkOrder(kOpIndex));
      order->execute(query_context_.get(), db_.get(), storage_manager_.get());
    }
  }

  // Create blocks, and execute the SortRunGenerationOperator.
  void executeSort(const std::vector<attribute_id> &attrs,
                   const std::vector<bool> &ordering,
                   const std::vector<bool> &null_ordering) {
    const relation_id output_relation_id = result_table_->getID();

    // Setup the InsertDestination proto in the query context proto.
    serialization::QueryContext query_context_proto;

    const QueryContext::insert_destination_id insert_destination_index =
        query_context_proto.insert_destinations_size();
    serialization::InsertDestination *insert_destination_proto = query_context_proto.add_insert_destinations();

    insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
    insert_destination_proto->set_relation_id(output_relation_id);
    insert_destination_proto->set_need_to_add_blocks_from_relation(false);
    insert_destination_proto->set_relational_op_index(kOpIndex);
    insert_destination_proto->set_foreman_client_id(thread_client_id_);

    // Setup the SortConfiguration proto.
    DCHECK_EQ(attrs.size(), ordering.size());
    DCHECK_EQ(attrs.size(), null_ordering.size());

    const QueryContext::sort_config_id sort_config_index = query_context_proto.sort_configs_size();
    serialization::SortConfiguration *sort_config_proto = query_context_proto.add_sort_configs();

    for (std::size_t i = 0; i < attrs.size(); ++i) {
      serialization::SortConfiguration::OrderBy *order_by_proto = sort_config_proto->add_order_by_list();

      unique_ptr<Scalar> scalar(
          new ScalarAttribute(*input_table_->getAttributeById(attrs[i])));
      order_by_proto->mutable_expression()->CopyFrom(scalar->getProto());
      order_by_proto->set_is_ascending(ordering[i]);
      order_by_proto->set_null_first(null_ordering[i]);
    }

    std::unique_ptr<RelationalOperator> run_gen(
        new SortRunGenerationOperator(*input_table_,
                                      output_relation_id,
                                      insert_destination_index,
                                      sort_config_index,
                                      true /* is_stored */));
    run_gen->setOperatorIndex(kOpIndex);

    // Set up the QueryContext.
    query_context_.reset(new QueryContext(query_context_proto, db_.get(), storage_manager_.get(), &bus_));

    executeOperator(run_gen.get());
  }

  // Check if the merged run is sorted.
  template <typename ComparatorT>
  void checkOutputRun(const ComparatorT comparator) {
    std::vector<block_id> sorted_blocks = result_table_->getBlocksSnapshot();
    std::unique_ptr<ValueAccessor> accessor;

    ASSERT_GT(sorted_blocks.size(), 0u);
    tuple_id num_tuples = 0;
    for (const block_id &id : sorted_blocks) {
      std::unique_ptr<Tuple> prev, current;
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

    EXPECT_EQ(expect_num_tuples_, num_tuples);
  }

  std::unique_ptr<QueryContext> query_context_;
  std::unique_ptr<StorageManager> storage_manager_;
  std::unique_ptr<CatalogDatabase> db_;
  CatalogRelation *input_table_, *result_table_;

  attribute_id col1_;
  attribute_id col2_;
  attribute_id col3_;
  attribute_id null_col1_;
  attribute_id null_col2_;
  attribute_id null_col3_;
  attribute_id tid_col_;
  tuple_id expect_num_tuples_;

  MessageBusImpl bus_;
  tmb::client_id thread_client_id_;
  // This map is needed for InsertDestination and some operators that send
  // messages to Foreman directly. To know the reason behind the design of this
  // map, see the note in InsertDestination.hpp.
  ClientIDMap *thread_id_map_;
};

const char SortRunGenerationOperatorTest::kTableName[] = "table";
const char SortRunGenerationOperatorTest::kResultTableName[] = "result-table";
const char SortRunGenerationOperatorTest::kStoragePath[] = "./test_data";

namespace {

static const std::size_t kNumBlocks = 10;
static const std::size_t kNumTuplesPerBlock = 10;

}  // namespace

TEST_F(SortRunGenerationOperatorTest, 1Column_NonNull_Asc) {
  std::vector<attribute_id> attrs{col1_};
  std::vector<bool> ordering{kSortAscending};
  std::vector<bool> null_ordering{kSortNullLast};

  // Comparator for col-1 ASC.
  auto comparator = [](const Tuple &left, const Tuple &right) -> bool {
    TestTupleAttrs l = TupleToTupleAttr(left);
    TestTupleAttrs r = TupleToTupleAttr(right);
    if (l.c1 < r.c1) {
      return true;
    }
    return false;
  };
  createBlocks(kNumTuplesPerBlock, kNumBlocks);
  executeSort(attrs, ordering, null_ordering);
  checkOutputRun(comparator);
}

TEST_F(SortRunGenerationOperatorTest, 1Column_NonNull_Desc) {
  std::vector<attribute_id> attrs{col1_};
  std::vector<bool> ordering{kSortDescending};
  std::vector<bool> null_ordering{kSortNullLast};

  // Comparator for col-1 DESC.
  auto comparator = [](const Tuple &left, const Tuple &right) -> bool {
    TestTupleAttrs l = TupleToTupleAttr(left);
    TestTupleAttrs r = TupleToTupleAttr(right);
    if (l.c1 > r.c1) {
      return true;
    }
    return false;
  };
  createBlocks(kNumTuplesPerBlock, kNumBlocks);
  executeSort(attrs, ordering, null_ordering);
  checkOutputRun(comparator);
}

TEST_F(SortRunGenerationOperatorTest, 1Column_NullLast_Asc) {
  std::vector<attribute_id> attrs{null_col1_};
  std::vector<bool> ordering{kSortAscending};
  std::vector<bool> null_ordering{kSortNullLast};

  // Comparator for null-col-1 ASC NULLS LAST.
  auto comparator = [](const Tuple &left, const Tuple &right) -> bool {
    TestTupleAttrs l = TupleToTupleAttr(left);
    TestTupleAttrs r = TupleToTupleAttr(right);
    l.c1 = l.null_c4 ? std::numeric_limits<int>::max() : l.c1;
    r.c1 = r.null_c4 ? std::numeric_limits<int>::max() : r.c1;
    if (l.c1 < r.c1) {
      return true;
    }
    return false;
  };
  createBlocks(kNumTuplesPerBlock, kNumBlocks);
  executeSort(attrs, ordering, null_ordering);
  checkOutputRun(comparator);
}

TEST_F(SortRunGenerationOperatorTest, 1Column_NullFirst_Asc) {
  std::vector<attribute_id> attrs{null_col1_};
  std::vector<bool> ordering{kSortAscending};
  std::vector<bool> null_ordering{kSortNullFirst};

  // Comparator for null-col-1 ASC NULLS FIRST.
  auto comparator = [](const Tuple &left, const Tuple &right) -> bool {
    TestTupleAttrs l = TupleToTupleAttr(left);
    TestTupleAttrs r = TupleToTupleAttr(right);
    l.c1 = l.null_c4 ? std::numeric_limits<int>::min() : l.c1;
    r.c1 = r.null_c4 ? std::numeric_limits<int>::min() : r.c1;
    if (l.c1 < r.c1) {
      return true;
    }
    return false;
  };
  createBlocks(kNumTuplesPerBlock, kNumBlocks);
  executeSort(attrs, ordering, null_ordering);
  checkOutputRun(comparator);
}

TEST_F(SortRunGenerationOperatorTest, 1Column_NullLast_Desc) {
  std::vector<attribute_id> attrs{null_col1_};
  std::vector<bool> ordering{kSortDescending};
  std::vector<bool> null_ordering{kSortNullLast};

  // Comparator for null-col-1 DESC NULLS LAST.
  auto comparator = [](const Tuple &left, const Tuple &right) -> bool {
    TestTupleAttrs l = TupleToTupleAttr(left);
    TestTupleAttrs r = TupleToTupleAttr(right);
    l.c1 = l.null_c4 ? std::numeric_limits<int>::min() : l.c1;
    r.c1 = r.null_c4 ? std::numeric_limits<int>::min() : r.c1;
    if (l.c1 > r.c1) {
      return true;
    }
    return false;
  };
  createBlocks(kNumTuplesPerBlock, kNumBlocks);
  executeSort(attrs, ordering, null_ordering);
  checkOutputRun(comparator);
}

TEST_F(SortRunGenerationOperatorTest, 1Column_NullFirst_Desc) {
  std::vector<attribute_id> attrs{null_col1_};
  std::vector<bool> ordering{kSortDescending};
  std::vector<bool> null_ordering{kSortNullFirst};

  // Comparator for null-col-1 DESC NULLS FIRST.
  auto comparator = [](const Tuple &left, const Tuple &right) -> bool {
    TestTupleAttrs l = TupleToTupleAttr(left);
    TestTupleAttrs r = TupleToTupleAttr(right);
    l.c1 = l.null_c4 ? std::numeric_limits<int>::max() : l.c1;
    r.c1 = r.null_c4 ? std::numeric_limits<int>::max() : r.c1;
    if (l.c1 > r.c1) {
      return true;
    }
    return false;
  };
  createBlocks(kNumTuplesPerBlock, kNumBlocks);
  executeSort(attrs, ordering, null_ordering);
  checkOutputRun(comparator);
}

TEST_F(SortRunGenerationOperatorTest, 3Column_NonNull_Asc) {
  std::vector<attribute_id> attrs{col1_, col2_, col3_};
  std::vector<bool> ordering{kSortAscending, kSortAscending, kSortAscending};
  std::vector<bool> null_ordering{kSortNullLast, kSortNullLast, kSortNullLast};

  // Comparator for col-1 ASC, col-2 ASC, col-3 ASC.
  auto comparator = [](const Tuple &left, const Tuple &right) -> bool {
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
  createBlocks(kNumTuplesPerBlock, kNumBlocks);
  executeSort(attrs, ordering, null_ordering);
  checkOutputRun(comparator);
}

TEST_F(SortRunGenerationOperatorTest, 3Column_NonNull_Desc) {
  std::vector<attribute_id> attrs{col1_, col2_, col3_};
  std::vector<bool> ordering{kSortDescending, kSortDescending, kSortDescending};
  std::vector<bool> null_ordering{kSortNullLast, kSortNullLast, kSortNullLast};

  // Comparator for col-1 DESC, col-2 DESC, col-3 DESC.
  auto comparator = [](const Tuple &left, const Tuple &right) -> bool {
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
  createBlocks(kNumTuplesPerBlock, kNumBlocks);
  executeSort(attrs, ordering, null_ordering);
  checkOutputRun(comparator);
}

TEST_F(SortRunGenerationOperatorTest, 3Column_NullLast_Asc) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortAscending, kSortAscending, kSortAscending};
  std::vector<bool> null_ordering{kSortNullLast, kSortNullLast, kSortNullLast};

  // Comparator for null-col-1 ASC NULLS LAST, null-col-2 ASC NULLS LAST,
  // null-col-3 ASC NULLS LAST.
  auto comparator = [this](const Tuple &left, const Tuple &right) -> bool {
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
  createBlocks(kNumTuplesPerBlock, kNumBlocks);
  executeSort(attrs, ordering, null_ordering);
  checkOutputRun(comparator);
}

TEST_F(SortRunGenerationOperatorTest, 3Column_NullLast_Desc) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortDescending, kSortDescending, kSortDescending};
  std::vector<bool> null_ordering{kSortNullLast, kSortNullLast, kSortNullLast};

  // Comparator for null-col-1 DESC NULLS LAST, null-col-2 DESC NULLS LAST,
  // null-col-3 DESC NULLS LAST.
  auto comparator = [](const Tuple &left, const Tuple &right) -> bool {
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
  createBlocks(kNumTuplesPerBlock, kNumBlocks);
  executeSort(attrs, ordering, null_ordering);
  checkOutputRun(comparator);
}

TEST_F(SortRunGenerationOperatorTest, 3Column_NullFirst_Asc) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortAscending, kSortAscending, kSortAscending};
  std::vector<bool> null_ordering{kSortNullFirst, kSortNullFirst, kSortNullFirst};

  // Comparator for null-col-1 ASC NULLS FIRST, null-col-2 ASC NULLS FIRST,
  // null-col-3 ASC NULLS FIRST.
  auto comparator = [](const Tuple &left, const Tuple &right) -> bool {
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
  createBlocks(kNumTuplesPerBlock, kNumBlocks);
  executeSort(attrs, ordering, null_ordering);
  checkOutputRun(comparator);
}

TEST_F(SortRunGenerationOperatorTest, 3Column_NullFirst_Desc) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortDescending, kSortDescending, kSortDescending};
  std::vector<bool> null_ordering{kSortNullFirst, kSortNullFirst, kSortNullFirst};

  // Comparator for null-col-1 DESC NULLS FIRST, null-col-2 DESC NULLS FIRST,
  // null-col-3 DESC NULLS FIRST.
  auto comparator = [](const Tuple &left, const Tuple &right) -> bool {
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
  createBlocks(kNumTuplesPerBlock, kNumBlocks);
  executeSort(attrs, ordering, null_ordering);
  checkOutputRun(comparator);
}

TEST_F(SortRunGenerationOperatorTest, 3Column_MixedNullOrdering_MixedOrdering) {
  std::vector<attribute_id> attrs{null_col1_, null_col2_, null_col3_};
  std::vector<bool> ordering{kSortAscending, kSortDescending, kSortAscending};
  std::vector<bool> null_ordering{kSortNullFirst, kSortNullLast, kSortNullLast};

  // Comparator for null-col-1 ASC NULLS FIRST, null-col-2 DESC NULLS LAST,
  // null-col-3 ASC NULLS LAST.
  auto comparator = [](const Tuple &left, const Tuple &right) -> bool {
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
  createBlocks(kNumTuplesPerBlock, kNumBlocks);
  executeSort(attrs, ordering, null_ordering);
  checkOutputRun(comparator);
}

}  // namespace quickstep
