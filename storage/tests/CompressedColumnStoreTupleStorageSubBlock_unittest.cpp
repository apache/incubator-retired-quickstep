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

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/scalar/ScalarLiteral.hpp"
#include "storage/CompressedColumnStoreTupleStorageSubBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageConstants.hpp"
#include "storage/StorageErrors.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/tests/TupleStorePredicateUtil.hpp"
#include "types/CharType.hpp"
#include "types/DoubleType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypedValue.hpp"
#include "types/TypeID.hpp"
#include "types/VarCharType.hpp"
#include "types/containers/Tuple.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrMap.hpp"
#include "utility/PtrVector.hpp"
#include "utility/ScopedBuffer.hpp"

using std::numeric_limits;
using std::pair;
using std::size_t;
using std::snprintf;
using std::string;
using std::vector;

namespace quickstep {

class CompressedColumnStoreTupleStorageSubBlockTest : public ::testing::TestWithParam<bool> {
 protected:
  // 3 MB is just large enough to make sure that all attributes in our schema
  // get the compression that we desire for testing.
  static const size_t kSubBlockSize = 0x300000;
  static const size_t kExpectedTupleSize = 26;

  virtual void SetUp() {
    // Create a sample relation with a variety of attribute types.
    relation_.reset(new CatalogRelation(nullptr, "TestRelation"));

    // An attribute which will be compressed by truncation to 1 byte.
    CatalogAttribute *current_attr = new CatalogAttribute(relation_.get(),
                                                          "int_attr_trunc_byte",
                                                          TypeFactory::GetType(kInt, GetParam()));
    ASSERT_EQ(0, relation_->addAttribute(current_attr));

    // A string attribute which will be dictionary-compressed as 1 byte.
    current_attr = new CatalogAttribute(relation_.get(),
                                        "char_attr_dict_byte",
                                        TypeFactory::GetType(kChar, 8, GetParam()));
    ASSERT_EQ(1, relation_->addAttribute(current_attr));

    // An attribute which will by compressed by truncation to 4 bytes.
    current_attr = new CatalogAttribute(relation_.get(),
                                        "long_attr_trunc_word",
                                        TypeFactory::GetType(kLong, GetParam()));
    ASSERT_EQ(2, relation_->addAttribute(current_attr));

    // A varchar attribute, which must be dictionary-compressed.
    current_attr = new CatalogAttribute(relation_.get(),
                                        "varchar_attr_dict_short",
                                        TypeFactory::GetType(kVarChar, 16, GetParam()));
    ASSERT_EQ(3, relation_->addAttribute(current_attr));

    // An integer attribute which will be dictionary-compressed to 2 bytes.
    current_attr = new CatalogAttribute(relation_.get(),
                                        "int_attr_dict_short",
                                        TypeFactory::GetType(kInt, GetParam()));
    ASSERT_EQ(4, relation_->addAttribute(current_attr));

    // An attribute which can't be compressed (though we will try).
    current_attr = new CatalogAttribute(relation_.get(),
                                        "int_attr_fail_compression",
                                        TypeFactory::GetType(kInt, GetParam()));
    ASSERT_EQ(5, relation_->addAttribute(current_attr));

    // An attribute which could be compressed, but which we will not attempt to
    // compress.
    current_attr = new CatalogAttribute(relation_.get(),
                                        "long_attr_no_compression",
                                        TypeFactory::GetType(kLong, GetParam()));
    ASSERT_EQ(6, relation_->addAttribute(current_attr));

    // An attribute which records the insertion order of tuples, which is the
    // "seed" for generating the other attribute values.
    current_attr = new CatalogAttribute(relation_.get(),
                                        "insert_order",
                                        TypeFactory::GetType(kInt, false));
    ASSERT_EQ(7, relation_->addAttribute(current_attr));

    // Don't initialize the block yet. Different tests will use different
    // params.
    tuple_store_memory_.reset();
    tuple_store_description_.reset();
    tuple_store_.reset();
  }

  void initializeTupleStoreDescription(const attribute_id sort_attribute_id) {
    tuple_store_description_.reset(new TupleStorageSubBlockDescription());
    tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE);
    tuple_store_description_->SetExtension(
        CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id,
        sort_attribute_id);
    for (attribute_id attr_id = 0; attr_id < 6; ++attr_id) {
      tuple_store_description_->AddExtension(
          CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id,
          attr_id);
    }
  }

  void initializeNewBlock(const size_t block_size) {
    tuple_store_memory_.reset(block_size);
    tuple_store_.reset(new CompressedColumnStoreTupleStorageSubBlock(*relation_,
                                                                     *tuple_store_description_,
                                                                     true,
                                                                     tuple_store_memory_.get(),
                                                                     block_size));
  }

  // Caller takes ownership of new heap-created Tuple.
  Tuple* createSampleTuple(const tuple_id tid) const {
    std::vector<TypedValue> attrs;

    // int_attr_trunc_byte - never null, since that prevents truncation.
    attrs.emplace_back(static_cast<int>(tid % numeric_limits<int8_t>::max()));

    // char_attr_dict_byte
    int char_attr_dict_byte_seed = tid % numeric_limits<int8_t>::max();
    if (GetParam() && (char_attr_dict_byte_seed % 8 == 2)) {
      attrs.emplace_back(kChar);
    } else {
      char char_buffer[8];
      const int written
          = snprintf(char_buffer, sizeof(char_buffer), "%d", char_attr_dict_byte_seed);
      attrs.emplace_back(CharType::InstanceNonNullable(8).makeValue(
          char_buffer,
          written + 1).ensureNotReference());
    }

    // long_attr_trunc_word - never null, since that prevents truncation.
    attrs.emplace_back(static_cast<std::int64_t>(numeric_limits<int32_t>::max() - tid));

    // varchar_attr_dict_short
    int varchar_attr_dict_short_seed = tid % numeric_limits<int16_t>::max();
    if (GetParam() && (varchar_attr_dict_short_seed % 8 == 0)) {
      attrs.emplace_back(kVarChar);
    } else {
      char varchar_buffer[8];
      const int written
          = snprintf(varchar_buffer, sizeof(varchar_buffer), "%d", varchar_attr_dict_short_seed);
      attrs.emplace_back(VarCharType::InstanceNonNullable(16).makeValue(
          varchar_buffer,
          written + 1).ensureNotReference());
    }

    // int_attr_dict_short
    int int_attr_dict_short_seed = tid % numeric_limits<int16_t>::max();
    if (GetParam() && (int_attr_dict_short_seed % 8 == 4)) {
      attrs.emplace_back(kInt);
    } else {
      if (int_attr_dict_short_seed & 0x1) {
        attrs.emplace_back(static_cast<int>(0x1000000 + int_attr_dict_short_seed));
      } else {
        attrs.emplace_back(static_cast<int>(-int_attr_dict_short_seed));
      }
    }

    // int_attr_fail_compression
    if (GetParam() && (tid % 8 == 6)) {
      attrs.emplace_back(kInt);
    } else {
      attrs.emplace_back(static_cast<int>(numeric_limits<int32_t>::max() - tid));
    }

    // long_attr_no_compression (could easily be truncated to one byte if we
    // actually tried).
    attrs.emplace_back(static_cast<std::int64_t>(tid % numeric_limits<int8_t>::max()));

    // insert_order (just the tid)
    attrs.emplace_back(static_cast<int>(tid));

    return new Tuple(std::move(attrs));
  }

  void fillBlockWithSampleData() {
    tuple_id current_tid = 0;
    std::unique_ptr<Tuple> current_tuple(createSampleTuple(current_tid));
    while (tuple_store_->insertTupleInBatch(*current_tuple)) {
      ++current_tid;
      current_tuple.reset(createSampleTuple(current_tid));
    }

    tuple_store_->rebuild();
  }

