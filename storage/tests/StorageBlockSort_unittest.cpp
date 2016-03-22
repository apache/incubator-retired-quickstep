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
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "glog/logging.h"

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageManager.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/containers/Tuple.hpp"

namespace quickstep {

template <typename ScalarType>
Scalar* SetupColumn(const std::string &attr, const CatalogRelation &table);

template <>
Scalar* SetupColumn<ScalarAttribute>(const std::string &attr, const CatalogRelation &table) {
  return new ScalarAttribute(*table.getAttributeByName(attr));
}

namespace {

class TestTuple {
 public:
  explicit TestTuple(int num)
      : tid_(num), col1_(Bits(num, 3, 5)), col2_(Bits(num, 6, 2)), col3_(Bits(num, 2, 4)), col4_(Bits(num, 1, 5)) {}

  int tid_;
  int col1_;
  int col2_;
  int col3_;
  int col4_;

  static inline int Bits(int value, int offset, int length) { return (value >> offset) & (0xffff >> (16 - length)); }
};

}  // namespace

class StorageBlockSortTest : public ::testing::Test {
 public:
  static const int kNumTuples;

 protected:
  static const char kStoragePath[];
  static const char kTableName[];
  static const relation_id kTableId = 100;
  static const block_id kBlockId = 101;

  static constexpr bool kAscending = true;
  static constexpr bool kDescending = false;

  virtual void SetUp() {
    storage_manager_.reset(new StorageManager(kStoragePath));

    table_.reset(new CatalogRelation(nullptr, kTableName, kTableId));

    // Add attributes
    const Type &int_type = IntType::InstanceNullable();
    table_->addAttribute(new CatalogAttribute(table_.get(), "tid", int_type));
    table_->addAttribute(new CatalogAttribute(table_.get(), "col-1", int_type));
    table_->addAttribute(new CatalogAttribute(table_.get(), "col-2", int_type));
    table_->addAttribute(new CatalogAttribute(table_.get(), "col-3", int_type));
    table_->addAttribute(new CatalogAttribute(table_.get(), "col-4", int_type));

    std::unique_ptr<StorageBlockLayout> layout(StorageBlockLayout::GenerateDefaultLayout(*table_, false));
    block_id_ = storage_manager_->createBlock(*table_, *layout);
    MutableBlockReference block = storage_manager_->getBlockMutable(block_id_, *table_);
    table_->addBlock(block_id_);

    for (int i = 0; i < kNumTuples; ++i) {
      tuple_mapping_.emplace_back(i);
    }
    std::random_shuffle(tuple_mapping_.begin(), tuple_mapping_.end());

    tuple_mapping_.reserve(kNumTuples);
    for (size_t i = 0; i < tuple_mapping_.size(); ++i) {
      std::unique_ptr<Tuple> tuple(createTuple(tuple_mapping_[i]));
      block->insertTupleInBatch(*tuple);
    }

    block->rebuild();
  }

  inline void addAttributeValue(std::vector<TypedValue> *values, int value) {
    if (value) {
      values->emplace_back(static_cast<int>(value));
    } else {
      // 0 => NULL
      values->emplace_back(TypeID::kInt);
    }
  }

  inline Tuple* createTuple(int num) {
    std::vector<TypedValue> values;
    TestTuple row(num);
    addAttributeValue(&values, row.tid_);
    addAttributeValue(&values, row.col1_);
    addAttributeValue(&values, row.col2_);
    addAttributeValue(&values, row.col3_);
    addAttributeValue(&values, row.col4_);
    Tuple *tuple = new Tuple(std::move(values));
    return tuple;
  }

  template <typename Configuration, typename ScalarType>
  void testSort(void) {
    Configuration test_instance;

    PtrVector<Scalar> sort_columns;
    std::vector<bool> sort_is_ascending;
    std::vector<bool> null_first;
    test_instance.template configure<ScalarType>(*table_, &sort_columns, &sort_is_ascending, &null_first);

    OrderedTupleIdSequence sorted_sequence;
    MutableBlockReference block = storage_manager_->getBlockMutable(block_id_, *table_);
    block->sort(sort_columns, sort_is_ascending, null_first, &sorted_sequence, nullptr);

    ASSERT_EQ(static_cast<std::size_t>(kNumTuples), sorted_sequence.size());
    test_instance.assertTupleIdSequence(sorted_sequence, tuple_mapping_);
  }

