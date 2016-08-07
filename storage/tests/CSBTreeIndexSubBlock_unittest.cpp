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

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/scalar/ScalarLiteral.hpp"
#include "storage/CSBTreeIndexSubBlock.hpp"
#include "storage/CompressedPackedRowStoreTupleStorageSubBlock.hpp"
#include "storage/CompressedTupleStorageSubBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageErrors.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/tests/MockTupleStorageSubBlock.hpp"
#include "types/CharType.hpp"
#include "types/FloatType.hpp"
#include "types/LongType.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypedValue.hpp"
#include "types/TypeID.hpp"
#include "types/VarCharType.hpp"
#include "types/containers/Tuple.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/comparisons/ComparisonUtil.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/ScopedBuffer.hpp"

using std::binary_search;
using std::int64_t;
using std::numeric_limits;
using std::ostringstream;
using std::set;
using std::size_t;
using std::string;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::vector;

namespace quickstep {

namespace {

template <typename CodeType>
class CompressedCodeLessComparator : public UncheckedComparator {
 public:
  ~CompressedCodeLessComparator() {
  }

  bool compareTypedValues(const TypedValue &left, const TypedValue &right) const {
    FATAL_ERROR("Can not use CompressedCodeLessComparator to compare TypedValue.");
  }

  inline bool compareDataPtrs(const void *left, const void *right) const {
    return *static_cast<const CodeType*>(left) < *static_cast<const CodeType*>(right);
  }

  bool compareTypedValueWithDataPtr(const TypedValue &left, const void *right) const {
    FATAL_ERROR("Can not use CompressedCodeLessComparator to compare TypedValue.");
  }

  bool compareDataPtrWithTypedValue(const void *left, const TypedValue &right) const {
    FATAL_ERROR("Can not use CompressedCodeLessComparator to compare TypedValue.");
  }
};

}  // anonymous namespace

class CSBTreeIndexSubBlockTest : public ::testing::Test {
 protected:
  static const size_t kIndexSubBlockSize = 0x100000;  // 1 MB.
  // Hack: replace long_attr values equal to this with NULL.
  static const int64_t kLongAttrNullValue = -55555;
  static const char kCharAttrNullValue[];

  static const int kNumSampleTuples = 37;
  static const tuple_id kDeleteSequence[kNumSampleTuples];

  virtual void SetUp() {
    // Create a sample relation with a variety of attribute types.
    relation_.reset(new CatalogRelation(NULL, "TestRelation"));

    CatalogAttribute *long_attr = new CatalogAttribute(relation_.get(),
                                                       "long_attr",
                                                       TypeFactory::GetType(kLong, false));
    ASSERT_EQ(0, relation_->addAttribute(long_attr));

    CatalogAttribute *nullable_long_attr = new CatalogAttribute(relation_.get(),
                                                                "nullable_long_attr",
                                                                TypeFactory::GetType(kLong, true));
    ASSERT_EQ(1, relation_->addAttribute(nullable_long_attr));

    CatalogAttribute *float_attr = new CatalogAttribute(relation_.get(),
                                                        "float_attr",
                                                        TypeFactory::GetType(kFloat, false));
    ASSERT_EQ(2, relation_->addAttribute(float_attr));

    CatalogAttribute *char_attr = new CatalogAttribute(relation_.get(),
                                                       "char_attr",
                                                       TypeFactory::GetType(kChar, 4, false));
    ASSERT_EQ(3, relation_->addAttribute(char_attr));

    CatalogAttribute *nullable_char_attr = new CatalogAttribute(relation_.get(),
                                                                "nullable_char_attr",
                                                                TypeFactory::GetType(kChar, 4, true));
    ASSERT_EQ(4, relation_->addAttribute(nullable_char_attr));

    CatalogAttribute *big_char_attr = new CatalogAttribute(relation_.get(),
                                                           "big_char_attr",
                                                           TypeFactory::GetType(kChar, 80, false));
    ASSERT_EQ(5, relation_->addAttribute(big_char_attr));

    CatalogAttribute *varchar_attr = new CatalogAttribute(relation_.get(),
                                                          "varchar_attr",
                                                          TypeFactory::GetType(kVarChar, 8, false));
    ASSERT_EQ(6, relation_->addAttribute(varchar_attr));

    // Create a MockTupleStorageSubBlock to hold tuples for testing.
    tuple_store_.reset(new MockTupleStorageSubBlock(*relation_));

    // Don't initialize the CSBTreeIndexSubBlock yet. Different tests will
    // index on different attributes.
    index_memory_.reset();
    index_description_.reset();
    index_.reset();
  }

  // Create index_ of size 'index_memory_size' on the specified 'index_attrs'.
  void createIndex(const vector<attribute_id> &index_attrs, const size_t index_memory_size) {
    ASSERT_LT(0u, index_attrs.size());
    ASSERT_LE(index_attrs.size(), numeric_limits<uint16_t>::max());

    // Make the IndexSubBlockDescription.
    index_description_.reset(new IndexSubBlockDescription());
    index_description_->set_sub_block_type(IndexSubBlockDescription::CSB_TREE);
    for (vector<attribute_id>::const_iterator attr_it = index_attrs.begin();
         attr_it != index_attrs.end();
         ++attr_it) {
      index_description_->add_indexed_attribute_ids(*attr_it);
    }

    index_memory_.reset(index_memory_size);
    std::memset(index_memory_.get(), 0x0, index_memory_size);
    index_.reset(new CSBTreeIndexSubBlock(*tuple_store_,
                                          *index_description_,
                                          true,
                                          index_memory_.get(),
                                          index_memory_size));
  }

  // Insert a tuple with the specified attribute values into tuple_store_.
  tuple_id insertTupleInTupleStore(const int64_t long_val,
                                   const int64_t nullable_long_val,
                                   const float float_val,
                                   const string &char_val,
                                   const string &nullable_char_val,
                                   const string &big_char_val,
                                   const string &varchar_val) {
    std::vector<TypedValue> attrs;

    attrs.emplace_back(LongType::InstanceNonNullable().makeValue(&long_val));

    if (nullable_long_val == kLongAttrNullValue) {
      attrs.emplace_back(LongType::InstanceNullable().makeNullValue());
    } else {
      attrs.emplace_back(LongType::InstanceNullable().makeValue(&nullable_long_val));
    }

    attrs.emplace_back(FloatType::InstanceNonNullable().makeValue(&float_val));

    attrs.emplace_back(CharType::InstanceNonNullable(4).makeValue(
        char_val.c_str(),
        char_val.size() >= 4 ? 4 : char_val.size() + 1).ensureNotReference());

    if (nullable_char_val == kCharAttrNullValue) {
      attrs.emplace_back(CharType::InstanceNullable(4).makeNullValue());
    } else {
      attrs.emplace_back(CharType::InstanceNonNullable(4).makeValue(
          nullable_char_val.c_str(),
          nullable_char_val.size() >= 4 ? 4 : nullable_char_val.size() + 1).ensureNotReference());
    }

    attrs.emplace_back(CharType::InstanceNonNullable(80).makeValue(
        big_char_val.c_str(),
        big_char_val.size() >= 80 ? 80 : big_char_val.size() + 1).ensureNotReference());

    TypedValue varchar_typed_value
        = VarCharType::InstanceNonNullable(varchar_val.size()).makeValue(
            varchar_val.c_str(),
            varchar_val.size() + 1);
    // Test strings are sometimes longer than 8 characters, so truncate if
    // needed.
    varchar_typed_value = VarCharType::InstanceNonNullable(8).coerceValue(
        varchar_typed_value,
        VarCharType::InstanceNonNullable(varchar_val.size()));
    varchar_typed_value.ensureNotReference();
    attrs.emplace_back(std::move(varchar_typed_value));

    // MockTupleStorageSubBlock takes ownership of new tuple passed in with
    // addTupleMock() method.
    return tuple_store_->addTupleMock(new Tuple(std::move(attrs)));
  }