  int computeCompressedBlockInfoSize() const {
    CompressedBlockInfo info;

    for (attribute_id attr_id = 0;
         attr_id <= relation_->getMaxAttributeId();
         ++attr_id) {
      info.add_attribute_size(0);
      info.add_dictionary_size(0);
      info.add_uncompressed_attribute_has_nulls(false);
    }

    info.set_null_bitmap_bits(0);

    return info.ByteSize();
  }

  size_t computeTotalDictionarySize(const tuple_id num_tuples) const {
    size_t total_size = 0;

    // char_attr_dict_byte
    total_size += 2 * sizeof(std::uint32_t);
    size_t non_null_values = num_tuples < numeric_limits<int8_t>::max() ? num_tuples
                                                                        : numeric_limits<int8_t>::max();
    if (GetParam()) {
      if (non_null_values % 8 > 2) {
        non_null_values -= non_null_values / 8;
        --non_null_values;
      } else {
        non_null_values -= non_null_values / 8;
      }
    }
    total_size += non_null_values * 8;

    // varchar_attr_dict_short
    total_size += 2 * sizeof(std::uint32_t);
    non_null_values = num_tuples < numeric_limits<int16_t>::max() ? num_tuples
                                                                  : numeric_limits<int16_t>::max();
    if (GetParam()) {
      non_null_values -= non_null_values / 8;
      --non_null_values;
    }
    total_size += non_null_values * sizeof(std::uint32_t);

    if (num_tuples < 10) {
      total_size += num_tuples * 2;
    } else {
      total_size += 10 * 2;
      if (num_tuples < 100) {
        total_size += (num_tuples - 10) * 3;
      } else {
        total_size += 90 * 3;
        if (num_tuples < 1000) {
          total_size += (num_tuples - 100) * 4;
        } else {
          total_size += 900 * 4;
          if (num_tuples < 10000) {
            total_size += (num_tuples - 1000) * 5;
          } else {
            total_size += 9000 * 5;
            if (num_tuples < numeric_limits<int16_t>::max()) {
              total_size += (num_tuples - 10000) * 6;
            } else {
              total_size += (numeric_limits<int16_t>::max() - 10000) * 6;
            }
          }
        }
      }
    }

    // Go back through and subtract out nulls.
    if (GetParam()) {
      total_size -= 2;
      if (num_tuples < 10) {
        total_size -= (num_tuples / 8) * 2;
      } else {
        total_size -= 2;
        if (num_tuples < 100) {
          total_size -= ((num_tuples / 8) - 1) * 3;
        } else {
          total_size -= 11 * 3;
          if (num_tuples < 1000) {
            total_size -= ((num_tuples / 8) - 12) * 4;
          } else {
            total_size -= 112 * 4;
            if (num_tuples < 10000) {
              total_size -= ((num_tuples / 8) - 124) * 5;
            } else {
              total_size -= 1125 * 5;
              if (num_tuples < numeric_limits<int16_t>::max()) {
                total_size -= ((num_tuples / 8) - 1249) * 6;
              } else {
                total_size -= ((numeric_limits<int16_t>::max() / 8) - 1249) * 6;
              }
            }
          }
        }
      }
    }

    // int_attr_dict_short
    total_size += 2 * sizeof(std::uint32_t);
    non_null_values = num_tuples < numeric_limits<int16_t>::max() ? num_tuples
                                                                  : numeric_limits<int16_t>::max();
    if (GetParam()) {
      if (non_null_values % 8 > 4) {
        non_null_values -= non_null_values / 8;
        --non_null_values;
      } else {
        non_null_values -= non_null_values / 8;
      }
    }
    total_size += non_null_values * 4;

    return total_size;
  }

  size_t computeRequiredStorage(const tuple_id num_tuples) const {
    return sizeof(tuple_id) + sizeof(int)
           + computeCompressedBlockInfoSize()
           + computeTotalDictionarySize(num_tuples)
           + num_tuples * kExpectedTupleSize
           + GetParam() * BitVector<false>::BytesNeeded(num_tuples);
  }

  // Check that every attribute value of every tuple in tuple_store_ is as
  // expected, and is properly ordered according to the sort column.
  //
  // TODO(chasseur): Check specific deleted tuples.
  void checkBlockValues(const attribute_id sort_attribute_id, const size_t num_deleted = 0) const {
    EXPECT_FALSE(tuple_store_->isEmpty());
    EXPECT_TRUE(tuple_store_->isPacked());

    // Truncated attributes don't support untyped access. Dictionary-compressed
    // and uncompressed attributes do.
    EXPECT_FALSE(tuple_store_->supportsUntypedGetAttributeValue(0));
    EXPECT_TRUE(tuple_store_->supportsUntypedGetAttributeValue(1));
    EXPECT_FALSE(tuple_store_->supportsUntypedGetAttributeValue(2));
    EXPECT_TRUE(tuple_store_->supportsUntypedGetAttributeValue(3));
    EXPECT_TRUE(tuple_store_->supportsUntypedGetAttributeValue(4));
    EXPECT_TRUE(tuple_store_->supportsUntypedGetAttributeValue(5));
    EXPECT_TRUE(tuple_store_->supportsUntypedGetAttributeValue(6));
    EXPECT_TRUE(tuple_store_->supportsUntypedGetAttributeValue(7));

    // Set up an UncheckedComparator to check the ordering of tuples by the
    // sort column.
    const Type &sort_attribute_type = relation_->getAttributeById(sort_attribute_id)->getType();
    std::unique_ptr<UncheckedComparator> sort_attribute_comparator(
        ComparisonFactory::GetComparison(ComparisonID::kLessOrEqual).makeUncheckedComparatorForTypes(
            sort_attribute_type,
            sort_attribute_type));

    // Set up some UncheckedComparators to do the equals comparison for each
    // attribute.
    PtrVector<UncheckedComparator> eq_comparators;
    for (CatalogRelation::const_iterator attr_it = relation_->begin();
         attr_it != relation_->end();
         ++attr_it) {
      eq_comparators.push_back(ComparisonFactory::GetComparison(ComparisonID::kEqual).makeUncheckedComparatorForTypes(
          attr_it->getType(), attr_it->getType()));
    }

    // Go through all of the tuples in the block and check that their values
    // are as expected.
    //
    // Check that no tuples are repeated with this vector:
    vector<bool> existence_check_vector;
    existence_check_vector.resize(tuple_store_->getMaxTupleID() + 1 + num_deleted,
                                  false);
    // Keep track of the previous value in the sort column to check that tuples
    // are in ascending order.
    std::unique_ptr<TypedValue> previous_sort_attribute_value;
    for (tuple_id tid = 0;
         tid <= tuple_store_->getMaxTupleID();
         ++tid) {
      EXPECT_TRUE(tuple_store_->hasTupleWithID(tid));

      TypedValue insert_order_value(tuple_store_->getAttributeValueTyped(tid, 7));
      EXPECT_FALSE(existence_check_vector[insert_order_value.getLiteral<int>()]);
      existence_check_vector[insert_order_value.getLiteral<int>()] = true;

      TypedValue sort_attribute_value(tuple_store_->getAttributeValueTyped(tid, sort_attribute_id));
      if (previous_sort_attribute_value.get() != nullptr) {
        if (previous_sort_attribute_value->isNull()) {
          // Nulls are sorted after all "real" values.
          EXPECT_TRUE(sort_attribute_value.isNull());
        } else if (!sort_attribute_value.isNull()) {
          EXPECT_TRUE(sort_attribute_comparator->compareTypedValues(*previous_sort_attribute_value,
                                                                    sort_attribute_value));
        }
      }
      previous_sort_attribute_value.reset(new TypedValue(sort_attribute_value));

      std::unique_ptr<Tuple> check_tuple(createSampleTuple(insert_order_value.getLiteral<int>()));
      for (attribute_id attr_id = 0;
           attr_id <= relation_->getMaxAttributeId();
           ++attr_id) {
        if (GetParam() && check_tuple->getAttributeValue(attr_id).isNull()) {
          EXPECT_EQ(nullptr, tuple_store_->getAttributeValue(tid, attr_id));
          EXPECT_TRUE(tuple_store_->getAttributeValueTyped(tid, attr_id).isNull());
        } else {
          if ((attr_id != 0) && (attr_id != 2)) {
            EXPECT_TRUE(eq_comparators[attr_id].compareDataPtrs(
                check_tuple->getAttributeValue(attr_id).getDataPtr(),
                tuple_store_->getAttributeValue(tid, attr_id)));
          }

          TypedValue attr_value(tuple_store_->getAttributeValueTyped(tid, attr_id));
          EXPECT_TRUE(eq_comparators[attr_id].compareTypedValues(
              check_tuple->getAttributeValue(attr_id),
              attr_value));
        }
      }
    }
  }