  std::unique_ptr<StorageManager> storage_manager_;
  std::unique_ptr<CatalogRelation> table_;
  std::vector<tuple_id> tuple_mapping_;
  block_id block_id_;
};

const int StorageBlockSortTest::kNumTuples = 1 << 8;
const char StorageBlockSortTest::kStoragePath[] = "table";
const char StorageBlockSortTest::kTableName[] = "./test_data";

namespace {

template <bool ascending>
class SingleColumnTest {
 public:
  template <typename ScalarType>
  void configure(const CatalogRelation &table,
                 PtrVector<Scalar> *sort_columns,
                 std::vector<bool> *sort_is_ascending,
                 std::vector<bool> *sort_null_first) {
    sort_columns->push_back(SetupColumn<ScalarType>("tid", table));
    sort_is_ascending->push_back(ascending);
    sort_null_first->push_back(false);
  }

  void assertTupleIdSequence(const OrderedTupleIdSequence &sequence, const std::vector<tuple_id> &tuple_mapping) {
    for (size_t id = 1; id < sequence.size(); ++id) {
      TestTuple prev(tuple_mapping[sequence[id-1]]);
      TestTuple current(tuple_mapping[sequence[id]]);
      // NOTE: All values are unique in this test. Hence, we use LT and GT.
      ASSERT_GT(prev.tid_, 0);
      if (current.tid_ != 0) {
        if (ascending) {
          ASSERT_LT(prev.tid_, current.tid_);
        } else {
          ASSERT_GT(prev.tid_, current.tid_);
        }
      }
    }
  }
};

template <bool col1_ascending, bool tid_ascending>
class TwoColumnTest {
 public:
  template <typename ScalarType>
  void configure(const CatalogRelation &table,
                 PtrVector<Scalar> *sort_columns,
                 std::vector<bool> *sort_is_ascending,
                 std::vector<bool> *null_first) {
    sort_columns->push_back(SetupColumn<ScalarType>("col-1", table));
    sort_is_ascending->push_back(col1_ascending);
    null_first->push_back(false);

    sort_columns->push_back(SetupColumn<ScalarType>("tid", table));
    sort_is_ascending->push_back(tid_ascending);
    null_first->push_back(false);
  }

  ::testing::AssertionResult compareTupleId(const int prev, const int current) {
    if (tid_ascending) {
      if (prev > current) {
        return ::testing::AssertionFailure() << "TID " << prev << " not less than " << current;
      }
    } else {
      if (prev < current) {
        return ::testing::AssertionFailure() << "TID " << prev << " not greater than " << current;
      }
    }
    return ::testing::AssertionSuccess();
  }

  ::testing::AssertionResult compare(const TestTuple &prev, const TestTuple &current) {
    if (col1_ascending) {
      if (prev.col1_ > current.col1_) {
        return ::testing::AssertionFailure() << "Col-1 " << prev.col1_ << " not less than " << current.col1_;
      } else if (prev.col1_ == current.col1_) {
        return compareTupleId(prev.tid_, current.tid_);
      }
    } else {
      if (prev.col1_ < current.col1_) {
        return ::testing::AssertionFailure() << "Col-1 " << prev.col1_ << " not greater than " << current.col1_;
      } else if (prev.col1_ == current.col1_) {
        return compareTupleId(prev.tid_, current.tid_);
      }
    }
    return ::testing::AssertionSuccess();
  }

  inline void fixupTestTupleForNULLs(TestTuple *tuple) {
    if (tuple->col1_ == 0) {
      tuple->col1_ = col1_ascending ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min();
    }
    if (tuple->tid_ == 0) {
      tuple->tid_ = tid_ascending ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min();
    }
  }