  // Generate a sample tuple based on 'base_value' and insert in into
  // tuple_store_. The sample tuple will have long_attr equal to 'base_value',
  // float_attr equal to 0.25 * base_value, and each of char_attr,
  // big_char_attr, and varchar_attr equal to the string representation of
  // 'base_value' with 'string_suffix' appended on to it. If 'generate_nulls'
  // is true, then both nullable_long_attr and nullable_char_attr will be NULL,
  // otherwise nullable_long_attr will be equal to 'base_value' and
  // nullable_char_attr will be equal to the other string values. Returns the
  // tuple_id of the inserted tuple.
  tuple_id generateAndInsertTuple(const int64_t base_value,
                                  const bool generate_nulls,
                                  const string &string_suffix) {
    ostringstream string_value_buffer;
    string_value_buffer << base_value << string_suffix;
    if (generate_nulls) {
      return insertTupleInTupleStore(base_value,
                                     kLongAttrNullValue,
                                     0.25 * base_value,
                                     string_value_buffer.str(),
                                     kCharAttrNullValue,
                                     string_value_buffer.str(),
                                     string_value_buffer.str());
    } else {
      return insertTupleInTupleStore(base_value,
                                     base_value,
                                     0.25 * base_value,
                                     string_value_buffer.str(),
                                     string_value_buffer.str(),
                                     string_value_buffer.str(),
                                     string_value_buffer.str());
    }
  }

  // Put some sample tuples in 'tuple_store_'. If 'add_to_index' is true, an
  // entry is added to 'index_' after each tuple is inserted. If
  // 'rebuild_index' is true, 'index_' is entirely rebuilt after each tuple is
  // inserted. If 'check_consistency' is true, checkIndexConsistency() is
  // called after every change to the index.
  void insertSampleData(const bool add_to_index,
                        const bool rebuild_index,
                        const bool check_consistency) {
    tuple_id previous_inserted_id = -1;
    tuple_id inserted_id = -1;

    // 10 tuples in order.
    for (int tnum = 0; tnum < 10; ++tnum) {
      inserted_id = generateAndInsertTuple(tnum, false, "aa");
      ASSERT_EQ(previous_inserted_id + 1, inserted_id);
      previous_inserted_id = inserted_id;
      updateIndex(inserted_id, add_to_index, rebuild_index, check_consistency);
    }

    // A couple of tuples in the middle.
    inserted_id = generateAndInsertTuple(4, false, "bb");
    ASSERT_EQ(previous_inserted_id + 1, inserted_id);
    previous_inserted_id = inserted_id;
    updateIndex(inserted_id, add_to_index, rebuild_index, check_consistency);

    inserted_id = generateAndInsertTuple(4, false, "cc");
    ASSERT_EQ(previous_inserted_id + 1, inserted_id);
    previous_inserted_id = inserted_id;
    updateIndex(inserted_id, add_to_index, rebuild_index, check_consistency);

    // 10 tuples in reverse order.
    for (int tnum = 0; tnum < 10; ++tnum) {
      inserted_id = generateAndInsertTuple(-tnum, false, "dd");
      ASSERT_EQ(previous_inserted_id + 1, inserted_id);
      previous_inserted_id = inserted_id;
      updateIndex(inserted_id, add_to_index, rebuild_index, check_consistency);
    }

    // Some tuples with NULLs.
    inserted_id = generateAndInsertTuple(0, true, "aa");
    ASSERT_EQ(previous_inserted_id + 1, inserted_id);
    previous_inserted_id = inserted_id;
    updateIndex(inserted_id, add_to_index, rebuild_index, check_consistency);

    inserted_id = generateAndInsertTuple(7, true, "aa");
    ASSERT_EQ(previous_inserted_id + 1, inserted_id);
    previous_inserted_id = inserted_id;
    updateIndex(inserted_id, add_to_index, rebuild_index, check_consistency);

    inserted_id = generateAndInsertTuple(-7, true, "dd");
    ASSERT_EQ(previous_inserted_id + 1, inserted_id);
    previous_inserted_id = inserted_id;
    updateIndex(inserted_id, add_to_index, rebuild_index, check_consistency);

    // Some way-out-there values.
    inserted_id = generateAndInsertTuple(numeric_limits<int64_t>::min(), false, "0000");
    ASSERT_EQ(previous_inserted_id + 1, inserted_id);
    previous_inserted_id = inserted_id;
    updateIndex(inserted_id, add_to_index, rebuild_index, check_consistency);

    inserted_id = generateAndInsertTuple(numeric_limits<int64_t>::max(), false, "zzzz");
    ASSERT_EQ(previous_inserted_id + 1, inserted_id);
    previous_inserted_id = inserted_id;
    updateIndex(inserted_id, add_to_index, rebuild_index, check_consistency);

    // Some duplicate values.
    for (int tnum = 0; tnum < 5; ++tnum) {
      inserted_id = generateAndInsertTuple(2 * tnum, false, "aa");
      ASSERT_EQ(previous_inserted_id + 1, inserted_id);
      previous_inserted_id = inserted_id;
      updateIndex(inserted_id, add_to_index, rebuild_index, check_consistency);

      inserted_id = generateAndInsertTuple(-2 * tnum, false, "dd");
      ASSERT_EQ(previous_inserted_id + 1, inserted_id);
      previous_inserted_id = inserted_id;
      updateIndex(inserted_id, add_to_index, rebuild_index, check_consistency);
    }
  }

  // Helper method for insertSampleData(). If 'add_to_index' is true, an entry
  // for 'new_tuple' is added to 'index_'. If 'rebuild_index' is true, then
  // index_->rebuild() is called. If 'check_consistency' is true,
  // checkIndexConsistency() is called after 'index_' is modified.
  void updateIndex(const tuple_id new_tuple,
                   const bool add_to_index,
                   const bool rebuild_index,
                   const bool check_consistency) {
    if (add_to_index) {
      EXPECT_TRUE(index_->addEntry(new_tuple));
    }
    if (rebuild_index) {
      EXPECT_TRUE(index_->rebuild());
    }
    if (check_consistency) {
      checkIndexConsistency();
    }
  }

  // Remove tuples previously inserted by insertSampleData() in a different
  // order. If 'remove_from_index' is true, entries for each removed tuple will
  // be removed from 'index_' one-by-one. If 'rebuild_index' is true, then
  // 'index_' will be rebuilt after each tuple is removed. If
  // 'check_consistency' is true, checkIndexConsistency() is called after each
  // modification to 'index_'.
  void deleteSampleData(const bool remove_from_index,
                        const bool rebuild_index,
                        const bool check_consistency) {
    for (int i = 0; i < kNumSampleTuples; ++i) {
      ASSERT_TRUE(tuple_store_->hasTupleWithID(kDeleteSequence[i]));
      if (remove_from_index) {
        index_->removeEntry(kDeleteSequence[i]);
        if (check_consistency) {
          checkIndexConsistency();
        }
      }
      ASSERT_FALSE(tuple_store_->deleteTuple(kDeleteSequence[i]));
      if (rebuild_index) {
        EXPECT_TRUE(index_->rebuild());
        if (check_consistency) {
          checkIndexConsistency();
        }
      }
    }
  }

  // Check that index_'s data structures are internally consistent.
  void checkIndexConsistency() {
    checkIndexBalance();
    checkIndexOrdering();
    checkIndexNodeGroupAllocationConsistency();
    checkIndexSiblingConsistency();
  }

  // Determine the depth along the left edge of the tree, then verify that all
  // leaves are at the same level.
  void checkIndexBalance() const {
    int depth = 0;
    const CSBTreeIndexSubBlock::NodeHeader *current_node_header
        = static_cast<const CSBTreeIndexSubBlock::NodeHeader*>(index_->getRootNode());
    while (!current_node_header->is_leaf) {
      ++depth;
      current_node_header = static_cast<const CSBTreeIndexSubBlock::NodeHeader*>(
          index_->getNode(current_node_header->node_group_reference, 0));
    }

    checkNodeBalance(index_->getRootNode(), depth);
  }

  // Check that '*node' is balanced. 'depth' counts down level-by-level to 0 at
  // the leaves.
  void checkNodeBalance(const void *node, const int depth) const {
    const CSBTreeIndexSubBlock::NodeHeader *node_header
        = static_cast<const CSBTreeIndexSubBlock::NodeHeader*>(node);
    if (node_header->is_leaf) {
      EXPECT_EQ(0, depth);
    } else {
      ASSERT_GT(node_header->num_keys, 0);
      ASSERT_LE(node_header->num_keys, index_->max_keys_internal_);
      ASSERT_GE(node_header->node_group_reference, 0);
      // Cast to unsigned to suppress GCC warning. Safe because of previous
      // assertion.
      ASSERT_LE(static_cast<unsigned int>(node_header->node_group_reference),
                index_->node_group_used_bitmap_->size());

      // If an internal node is not the root, it should be at least half-full.
      if (node != index_->getRootNode()) {
        EXPECT_GE(node_header->num_keys, index_->max_keys_internal_ / 2);
      }

      for (uint16_t child_number = 0;
           child_number <= node_header->num_keys;
           ++child_number) {
        checkNodeBalance(index_->getNode(node_header->node_group_reference, child_number), depth - 1);
      }
    }
  }