  // Create a ComparisonPredicate of the form "attribute comp literal".
  template <typename AttributeType>
  Predicate* generateNumericComparisonPredicate(const ComparisonID comp,
                                                const attribute_id attribute,
                                                const typename AttributeType::cpptype literal) const {
    ScalarAttribute *scalar_attribute = new ScalarAttribute(*relation_->getAttributeById(attribute));
    ScalarLiteral *scalar_literal
        = new ScalarLiteral(AttributeType::InstanceNonNullable().makeValue(&literal),
                            AttributeType::InstanceNonNullable());
    return new ComparisonPredicate(ComparisonFactory::GetComparison(comp), scalar_attribute, scalar_literal);
  }

  Predicate* generateStringComparisonPredicate(const ComparisonID comp,
                                               const attribute_id attribute,
                                               const string &literal) const {
    ScalarAttribute *scalar_attribute = new ScalarAttribute(*relation_->getAttributeById(attribute));
    ScalarLiteral *scalar_literal = new ScalarLiteral(
        VarCharType::InstanceNonNullable(literal.size()).makeValue(
            literal.c_str(),
            literal.size() + 1).ensureNotReference(),
        VarCharType::InstanceNonNullable(literal.size()));
    return new ComparisonPredicate(ComparisonFactory::GetComparison(comp),
                                   scalar_attribute,
                                   scalar_literal);
  }

  template <typename ValueType>
  inline static bool matchExpected(const ComparisonID comp,
                                   const ValueType &left,
                                   const ValueType &right) {
    switch (comp) {
      case ComparisonID::kEqual:
        return (left == right);
      case ComparisonID::kNotEqual:
        return (left != right);
      case ComparisonID::kLess:
        return (left < right);
      case ComparisonID::kLessOrEqual:
        return (left <= right);
      case ComparisonID::kGreater:
        return (left > right);
      case ComparisonID::kGreaterOrEqual:
        return (left >= right);
      default:
        FATAL_ERROR("Unrecognized ComparisonID");
    }
  }

  void checkPredicateAttr0(const ComparisonID comp) const {
    std::unique_ptr<Predicate> predicate(
        generateNumericComparisonPredicate<IntType>(comp,
                                                    0,
                                                    numeric_limits<int8_t>::max() >> 1));
    std::unique_ptr<TupleIdSequence> matches(
        TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));