  void assertTupleIdSequence(const OrderedTupleIdSequence &sequence, const std::vector<tuple_id> &tuple_mapping) {
    for (size_t id = 1; id < sequence.size(); ++id) {
      TestTuple prev(tuple_mapping[sequence[id-1]]);
      fixupTestTupleForNULLs(&prev);
      TestTuple current(tuple_mapping[sequence[id]]);
      fixupTestTupleForNULLs(&current);
      ASSERT_TRUE(compare(prev, current));
    }
  }
};

class ThreeColumnTest {
 public:
  template <typename ScalarType>
  void configure(const CatalogRelation &table,
                 PtrVector<Scalar> *sort_columns,
                 std::vector<bool> *sort_is_ascending,
                 std::vector<bool> *null_first) {
    sort_columns->push_back(SetupColumn<ScalarType>("col-1", table));
    sort_is_ascending->push_back(true);
    null_first->push_back(false);

    sort_columns->push_back(SetupColumn<ScalarType>("col-3", table));
    sort_is_ascending->push_back(true);
    null_first->push_back(true);

    sort_columns->push_back(SetupColumn<ScalarType>("tid", table));
    sort_is_ascending->push_back(true);
    null_first->push_back(false);
  }

  ::testing::AssertionResult compare(const TestTuple &prev, const TestTuple &current) {
    if (prev.col1_ > current.col1_) {
      return ::testing::AssertionFailure() << "Col-1 " << prev.col1_ << " is greater than " << current.col1_;
    } else if (prev.col1_ == current.col1_) {
      if (prev.col3_ > current.col3_) {
        return ::testing::AssertionFailure() << "Col-3 " << prev.col3_ << " is greater than " << current.col3_;
      } else if (prev.col3_ == current.col3_) {
        if (prev.tid_ > current.tid_) {
          return ::testing::AssertionFailure() << "TID " << prev.tid_ << " is greater than " << current.tid_;
        }
      }
    }
    return ::testing::AssertionSuccess();
  }

  inline void fixupTestTupleForNULLs(TestTuple *tuple) {
    if (tuple->col1_ == 0) {
      tuple->col1_ = std::numeric_limits<int>::max();
    }
    if (tuple->col3_ == 0) {
      tuple->col3_ = std::numeric_limits<int>::min();
    }
    if (tuple->tid_ == 0) {
      tuple->tid_ = std::numeric_limits<int>::max();
    }
  }

  inline void printTuple(const char *prefix, const TestTuple &tuple) {
    if (VLOG_IS_ON(2)) {
      VLOG(2) << prefix << tuple.col1_ << '|' << tuple.col3_ << '|' << tuple.tid_;
    }
  }

  void assertTupleIdSequence(const OrderedTupleIdSequence &sequence, const std::vector<tuple_id> &tuple_mapping) {
    for (size_t id = 1; id < sequence.size(); ++id) {
      TestTuple prev(tuple_mapping[sequence[id-1]]);
      fixupTestTupleForNULLs(&prev);
      TestTuple current(tuple_mapping[sequence[id]]);
      fixupTestTupleForNULLs(&current);
      if (id == 1) printTuple("", prev);
      printTuple("", current);
      ASSERT_TRUE(compare(prev, current));
    }
  }
};

class AllColumnTest {
 public:
  template <typename ScalarType>
  void configure(const CatalogRelation &table,
                 PtrVector<Scalar> *sort_columns,
                 std::vector<bool> *sort_is_ascending,
                 std::vector<bool> *null_first) {
    sort_columns->push_back(SetupColumn<ScalarType>("col-1", table));
    sort_is_ascending->push_back(true);
    null_first->push_back(false);

    sort_columns->push_back(SetupColumn<ScalarType>("col-2", table));
    sort_is_ascending->push_back(true);
    null_first->push_back(false);

    sort_columns->push_back(SetupColumn<ScalarType>("col-3", table));
    sort_is_ascending->push_back(true);
    null_first->push_back(true);

    sort_columns->push_back(SetupColumn<ScalarType>("col-4", table));
    sort_is_ascending->push_back(true);
    null_first->push_back(true);

    sort_columns->push_back(SetupColumn<ScalarType>("tid", table));
    sort_is_ascending->push_back(true);
    null_first->push_back(true);
  }