  // Check that the nodes in index_ are well-ordered.
  void checkIndexOrdering() const {
    if (index_->key_is_composite_) {
      checkIndexOrderingWithComparator(*(index_->composite_key_comparator_));
    } else if (index_->key_is_compressed_) {
      const CompressedTupleStorageSubBlock &compressed_tuple_store
          = static_cast<const CompressedTupleStorageSubBlock&>(index_->tuple_store_);
      switch (compressed_tuple_store.compressedGetCompressedAttributeSize(
                  index_->indexed_attribute_ids_.front())) {
        case 1:
          checkIndexOrderingWithComparator(CompressedCodeLessComparator<std::uint8_t>());
          break;
        case 2:
          checkIndexOrderingWithComparator(CompressedCodeLessComparator<std::uint16_t>());
          break;
        case 4:
          checkIndexOrderingWithComparator(CompressedCodeLessComparator<std::uint32_t>());
          break;
        default:
          FATAL_ERROR("Unexpected compressed key byte-length (not 1, 2, or 4) encountered "
                      "in CSBTreeIndexSubBlockTest::checkIndexOrdering()");
      }
    } else {
      InvokeOnLessComparatorForTypeIgnoreNullability(
          *(index_->key_type_),
          [&](const auto &comp) {  // NOLINT(build/c++11)
        this->checkIndexOrderingWithComparator(comp);
      });
    }
  }

  template <typename ComparatorT>
  void checkIndexOrderingWithComparator(const ComparatorT &key_comparator) const {
    const void *root_node = index_->getRootNode();
    if (static_cast<const CSBTreeIndexSubBlock::NodeHeader*>(root_node)->is_leaf) {
      checkLeafNodeOrdering(root_node, nullptr, nullptr, key_comparator);
    } else {
      checkInternalNodeOrdering(root_node, nullptr, nullptr, key_comparator);
    }
  }

  // Check that the subtree of 'index_' anchored at '*node' is properly
  // ordered. If '*min_bound_key' is not NULL, all entries in the subtree
  // should be greater than or equal to '*min_bound_key'. If '*max_bound_key'
  // is not NULL, all entries in the subtree should be less than or equal to
  // '*max_bound_key'. In all cases, keys within a node should be in ascending
  // order, and non-leaf nodes should have at least one key.
  template <typename ComparatorT>
  void checkInternalNodeOrdering(const void *node,
                                 const void *min_bound_key,
                                 const void *max_bound_key,
                                 const ComparatorT &key_comparator) const {
    const CSBTreeIndexSubBlock::NodeHeader *node_header
        = static_cast<const CSBTreeIndexSubBlock::NodeHeader*>(node);
    ASSERT_FALSE(node_header->is_leaf);

    // NodeHeader sanity check.
    ASSERT_GT(node_header->num_keys, 0);
    ASSERT_LE(node_header->num_keys, index_->max_keys_internal_);
    ASSERT_GE(node_header->node_group_reference, 0);
    // Cast to unsigned to supress GCC warning. Safe due to previous assertion.
    ASSERT_LE(static_cast<unsigned int>(node_header->node_group_reference),
              index_->node_group_used_bitmap_->size());
    EXPECT_TRUE(index_->node_group_used_bitmap_->getBit(node_header->node_group_reference));

    // Check the first child.
    const void *first_child = index_->getNode(node_header->node_group_reference, 0);
    const void *first_key = static_cast<const char*>(node) + sizeof(CSBTreeIndexSubBlock::NodeHeader);
    bool children_are_leaves;
    if (static_cast<const CSBTreeIndexSubBlock::NodeHeader*>(first_child)->is_leaf) {
      children_are_leaves = true;
      checkLeafNodeOrdering(first_child, min_bound_key, first_key, key_comparator);
    } else {
      children_are_leaves = false;
      checkInternalNodeOrdering(first_child, min_bound_key, first_key, key_comparator);
    }

    // Check other children.
    for (uint16_t key_num = 0;
         key_num < node_header->num_keys;
         ++key_num) {
      const void *key_ptr = static_cast<const char*>(node)
                            + sizeof(CSBTreeIndexSubBlock::NodeHeader)
                            + key_num * index_->key_length_bytes_;
      if (key_num > 0) {
        const void *previous_key_ptr = static_cast<const char*>(key_ptr) - index_->key_length_bytes_;
        // Previous key should be less than this key OR equal.
        if (!key_comparator.compareDataPtrs(previous_key_ptr, key_ptr)) {
          EXPECT_FALSE(key_comparator.compareDataPtrs(key_ptr, previous_key_ptr));
        }
      }

      const void *child_node = index_->getNode(node_header->node_group_reference, key_num + 1);
      if (children_are_leaves) {
        EXPECT_TRUE(static_cast<const CSBTreeIndexSubBlock::NodeHeader*>(child_node)->is_leaf);
      } else {
        EXPECT_FALSE(static_cast<const CSBTreeIndexSubBlock::NodeHeader*>(child_node)->is_leaf);
      }

      if (key_num == node_header->num_keys - 1) {
        // Last child has same maximum bound as parent.
        if (static_cast<const CSBTreeIndexSubBlock::NodeHeader*>(child_node)->is_leaf) {
          checkLeafNodeOrdering(child_node, key_ptr, max_bound_key, key_comparator);
        } else {
          checkInternalNodeOrdering(child_node, key_ptr, max_bound_key, key_comparator);
        }
      } else {
        const void *next_key_ptr = static_cast<const char*>(key_ptr) + index_->key_length_bytes_;
        if (static_cast<const CSBTreeIndexSubBlock::NodeHeader*>(child_node)->is_leaf) {
          checkLeafNodeOrdering(child_node, key_ptr, next_key_ptr, key_comparator);
        } else {
          checkInternalNodeOrdering(child_node, key_ptr, next_key_ptr, key_comparator);
        }
      }
    }
  }

  // Check that the keys in the leaf node '*node' are in ascending order. If
  // '*min_bound_key' is non-NULL, make sure that all keys in '*node' are
  // greater than or equal to '*min_bound_key'. If '*max_bound_key' is
  // non-NULL, make sure that all keys in '*node' are less than or equal to
  // '*max_bound_key'. Also check that all tuple_ids referenced in '*node' are
  // valid.
  template <typename ComparatorT>
  void checkLeafNodeOrdering(const void *node,
                             const void *min_bound_key,
                             const void *max_bound_key,
                             const ComparatorT &key_comparator) const {
    const CSBTreeIndexSubBlock::NodeHeader *node_header
        = static_cast<const CSBTreeIndexSubBlock::NodeHeader*>(node);
    ASSERT_TRUE(node_header->is_leaf);

    // NodeHeader sanity check.
    EXPECT_LE(node_header->num_keys, index_->max_keys_leaf_);
    if (node_header->node_group_reference == CSBTreeIndexSubBlock::kNodeGroupNextLeaf) {
      // Every leaf node but the last should have a '*max_bound_key'.
      EXPECT_NE(max_bound_key, nullptr);
    } else if (node_header->node_group_reference == CSBTreeIndexSubBlock::kNodeGroupNone) {
      // The rightmost leaf should have no maximum bound.
      EXPECT_EQ(nullptr, max_bound_key);
    } else {
      // Last node in a group.
      EXPECT_GE(node_header->node_group_reference, 0);
      EXPECT_TRUE(index_->node_group_used_bitmap_->getBit(node_header->node_group_reference));
      EXPECT_NE(max_bound_key, nullptr);
    }

    // Check the entries.
    for (uint16_t entry_num = 0;
         entry_num < node_header->num_keys;
         ++entry_num) {
      const void *key_ptr = static_cast<const char*>(node)
                            + sizeof(CSBTreeIndexSubBlock::NodeHeader)
                            + entry_num * index_->key_tuple_id_pair_length_bytes_;
      if (entry_num > 0) {
        const void *previous_key_ptr = static_cast<const char*>(key_ptr)
                                       - index_->key_tuple_id_pair_length_bytes_;
        // Previous key should be less than this key OR equal.
        if (!key_comparator.compareDataPtrs(previous_key_ptr, key_ptr)) {
          EXPECT_FALSE(key_comparator.compareDataPtrs(key_ptr, previous_key_ptr));
        }
      }

      if (min_bound_key != nullptr) {
        if (!key_comparator.compareDataPtrs(min_bound_key, key_ptr)) {
          EXPECT_FALSE(key_comparator.compareDataPtrs(key_ptr, min_bound_key));
        }
      }
      if (max_bound_key != nullptr) {
        if (!key_comparator.compareDataPtrs(key_ptr, max_bound_key)) {
          EXPECT_FALSE(key_comparator.compareDataPtrs(max_bound_key, key_ptr));
        }
      }

      const tuple_id *tuple_ptr = reinterpret_cast<const tuple_id*>(
          static_cast<const char*>(key_ptr) + index_->key_length_bytes_);
      EXPECT_GE(*tuple_ptr, 0);
    }
  }

