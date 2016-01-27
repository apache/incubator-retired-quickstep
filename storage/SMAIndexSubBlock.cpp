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

#include "storage/SMAIndexSubBlock.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <utility>
#include <exception>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "compression/CompressionDictionary.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/predicate/PredicateCost.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "storage/CompressedTupleStorageSubBlock.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageConstants.hpp"
#include "storage/StorageErrors.hpp"
#include "storage/SubBlockTypeRegistry.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/comparisons/ComparisonUtil.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"
#include "utility/ScopedBuffer.hpp"

#include "glog/logging.h"


using std::memcpy;
using std::memmove;
using std::pair;
using std::size_t;
using std::sort;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::vector;
using std::map;
using std::exception;

using std::string; // temporary

namespace quickstep {

QUICKSTEP_REGISTER_INDEX(SMAIndexSubBlock, SMA);

typedef smaindex_internal::EntryReference EntryReference;
typedef smaindex_internal::SMAPredicate SMAPredicate;
typedef smaindex_internal::SMAEntry SMAEntry;


string predicateString(const ComparisonPredicate& predicate) {
  string str = "(";
  if(predicate.getLeftOperand().hasStaticValue()) {

    //str += *reinterpret_cast<const int*>(predicate.getLeftOperand().getStaticValue().getDataPtr());
    str += std::to_string(predicate.getLeftOperand().getStaticValue().getLiteral<std::int64_t>());
    str += " ";
    str += kComparisonShortNames[(int) predicate.getComparison().getComparisonID()];
    str += " ";

    const CatalogAttribute& comparison_attribute = static_cast<const ScalarAttribute&>(
                                predicate.getRightOperand()).getAttribute();
    str += comparison_attribute.getName();

  } else {

    const CatalogAttribute& comparison_attribute = static_cast<const ScalarAttribute&>(
                                predicate.getLeftOperand()).getAttribute();
    str += comparison_attribute.getName();
    
    str += " ";
    str += kComparisonShortNames[(int) predicate.getComparison().getComparisonID()];
    str += " ";

    str += std::to_string(predicate.getRightOperand().getStaticValue().getLiteral<std::int64_t>());
    //*reinterpret_cast<const int*>(predicate.getRightOperand().getStaticValue().getDataPtr());
  }

  str += ")";

  return str;
}

attribute_id extractAttributeID(const ComparisonPredicate& predicate) {
  if (predicate.getLeftOperand().hasStaticValue()) {
    return (static_cast<const ScalarAttribute&>(
                                  predicate.getRightOperand()).getAttribute()).getID();
  } else {
    return (static_cast<const ScalarAttribute&>(
                                  predicate.getLeftOperand()).getAttribute()).getID();
  }
}

namespace smaindex_internal {

/// SMAPredicate methods

//TODO(marc)[1.15.2016] it will be faster to store the attribute ID or
// the index in the SMA's array
SMAPredicate* SMAPredicate::Create(const ComparisonPredicate& predicate) {
  const CatalogAttribute* comparison_attribute;
  TypedValue comparison_literal;
  ComparisonID comparison_id = predicate.getComparison().getComparisonID();

  if (predicate.getLeftOperand().hasStaticValue()) {
    DLOG_IF(FATAL, predicate.getRightOperand().getDataSource() != Scalar::kAttribute);

    comparison_attribute = &(static_cast<const ScalarAttribute&>(
                                  predicate.getRightOperand()).getAttribute());
    comparison_literal = predicate.getLeftOperand().getStaticValue().makeReferenceToThis();
    comparison_id = flipComparisonID(comparison_id);

  } else {
    DLOG_IF(FATAL, predicate.getLeftOperand().getDataSource() != Scalar::kAttribute);

    comparison_attribute = &(static_cast<const ScalarAttribute&>(
                                  predicate.getLeftOperand()).getAttribute());
    comparison_literal = predicate.getRightOperand().getStaticValue().makeReferenceToThis();
  }

  return new SMAPredicate( *comparison_attribute,
                            comparison_id,
                            comparison_literal);
}

string SMAPredicate::selectivityStr() const {
  switch (selectivity_) {
    case kAll:
      return string("all");
    case kNone:
      return string("none");
    case kSome: 
      return string("some");
    case kUnknown:
      return string("unknown");
    case kUnsolved:
      return string("unsolved");
    default:
      return string("ERROR");
  }
}

/// EntryReference methods

void EntryReference::set(tuple_id id, const TypedValue & value){
  DLOG_IF(FATAL, value.isReference()) << "EntryReferences do not support variable length values";

  tid_ = id;
  value_ = value;
}

/// SMAEntry methods

void SMAEntry::initialize(bool new_block, const attribute_id attr_id,
                                          const TupleStorageSubBlock& tuple_store) {

  const Type& attr_type = tuple_store.getRelation().getAttributeById(attr_id)->getType();

  if(new_block){
    attr_id_ = attr_id;
    attr_typeID_ = attr_type.getTypeID();
    reset();
    requires_rebuild_ = false;
  } else {
    // check to ensure values were stored correctly, there's no reason they
    // shouldn't have been
    CHECK(attr_id_ == attr_id);
    const Comparison& equals = ComparisonFactory::GetComparison(ComparisonID::kEqual);

    DLOG_IF(FATAL, !equals.compareTypedValuesChecked(min_.getValue(),
        tuple_store.getAttributeValueTyped(min_.getTupleID(), attr_id)));
    DLOG_IF(FATAL, !equals.compareTypedValuesChecked(max_.getValue(),
        tuple_store.getAttributeValueTyped(max_.getTupleID(), attr_id)));
  }

  // TODO fix, this is a hack to prevent segfaults occurring in some situations
  add_operator_.release();
  sub_operator_.release();
  less_comparison_.release();
  equal_comparison_.release();

  add_operator_.reset(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd)
                       .makeUncheckedBinaryOperatorForTypes(TypeFactory::GetType(sum_.getTypeID()), attr_type));