  inline void fixupTestTupleForNULLs(TestTuple *tuple) {
    if (tuple->col1_ == 0) {
      tuple->col1_ = std::numeric_limits<int>::max();
    }
    if (tuple->col2_ == 0) {
      tuple->col2_ = std::numeric_limits<int>::max();
    }
    if (tuple->col3_ == 0) {
      tuple->col3_ = std::numeric_limits<int>::min();
    }
    if (tuple->col4_ == 0) {
      tuple->col4_ = std::numeric_limits<int>::min();
    }
    if (tuple->tid_ == 0) {
      tuple->tid_ = std::numeric_limits<int>::min();
    }
  }

  inline void printTuple(const TestTuple &test) {
    if (VLOG_IS_ON(2)) {
      VLOG(2) << test.col1_ << '|' << test.col2_ << '|' << test.col3_ << '|'
              << test.col4_ << '|' << test.tid_;
    }
  }

  ::testing::AssertionResult compare(const TestTuple &prev, const TestTuple &current) {
    if (prev.col1_ > current.col1_) {
      return ::testing::AssertionFailure() << "Col-1 " << prev.col1_ << " is greater than " << current.col1_;
    } else if (prev.col1_ == current.col1_) {
      if (prev.col2_ > current.col2_) {
        return ::testing::AssertionFailure() << "Col-2 " << prev.col2_ << " is greater than " << current.col2_;
      } else if (prev.col2_ == current.col2_) {
        if (prev.col3_ > current.col3_) {
          return ::testing::AssertionFailure() << "Col-3 " << prev.col3_ << " is greater than " << current.col3_;
        } else if (prev.col3_ == current.col3_) {
          if (prev.col4_ > current.col4_) {
            return ::testing::AssertionFailure() << "Col-4 " << prev.col4_ << " is greater than " << current.col4_;
          } else if (prev.col4_ == current.col4_) {
            if (prev.tid_ > current.tid_) {
              return ::testing::AssertionFailure() << "TID " << prev.tid_ << " is greater than " << current.tid_;
            }
          }
        }
      }
    }
    return ::testing::AssertionSuccess();
  }

  void assertTupleIdSequence(const OrderedTupleIdSequence &sequence, const std::vector<tuple_id> &tuple_mapping) {
    for (size_t id = 1; id < sequence.size(); ++id) {
      TestTuple prev(tuple_mapping[sequence[id-1]]);
      fixupTestTupleForNULLs(&prev);
      TestTuple current(tuple_mapping[sequence[id]]);
      fixupTestTupleForNULLs(&current);
      if (id == 1) printTuple(prev);
      printTuple(current);
      ASSERT_TRUE(compare(prev, current));
    }
  }
};

}  // namespace

TEST_F(StorageBlockSortTest, SingleColumnSort_Ascending_ScalarAttribute) {
  testSort<SingleColumnTest<kAscending>, ScalarAttribute>();
}

TEST_F(StorageBlockSortTest, SingleColumnSort_Descending_ScalarAttribute) {
  testSort<SingleColumnTest<kDescending>, ScalarAttribute>();
}

TEST_F(StorageBlockSortTest, TwoColumnSort_AscendingAscending_ScalarAttribute) {
  testSort<TwoColumnTest<kAscending, kAscending>, ScalarAttribute>();
}

TEST_F(StorageBlockSortTest, TwoColumnSort_DescendingAscending_ScalarAttribute) {
  testSort<TwoColumnTest<kDescending, kAscending>, ScalarAttribute>();
}

TEST_F(StorageBlockSortTest, TwoColumnSort_AscendingDescending_ScalarAttribute) {
  testSort<TwoColumnTest<kAscending, kDescending>, ScalarAttribute>();
}

TEST_F(StorageBlockSortTest, TwoColumnSort_DescendingDescending_ScalarAttribute) {
  testSort<TwoColumnTest<kDescending, kDescending>, ScalarAttribute>();
}

TEST_F(StorageBlockSortTest, ThreeColumnSort_ScalarAttribute) {
  testSort<ThreeColumnTest, ScalarAttribute>();
}

TEST_F(StorageBlockSortTest, AllColumnSort_ScalarAttribute) {
  testSort<AllColumnTest, ScalarAttribute>();
}

}  // namespace quickstep