    for (tuple_id tid = 0;
         tid < static_cast<tuple_id>(matches->length());
         ++tid) {
      TypedValue insert_order_value(tuple_store_->getAttributeValueTyped(tid, 7));
      if (matchExpected(comp,
                        insert_order_value.getLiteral<int>() % numeric_limits<int8_t>::max(),
                        numeric_limits<int8_t>::max() >> 1)) {
        EXPECT_TRUE(matches->get(tid));
      } else {
        EXPECT_FALSE(matches->get(tid));
      }
    }
  }

  void checkPredicateAttr1(const ComparisonID comp) const {
    string string_literal("50");
    std::unique_ptr<Predicate> predicate(generateStringComparisonPredicate(comp,
                                                                           1,
                                                                           string_literal));
    std::unique_ptr<TupleIdSequence> matches(
        TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));

    for (tuple_id tid = 0;
         tid < static_cast<tuple_id>(matches->length());
         ++tid) {
      TypedValue insert_order_value(tuple_store_->getAttributeValueTyped(tid, 7));
      if (GetParam()
          && (insert_order_value.getLiteral<int>() % numeric_limits<int8_t>::max() % 8 == 2)) {
        EXPECT_FALSE(matches->get(tid));
        continue;
      }

      char str_buffer[8];
      snprintf(str_buffer,
               sizeof(str_buffer),
               "%d",
               (insert_order_value.getLiteral<int>() % numeric_limits<int8_t>::max()));
      if (matchExpected(comp, string(str_buffer), string_literal)) {
        EXPECT_TRUE(matches->get(tid));
      } else {
        EXPECT_FALSE(matches->get(tid));
      }
    }
  }

  void checkPredicateAttr2(const ComparisonID comp) const {
    LongType::cpptype literal_value = numeric_limits<int32_t>::max() - (tuple_store_->getMaxTupleID() >> 1);
    std::unique_ptr<Predicate> predicate(generateNumericComparisonPredicate<LongType>(comp,
                                                                                      2,
                                                                                      literal_value));
    std::unique_ptr<TupleIdSequence> matches(
        TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));

    for (tuple_id tid = 0;
         tid < static_cast<tuple_id>(matches->length());
         ++tid) {
      TypedValue insert_order_value(tuple_store_->getAttributeValueTyped(tid, 7));
      if (matchExpected(comp,
                        static_cast<LongType::cpptype>(numeric_limits<int32_t>::max()
                            - insert_order_value.getLiteral<int>()),
                        literal_value)) {
        EXPECT_TRUE(matches->get(tid));
      } else {
        EXPECT_FALSE(matches->get(tid));
      }
    }
  }

  void checkPredicateAttr3(const ComparisonID comp) const {
    string string_literal("5000");
    std::unique_ptr<Predicate> predicate(generateStringComparisonPredicate(comp,
                                                                     3,
                                                                     string_literal));
    std::unique_ptr<TupleIdSequence> matches(
        TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));

    for (tuple_id tid = 0;
         tid < static_cast<tuple_id>(matches->length());
         ++tid) {
      TypedValue insert_order_value(tuple_store_->getAttributeValueTyped(tid, 7));
      if (GetParam()
          && (insert_order_value.getLiteral<int>() % numeric_limits<int16_t>::max() % 8 == 0)) {
        EXPECT_FALSE(matches->get(tid));
        continue;
      }

      char str_buffer[8];
      snprintf(str_buffer,
               sizeof(str_buffer),
               "%d",
               (insert_order_value.getLiteral<int>() % numeric_limits<int16_t>::max()));
      if (matchExpected(comp, string(str_buffer), string_literal)) {
        EXPECT_TRUE(matches->get(tid));
      } else {
        EXPECT_FALSE(matches->get(tid));
      }
    }
  }

  void checkPredicateAttr4(const ComparisonID comp) const {
    std::unique_ptr<Predicate> predicate(generateNumericComparisonPredicate<IntType>(comp,
                                                                                     4,
                                                                                     0));
    std::unique_ptr<TupleIdSequence> matches(
        TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));

    for (tuple_id tid = 0;
         tid < static_cast<tuple_id>(matches->length());
         ++tid) {
      TypedValue insert_order_value(tuple_store_->getAttributeValueTyped(tid, 7));
      if (GetParam()
          && (insert_order_value.getLiteral<int>() % numeric_limits<int16_t>::max() % 8 == 4)) {
        EXPECT_FALSE(matches->get(tid));
        continue;
      }

      int expected_attr_value = (insert_order_value.getLiteral<int>() % numeric_limits<int16_t>::max()) & 0x1 ?
                                0x1000000 + insert_order_value.getLiteral<int>() % numeric_limits<int16_t>::max()
                                : -(insert_order_value.getLiteral<int>() % numeric_limits<int16_t>::max());
      if (matchExpected(comp, expected_attr_value, 0)) {
        EXPECT_TRUE(matches->get(tid));
      } else {
        EXPECT_FALSE(matches->get(tid));
      }
    }
  }

  void checkPredicateAttr5(const ComparisonID comp) const {
    int literal_value = numeric_limits<int32_t>::max() - (tuple_store_->getMaxTupleID() >> 1);
    std::unique_ptr<Predicate> predicate(generateNumericComparisonPredicate<IntType>(comp,
                                                                                     5,
                                                                                     literal_value));
    std::unique_ptr<TupleIdSequence> matches(
        TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));

    for (tuple_id tid = 0;
         tid < static_cast<tuple_id>(matches->length());
         ++tid) {
      TypedValue insert_order_value(tuple_store_->getAttributeValueTyped(tid, 7));
      if (GetParam() && (insert_order_value.getLiteral<int>() % 8 == 6)) {
        EXPECT_FALSE(matches->get(tid));
        continue;
      }

      if (matchExpected(comp,
                        numeric_limits<int32_t>::max() - insert_order_value.getLiteral<int>(),
                        literal_value)) {
        EXPECT_TRUE(matches->get(tid));
      } else {
        EXPECT_FALSE(matches->get(tid));
      }
    }
  }

  void checkPredicateAttr6(const ComparisonID comp) const {
    std::unique_ptr<Predicate> predicate(
        generateNumericComparisonPredicate<LongType>(comp,
                                                     6,
                                                     numeric_limits<int8_t>::max() >> 1));
    std::unique_ptr<TupleIdSequence> matches(
        TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));

    for (tuple_id tid = 0;
         tid < static_cast<tuple_id>(matches->length());
         ++tid) {
      TypedValue insert_order_value(tuple_store_->getAttributeValueTyped(tid, 7));
      if (matchExpected(comp,
                        insert_order_value.getLiteral<int>() % numeric_limits<int8_t>::max(),
                        numeric_limits<int8_t>::max() >> 1)) {
        EXPECT_TRUE(matches->get(tid));
      } else {
        EXPECT_FALSE(matches->get(tid));
      }
    }
  }

  void runCheckPredicateTest(const attribute_id sort_attribute_id) {
    initializeTupleStoreDescription(sort_attribute_id);
    initializeNewBlock(kSubBlockSize);
    fillBlockWithSampleData();

    checkPredicateAttr0(ComparisonID::kEqual);
    checkPredicateAttr0(ComparisonID::kNotEqual);
    checkPredicateAttr0(ComparisonID::kLess);
    checkPredicateAttr0(ComparisonID::kLessOrEqual);
    checkPredicateAttr0(ComparisonID::kGreater);
    checkPredicateAttr0(ComparisonID::kGreaterOrEqual);

    checkPredicateAttr1(ComparisonID::kEqual);
    checkPredicateAttr1(ComparisonID::kNotEqual);
    checkPredicateAttr1(ComparisonID::kLess);
    checkPredicateAttr1(ComparisonID::kLessOrEqual);
    checkPredicateAttr1(ComparisonID::kGreater);
    checkPredicateAttr1(ComparisonID::kGreaterOrEqual);

    checkPredicateAttr2(ComparisonID::kEqual);
    checkPredicateAttr2(ComparisonID::kNotEqual);
    checkPredicateAttr2(ComparisonID::kLess);
    checkPredicateAttr2(ComparisonID::kLessOrEqual);
    checkPredicateAttr2(ComparisonID::kGreater);
    checkPredicateAttr2(ComparisonID::kGreaterOrEqual);

    checkPredicateAttr3(ComparisonID::kEqual);
    checkPredicateAttr3(ComparisonID::kNotEqual);
    checkPredicateAttr3(ComparisonID::kLess);
    checkPredicateAttr3(ComparisonID::kLessOrEqual);
    checkPredicateAttr3(ComparisonID::kGreater);
    checkPredicateAttr3(ComparisonID::kGreaterOrEqual);

    checkPredicateAttr4(ComparisonID::kEqual);
    checkPredicateAttr4(ComparisonID::kNotEqual);
    checkPredicateAttr4(ComparisonID::kLess);
    checkPredicateAttr4(ComparisonID::kLessOrEqual);
    checkPredicateAttr4(ComparisonID::kGreater);
    checkPredicateAttr4(ComparisonID::kGreaterOrEqual);

    checkPredicateAttr5(ComparisonID::kEqual);
    checkPredicateAttr5(ComparisonID::kNotEqual);
    checkPredicateAttr5(ComparisonID::kLess);
    checkPredicateAttr5(ComparisonID::kLessOrEqual);
    checkPredicateAttr5(ComparisonID::kGreater);
    checkPredicateAttr5(ComparisonID::kGreaterOrEqual);

    checkPredicateAttr6(ComparisonID::kEqual);
    checkPredicateAttr6(ComparisonID::kNotEqual);
    checkPredicateAttr6(ComparisonID::kLess);
    checkPredicateAttr6(ComparisonID::kLessOrEqual);
    checkPredicateAttr6(ComparisonID::kGreater);
    checkPredicateAttr6(ComparisonID::kGreaterOrEqual);

    // These are used for checking some special predicates below.
    std::unique_ptr<Predicate> predicate;
    std::unique_ptr<TupleIdSequence> matches;

    // Check some predicates which compare an attribute with a literal of a
    // different type.
    // A truncation-compressed attribute:
    predicate.reset(generateNumericComparisonPredicate<DoubleType>(ComparisonID::kEqual, 0, 55.5));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_TRUE(matches->empty());

    predicate.reset(generateNumericComparisonPredicate<DoubleType>(ComparisonID::kNotEqual, 0, 55.5));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_EQ(static_cast<std::size_t>(tuple_store_->getMaxTupleID() + 1),
              matches->numTuples());

    predicate.reset(generateNumericComparisonPredicate<DoubleType>(ComparisonID::kGreater, 0, 55.5));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    for (tuple_id tid = 0;
         tid < static_cast<tuple_id>(matches->length());
         ++tid) {
      TypedValue insert_order_value(tuple_store_->getAttributeValueTyped(tid, 7));
      if (matchExpected(ComparisonID::kGreater,
                        insert_order_value.getLiteral<int>() % numeric_limits<int8_t>::max(),
                        55)) {
        EXPECT_TRUE(matches->get(tid));
      } else {
        EXPECT_FALSE(matches->get(tid));
      }
    }

    // A dictionary-compressed attribute:
    predicate.reset(generateNumericComparisonPredicate<DoubleType>(ComparisonID::kEqual, 4, -55.5));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_TRUE(matches->empty());

    predicate.reset(generateNumericComparisonPredicate<DoubleType>(ComparisonID::kNotEqual, 4, -55.5));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    if (GetParam()) {
      // nulls don't match.
      TupleIdSequence::size_type expected_matches = tuple_store_->getMaxTupleID() + 1;
      TupleIdSequence::size_type num_nulls
          = (expected_matches / numeric_limits<int16_t>::max())
                * ((numeric_limits<int16_t>::max() / 8) + (numeric_limits<int16_t>::max() % 8 > 4))
            + ((expected_matches % numeric_limits<int16_t>::max()) / 8)
            + (expected_matches % numeric_limits<int16_t>::max() % 8 > 4);
      expected_matches -= num_nulls;
      EXPECT_EQ(expected_matches, matches->numTuples());
    } else {
      EXPECT_EQ(static_cast<TupleIdSequence::size_type>(tuple_store_->getMaxTupleID() + 1),
                matches->numTuples());
    }

    predicate.reset(generateNumericComparisonPredicate<DoubleType>(ComparisonID::kGreater, 4, -55.5));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    for (tuple_id tid = 0;
         tid < static_cast<tuple_id>(matches->length());
         ++tid) {
      TypedValue insert_order_value(tuple_store_->getAttributeValueTyped(tid, 7));
      if (GetParam()
          && (insert_order_value.getLiteral<int>() % numeric_limits<int16_t>::max() % 8 == 4)) {
        EXPECT_FALSE(matches->get(tid));
        continue;
      }

      int expected_attr4_value =
          (insert_order_value.getLiteral<int>() % numeric_limits<int16_t>::max()) & 0x1 ?
          0x1000000 + insert_order_value.getLiteral<int>() % numeric_limits<int16_t>::max()
          : -(insert_order_value.getLiteral<int>() % numeric_limits<int16_t>::max());
      if (matchExpected(ComparisonID::kGreater,
                        expected_attr4_value,
                        -56)) {
        EXPECT_TRUE(matches->get(tid));
      } else {
        EXPECT_FALSE(matches->get(tid));
      }
    }

    // An uncompressed attribute:
    predicate.reset(generateNumericComparisonPredicate<DoubleType>(
        ComparisonID::kEqual,
        5,
        0.5 + numeric_limits<int32_t>::max() - (tuple_store_->getMaxTupleID() >> 1)));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_TRUE(matches->empty());

    predicate.reset(generateNumericComparisonPredicate<DoubleType>(
        ComparisonID::kNotEqual,
        5,
        0.5 + numeric_limits<int32_t>::max() - (tuple_store_->getMaxTupleID() >> 1)));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    if (GetParam()) {
      // nulls don't match.
      TupleIdSequence::size_type expected_matches = tuple_store_->getMaxTupleID() + 1;
      if (expected_matches % 8 > 6) {
        expected_matches -= expected_matches / 8;
        --expected_matches;
      } else {
        expected_matches -= expected_matches / 8;
      }
      EXPECT_EQ(expected_matches, matches->numTuples());
    } else {
      EXPECT_EQ(static_cast<TupleIdSequence::size_type>(tuple_store_->getMaxTupleID() + 1),
                matches->numTuples());
    }

    predicate.reset(generateNumericComparisonPredicate<DoubleType>(
        ComparisonID::kGreater,
        5,
        0.5 + numeric_limits<int32_t>::max() - (tuple_store_->getMaxTupleID() >> 1)));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    for (tuple_id tid = 0;
         tid < static_cast<tuple_id>(matches->length());
         ++tid) {
      TypedValue insert_order_value(tuple_store_->getAttributeValueTyped(tid, 7));
      if (GetParam() && (insert_order_value.getLiteral<int>() % 8 == 6)) {
        EXPECT_FALSE(matches->get(tid));
        continue;
      }

      if (matchExpected(ComparisonID::kGreater,
                        numeric_limits<int32_t>::max() - insert_order_value.getLiteral<int>(),
                        numeric_limits<int32_t>::max() - (tuple_store_->getMaxTupleID() >> 1))) {
        EXPECT_TRUE(matches->get(tid));
      } else {
        EXPECT_FALSE(matches->get(tid));
      }
    }

    // Check special fast paths for out-of-range or nonexistent values.
    // A truncated attribute:
    predicate.reset(generateNumericComparisonPredicate<IntType>(ComparisonID::kEqual, 0, -1));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_TRUE(matches->empty());

    predicate.reset(generateNumericComparisonPredicate<IntType>(ComparisonID::kEqual,
                                                                0,
                                                                numeric_limits<uint8_t>::max() + 1));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_TRUE(matches->empty());

    predicate.reset(generateNumericComparisonPredicate<IntType>(ComparisonID::kNotEqual, 0, -1));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_EQ(static_cast<TupleIdSequence::size_type>(tuple_store_->getMaxTupleID() + 1),
              matches->numTuples());

    predicate.reset(generateNumericComparisonPredicate<IntType>(ComparisonID::kNotEqual,
                                                                0,
                                                                numeric_limits<uint8_t>::max() + 1));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_EQ(static_cast<TupleIdSequence::size_type>(tuple_store_->getMaxTupleID() + 1),
              matches->numTuples());

    predicate.reset(generateNumericComparisonPredicate<IntType>(ComparisonID::kLess, 0, 0));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_TRUE(matches->empty());

    predicate.reset(generateNumericComparisonPredicate<IntType>(ComparisonID::kLess,
                                                                0,
                                                                numeric_limits<uint8_t>::max() + 1));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_EQ(static_cast<TupleIdSequence::size_type>(tuple_store_->getMaxTupleID() + 1),
              matches->numTuples());

    predicate.reset(generateNumericComparisonPredicate<IntType>(ComparisonID::kLessOrEqual, 0, -1));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_TRUE(matches->empty());

    predicate.reset(generateNumericComparisonPredicate<IntType>(ComparisonID::kLessOrEqual,
                                                                0,
                                                                numeric_limits<uint8_t>::max()));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_EQ(static_cast<TupleIdSequence::size_type>(tuple_store_->getMaxTupleID() + 1),
              matches->numTuples());

    predicate.reset(generateNumericComparisonPredicate<IntType>(ComparisonID::kGreater, 0, -1));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_EQ(static_cast<TupleIdSequence::size_type>(tuple_store_->getMaxTupleID() + 1),
              matches->numTuples());

    predicate.reset(generateNumericComparisonPredicate<IntType>(ComparisonID::kGreater,
                                                                0,
                                                                numeric_limits<uint8_t>::max()));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_TRUE(matches->empty());

    predicate.reset(generateNumericComparisonPredicate<IntType>(ComparisonID::kGreaterOrEqual, 0, 0));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_EQ(static_cast<TupleIdSequence::size_type>(tuple_store_->getMaxTupleID() + 1),
              matches->numTuples());

    predicate.reset(generateNumericComparisonPredicate<IntType>(ComparisonID::kGreaterOrEqual,
                                                                0,
                                                                numeric_limits<uint8_t>::max() + 1));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_TRUE(matches->empty());

    // A dictionary-coded attribute:
    TupleIdSequence::size_type non_null_values = tuple_store_->getMaxTupleID() + 1;
    if (GetParam()) {
      TupleIdSequence::size_type num_nulls
          = (non_null_values / numeric_limits<int8_t>::max())
                * ((numeric_limits<int8_t>::max() / 8) + (numeric_limits<int8_t>::max() % 8 > 2))
            + ((non_null_values % numeric_limits<int8_t>::max()) / 8)
            + (non_null_values % numeric_limits<int8_t>::max() % 8 > 2);
      non_null_values -= num_nulls;
    }

    predicate.reset(generateStringComparisonPredicate(ComparisonID::kEqual, 1, "5000"));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_TRUE(matches->empty());

    predicate.reset(generateStringComparisonPredicate(ComparisonID::kNotEqual, 1, "5000"));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_EQ(non_null_values, matches->numTuples());

    predicate.reset(generateStringComparisonPredicate(ComparisonID::kLess, 1, "0"));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_TRUE(matches->empty());

    predicate.reset(generateStringComparisonPredicate(ComparisonID::kLess, 1, "a"));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_EQ(non_null_values, matches->numTuples());

    predicate.reset(generateStringComparisonPredicate(ComparisonID::kLessOrEqual, 1, " "));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_TRUE(matches->empty());

    predicate.reset(generateStringComparisonPredicate(ComparisonID::kLessOrEqual, 1, "99"));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_EQ(non_null_values, matches->numTuples());

    predicate.reset(generateStringComparisonPredicate(ComparisonID::kGreater, 1, " "));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_EQ(non_null_values, matches->numTuples());

    predicate.reset(generateStringComparisonPredicate(ComparisonID::kGreater, 1, "99"));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_TRUE(matches->empty());

    predicate.reset(generateStringComparisonPredicate(ComparisonID::kGreaterOrEqual, 1, "0"));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_EQ(non_null_values, matches->numTuples());

    predicate.reset(generateStringComparisonPredicate(ComparisonID::kGreaterOrEqual, 1, "a"));
    matches.reset(TupleStorePredicateUtil::GetMatchesForPredicateOnTupleStore(*predicate, *tuple_store_));
    EXPECT_TRUE(matches->empty());
  }

  std::unique_ptr<CatalogRelation> relation_;
  ScopedBuffer tuple_store_memory_;
  std::unique_ptr<TupleStorageSubBlockDescription> tuple_store_description_;
  std::unique_ptr<CompressedColumnStoreTupleStorageSubBlock> tuple_store_;
};