  // Check that all node groups reachable from the root node are marked as used
  // in the index's node_group_used_bitmap_, and that no other groups are
  // marked as used. Also check that the root node group is not a child of any
  // node, and all other node groups are the child of exactly one internal
  // node. Finally, check that the count of free node groups
  // (index_->num_free_node_groups_) and the location of the first free node
  // group (index_->next_free_node_group_) are correct.
  void checkIndexNodeGroupAllocationConsistency() {
    set<int> referenced_node_groups;
    ASSERT_GE(index_->getRootNodeGroupNumber(), 0);
    referenced_node_groups.insert(index_->getRootNodeGroupNumber());
    collectReferencedNodeGroups(index_->getRootNode(), &referenced_node_groups);

    int first_free_group = CSBTreeIndexSubBlock::kNodeGroupNone;
    for (unsigned int node_group_num = 0;
         node_group_num < index_->node_group_used_bitmap_->size();
         ++node_group_num) {
      if (referenced_node_groups.find(node_group_num) == referenced_node_groups.end()) {
        EXPECT_FALSE(index_->node_group_used_bitmap_->getBit(node_group_num));
        if (first_free_group == CSBTreeIndexSubBlock::kNodeGroupNone) {
          first_free_group = node_group_num;
        }
      } else {
        EXPECT_TRUE(index_->node_group_used_bitmap_->getBit(node_group_num));
      }
    }

    EXPECT_EQ(first_free_group, index_->next_free_node_group_);
    ASSERT_GE(index_->num_free_node_groups_, 0);
    EXPECT_EQ(index_->node_group_used_bitmap_->size() - referenced_node_groups.size(),
              static_cast<unsigned int>(index_->num_free_node_groups_));
  }

  void collectReferencedNodeGroups(const void *node,
                                   set<int> *referenced_node_groups) {
    const CSBTreeIndexSubBlock::NodeHeader *node_header
        = static_cast<const CSBTreeIndexSubBlock::NodeHeader*>(node);
    if (!node_header->is_leaf) {
      ASSERT_GE(node_header->node_group_reference, 0);
      // The same node group should not be the child of more than one node.
      EXPECT_EQ(0u, referenced_node_groups->count(node_header->node_group_reference));
      referenced_node_groups->insert(node_header->node_group_reference);
      for (uint16_t child_num = 0;
           child_num < node_header->num_keys + 1;
           ++child_num) {
        collectReferencedNodeGroups(index_->getNode(node_header->node_group_reference, child_num),
                                    referenced_node_groups);
      }
    }
  }

  void checkIndexSiblingConsistency() {
    int last_leaf_sibling = checkNodeSiblingConsistency(index_->getRootNode(),
                                                        index_->getRootNodeGroupNumber(),
                                                        CSBTreeIndexSubBlock::kNodeGroupNone);
    EXPECT_EQ(CSBTreeIndexSubBlock::kNodeGroupNone, last_leaf_sibling);
  }

  int checkNodeSiblingConsistency(const void *node,
                                  const int node_group_from_parent,
                                  int node_group_from_sibling) {
    const CSBTreeIndexSubBlock::NodeHeader *node_header
        = static_cast<const CSBTreeIndexSubBlock::NodeHeader*>(node);
    if (node_header->is_leaf) {
      EXPECT_GE(node_group_from_parent, 0);
      if (node_group_from_sibling != CSBTreeIndexSubBlock::kNodeGroupNone) {
        EXPECT_EQ(node_group_from_parent, node_group_from_sibling);
      }
      if (node_header->node_group_reference == CSBTreeIndexSubBlock::kNodeGroupNextLeaf) {
        return node_group_from_parent;
      } else {
        return node_header->node_group_reference;
      }
    } else {
      for (uint16_t child_num = 0;
           child_num < node_header->num_keys + 1;
           ++child_num) {
        node_group_from_sibling
            = checkNodeSiblingConsistency(index_->getNode(node_header->node_group_reference, child_num),
                                          node_header->node_group_reference,
                                          node_group_from_sibling);
      }
      return node_group_from_sibling;
    }
  }

  // Create a ComparisonPredicate of the form "attribute comp literal".
  template <typename AttributeType>
  ComparisonPredicate* generateNumericComparisonPredicate(
      const ComparisonID comp,
      const attribute_id attribute,
      const typename AttributeType::cpptype literal) {
    ScalarAttribute *scalar_attribute = new ScalarAttribute(*relation_->getAttributeById(attribute));
    ScalarLiteral *scalar_literal
        = new ScalarLiteral(AttributeType::InstanceNonNullable().makeValue(&literal),
                            AttributeType::InstanceNonNullable());
    return new ComparisonPredicate(ComparisonFactory::GetComparison(comp), scalar_attribute, scalar_literal);
  }

  ComparisonPredicate* generateStringComparisonPredicate(
      const ComparisonID comp,
      const attribute_id attribute,
      const string &literal) {
    ScalarAttribute *scalar_attribute = new ScalarAttribute(*relation_->getAttributeById(attribute));
    ScalarLiteral *scalar_literal = new ScalarLiteral(
        VarCharType::InstanceNonNullable(literal.size()).makeValue(
            literal.c_str(),
            literal.size() + 1).ensureNotReference(),
        VarCharType::InstanceNonNullable(literal.size()));
    return new ComparisonPredicate(ComparisonFactory::GetComparison(comp), scalar_attribute, scalar_literal);
  }

  std::unique_ptr<CatalogRelation> relation_;
  std::unique_ptr<MockTupleStorageSubBlock> tuple_store_;
  ScopedBuffer index_memory_;
  std::unique_ptr<IndexSubBlockDescription> index_description_;
  std::unique_ptr<CSBTreeIndexSubBlock> index_;
};

const char CSBTreeIndexSubBlockTest::kCharAttrNullValue[] = "_NULLSTRING";
const tuple_id CSBTreeIndexSubBlockTest::kDeleteSequence[]
    = { 0, 5, 10, 20, 30, 36, 1, 2, 3, 4, 9, 8, 7, 6, 35, 12, 14, 16, 18, 11,
        13, 15, 17, 19, 21, 22, 29, 28, 23, 27, 24, 26, 25, 34, 31, 32, 33 };

typedef CSBTreeIndexSubBlockTest CSBTreeIndexSubBlockDeathTest;

TEST_F(CSBTreeIndexSubBlockTest, BlockTooSmallTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(0);  // long_attr
  EXPECT_THROW(createIndex(indexed_attrs, 100), BlockMemoryTooSmall);
}

TEST_F(CSBTreeIndexSubBlockTest, KeyTooLargeTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(5);  // big_char_attr
  EXPECT_THROW(createIndex(indexed_attrs, kIndexSubBlockSize), CSBTreeKeyTooLarge);
}

TEST_F(CSBTreeIndexSubBlockTest, LongKeyInsertTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(0);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, true);
}

TEST_F(CSBTreeIndexSubBlockTest, LongKeyDeleteTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(0);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, false);
  deleteSampleData(true, false, true);
}

TEST_F(CSBTreeIndexSubBlockTest, LongKeyRebuildTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(0);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(false, true, true);
  deleteSampleData(false, true, true);
}