  sub_operator_.reset(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kSubtract)
                       .makeUncheckedBinaryOperatorForTypes(TypeFactory::GetType(sum_.getTypeID()), attr_type));

  less_comparison_.reset(ComparisonFactory::GetComparison(ComparisonID::kLess)
                           .makeUncheckedComparatorForTypes(attr_type, attr_type));

  equal_comparison_.reset(ComparisonFactory::GetComparison(ComparisonID::kEqual)
                           .makeUncheckedComparatorForTypes(attr_type, attr_type));

}

void SMAEntry::reset() {
  min_.reset();
  max_.reset();
  count_ = 0;
  sum_ = AggregationStateSum::SumZero(TypeFactory::GetType(attr_typeID_, true));
  requires_rebuild_ = true;
}

void SMAEntry::insertUpdate(const tuple_id tid, const TupleStorageSubBlock& tuple_store) {

  TypedValue insertValue = tuple_store.getAttributeValueTyped(tid, attr_id_);

  // update mins and maxes
  if(count_ == 0) {
    max_.set(tid, insertValue);
    min_.set(tid, insertValue);
  } else if(less_comparison_->compareTypedValues(max_.getValue(), insertValue)) {
    max_.set(tid, insertValue);
  } else if(less_comparison_->compareTypedValues(insertValue, min_.getValue())) {
    min_.set(tid, insertValue);
  }

  // update count and sum
  count_++;
  if(!insertValue.isNull()) {
    sum_ = add_operator_->applyToTypedValues(sum_, insertValue);
  }
}

void SMAEntry::removeUpdate(const tuple_id tid, const TupleStorageSubBlock& tuple_store) {
  // if we require a scan, don't bother trying to update
  if(requires_rebuild_)
    return;

  const TypedValue& removedValue = tuple_store.getAttributeValueTyped(tid, attr_id_);

  // check if they are one of the min or max values
  if(equal_comparison_->compareTypedValues(max_.getValue(), removedValue) ||
      equal_comparison_->compareTypedValues(removedValue, min_.getValue())) {
    requires_rebuild_ = true;
  }

  count_--;
  if(!removedValue.isNull()){
    sum_ = sub_operator_->applyToTypedValues(sum_, removedValue);
  }
}