typedef CompressedColumnStoreTupleStorageSubBlockTest
        CompressedColumnStoreTupleStorageSubBlockDeathTest;

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, DescriptionIsValidTest) {
  tuple_store_description_.reset(new TupleStorageSubBlockDescription());

  // An uninitialized description is not valid.
  EXPECT_FALSE(CompressedColumnStoreTupleStorageSubBlock::DescriptionIsValid(*relation_,
                                                                             *tuple_store_description_));

  // A description that specifies the wrong sub_block_type is not valid.
  tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::BASIC_COLUMN_STORE);
  EXPECT_FALSE(CompressedColumnStoreTupleStorageSubBlock::DescriptionIsValid(*relation_,
                                                                             *tuple_store_description_));

  // A description with no sort column specified is not valid.
  tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE);
  EXPECT_FALSE(CompressedColumnStoreTupleStorageSubBlock::DescriptionIsValid(*relation_,
                                                                             *tuple_store_description_));

  // A description which has an uncompressed variable-length attribute is not
  // valid.
  tuple_store_description_->SetExtension(
      CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id,
      0);
  EXPECT_FALSE(CompressedColumnStoreTupleStorageSubBlock::DescriptionIsValid(*relation_,
                                                                             *tuple_store_description_));

  // Specifying that the variable-length attribute is compressed should make
  // the description valid.
  tuple_store_description_->AddExtension(
      CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id,
      3);
  EXPECT_TRUE(CompressedColumnStoreTupleStorageSubBlock::DescriptionIsValid(*relation_,
                                                                            *tuple_store_description_));

  // Specifying a non-existent attribute for compression causes the description
  // to be invalid.
  tuple_store_description_->AddExtension(
      CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id,
      64);
  EXPECT_FALSE(CompressedColumnStoreTupleStorageSubBlock::DescriptionIsValid(*relation_,
                                                                             *tuple_store_description_));

  // A relation with a nullable attribute is OK.
  std::unique_ptr<CatalogRelation> nullable_relation(new CatalogRelation(NULL, "nullable_relation"));
  CatalogAttribute *nullable_attribute = new CatalogAttribute(nullable_relation.get(),
                                                              "nullable_attr",
                                                              TypeFactory::GetType(kInt, true));
  ASSERT_EQ(0, nullable_relation->addAttribute(nullable_attribute));
  tuple_store_description_.reset(new TupleStorageSubBlockDescription());
  tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE);
  tuple_store_description_->SetExtension(
      CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id,
      0);
  EXPECT_TRUE(CompressedColumnStoreTupleStorageSubBlock::DescriptionIsValid(*nullable_relation,
                                                                            *tuple_store_description_));

  // It's also OK if the nullable attribute is compressed.
  tuple_store_description_->AddExtension(
      CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id,
      0);
  EXPECT_TRUE(CompressedColumnStoreTupleStorageSubBlock::DescriptionIsValid(*nullable_relation,
                                                                            *tuple_store_description_));

  // The description we use for the other tests should be valid.
  initializeTupleStoreDescription(0);
  EXPECT_TRUE(CompressedColumnStoreTupleStorageSubBlock::DescriptionIsValid(*relation_,
                                                                            *tuple_store_description_));
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockDeathTest, ConstructWithInvalidDescriptionTest) {
  tuple_store_description_.reset(new TupleStorageSubBlockDescription());
  tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE);
  EXPECT_DEATH(initializeNewBlock(kSubBlockSize), "");
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, MemoryTooSmallTest) {
  initializeTupleStoreDescription(0);
  // 1 byte short.
  EXPECT_THROW(initializeNewBlock(computeCompressedBlockInfoSize() + sizeof(tuple_id) + sizeof(int) - 1),
               BlockMemoryTooSmall);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, InsertTest) {
  initializeTupleStoreDescription(0);
  initializeNewBlock(kSubBlockSize);

  EXPECT_FALSE(tuple_store_->supportsAdHocInsert());
  EXPECT_FALSE(tuple_store_->adHocInsertIsEfficient());

  EXPECT_TRUE(tuple_store_->isCompressed());
  EXPECT_FALSE(tuple_store_->compressedBlockIsBuilt());
  EXPECT_TRUE(tuple_store_->compressedUnbuiltBlockAttributeMayBeCompressed(0));
  EXPECT_TRUE(tuple_store_->compressedUnbuiltBlockAttributeMayBeCompressed(1));
  EXPECT_TRUE(tuple_store_->compressedUnbuiltBlockAttributeMayBeCompressed(2));
  EXPECT_TRUE(tuple_store_->compressedUnbuiltBlockAttributeMayBeCompressed(3));
  EXPECT_TRUE(tuple_store_->compressedUnbuiltBlockAttributeMayBeCompressed(4));
  EXPECT_TRUE(tuple_store_->compressedUnbuiltBlockAttributeMayBeCompressed(5));
  EXPECT_FALSE(tuple_store_->compressedUnbuiltBlockAttributeMayBeCompressed(6));

  // The ad-hoc path should fail to insert.
  std::unique_ptr<Tuple> current_tuple(createSampleTuple(0));
  TupleStorageSubBlock::InsertResult ad_hoc_insert_result = tuple_store_->insertTuple(*current_tuple);
  EXPECT_EQ(-1, ad_hoc_insert_result.inserted_id);
  EXPECT_FALSE(ad_hoc_insert_result.ids_mutated);

  // The block should report itself as empty.
  EXPECT_TRUE(tuple_store_->isEmpty());

  tuple_id current_tid = 0;
  while (computeRequiredStorage(current_tid + 1) < kSubBlockSize) {
    EXPECT_TRUE(tuple_store_->insertTupleInBatch(*current_tuple));
    ++current_tid;
    current_tuple.reset(createSampleTuple(current_tid));
  }

  EXPECT_FALSE(tuple_store_->insertTupleInBatch(*current_tuple));
  EXPECT_FALSE(tuple_store_->isEmpty());

  // Actually build the compressed store and check its metadata afterwards.
  tuple_store_->rebuild();
  EXPECT_FALSE(tuple_store_->isEmpty());
  EXPECT_TRUE(tuple_store_->isPacked());
  EXPECT_EQ(current_tid - 1, tuple_store_->getMaxTupleID());

  EXPECT_TRUE(tuple_store_->isCompressed());
  EXPECT_TRUE(tuple_store_->compressedBlockIsBuilt());

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(0));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(0));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(0));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(1));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(1));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(1));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(2));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(2));
  EXPECT_EQ(4u, tuple_store_->compressedGetCompressedAttributeSize(2));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(3));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(3));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(3));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(4));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(4));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(4));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(5));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(5));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(6));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(6));
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, InsertAfterBuildTest) {
  initializeTupleStoreDescription(0);
  initializeNewBlock(kSubBlockSize);

  // Insert just a couple of tuples.
  std::unique_ptr<Tuple> current_tuple(createSampleTuple(0));
  EXPECT_TRUE(tuple_store_->insertTupleInBatch(*current_tuple));
  current_tuple.reset(createSampleTuple(1));
  EXPECT_TRUE(tuple_store_->insertTupleInBatch(*current_tuple));

  tuple_store_->rebuild();
  EXPECT_FALSE(tuple_store_->isEmpty());
  EXPECT_TRUE(tuple_store_->isPacked());
  EXPECT_EQ(1, tuple_store_->getMaxTupleID());

  // Even though there is plenty of space, we still expect failure when
  // we attempt to insert in a batch after the block is built.
  current_tuple.reset(createSampleTuple(2));
  EXPECT_FALSE(tuple_store_->insertTupleInBatch(*current_tuple));
  EXPECT_FALSE(tuple_store_->isEmpty());
  EXPECT_TRUE(tuple_store_->isPacked());
  EXPECT_EQ(1, tuple_store_->getMaxTupleID());
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, GetAttributeValueSortAttr0Test) {
  initializeTupleStoreDescription(0);
  initializeNewBlock(kSubBlockSize);
  fillBlockWithSampleData();

  // Check compression metadata:
  EXPECT_TRUE(tuple_store_->isCompressed());
  EXPECT_TRUE(tuple_store_->compressedBlockIsBuilt());

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(0));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(0));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(0));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(1));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(1));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(1));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(2));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(2));
  EXPECT_EQ(4u, tuple_store_->compressedGetCompressedAttributeSize(2));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(3));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(3));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(3));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(4));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(4));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(4));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(5));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(5));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(6));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(6));

  checkBlockValues(0);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, GetAttributeValueSortAttr1Test) {
  initializeTupleStoreDescription(1);
  initializeNewBlock(kSubBlockSize);
  fillBlockWithSampleData();

  // Check compression metadata:
  EXPECT_TRUE(tuple_store_->isCompressed());
  EXPECT_TRUE(tuple_store_->compressedBlockIsBuilt());

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(0));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(0));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(0));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(1));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(1));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(1));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(2));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(2));
  EXPECT_EQ(4u, tuple_store_->compressedGetCompressedAttributeSize(2));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(3));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(3));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(3));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(4));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(4));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(4));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(5));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(5));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(6));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(6));

  checkBlockValues(1);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, GetAttributeValueSortAttr2Test) {
  initializeTupleStoreDescription(2);
  initializeNewBlock(kSubBlockSize);
  fillBlockWithSampleData();

  // Check compression metadata:
  EXPECT_TRUE(tuple_store_->isCompressed());
  EXPECT_TRUE(tuple_store_->compressedBlockIsBuilt());

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(0));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(0));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(0));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(1));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(1));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(1));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(2));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(2));
  EXPECT_EQ(4u, tuple_store_->compressedGetCompressedAttributeSize(2));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(3));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(3));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(3));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(4));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(4));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(4));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(5));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(5));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(6));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(6));

  checkBlockValues(2);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, GetAttributeValueSortAttr3Test) {
  initializeTupleStoreDescription(3);
  initializeNewBlock(kSubBlockSize);
  fillBlockWithSampleData();

  // Check compression metadata:
  EXPECT_TRUE(tuple_store_->isCompressed());
  EXPECT_TRUE(tuple_store_->compressedBlockIsBuilt());

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(0));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(0));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(0));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(1));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(1));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(1));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(2));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(2));
  EXPECT_EQ(4u, tuple_store_->compressedGetCompressedAttributeSize(2));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(3));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(3));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(3));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(4));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(4));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(4));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(5));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(5));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(6));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(6));

  checkBlockValues(3);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, GetAttributeValueSortAttr4Test) {
  initializeTupleStoreDescription(4);
  initializeNewBlock(kSubBlockSize);
  fillBlockWithSampleData();

  // Check compression metadata:
  EXPECT_TRUE(tuple_store_->isCompressed());
  EXPECT_TRUE(tuple_store_->compressedBlockIsBuilt());

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(0));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(0));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(0));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(1));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(1));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(1));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(2));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(2));
  EXPECT_EQ(4u, tuple_store_->compressedGetCompressedAttributeSize(2));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(3));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(3));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(3));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(4));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(4));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(4));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(5));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(5));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(6));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(6));

  checkBlockValues(4);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, GetAttributeValueSortAttr5Test) {
  initializeTupleStoreDescription(5);
  initializeNewBlock(kSubBlockSize);
  fillBlockWithSampleData();

  // Check compression metadata:
  EXPECT_TRUE(tuple_store_->isCompressed());
  EXPECT_TRUE(tuple_store_->compressedBlockIsBuilt());

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(0));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(0));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(0));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(1));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(1));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(1));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(2));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(2));
  EXPECT_EQ(4u, tuple_store_->compressedGetCompressedAttributeSize(2));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(3));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(3));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(3));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(4));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(4));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(4));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(5));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(5));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(6));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(6));

  checkBlockValues(5);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, GetAttributeValueSortAttr6Test) {
  initializeTupleStoreDescription(6);
  initializeNewBlock(kSubBlockSize);
  fillBlockWithSampleData();

  // Check compression metadata:
  EXPECT_TRUE(tuple_store_->isCompressed());
  EXPECT_TRUE(tuple_store_->compressedBlockIsBuilt());

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(0));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(0));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(0));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(1));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(1));
  EXPECT_EQ(1u, tuple_store_->compressedGetCompressedAttributeSize(1));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(2));
  EXPECT_TRUE(tuple_store_->compressedAttributeIsTruncationCompressed(2));
  EXPECT_EQ(4u, tuple_store_->compressedGetCompressedAttributeSize(2));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(3));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(3));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(3));

  EXPECT_TRUE(tuple_store_->compressedAttributeIsDictionaryCompressed(4));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(4));
  EXPECT_EQ(2u, tuple_store_->compressedGetCompressedAttributeSize(4));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(5));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(5));

  EXPECT_FALSE(tuple_store_->compressedAttributeIsDictionaryCompressed(6));
  EXPECT_FALSE(tuple_store_->compressedAttributeIsTruncationCompressed(6));

  checkBlockValues(6);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, ReloadAndGetAttributeValueTest) {
  initializeTupleStoreDescription(0);
  initializeNewBlock(kSubBlockSize);
  fillBlockWithSampleData();
  tuple_store_.reset(new CompressedColumnStoreTupleStorageSubBlock(*relation_,
                                                                   *tuple_store_description_,
                                                                   false,
                                                                   tuple_store_memory_.get(),
                                                                   kSubBlockSize));

  checkBlockValues(0);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockDeathTest, SetAttributeValueTypedTest) {
  initializeTupleStoreDescription(0);
  initializeNewBlock(kSubBlockSize);

  // Insert just a couple of tuples.
  std::unique_ptr<Tuple> current_tuple(createSampleTuple(0));
  EXPECT_TRUE(tuple_store_->insertTupleInBatch(*current_tuple));
  current_tuple.reset(createSampleTuple(1));
  EXPECT_TRUE(tuple_store_->insertTupleInBatch(*current_tuple));

  tuple_store_->rebuild();
  EXPECT_FALSE(tuple_store_->isEmpty());
  EXPECT_TRUE(tuple_store_->isPacked());
  EXPECT_EQ(1, tuple_store_->getMaxTupleID());

  for (attribute_id attr_id = 0;
       attr_id < relation_->getMaxAttributeId();
       ++attr_id) {
    std::unordered_map<attribute_id, TypedValue> new_values;
    new_values[attr_id] = TypedValue(0);
    EXPECT_FALSE(tuple_store_->canSetAttributeValuesInPlaceTyped(0, new_values));
  }

  // Now attempt to actually set a value in place.
  TypedValue dummy_value(0);
  EXPECT_DEATH(tuple_store_->setAttributeValueInPlaceTyped(0, 0, dummy_value), "");
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, DeleteTest) {
  initializeTupleStoreDescription(0);
  initializeNewBlock(kSubBlockSize);
  fillBlockWithSampleData();

  tuple_id original_max_tuple_id = tuple_store_->getMaxTupleID();
  size_t num_deleted = 0;

  // Delete the very last tuple.
  EXPECT_FALSE(tuple_store_->deleteTuple(tuple_store_->getMaxTupleID()));
  EXPECT_EQ(original_max_tuple_id - 1, tuple_store_->getMaxTupleID());
  ++num_deleted;

  // Delete the hundredth tuple.
  EXPECT_TRUE(tuple_store_->deleteTuple(100));
  EXPECT_EQ(original_max_tuple_id - 2, tuple_store_->getMaxTupleID());
  ++num_deleted;

  // Delete a sequence of tuples.
  TupleIdSequence delete_sequence(tuple_store_->getMaxTupleID() + 1);
  for (tuple_id tid = 199;
       tid <= tuple_store_->getMaxTupleID();
       tid += 100) {
    delete_sequence.set(tid, true);
  }
  EXPECT_TRUE(tuple_store_->bulkDeleteTuples(&delete_sequence));
  EXPECT_EQ(static_cast<tuple_id>(
                original_max_tuple_id - delete_sequence.numTuples() - 2),
            tuple_store_->getMaxTupleID());
  num_deleted += delete_sequence.numTuples();

  // Delete a couple of tuples from the very end of the block.
  TupleIdSequence end_delete_sequence(tuple_store_->getMaxTupleID() + 1);
  end_delete_sequence.set(tuple_store_->getMaxTupleID(), true);
  end_delete_sequence.set(tuple_store_->getMaxTupleID() - 1, true);
  EXPECT_FALSE(tuple_store_->bulkDeleteTuples(&end_delete_sequence));
  EXPECT_EQ(static_cast<tuple_id>(
                original_max_tuple_id
                - delete_sequence.numTuples()
                - end_delete_sequence.numTuples()
                - 2),
            tuple_store_->getMaxTupleID());
  num_deleted += end_delete_sequence.numTuples();

  // Check that the remaining values are what we expect.
  checkBlockValues(0, num_deleted);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, GetMatchesForPredicateSortAttr0Test) {
  runCheckPredicateTest(0);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, GetMatchesForPredicateSortAttr1Test) {
  runCheckPredicateTest(1);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, GetMatchesForPredicateSortAttr2Test) {
  runCheckPredicateTest(2);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, GetMatchesForPredicateSortAttr3Test) {
  runCheckPredicateTest(3);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, GetMatchesForPredicateSortAttr4Test) {
  runCheckPredicateTest(4);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, GetMatchesForPredicateSortAttr5Test) {
  runCheckPredicateTest(5);
}