TEST_F(CSBTreeIndexSubBlockTest, LongKeyGetMatchesForPredicateTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(0);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, false);
  std::unique_ptr<ComparisonPredicate> pred;
  std::unique_ptr<TupleIdSequence> match_sequence;

  // Test an equality predicate with several matches.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual, 0, 0));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(5u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(0));
  EXPECT_TRUE(match_sequence->get(12));
  EXPECT_TRUE(match_sequence->get(22));
  EXPECT_TRUE(match_sequence->get(27));
  EXPECT_TRUE(match_sequence->get(28));

  // Test an equality predicate with a single match.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual, 0, -3));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(1u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(15));

  // Test an equality predicate with no matches.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual, 0, 42));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_TRUE(match_sequence->empty());

  // Test an equality predicate with the least value.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual, 0, numeric_limits<int64_t>::min()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(1u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(25));

  // Test an equality predicate with the greatest value.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual, 0, numeric_limits<int64_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(1u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(26));

  // Test an inequality predicate.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kNotEqual, 0, 0));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(32u, match_sequence->numTuples());
  EXPECT_FALSE(match_sequence->get(0));
  EXPECT_FALSE(match_sequence->get(12));
  EXPECT_FALSE(match_sequence->get(22));
  EXPECT_FALSE(match_sequence->get(27));
  EXPECT_FALSE(match_sequence->get(28));

  // Test a less-than predicate.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLess, 0, -6));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(6u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(19));
  EXPECT_TRUE(match_sequence->get(20));
  EXPECT_TRUE(match_sequence->get(21));
  EXPECT_TRUE(match_sequence->get(24));
  EXPECT_TRUE(match_sequence->get(25));
  EXPECT_TRUE(match_sequence->get(36));

  // Test a less-than-or-equal predicate.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLessOrEqual, 0, -6));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(8u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(18));
  EXPECT_TRUE(match_sequence->get(19));
  EXPECT_TRUE(match_sequence->get(20));
  EXPECT_TRUE(match_sequence->get(21));
  EXPECT_TRUE(match_sequence->get(24));
  EXPECT_TRUE(match_sequence->get(25));
  EXPECT_TRUE(match_sequence->get(34));
  EXPECT_TRUE(match_sequence->get(36));

  // Test a greater-than predicate.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreater, 0, 6));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(6u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(7));
  EXPECT_TRUE(match_sequence->get(8));
  EXPECT_TRUE(match_sequence->get(9));
  EXPECT_TRUE(match_sequence->get(23));
  EXPECT_TRUE(match_sequence->get(26));
  EXPECT_TRUE(match_sequence->get(35));

  // Test a greater-than-or-equal predicate.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreaterOrEqual, 0, 6));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(8u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(6));
  EXPECT_TRUE(match_sequence->get(7));
  EXPECT_TRUE(match_sequence->get(8));
  EXPECT_TRUE(match_sequence->get(9));
  EXPECT_TRUE(match_sequence->get(23));
  EXPECT_TRUE(match_sequence->get(26));
  EXPECT_TRUE(match_sequence->get(33));
  EXPECT_TRUE(match_sequence->get(35));
}

TEST_F(CSBTreeIndexSubBlockTest, NullableLongKeyInsertTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(1);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, true);
}

TEST_F(CSBTreeIndexSubBlockTest, NullableLongKeyDeleteTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(1);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, false);
  deleteSampleData(true, false, true);
}

TEST_F(CSBTreeIndexSubBlockTest, NullableLongKeyRebuildTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(1);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(false, true, true);
  deleteSampleData(false, true, true);
}

TEST_F(CSBTreeIndexSubBlockTest, NullableLongKeyGetMatchesForPredicateTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(1);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, false);
  std::unique_ptr<ComparisonPredicate> pred;
  std::unique_ptr<TupleIdSequence> match_sequence;

  // Test an equality predicate with several matches.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual, 1, 0));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(4u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(0));
  EXPECT_TRUE(match_sequence->get(12));
  EXPECT_TRUE(match_sequence->get(27));
  EXPECT_TRUE(match_sequence->get(28));

  // Test an inequality predicate.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kNotEqual, 1, 0));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(30u, match_sequence->numTuples());
  EXPECT_FALSE(match_sequence->get(0));
  EXPECT_FALSE(match_sequence->get(12));
  EXPECT_FALSE(match_sequence->get(22));
  EXPECT_FALSE(match_sequence->get(23));
  EXPECT_FALSE(match_sequence->get(24));
  EXPECT_FALSE(match_sequence->get(27));
  EXPECT_FALSE(match_sequence->get(28));

  // Test a less-than predicate.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLess, 1, -6));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(5u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(19));
  EXPECT_TRUE(match_sequence->get(20));
  EXPECT_TRUE(match_sequence->get(21));
  EXPECT_TRUE(match_sequence->get(25));
  EXPECT_TRUE(match_sequence->get(36));

  // Test a less-than-or-equal predicate.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLessOrEqual, 1, -6));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(7u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(18));
  EXPECT_TRUE(match_sequence->get(19));
  EXPECT_TRUE(match_sequence->get(20));
  EXPECT_TRUE(match_sequence->get(21));
  EXPECT_TRUE(match_sequence->get(25));
  EXPECT_TRUE(match_sequence->get(34));
  EXPECT_TRUE(match_sequence->get(36));

  // Test a greater-than predicate.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreater, 1, 6));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(5u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(7));
  EXPECT_TRUE(match_sequence->get(8));
  EXPECT_TRUE(match_sequence->get(9));
  EXPECT_TRUE(match_sequence->get(26));
  EXPECT_TRUE(match_sequence->get(35));

  // Test a greater-than-or-equal predicate.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreaterOrEqual, 1, 6));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(7u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(6));
  EXPECT_TRUE(match_sequence->get(7));
  EXPECT_TRUE(match_sequence->get(8));
  EXPECT_TRUE(match_sequence->get(9));
  EXPECT_TRUE(match_sequence->get(26));
  EXPECT_TRUE(match_sequence->get(33));
  EXPECT_TRUE(match_sequence->get(35));
}

TEST_F(CSBTreeIndexSubBlockTest, FloatKeyInsertTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(2);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, true);
}

TEST_F(CSBTreeIndexSubBlockTest, FloatKeyDeleteTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(2);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, false);
  deleteSampleData(true, false, true);
}

TEST_F(CSBTreeIndexSubBlockTest, FloatKeyRebuildTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(2);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(false, true, true);
  deleteSampleData(false, true, true);
}