void SMAEntry::solvePredicate(SMAPredicate& predicate) const {
  DLOG_IF(FATAL, predicate.attribute_.getID() != attr_id_) << "comparison made against incorrect entry";

  if(getCount() == 0) {
    predicate.selectivity_ = SMAPredicate::Selectivity::kNone;
    return;
  }

  const TypedValue& min = min_.getValue();
  const TypedValue& max = max_.getValue();

  switch(predicate.comparisonid_) {
  case ComparisonID::kEqual:
    if(less_comparison_->compareTypedValues(predicate.literal_, min) ||
        less_comparison_->compareTypedValues(max, predicate.literal_)) {
      predicate.selectivity_ = SMAPredicate::Selectivity::kNone;
    } else {
      predicate.selectivity_ = SMAPredicate::Selectivity::kUnknown;
    }
    return;

  case ComparisonID::kNotEqual:
    predicate.selectivity_ = SMAPredicate::Selectivity::kUnknown;
    return;

  case ComparisonID::kLess:
    if(less_comparison_->compareTypedValues(max, predicate.literal_)) {
      predicate.selectivity_ = SMAPredicate::Selectivity::kAll;
    } else if (less_comparison_->compareTypedValues(predicate.literal_, min)) {
      predicate.selectivity_ = SMAPredicate::Selectivity::kNone;
    } else {
      predicate.selectivity_ = SMAPredicate::Selectivity::kUnknown;
    }
    return;

  case ComparisonID::kLessOrEqual:
    if(less_comparison_->compareTypedValues(max, predicate.literal_) ||
        equal_comparison_->compareTypedValues(max, predicate.literal_)) {
      predicate.selectivity_ = SMAPredicate::Selectivity::kAll;
    } else if (less_comparison_->compareTypedValues(predicate.literal_, min)) {
      predicate.selectivity_ = SMAPredicate::Selectivity::kNone;
    } else {
      predicate.selectivity_ = SMAPredicate::Selectivity::kUnknown;
    }
    return;

  case ComparisonID::kGreater:
    if(less_comparison_->compareTypedValues(predicate.literal_, min)) {
      predicate.selectivity_ = SMAPredicate::Selectivity::kAll;
    } else if (less_comparison_->compareTypedValues(max, predicate.literal_)) {
      predicate.selectivity_ = SMAPredicate::Selectivity::kNone;
    } else {
      predicate.selectivity_ = SMAPredicate::Selectivity::kUnknown;
    }
    return;

  case ComparisonID::kGreaterOrEqual:
    if(less_comparison_->compareTypedValues(predicate.literal_, min) ||
        equal_comparison_->compareTypedValues(predicate.literal_, min)) {
      predicate.selectivity_ = SMAPredicate::Selectivity::kAll;
    } else if (less_comparison_->compareTypedValues(max, predicate.literal_)) {
      predicate.selectivity_ = SMAPredicate::Selectivity::kNone;
    } else {
      predicate.selectivity_ = SMAPredicate::Selectivity::kUnknown;
    }
    return;

  default:
    DLOG_IF(FATAL, false) << "SMAEntry::getSelectivity: Unrecognized comparison";
  }
}

} // namespace smaindex_internal


SMAIndexSubBlock::SMAIndexSubBlock(const TupleStorageSubBlock &tuple_store,
                                   const IndexSubBlockDescription &description,
                                   const bool new_block,
                                   void *sub_block_memory,
                                   const std::size_t sub_block_memory_size)
                                          : IndexSubBlock(tuple_store,
                                                          description,
                                                          new_block,
                                                          sub_block_memory,
                                                          sub_block_memory_size),
                                                          initialized_(false) {
  CHECK(DescriptionIsValid(relation_, description_)) <<\
    "Attempted to construct an SMAIndexSubBlock from an invalid description.";

  num_entries_ = description.ExtensionSize(SMAIndexSubBlockDescription::indexed_attribute_id);

  CHECK(num_entries_ * sizeof(SMAEntry) <= sub_block_memory_size_) <<\
    "Attempted to create SMAIndexSubBlock without enough space allocated.";

  // iterate through each attribute which is being indexed
  for( int indexed_attribute_num = 0;
       indexed_attribute_num < num_entries_;
       ++indexed_attribute_num) {

    const attribute_id attr_id = description_.GetExtension(
                                  SMAIndexSubBlockDescription::indexed_attribute_id,
                                  indexed_attribute_num );

    attr_ids_.push_back(attr_id);
    // associate an attribute ID to the index of its SMAEntry
    attr_to_index_[attr_id] = indexed_attribute_num;


    // cast raw memory into entry
    SMAEntry* entry = reinterpret_cast<SMAEntry*>(sub_block_memory_);
    entry = entry + indexed_attribute_num; // index in to correct entry
    entry->initialize(new_block, attr_id, tuple_store);
  }
}

