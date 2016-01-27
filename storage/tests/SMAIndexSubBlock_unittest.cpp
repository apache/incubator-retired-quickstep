/**
 *   Copyright 2016 Pivotal Software, Inc.
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
#include <limits>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "glog/logging.h"

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/TrivialPredicates.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/scalar/ScalarLiteral.hpp"
#include "storage/CompressedPackedRowStoreTupleStorageSubBlock.hpp"
#include "storage/CompressedTupleStorageSubBlock.hpp"
#include "storage/SMAIndexSubBlock.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageErrors.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/tests/MockTupleStorageSubBlock.hpp"
#include "types/CharType.hpp"
#include "types/FloatType.hpp"
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
#include "types/operations/comparisons/ComparisonUtil.hpp"
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

typedef smaindex_internal::EntryReference EntryReference;
typedef smaindex_internal::SMAEntry SMAEntry;
typedef smaindex_internal::SMAPredicate SMAPredicate;

class SMAIndexSubBlockTest : public ::testing::Test {
 protected:
  static const size_t kIndexSubBlockSize = 0x100000;  // 1 MB
  // Hack: replace long_attr values equal to this with NULL.
  static const int64_t kLongAttrNullValue = -55555;
  static const char kCharAttrNullValue[];

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

    // Don't initialize the SMAIndexSubBlock yet. Different tests will
    // index on different attributes.
    index_memory_.reset();
    index_description_.reset();
    index_.reset();
  }



  void createIndex(const vector<attribute_id> &indexed_attrs, const size_t index_memory_size){
    // Make the IndexSubBlockDescription.
    index_description_.reset(new IndexSubBlockDescription());
    index_description_->set_sub_block_type(IndexSubBlockDescription::SMA);
    for(int i = 0; i < indexed_attrs.size(); ++i){
      index_description_->AddExtension(SMAIndexSubBlockDescription::indexed_attribute_id,
                                        indexed_attrs[i]);
    }

    index_memory_.reset(index_memory_size);
    index_.reset(new SMAIndexSubBlock(*tuple_store_,
                                      *index_description_,
                                      true,
                                      index_memory_.get(),
                                      index_memory_size));
  }

  Tuple* createTuple() const {
    return new Tuple(*relation_);
  }

  static void AppendValueToTuple(Tuple *tuple, const TypedValue &value) {
    tuple->append(value);
  }

  // we must make accessors for each private member of the index we wish to
  // access in the test because TEST_F creates a subclass which is no longer
  // friends with the index
  SMAEntry* getEntryForAttribute(attribute_id a_id) const {
    return const_cast<SMAEntry*>(index_->getEntry(a_id));
  }

  // private method accessor
  SMAPredicate* callSolvePredicate(const ComparisonPredicate &predicate) {
    return index_->solvePredicate(predicate);
  }

  // private variable accessor
  bool getRequiresRebuild() const {
    return index_->requiresRebuild();
  }

  // Insert a tuple with the specified attribute values into tuple_store_.
  tuple_id insertTupleInTupleStore(const int64_t long_val,
                                   const int64_t nullable_long_val,
                                   const float float_val,
                                   const string &char_val,
                                   const string &nullable_char_val,
                                   const string &big_char_val,
                                   const string &varchar_val) {
    std::unique_ptr<Tuple> new_tuple(new Tuple(*relation_));

    new_tuple->append(LongType::InstanceNonNullable().makeValue(&long_val));

    if (nullable_long_val == kLongAttrNullValue) {
      new_tuple->append(LongType::InstanceNullable().makeNullValue());
    } else {
      new_tuple->append(LongType::InstanceNullable().makeValue(&nullable_long_val));
    }

    new_tuple->append(FloatType::InstanceNonNullable().makeValue(&float_val));

    new_tuple->append(CharType::InstanceNonNullable(4).makeValue(
        char_val.c_str(),
        char_val.size() >= 4 ? 4 : char_val.size() + 1).ensureNotReference());

    if (nullable_char_val == kCharAttrNullValue) {
      new_tuple->append(CharType::InstanceNullable(4).makeNullValue());
    } else {
      new_tuple->append(CharType::InstanceNonNullable(4).makeValue(
          nullable_char_val.c_str(),
          nullable_char_val.size() >= 4 ? 4 : nullable_char_val.size() + 1).ensureNotReference());
    }

    new_tuple->append(CharType::InstanceNonNullable(80).makeValue(
        big_char_val.c_str(),
        big_char_val.size() >= 80 ? 80 : big_char_val.size() + 1).ensureNotReference());

    new_tuple->append(VarCharType::InstanceNonNullable(varchar_val.size()).makeValue(
        varchar_val.c_str(),
        varchar_val.size() + 1)
            // Sometimes we test with strings bigger than 8 chars, so coerce.
            .coerceInPlace(VarCharType::InstanceNonNullable(8).getSignature())
            .ensureNotReference());

    return tuple_store_->addTupleMock(new_tuple.release());
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

  // Create a ComparisonPredicate of the form "attribute comp literal".
  template <typename AttributeType>
  ComparisonPredicate* generateNumericComparisonPredicate(
      const ComparisonID comp,
      const attribute_id attribute,
      const typename AttributeType::cpptype literal) {
    ScalarAttribute *scalar_attribute = new ScalarAttribute(*relation_->getAttributeById(attribute));
    ScalarLiteral *scalar_literal
        = new ScalarLiteral(AttributeType::InstanceNonNullable().makeValue(&literal));

    return new ComparisonPredicate(ComparisonFactory::GetComparison(comp), scalar_attribute, scalar_literal);
  }

  std::unique_ptr<CatalogRelation> relation_;
  std::unique_ptr<MockTupleStorageSubBlock> tuple_store_;
  ScopedBuffer index_memory_;
  std::unique_ptr<IndexSubBlockDescription> index_description_;
  std::unique_ptr<SMAIndexSubBlock> index_;

};

const char SMAIndexSubBlockTest::kCharAttrNullValue[] = "_NULLSTRING";

TEST_F(SMAIndexSubBlockTest, TestConstructor) {
  vector<attribute_id> attrs;
  attrs.push_back(0);
  createIndex(attrs, kIndexSubBlockSize);

  std::unique_ptr<ComparisonPredicate> pred(generateNumericComparisonPredicate<LongType>(ComparisonID::kEqual, 0, 0)); // attr0 == 0
  std::unique_ptr<TupleIdSequence> match_sequence(index_->getMatchesForPredicate(*pred, nullptr));
  
  EXPECT_TRUE(match_sequence->numTuples() == 0); // no matches
}

TEST_F(SMAIndexSubBlockTest, TestRebuild) {
  vector<attribute_id> attrs;
  attrs.push_back(0); // long
  attrs.push_back(2); // float
  createIndex(attrs, kIndexSubBlockSize);
  int f_id = -1, l_id = -1; // first, last ids
  int min = 0, max = 9010, step = 10;
  for(unsigned i = min; i <= max; i+=step){
    if(i == 0){
      f_id = generateAndInsertTuple(i, false, "suffix");
    } else if( i == max){
      l_id = generateAndInsertTuple(i, false, "suffix");
    } else {
      generateAndInsertTuple(i, false, "suffix");
    }
  }
  index_->rebuild();
  SMAEntry* attr0Entry = getEntryForAttribute(0);
  SMAEntry* attr2Entry = getEntryForAttribute(2);
  EXPECT_EQ(f_id, attr0Entry->getMinEntryReference().getTupleID());
  EXPECT_EQ(l_id, attr0Entry->getMaxEntryReference().getTupleID());
  EXPECT_EQ(f_id, attr2Entry->getMinEntryReference().getTupleID());
  EXPECT_EQ(l_id, attr2Entry->getMaxEntryReference().getTupleID());
  // make the tuple store unpacked
  tuple_store_->deleteTuple(1);
  tuple_store_->deleteTuple(2);
  index_->rebuild();
  EXPECT_EQ(f_id, attr0Entry->getMinEntryReference().getTupleID());
  EXPECT_EQ(l_id, attr0Entry->getMaxEntryReference().getTupleID());
  EXPECT_EQ(f_id, attr2Entry->getMinEntryReference().getTupleID());
  EXPECT_EQ(l_id, attr2Entry->getMaxEntryReference().getTupleID());
  // bulk remove
  std::unique_ptr<TupleIdSequence> tid_seq(new TupleIdSequence(tuple_store_->getMaxTupleID()));
  tid_seq->set(10, true);
  tid_seq->set(20, true);
  tid_seq->set(30, true);
  index_->bulkRemoveEntries(*(tid_seq.get()));
  tuple_store_->bulkDeleteTuples(tid_seq.get());
  EXPECT_EQ(true, getRequiresRebuild());
  index_->removeEntry(l_id);
  tuple_store_->deleteTuple(l_id);
  EXPECT_EQ(true, getRequiresRebuild());
  l_id -= 1;
  index_->rebuild(); // since requires a rebuild, we rebuild
  EXPECT_EQ(l_id, attr0Entry->getMaxEntryReference().getTupleID());
  EXPECT_EQ(false, getRequiresRebuild());
}

TEST_F(SMAIndexSubBlockTest, TestExtractComparison) {
  const int indexed_attr = 0;
  vector<attribute_id> attrs;
  attrs.push_back(indexed_attr);
  createIndex(attrs, kIndexSubBlockSize);
  std::unique_ptr<ComparisonPredicate> pred(
      generateNumericComparisonPredicate<LongType>( ComparisonID::kEqual,
                                                    indexed_attr, 0));
  std::unique_ptr<SMAPredicate> smapredicate(callSolvePredicate(*pred));

  EXPECT_EQ(relation_->getAttributeById(0)->getType().getTypeID(),
              smapredicate->attribute_.getType().getTypeID());

  EXPECT_EQ(ComparisonID::kEqual, smapredicate->comparisonid_);

  EXPECT_EQ(0, smapredicate->literal_.getLiteral<std::int64_t>());
}

TEST_F(SMAIndexSubBlockTest, TestGetSelectivity) {
  vector<attribute_id> attrs;
  attrs.push_back(0); // long
  attrs.push_back(2); // float
  createIndex(attrs, kIndexSubBlockSize);
  int f_id = -1, l_id = -1;  // first/last id
  int min = 0, max = 9010, step = 10;
  for(unsigned i = min; i <= max; i+=step){
    if(i == min){
      f_id = generateAndInsertTuple(i, false, "suffix");
    } else if( i == max){
      l_id = generateAndInsertTuple(i, false, "suffix");
    } else {
      generateAndInsertTuple(i, false, "suffix");
    }
  }
  index_->rebuild();

  /// test EQUALS
  std::unique_ptr<ComparisonPredicate> pred;
  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kEqual,
                                                          *(attrs.begin()),
                                                          min - 10));
  std::unique_ptr<SMAPredicate> smapredicate(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kNone, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kEqual,
                                                          *(attrs.begin()),
                                                          max + 10));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kNone, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kEqual,
                                                            *(attrs.begin()),
                                                            max - ((max-min) / 2)));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kUnknown, smapredicate->selectivity_);

  /// test LESS THAN
  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kLess,
                                                          *(attrs.begin()),
                                                          min - 10));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kNone, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kLess,
                                                          *(attrs.begin()),
                                                          max + 10));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kAll, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kLess,
                                                            *(attrs.begin()),
                                                            max - ((max-min) / 2)));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kUnknown, smapredicate->selectivity_);

  /// test LESS OR EQUAL
  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kLessOrEqual,
                                                           *(attrs.begin()),
                                                           min));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kUnknown, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kLessOrEqual,
                                                          *(attrs.begin()),
                                                          max));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kAll, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kLessOrEqual,
                                                            *(attrs.begin()),
                                                            max - ((max-min) / 2)));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kUnknown, smapredicate->selectivity_);

  /// test GREATER THAN
  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kGreater,
                                                          *(attrs.begin()),
                                                          min - 10));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kAll, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kGreater,
                                                          *(attrs.begin()),
                                                          max + 10));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kNone, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kGreater,
                                                            *(attrs.begin()),
                                                            max - ((max-min) / 2)));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kUnknown, smapredicate->selectivity_);

  // test GREATER OR EQUAL
  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kGreaterOrEqual,
                                                           *(attrs.begin()),
                                                           min));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kAll, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kGreaterOrEqual,
                                                          *(attrs.begin()),
                                                          max));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kUnknown, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kGreaterOrEqual,
                                                            *(attrs.begin()),
                                                            max - ((max-min) / 2)));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kUnknown, smapredicate->selectivity_);
}

// Test special functionality with a CompressedTupleStorageSubBlock and
// truncation-compressed key.
TEST_F(SMAIndexSubBlockTest, TruncatedCompressedKeyTest) {
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
  compressed_index_description.set_sub_block_type(IndexSubBlockDescription::SMA);
  compressed_index_description.AddExtension(SMAIndexSubBlockDescription::indexed_attribute_id, 0);

  index_memory_.reset(kIndexSubBlockSize);
  index_.reset(new SMAIndexSubBlock(*compressed_tuple_store,
                                        compressed_index_description,
                                        true,
                                        index_memory_.get(),
                                        kIndexSubBlockSize));

  // Bulk-load some tuples into the compressed tuple store.
  int min = 0, max = 9010, step = 10;
  for(unsigned i = min; i <= max; i+=step){
    std::unique_ptr<Tuple> tuple(createTuple());
    int64_t value_buf = i;
    AppendValueToTuple(tuple.get(),
                         LongType::InstanceNonNullable().makeValue(&value_buf));
    ASSERT_TRUE(compressed_tuple_store->insertTupleInBatch(*tuple, kNone));
  }


  // Build the tuple-store.
  compressed_tuple_store->rebuild();
  EXPECT_FALSE(compressed_tuple_store->compressedAttributeIsDictionaryCompressed(0));
  EXPECT_TRUE(compressed_tuple_store->compressedAttributeIsTruncationCompressed(0));
  EXPECT_EQ(2u, compressed_tuple_store->compressedGetCompressedAttributeSize(0));

  // Build the index.
  ASSERT_TRUE(index_->rebuild());
  // test equals
  std::unique_ptr<ComparisonPredicate> pred;
  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kEqual,
                                                          0,
                                                          min - 10));
  std::unique_ptr<SMAPredicate> smapredicate(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kNone, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kEqual,
                                                          0,
                                                          max + 10));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kNone, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kEqual,
                                                            0,
                                                            max - ((max-min) / 2)));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kUnknown, smapredicate->selectivity_);

  // test less than
  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kLess,
                                                          0,
                                                          min - 10));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kNone, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kLess,
                                                          0,
                                                          max + 10));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kAll, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kLess,
                                                            0,
                                                            max - ((max-min) / 2)));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kUnknown, smapredicate->selectivity_);

  // test less or equal
  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kLessOrEqual,
                                                           0,
                                                           min));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kUnknown, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kLessOrEqual,
                                                          0,
                                                          max));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kAll, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kLessOrEqual,
                                                            0,
                                                            max - ((max-min) / 2)));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kUnknown, smapredicate->selectivity_);

  // test greater than
  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kGreater,
                                                          0,
                                                          min - 10));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kAll, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kGreater,
                                                          0,
                                                          max + 10));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kNone, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kGreater,
                                                            0,
                                                            max - ((max-min) / 2)));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kUnknown, smapredicate->selectivity_);

  // test greater or equal
  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kGreaterOrEqual,
                                                           0,
                                                           min));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kAll, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kGreaterOrEqual,
                                                          0,
                                                          max));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kUnknown, smapredicate->selectivity_);

  pred.reset(generateNumericComparisonPredicate<LongType>( ComparisonID::kGreaterOrEqual,
                                                            0,
                                                            max - ((max-min) / 2)));
  smapredicate.reset(callSolvePredicate(*pred));
  EXPECT_EQ(SMAPredicate::Selectivity::kUnknown, smapredicate->selectivity_);
}

TEST_F(SMAIndexSubBlockTest, TestCount) {
  vector<attribute_id> attrs;
  attrs.push_back(0); // long
  attrs.push_back(2); // float
  createIndex(attrs, kIndexSubBlockSize);
  int min = 0, max = 10000, step = 10;
  unsigned realcount = 0;
  for(unsigned i = min; i <= max; i+=step){
    generateAndInsertTuple(i, false, "suffix");
    realcount++;
  }
  index_->rebuild();
  EXPECT_EQ(realcount, index_->getCount());
  for(unsigned i = 0; i < 10; i++){
    tuple_id added = generateAndInsertTuple(301 + i, false, "suffix");
    index_->addEntry(added);
    realcount++;
  }
  EXPECT_EQ(false, index_->requiresRebuild());
  EXPECT_EQ(realcount, index_->getCount());
  for(tuple_id i = 30; i < 50; i++){
    index_->removeEntry(i);
    tuple_store_->deleteTuple(i);
    realcount--;
  }
  EXPECT_EQ(false, index_->requiresRebuild());
  EXPECT_EQ(realcount, index_->getCount());
}

TEST_F(SMAIndexSubBlockTest, TestSum) {
  vector<attribute_id> attrs;
  attrs.push_back(0); // long
  attrs.push_back(1); // nullable long
  attrs.push_back(2); // float
  createIndex(attrs, kIndexSubBlockSize);
  int min = 0, max = 10000, step = 10;
  int count = 0;
  long sum0 = 0; // sum of attr0
  long sum1 = 0; 
  double sum2 = 0.0;
  for(unsigned i = min; i < max; i+=step){
    if(i % 2){
      generateAndInsertTuple(i, true, "suffix");
    } else {
      generateAndInsertTuple(i, false, "suffix");
      sum1 += i;
    }
    sum0 += i;
    sum2 += ((double)i)* 0.25;
    count++;
  }
  index_->rebuild();
  long sma_sum0 = index_->getEntry(0)->getSum().getLiteral<long>();
  long sma_sum1 = index_->getEntry(1)->getSum().getLiteral<long>();
  double sma_sum2 = index_->getEntry(2)->getSum().getLiteral<double>();
  EXPECT_EQ(sum0, sma_sum0);
  EXPECT_EQ(sum1, sma_sum1);
  EXPECT_EQ(sum2, sma_sum2);

  // add some more values an do not call rebuild
  long after_add0 = sum0;
  long after_add1 = sum1;
  double after_add2 = sum2;
  vector<tuple_id> added_ids;
  for(unsigned i = max; i < max + 10000; i++) {
    tuple_id added = 0;
    if(i % 2){
      added = generateAndInsertTuple(i, true, "suffix");
    } else {
      added = generateAndInsertTuple(i, false, "suffix");
      after_add1 += i;
    }
    after_add0 += i;
    after_add2 += ((double)i)* 0.25;
    index_->addEntry(added);
    added_ids.push_back(added);
    count++;
  }

  EXPECT_EQ(after_add0, index_->getEntry(0)->getSum().getLiteral<long>());
  EXPECT_EQ(after_add1, index_->getEntry(1)->getSum().getLiteral<long>());
  EXPECT_EQ(after_add2, index_->getEntry(2)->getSum().getLiteral<double>());

  // remove the added values
  for(tuple_id tid : added_ids){
    index_->removeEntry(tid);
    count--;
  }
  EXPECT_EQ(sum0, index_->getEntry(0)->getSum().getLiteral<long>());
  EXPECT_EQ(sum1, index_->getEntry(1)->getSum().getLiteral<long>());
  EXPECT_EQ(sum2, index_->getEntry(2)->getSum().getLiteral<double>());
  EXPECT_EQ(count, index_->getCount());
}


}