TEST_F(CSBTreeIndexSubBlockTest, FloatKeyGetMatchesForPredicateTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(2);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, false);
  std::unique_ptr<ComparisonPredicate> pred;
  std::unique_ptr<TupleIdSequence> match_sequence;

  // Test an equality predicate with several matches.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual, 2, 0.0));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(5u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(0));
  EXPECT_TRUE(match_sequence->get(12));
  EXPECT_TRUE(match_sequence->get(22));
  EXPECT_TRUE(match_sequence->get(27));
  EXPECT_TRUE(match_sequence->get(28));

  // Test an equality predicate with a single match.
  pred.reset(generateNumericComparisonPredicate<FloatType>(ComparisonID::kEqual, 2, -0.75));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(1u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(15));

  // Test an equality predicate with no matches.
  pred.reset(generateNumericComparisonPredicate<FloatType>(ComparisonID::kEqual, 2, 42.0));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_TRUE(match_sequence->empty());

  // Test an equality predicate with the least value.
  pred.reset(generateNumericComparisonPredicate<FloatType>(ComparisonID::kEqual,
                                                           2,
                                                           0.25 * numeric_limits<int64_t>::min()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(1u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(25));

  // Test an equality predicate with the greatest value.
  pred.reset(generateNumericComparisonPredicate<FloatType>(ComparisonID::kEqual,
                                                           2,
                                                           0.25 * numeric_limits<int64_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(1u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(26));

  // Test an inequality predicate.
  pred.reset(generateNumericComparisonPredicate<FloatType>(ComparisonID::kNotEqual, 2, 0.0));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(32u, match_sequence->numTuples());
  EXPECT_FALSE(match_sequence->get(0));
  EXPECT_FALSE(match_sequence->get(12));
  EXPECT_FALSE(match_sequence->get(22));
  EXPECT_FALSE(match_sequence->get(27));
  EXPECT_FALSE(match_sequence->get(28));

  // Test a less-than predicate.
  pred.reset(generateNumericComparisonPredicate<FloatType>(ComparisonID::kLess, 2, -1.5));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(6u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(19));
  EXPECT_TRUE(match_sequence->get(20));
  EXPECT_TRUE(match_sequence->get(21));
  EXPECT_TRUE(match_sequence->get(24));
  EXPECT_TRUE(match_sequence->get(25));
  EXPECT_TRUE(match_sequence->get(36));

  // Test a less-than-or-equal predicate.
  pred.reset(generateNumericComparisonPredicate<FloatType>(ComparisonID::kLessOrEqual, 2, -1.5));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(8u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(18));
  EXPECT_TRUE(match_sequence->get(19));
  EXPECT_TRUE(match_sequence->get(20));
  EXPECT_TRUE(match_sequence->get(21));
  EXPECT_TRUE(match_sequence->get(24));
  EXPECT_TRUE(match_sequence->get(25));
  EXPECT_TRUE(match_sequence->get(34));
  EXPECT_TRUE(match_sequence->get(36));

  // Test a greater-than predicate.
  pred.reset(generateNumericComparisonPredicate<FloatType>(ComparisonID::kGreater, 2, 1.5));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(6u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(7));
  EXPECT_TRUE(match_sequence->get(8));
  EXPECT_TRUE(match_sequence->get(9));
  EXPECT_TRUE(match_sequence->get(23));
  EXPECT_TRUE(match_sequence->get(26));
  EXPECT_TRUE(match_sequence->get(35));

  // Test a greater-than-or-equal predicate.
  pred.reset(generateNumericComparisonPredicate<FloatType>(ComparisonID::kGreaterOrEqual, 2, 1.5));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(8u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(6));
  EXPECT_TRUE(match_sequence->get(7));
  EXPECT_TRUE(match_sequence->get(8));
  EXPECT_TRUE(match_sequence->get(9));
  EXPECT_TRUE(match_sequence->get(23));
  EXPECT_TRUE(match_sequence->get(26));
  EXPECT_TRUE(match_sequence->get(33));
  EXPECT_TRUE(match_sequence->get(35));
}

TEST_F(CSBTreeIndexSubBlockTest, CharKeyInsertTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(3);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, true);
}

TEST_F(CSBTreeIndexSubBlockTest, CharKeyDeleteTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(3);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, false);
  deleteSampleData(true, false, true);
}

TEST_F(CSBTreeIndexSubBlockTest, CharKeyRebuildTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(3);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(false, true, true);
  deleteSampleData(false, true, true);
}

TEST_F(CSBTreeIndexSubBlockTest, CharKeyGetMatchesForPredicateTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(3);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, false);
  std::unique_ptr<ComparisonPredicate> pred;
  std::unique_ptr<TupleIdSequence> match_sequence;

  // Test an equality predicate with several matches.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kEqual, 3, "0aa"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(3u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(0));
  EXPECT_TRUE(match_sequence->get(22));
  EXPECT_TRUE(match_sequence->get(27));

  // Test an equality predicate with a single match.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kEqual, 3, "-3dd"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(1u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(15));

  // Test an equality predicate with no matches.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kEqual, 3, "foo"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_TRUE(match_sequence->empty());

  // Test an equality predicate with the least value.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kEqual, 3, "-1dd"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(1u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(13));

  // Test an equality predicate with the greatest value.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kEqual, 3, "9aa"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(1u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(9));

  // Test an inequality predicate.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kNotEqual, 3, "0aa"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(34u, match_sequence->numTuples());
  EXPECT_FALSE(match_sequence->get(0));
  EXPECT_FALSE(match_sequence->get(22));
  EXPECT_FALSE(match_sequence->get(27));

  // Test a less-than predicate.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kLess, 3, "-6dd"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(7u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(13));
  EXPECT_TRUE(match_sequence->get(14));
  EXPECT_TRUE(match_sequence->get(15));
  EXPECT_TRUE(match_sequence->get(16));
  EXPECT_TRUE(match_sequence->get(17));
  EXPECT_TRUE(match_sequence->get(30));
  EXPECT_TRUE(match_sequence->get(32));

  // Test a less-than-or-equal predicate.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kLessOrEqual, 3, "-6dd"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(9u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(13));
  EXPECT_TRUE(match_sequence->get(14));
  EXPECT_TRUE(match_sequence->get(15));
  EXPECT_TRUE(match_sequence->get(16));
  EXPECT_TRUE(match_sequence->get(17));
  EXPECT_TRUE(match_sequence->get(18));
  EXPECT_TRUE(match_sequence->get(30));
  EXPECT_TRUE(match_sequence->get(32));
  EXPECT_TRUE(match_sequence->get(34));

  // Test a greater-than predicate.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kGreater, 3, "6aa"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(6u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(7));
  EXPECT_TRUE(match_sequence->get(8));
  EXPECT_TRUE(match_sequence->get(9));
  EXPECT_TRUE(match_sequence->get(23));
  EXPECT_TRUE(match_sequence->get(26));
  EXPECT_TRUE(match_sequence->get(35));

  // Test a greater-than-or-equal predicate.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kGreaterOrEqual, 3, "6aa"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(8u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(6));
  EXPECT_TRUE(match_sequence->get(7));
  EXPECT_TRUE(match_sequence->get(8));
  EXPECT_TRUE(match_sequence->get(9));
  EXPECT_TRUE(match_sequence->get(23));
  EXPECT_TRUE(match_sequence->get(26));
  EXPECT_TRUE(match_sequence->get(33));
  EXPECT_TRUE(match_sequence->get(35));
}

TEST_F(CSBTreeIndexSubBlockTest, NullableCharKeyInsertTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(4);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, true);
}

TEST_F(CSBTreeIndexSubBlockTest, NullableCharKeyDeleteTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(4);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, false);
  deleteSampleData(true, false, true);
}

TEST_F(CSBTreeIndexSubBlockTest, NullableCharKeyRebuildTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(4);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(false, true, true);
  deleteSampleData(false, true, true);
}

TEST_F(CSBTreeIndexSubBlockTest, NullableCharKeyGetMatchesForPredicateTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(4);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, false);
  std::unique_ptr<ComparisonPredicate> pred;
  std::unique_ptr<TupleIdSequence> match_sequence;

  // Test an equality predicate with several matches.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kEqual, 4, "0aa"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(2u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(0));
  EXPECT_TRUE(match_sequence->get(27));

  // Test an inequality predicate.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kNotEqual, 4, "0aa"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(32u, match_sequence->numTuples());
  EXPECT_FALSE(match_sequence->get(0));
  EXPECT_FALSE(match_sequence->get(27));

  // Test a less-than predicate.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kLess, 4, "-6dd"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(7u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(13));
  EXPECT_TRUE(match_sequence->get(14));
  EXPECT_TRUE(match_sequence->get(15));
  EXPECT_TRUE(match_sequence->get(16));
  EXPECT_TRUE(match_sequence->get(17));
  EXPECT_TRUE(match_sequence->get(30));
  EXPECT_TRUE(match_sequence->get(32));

  // Test a less-than-or-equal predicate.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kLessOrEqual, 4, "-6dd"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(9u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(13));
  EXPECT_TRUE(match_sequence->get(14));
  EXPECT_TRUE(match_sequence->get(15));
  EXPECT_TRUE(match_sequence->get(16));
  EXPECT_TRUE(match_sequence->get(17));
  EXPECT_TRUE(match_sequence->get(18));
  EXPECT_TRUE(match_sequence->get(30));
  EXPECT_TRUE(match_sequence->get(32));
  EXPECT_TRUE(match_sequence->get(34));

  // Test a greater-than predicate.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kGreater, 4, "6aa"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(5u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(7));
  EXPECT_TRUE(match_sequence->get(8));
  EXPECT_TRUE(match_sequence->get(9));
  EXPECT_TRUE(match_sequence->get(26));
  EXPECT_TRUE(match_sequence->get(35));

  // Test a greater-than-or-equal predicate.
  pred.reset(generateStringComparisonPredicate(ComparisonID::kGreaterOrEqual, 4, "6aa"));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(7u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(6));
  EXPECT_TRUE(match_sequence->get(7));
  EXPECT_TRUE(match_sequence->get(8));
  EXPECT_TRUE(match_sequence->get(9));
  EXPECT_TRUE(match_sequence->get(26));
  EXPECT_TRUE(match_sequence->get(33));
  EXPECT_TRUE(match_sequence->get(35));
}

TEST_F(CSBTreeIndexSubBlockTest, CompositeKeyInsertTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(0);
  indexed_attrs.push_back(3);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, true);
}

TEST_F(CSBTreeIndexSubBlockTest, CompositeKeyDeleteTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(0);
  indexed_attrs.push_back(3);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, false);
  deleteSampleData(true, false, true);
}

TEST_F(CSBTreeIndexSubBlockTest, CompositeKeyRebuildTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(0);
  indexed_attrs.push_back(3);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(false, true, true);
  deleteSampleData(false, true, true);
}

TEST_F(CSBTreeIndexSubBlockTest, NullableCompositeKeyInsertTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(0);
  indexed_attrs.push_back(4);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, true);
}

TEST_F(CSBTreeIndexSubBlockTest, NullableCompositeKeyDeleteTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(0);
  indexed_attrs.push_back(4);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, false);
  deleteSampleData(true, false, true);
}

TEST_F(CSBTreeIndexSubBlockTest, NullableCompositeKeyRebuildTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(0);
  indexed_attrs.push_back(4);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(false, true, true);
  deleteSampleData(false, true, true);
}

TEST_F(CSBTreeIndexSubBlockDeathTest, UnsetSubBlockTypeTest) {
  index_description_.reset(new IndexSubBlockDescription());
  index_description_->add_indexed_attribute_ids(0);

  index_memory_.reset(kIndexSubBlockSize);

  EXPECT_DEATH(index_.reset(new CSBTreeIndexSubBlock(*tuple_store_,
                                                     *index_description_,
                                                     true,
                                                     index_memory_.get(),
                                                     kIndexSubBlockSize)),
               "");
}