bool SMAIndexSubBlock::DescriptionIsValid(const CatalogRelationSchema & relation,
                                          const IndexSubBlockDescription & description) {
  if (!description.IsInitialized()) {
   return false;
  }
  if (description.sub_block_type() != IndexSubBlockDescription::SMA) {
   return false;
  }
  if(description.ExtensionSize(SMAIndexSubBlockDescription::indexed_attribute_id) == 0){
    return false;
  }
  // Check that all key attributes exist and are fixed-length.
  for (int indexed_attribute_num = 0;
       indexed_attribute_num < description.ExtensionSize(SMAIndexSubBlockDescription::indexed_attribute_id);
       ++indexed_attribute_num) {
    attribute_id indexed_attribute_id
        = description.GetExtension(SMAIndexSubBlockDescription::indexed_attribute_id, indexed_attribute_num);
    if (!relation.hasAttributeWithId(indexed_attribute_id)) {
      return false;
    }
    const Type &attr_type = relation.getAttributeById(indexed_attribute_id)->getType();
    if (attr_type.isVariableLength()) {
      // we don't yet support string types
      // TODO(marc)[1.15.2015] support string types if compressed codes
      return false;
    }
  }
  return true;
}

bool SMAIndexSubBlock::bulkAddEntries(const TupleIdSequence & tuples) {
  // tuples will exist in the StorageBlock
  for(auto t_id_itr = tuples.begin(); t_id_itr != tuples.end(); ++t_id_itr){
    for(unsigned smaEntry = 0; smaEntry < num_entries_; ++smaEntry) {
      (reinterpret_cast<SMAEntry*>(sub_block_memory_))[smaEntry].insertUpdate(*t_id_itr, tuple_store_);
    }
  }
  return true;
}

void SMAIndexSubBlock::bulkRemoveEntries(const TupleIdSequence &tuples) {
  // entries are removed from the index before the storage block
  // simply invalidate all entries, requiring a rebuild
  for(unsigned smaEntry = 0; smaEntry < num_entries_; ++smaEntry) {
    (reinterpret_cast<SMAEntry*>(sub_block_memory_))[smaEntry].setRequiresRebuild(true);
  }
}

predicate_cost_t SMAIndexSubBlock::estimatePredicateEvaluationCost(
                                  const ComparisonPredicate &predicate) const {

 // DLOG(INFO) << "Estimate Predicate " << predicateString(predicate);

  //TODO(marc)[1.15.2016] refactor so we don't have to do a scan of the entries
  if( requiresRebuild() ||
      !predicate.isAttributeLiteralComparisonPredicate() ||
      !hasEntryForAttribute(extractAttributeID(predicate)) ) {

    //DLOG(INFO) << "Skipped: " << ((requiresRebuild()) ? "required rebuild" : "was not an ACL");

    return predicate_cost::kInfinite;
  }

  std::unique_ptr<SMAPredicate> sma_predicate(solvePredicate(predicate));

  // DLOG(INFO) << "Matched successfully on predicate " << predicateString(predicate) 
  //               << " with selectivity " << sma_predicate->selectivityStr();

  if( sma_predicate->selectivity_ == SMAPredicate::kAll || 
      sma_predicate->selectivity_ == SMAPredicate::kNone ) {

    auto nonconst = const_cast<SMAIndexSubBlock*>(this);
    nonconst->last_predicate_.reset(sma_predicate.release());

    return predicate_cost::kConstantTime;
  } 

  return predicate_cost::kInfinite;
}

std::size_t SMAIndexSubBlock::EstimateBytesPerTuple(
                                  const CatalogRelationSchema &relation,
                                  const IndexSubBlockDescription &description) {
  // TODO(marc)[1.13.2016]: 1 will give too much space. An enhancement would be to inform
  // the storage subblock that this index uses space w.r.t. # of attributes
  // being indexed, not tuples. Below is the actual space requirement:
  // std::size_t space_req = (std::size_t)
  //   (description.ExtensionSize(SMAIndexSubBlockDescription::indexed_attribute_id) * sizeof(SMAEntry));

  return 1;
}

bool SMAIndexSubBlock::addEntry(const tuple_id tuple) {
  for(unsigned smaEntry = 0; smaEntry < num_entries_; ++smaEntry){
    (reinterpret_cast<SMAEntry*>(sub_block_memory_))[smaEntry].insertUpdate(tuple, tuple_store_);
  }
  return true;
}

void SMAIndexSubBlock::removeEntry(const tuple_id tuple) {
  for(unsigned smaEntry = 0; smaEntry < num_entries_; ++smaEntry){
    (reinterpret_cast<SMAEntry*>(sub_block_memory_))[smaEntry].removeUpdate(tuple, tuple_store_);
  }
}