TEST_P(CompressedColumnStoreTupleStorageSubBlockTest, GetMatchesForPredicateSortAttr6Test) {
  runCheckPredicateTest(6);
}

TEST(CompressedColumnStoreTupleStorageSubBlockNullTypeTest, NullTypeTest) {
  // Set up a relation with an int attribute (need a sortable attribute to use
  // CompressedColumnStore) and a single NullType attribute.
  CatalogRelation test_relation(nullptr, "TestRelation");
  CatalogAttribute *int_attr = new CatalogAttribute(&test_relation,
                                                    "int_attr",
                                                    TypeFactory::GetType(kInt, false));
  ASSERT_EQ(0, test_relation.addAttribute(int_attr));
  CatalogAttribute *nulltype_attr = new CatalogAttribute(&test_relation,
                                                         "nulltype_attr",
                                                         TypeFactory::GetType(kNullType, true));
  ASSERT_EQ(1, test_relation.addAttribute(nulltype_attr));

  // Set up a minimal StorageBlockLayoutDescription.
  StorageBlockLayoutDescription layout_desc;
  layout_desc.set_num_slots(1);
  layout_desc.mutable_tuple_store_description()->set_sub_block_type(
      TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE);
  layout_desc.mutable_tuple_store_description()->SetExtension(
      CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id,
      0);

  // Check that the description is considered valid.
  EXPECT_TRUE(StorageBlockLayout::DescriptionIsValid(test_relation, layout_desc));

  StorageBlockLayout layout(test_relation, layout_desc);

  // Construct an actual CompressedColumnStoreTupleStorageSubBlock.
  ScopedBuffer tuple_store_memory(kSlotSizeBytes);
  CompressedColumnStoreTupleStorageSubBlock tuple_store(
      test_relation,
      layout_desc.tuple_store_description(),
      true,
      tuple_store_memory.get(),
      kSlotSizeBytes);

  // Insert some NullType values.
  std::vector<TypedValue> attr_values;
  attr_values.emplace_back(0);
  attr_values.emplace_back(kNullType);
  Tuple tuple(std::move(attr_values));

  for (tuple_id tid = 0; tid < 100; ++tid) {
    tuple_store.insertTupleInBatch(tuple);
  }
  tuple_store.rebuild();

  EXPECT_EQ(100, tuple_store.numTuples());

  // Delete some values.
  TupleIdSequence delete_sequence(100);
  delete_sequence.set(5, true);
  delete_sequence.set(25, true);
  delete_sequence.set(45, true);
  delete_sequence.set(65, true);
  delete_sequence.set(85, true);

  EXPECT_TRUE(tuple_store.bulkDeleteTuples(&delete_sequence));
  EXPECT_EQ(95, tuple_store.numTuples());
  ASSERT_EQ(94, tuple_store.getMaxTupleID());

  // Read out values.
  for (tuple_id tid = 0; tid < 95; ++tid) {
    ASSERT_TRUE(tuple_store.hasTupleWithID(tid));
    EXPECT_EQ(nullptr, tuple_store.getAttributeValue(tid, 1));

    TypedValue value = tuple_store.getAttributeValueTyped(tid, 1);
    EXPECT_TRUE(value.isNull());
    EXPECT_EQ(kNullType, value.getTypeID());
  }
}

// Note: INSTANTIATE_TEST_CASE_P has variadic arguments part. If the variable argument part
//       is empty, C++11 standard says it should produce a warning. A warning is converted
//       to an error since we use -Werror as a compiler parameter. It causes Travis to build.
//       This is the reason that we must give an empty string argument as a last parameter
//       to supress warning that clang gives.
INSTANTIATE_TEST_CASE_P(WithAndWithoutNullableAttributes,
                        CompressedColumnStoreTupleStorageSubBlockTest,
                        ::testing::Bool(),);  // NOLINT(whitespace/comma)

INSTANTIATE_TEST_CASE_P(WithAndWithoutNullableAttributes,
                        CompressedColumnStoreTupleStorageSubBlockDeathTest,
                        ::testing::Bool(),);  // NOLINT(whitespace/comma)

}  // namespace quickstep