TEST_F(CSBTreeIndexSubBlockDeathTest, NoIndexedAttributeTest) {
  index_description_.reset(new IndexSubBlockDescription());
  index_description_->set_sub_block_type(IndexSubBlockDescription::CSB_TREE);

  index_memory_.reset(kIndexSubBlockSize);

  EXPECT_DEATH(index_.reset(new CSBTreeIndexSubBlock(*tuple_store_,
                                                     *index_description_,
                                                     true,
                                                     index_memory_.get(),
                                                     kIndexSubBlockSize)),
               "");
}

TEST_F(CSBTreeIndexSubBlockDeathTest, IndexOnVariableLengthAttributeTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(6);  // varchar_attr
  EXPECT_DEATH(createIndex(indexed_attrs, kIndexSubBlockSize), "");
}

TEST_F(CSBTreeIndexSubBlockDeathTest, IndexOnNonexistentAttributeTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(7);
  EXPECT_DEATH(createIndex(indexed_attrs, kIndexSubBlockSize), "");
}

TEST_F(CSBTreeIndexSubBlockDeathTest, GetMatchesForPredicateOnNonIndexedAttributeTest) {
  vector<attribute_id> indexed_attrs;
  indexed_attrs.push_back(0);
  createIndex(indexed_attrs, kIndexSubBlockSize);

  insertSampleData(true, false, false);

  std::unique_ptr<ComparisonPredicate> pred;
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual, 1, 0));
  EXPECT_DEATH(index_->getMatchesForPredicate(*pred, nullptr), "");
}

// Test special functionality with a CompressedTupleStorageSubBlock and
// truncation-compressed key.
TEST_F(CSBTreeIndexSubBlockTest, TruncatedCompressedKeyTest) {
  // Create a relation with just a single attribute, which will be compressed.
  relation_.reset(new CatalogRelation(NULL, "TestRelation"));
  CatalogAttribute *truncated_long_attr = new CatalogAttribute(relation_.get(),
                                                               "truncated_long_attr",
                                                               TypeFactory::GetType(kLong, false));
  ASSERT_EQ(0, relation_->addAttribute(truncated_long_attr));

  // Create a CompressedPackedRowStoreTupleStorageSubBlock.
  TupleStorageSubBlockDescription compressed_tuple_store_description;
  compressed_tuple_store_description.set_sub_block_type(
      TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE);
  compressed_tuple_store_description.AddExtension(
      CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id,
      0);

  ScopedBuffer compressed_tuple_store_memory(kIndexSubBlockSize);
  std::unique_ptr<CompressedPackedRowStoreTupleStorageSubBlock> compressed_tuple_store;
  compressed_tuple_store.reset(new CompressedPackedRowStoreTupleStorageSubBlock(
      *relation_,
      compressed_tuple_store_description,
      true,
      compressed_tuple_store_memory.get(),
      kIndexSubBlockSize));

  // Create an index on the compressed tuple store.
  IndexSubBlockDescription compressed_index_description;
  compressed_index_description.set_sub_block_type(IndexSubBlockDescription::CSB_TREE);
  compressed_index_description.add_indexed_attribute_ids(0);

  index_memory_.reset(kIndexSubBlockSize);
  std::memset(index_memory_.get(), 0x0, kIndexSubBlockSize);
  index_.reset(new CSBTreeIndexSubBlock(*compressed_tuple_store,
                                        compressed_index_description,
                                        true,
                                        index_memory_.get(),
                                        kIndexSubBlockSize));

  // Bulk-load some tuples into the compressed tuple store.
  for (int tuple_num = 0; tuple_num < 100; ++tuple_num) {
    std::vector<TypedValue> attrs;
    attrs.emplace_back(static_cast<int64_t>(tuple_num));
    ASSERT_TRUE(compressed_tuple_store->insertTupleInBatch(Tuple(std::move(attrs))));
  }
  for (int tuple_num = 0; tuple_num < 100; ++tuple_num) {
    std::vector<TypedValue> attrs;
    attrs.emplace_back(static_cast<int64_t>(100 - tuple_num));
    ASSERT_TRUE(compressed_tuple_store->insertTupleInBatch(Tuple(std::move(attrs))));
  }

  // Build the tuple-store.
  compressed_tuple_store->rebuild();
  EXPECT_FALSE(compressed_tuple_store->compressedAttributeIsDictionaryCompressed(0));
  EXPECT_TRUE(compressed_tuple_store->compressedAttributeIsTruncationCompressed(0));
  EXPECT_EQ(1u, compressed_tuple_store->compressedGetCompressedAttributeSize(0));

  // Build the index.
  ASSERT_TRUE(index_->rebuild());
  checkIndexConsistency();

  // Evaluate some predicates.
  std::unique_ptr<ComparisonPredicate> pred;
  std::unique_ptr<TupleIdSequence> match_sequence;

  // Equality with a couple of matches.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual, 0, 50));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(2u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(50));
  EXPECT_TRUE(match_sequence->get(150));

  // Equality in-range, but with no match.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual, 0, 101));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(0u, match_sequence->numTuples());

  // Equality out-of-range.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual, 0, -1));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(0u, match_sequence->numTuples());

  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual,
                                                          0,
                                                          1 + numeric_limits<uint8_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(0u, match_sequence->numTuples());

  // Inequality (inverses of equality checks above).
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kNotEqual, 0, 50));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(198u, match_sequence->numTuples());
  EXPECT_FALSE(match_sequence->get(50));
  EXPECT_FALSE(match_sequence->get(150));

  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kNotEqual, 0, 101));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(200u, match_sequence->numTuples());

  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kNotEqual, 0, -1));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(200u, match_sequence->numTuples());

  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kNotEqual,
                                                          0,
                                                          1 + numeric_limits<uint8_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(200u, match_sequence->numTuples());

  // Less-than predicates.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLess, 0, 50));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(99u, match_sequence->numTuples());
  for (tuple_id tuple_num = 0; tuple_num < 200; ++tuple_num) {
    if ((tuple_num < 50) || (tuple_num > 150)) {
      EXPECT_TRUE(match_sequence->get(tuple_num));
    } else {
      EXPECT_FALSE(match_sequence->get(tuple_num));
    }
  }

  // Less-than predicate, in-range and matches everything.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLess, 0, 101));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(200u, match_sequence->numTuples());

  // Less-than predicate, out-of-range (matches nothing).
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLess, 0, 0));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(0u, match_sequence->numTuples());

  // Less-than predicate, out-of-range (matches everything).
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLess, 0, 1 + numeric_limits<uint8_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(200u, match_sequence->numTuples());

  // Less-than-or-equal predicates.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLessOrEqual, 0, 50));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(101u, match_sequence->numTuples());
  for (tuple_id tuple_num = 0; tuple_num < 200; ++tuple_num) {
    if ((tuple_num <= 50) || (tuple_num >= 150)) {
      EXPECT_TRUE(match_sequence->get(tuple_num));
    } else {
      EXPECT_FALSE(match_sequence->get(tuple_num));
    }
  }

  // Less-than-or-equal predicate, in-range and matches everything.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLessOrEqual, 0, 100));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(200u, match_sequence->numTuples());

  // Less-than-or-equal predicate, out-of-range (matches nothing).
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLessOrEqual, 0, -1));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(0u, match_sequence->numTuples());

  // Less-than-or-equal predicate, out-of-range (matches everything).
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLessOrEqual,
                                                          0,
                                                          numeric_limits<uint8_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(200u, match_sequence->numTuples());

  // Greater-than predicates.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreater, 0, 50));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(99u, match_sequence->numTuples());
  for (tuple_id tuple_num = 0; tuple_num < 200; ++tuple_num) {
    if ((tuple_num > 50) && (tuple_num < 150)) {
      EXPECT_TRUE(match_sequence->get(tuple_num));
    } else {
      EXPECT_FALSE(match_sequence->get(tuple_num));
    }
  }

  // Greater-than predicate, in-range and matches nothing.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreater, 0, 100));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(0u, match_sequence->numTuples());

  // Greater-than predicate, out-of-range (matches nothing).
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreater, 0, numeric_limits<uint8_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(0u, match_sequence->numTuples());

  // Greater-than predicate, out-of-range (matches everything).
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreater, 0, -1));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(200u, match_sequence->numTuples());

  // Greater-than-or-equal predicates.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreaterOrEqual, 0, 50));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(101u, match_sequence->numTuples());
  for (tuple_id tuple_num = 0; tuple_num < 200; ++tuple_num) {
    if ((tuple_num >= 50) && (tuple_num <= 150)) {
      EXPECT_TRUE(match_sequence->get(tuple_num));
    } else {
      EXPECT_FALSE(match_sequence->get(tuple_num));
    }
  }

  // Greater-than-or-equal predicate, in-range and matches nothing.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreaterOrEqual, 0, 101));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(0u, match_sequence->numTuples());

  // Greater-than-or-equal predicate, out-of-range (matches nothing).
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreaterOrEqual,
                                                          0,
                                                          1 + numeric_limits<uint8_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(0u, match_sequence->numTuples());

  // Greater-than-or-equal predicate, out-of-range (matches everything).
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreaterOrEqual, 0, 0));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(200u, match_sequence->numTuples());
}