TupleIdSequence* SMAIndexSubBlock::getMatchesForPredicate(
                                          const ComparisonPredicate &predicate,
                                          const TupleIdSequence *filter) const {

  DLOG_IF(FATAL, !predicate.isAttributeLiteralComparisonPredicate()) << \
    "SMAIndex predicate must be a simple comparison predicate";

  DLOG_IF(FATAL, requiresRebuild()) << "SMAIndex was inconsistent and used for a selection";

  LOG(INFO) << "Matches for Predicate " << predicateString(predicate);


  // TODO(marc) speed up here
  // std::unique_ptr<SMAPredicate> sma_predicate(solvePredicate(predicate));
  // auto nonconst = const_cast<SMAIndexSubBlock*>(this);
  // nonconst->last_predicate_.reset(sma_predicate);

  switch(last_predicate_->selectivity_){
    case SMAPredicate::kNone: {
      TupleIdSequence* tidseq = new TupleIdSequence(tuple_store_.numTuples());
      tidseq->setRange(0, tuple_store_.numTuples(), false);
      return tidseq;
    }
    case SMAPredicate::kAll: {
      TupleIdSequence* tidseq = new TupleIdSequence(tuple_store_.numTuples());
      if(tuple_store_.isPacked()){
        tidseq->setRange(0, tuple_store_.numTuples(), true);
      } else {
        for(tuple_id tid = 0; tid <= tuple_store_.getMaxTupleID(); ++tid) {
          if(tuple_store_.hasTupleWithID(tid)){
            tidseq->set(tid, true);
          }
        }
      }
      return tidseq;
    }
    default: {
      LOG(FATAL) << "SMAIndex failed to answer predicate. This means the SMA should not have been used";
    }
  }

  return nullptr;
}

SMAPredicate* SMAIndexSubBlock::solvePredicate(const ComparisonPredicate& predicate) const {
  // index to the corred SMAEntry, solve
  SMAPredicate* sma_predicate = SMAPredicate::Create(predicate);

  SMAEntry* entries = reinterpret_cast<SMAEntry*>(sub_block_memory_);

  unsigned entry_index = attr_to_index_.at(sma_predicate->attribute_.getID());

  entries[entry_index].solvePredicate(*sma_predicate);

  return sma_predicate;
}

bool SMAIndexSubBlock::rebuild() {
  // reset each entry
  for(int entry = 0; entry < num_entries_; ++entry){
    reinterpret_cast<SMAEntry*>(sub_block_memory_)[entry].reset();
  }

  if (tuple_store_.isPacked()) {
    for (tuple_id tid = 0; tid <= tuple_store_.getMaxTupleID(); ++tid) {
      for (unsigned attr_num = 0; attr_num < num_entries_; ++attr_num) {
        (reinterpret_cast<SMAEntry*>(sub_block_memory_))[attr_num].insertUpdate(tid, tuple_store_);
      }
    }
  } else {
    for (tuple_id tid = 0; tid <= tuple_store_.getMaxTupleID(); ++tid) {
      if (tuple_store_.hasTupleWithID(tid)) {
        for (unsigned attr_num = 0; attr_num < num_entries_; ++attr_num) {
          (reinterpret_cast<SMAEntry*>(sub_block_memory_))[attr_num].insertUpdate(tid, tuple_store_);
        }
      }
    }
  }
  // set each entry to rebuild state
  for(int entry = 0; entry < num_entries_; ++entry){
    reinterpret_cast<SMAEntry*>(sub_block_memory_)[entry].setRequiresRebuild(false);
  }
  return true;
}

bool SMAIndexSubBlock::requiresRebuild() const {
  for(int entry = 0; entry < num_entries_; ++entry){
    if(reinterpret_cast<SMAEntry*>(sub_block_memory_)[entry].getRequiresRebuild()) {
      return true;
    }
  }
  return false;
}

bool SMAIndexSubBlock::hasEntryForAttribute(attribute_id attribute) const {
  return attr_to_index_.find(attribute) != attr_to_index_.end();
}

const smaindex_internal::SMAEntry* SMAIndexSubBlock::getEntry(attribute_id attribute) const {
  auto itr = attr_to_index_.find(attribute);
  if(itr != attr_to_index_.end()){
    unsigned index = itr->second;
    return &(reinterpret_cast<const smaindex_internal::SMAEntry*>(sub_block_memory_)[index]);
  }
  return nullptr;
}

unsigned SMAIndexSubBlock::getCount() const {
  return reinterpret_cast<SMAEntry*>(sub_block_memory_)[0].getCount();
}

}  // namespace quickstep