// Test special functionality with a CompressedTupleStorageSubBlock and
// dictionary-compressed key.
TEST_F(CSBTreeIndexSubBlockTest, DictionaryCompressedKeyTest) {
  // Create a relation with just a single attribute, which will be compressed.
  relation_.reset(new CatalogRelation(NULL, "TestRelation"));
  CatalogAttribute *dict_long_attr = new CatalogAttribute(relation_.get(),
                                                          "dict_long_attr",
                                                          TypeFactory::GetType(kLong, false));
  ASSERT_EQ(0, relation_->addAttribute(dict_long_attr));

  // Create a CompressedPackedRowStoreTupleStorageSubBlock.
  TupleStorageSubBlockDescription compressed_tuple_store_description;
  compressed_tuple_store_description.set_sub_block_type(
      TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE);
  compressed_tuple_store_description.AddExtension(
      CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id,
      0);

  ScopedBuffer compressed_tuple_store_memory(kIndexSubBlockSize);
  std::unique_ptr<CompressedPackedRowStoreTupleStorageSubBlock> compressed_tuple_store;
  compressed_tuple_store.reset(new CompressedPackedRowStoreTupleStorageSubBlock(
      *relation_,
      compressed_tuple_store_description,
      true,
      compressed_tuple_store_memory.get(),
      kIndexSubBlockSize));

  // Create an index on the compressed tuple store.
  IndexSubBlockDescription compressed_index_description;
  compressed_index_description.set_sub_block_type(IndexSubBlockDescription::CSB_TREE);
  compressed_index_description.add_indexed_attribute_ids(0);

  index_memory_.reset(kIndexSubBlockSize);
  std::memset(index_memory_.get(), 0x0, kIndexSubBlockSize);
  index_.reset(new CSBTreeIndexSubBlock(*compressed_tuple_store,
                                        compressed_index_description,
                                        true,
                                        index_memory_.get(),
                                        kIndexSubBlockSize));

  // Bulk-load some tuples into the compressed tuple store.
  for (int tuple_num = 0; tuple_num < 100; ++tuple_num) {
    std::vector<TypedValue> attrs;
    attrs.emplace_back(static_cast<int64_t>(
        static_cast<int64_t>(tuple_num) + numeric_limits<uint32_t>::max()));
    ASSERT_TRUE(compressed_tuple_store->insertTupleInBatch(Tuple(std::move(attrs))));
  }
  for (int tuple_num = 0; tuple_num < 100; ++tuple_num) {
    std::vector<TypedValue> attrs;
    attrs.emplace_back(static_cast<int64_t>(
        static_cast<int64_t>(100 - tuple_num) + numeric_limits<uint32_t>::max()));
    ASSERT_TRUE(compressed_tuple_store->insertTupleInBatch(Tuple(std::move(attrs))));
  }

  // Build the tuple-store.
  compressed_tuple_store->rebuild();
  EXPECT_TRUE(compressed_tuple_store->compressedAttributeIsDictionaryCompressed(0));
  EXPECT_FALSE(compressed_tuple_store->compressedAttributeIsTruncationCompressed(0));
  EXPECT_EQ(1u, compressed_tuple_store->compressedGetCompressedAttributeSize(0));

  // Build the index.
  ASSERT_TRUE(index_->rebuild());
  checkIndexConsistency();

  // Evaluate some predicates.
  std::unique_ptr<ComparisonPredicate> pred;
  std::unique_ptr<TupleIdSequence> match_sequence;

  // Equality with a couple of matches.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual,
                                                          0,
                                                          static_cast<int64_t>(50) + numeric_limits<uint32_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(2u, match_sequence->numTuples());
  EXPECT_TRUE(match_sequence->get(50));
  EXPECT_TRUE(match_sequence->get(150));

  // Equality with no match.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual, 0, 0));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(0u, match_sequence->numTuples());

  // Inequality (inverses of equality checks above).
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kNotEqual,
                                                          0,
                                                          static_cast<int64_t>(50) + numeric_limits<uint32_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(198u, match_sequence->numTuples());
  EXPECT_FALSE(match_sequence->get(50));
  EXPECT_FALSE(match_sequence->get(150));

  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kNotEqual, 0, 0));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(200u, match_sequence->numTuples());

  // Less-than predicates.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLess,
                                                          0,
                                                          static_cast<int64_t>(50) + numeric_limits<uint32_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(99u, match_sequence->numTuples());
  for (tuple_id tuple_num = 0; tuple_num < 200; ++tuple_num) {
    if ((tuple_num < 50) || (tuple_num > 150)) {
      EXPECT_TRUE(match_sequence->get(tuple_num));
    } else {
      EXPECT_FALSE(match_sequence->get(tuple_num));
    }
  }

  // Less-than predicate which matches everything.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLess,
                                                          0,
                                                          static_cast<int64_t>(101)
                                                              + numeric_limits<uint32_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(200u, match_sequence->numTuples());

  // Less-than predicate which matches nothing.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLess, 0, numeric_limits<uint32_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(0u, match_sequence->numTuples());

  // Less-than-or-equal predicates.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLessOrEqual,
                                                          0,
                                                          static_cast<int64_t>(50) + numeric_limits<uint32_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(101u, match_sequence->numTuples());
  for (tuple_id tuple_num = 0; tuple_num < 200; ++tuple_num) {
    if ((tuple_num <= 50) || (tuple_num >= 150)) {
      EXPECT_TRUE(match_sequence->get(tuple_num));
    } else {
      EXPECT_FALSE(match_sequence->get(tuple_num));
    }
  }

  // Less-than-or-equal predicate which matches everything.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLessOrEqual,
                                                          0,
                                                          static_cast<int64_t>(100)
                                                              + numeric_limits<uint32_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(200u, match_sequence->numTuples());

  // Less-than-or-equal predicate which matches nothing.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kLessOrEqual,
                                                          0,
                                                          numeric_limits<uint32_t>::max() - 1));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(0u, match_sequence->numTuples());

  // Greater-than predicates.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreater,
                                                          0,
                                                          static_cast<int64_t>(50) + numeric_limits<uint32_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(99u, match_sequence->numTuples());
  for (tuple_id tuple_num = 0; tuple_num < 200; ++tuple_num) {
    if ((tuple_num > 50) && (tuple_num < 150)) {
      EXPECT_TRUE(match_sequence->get(tuple_num));
    } else {
      EXPECT_FALSE(match_sequence->get(tuple_num));
    }
  }

  // Greater-than predicate which matches nothing.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreater,
                                                          0,
                                                          static_cast<int64_t>(100)
                                                              + numeric_limits<uint32_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(0u, match_sequence->numTuples());

  // Greater-than predicate which matches everything.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreater,
                                                          0,
                                                          numeric_limits<uint32_t>::max() - 1));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(200u, match_sequence->numTuples());

  // Greater-than-or-equal predicates.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreaterOrEqual,
                                                          0,
                                                          static_cast<int64_t>(50) + numeric_limits<uint32_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(101u, match_sequence->numTuples());
  for (tuple_id tuple_num = 0; tuple_num < 200; ++tuple_num) {
    if ((tuple_num >= 50) && (tuple_num <= 150)) {
      EXPECT_TRUE(match_sequence->get(tuple_num));
    } else {
      EXPECT_FALSE(match_sequence->get(tuple_num));
    }
  }

  // Greater-than-or-equal predicate which matches nothing.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreaterOrEqual,
                                                          0,
                                                          static_cast<int64_t>(101)
                                                              + numeric_limits<uint32_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(0u, match_sequence->numTuples());

  // Greater-than-or-equal predicate which matches everything.
  pred.reset(generateNumericComparisonPredicate<LongType>(ComparisonID::kGreaterOrEqual,
                                                          0,
                                                          numeric_limits<uint32_t>::max()));
  match_sequence.reset(index_->getMatchesForPredicate(*pred, nullptr));
  EXPECT_EQ(200u, match_sequence->numTuples());
}

}  // namespace quickstep
