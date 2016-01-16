/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_STORAGE_TESTS_MOCK_TUPLE_STORAGE_SUB_BLOCK_HPP_
#define QUICKSTEP_STORAGE_TESTS_MOCK_TUPLE_STORAGE_SUB_BLOCK_HPP_

#include <unordered_map>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A mock memory-only TupleStorageSubBlock which can be used for testing
 *        purposes.
 **/
class MockTupleStorageSubBlock : public TupleStorageSubBlock {
 public:
  explicit MockTupleStorageSubBlock(const CatalogRelation &relation)
      : TupleStorageSubBlock(relation,
                             TupleStorageSubBlockDescription::default_instance(),
                             true,
                             NULL,
                             0),
        packed_(true) {
  }

  ~MockTupleStorageSubBlock() {
  }

  TupleStorageSubBlockType getTupleStorageSubBlockType() const override {
    FATAL_ERROR("getTupleStorageSubBlockType() is unimplemented in MockTupleStorageSubBlock.");
  }

  bool supportsUntypedGetAttributeValue(const attribute_id attr) const override {
    return true;
  }

  bool supportsAdHocInsert() const override {
    return true;
  }

  bool adHocInsertIsEfficient() const override {
    return true;
  }

  bool isEmpty() const override {
    return tuples_.emptyNullCheck();
  }

  bool isPacked() const override {
    return packed_;
  }

  tuple_id getMaxTupleID() const override {
    if (packed_) {
      return tuples_.size() - 1;
    } else {
      for (tuple_id tid = tuples_.size() -1; tid >= 0; --tid) {
        if (!tuples_.elementIsNull(tid)) {
          return tid;
        }
      }
      return -1;
    }
  }

  bool hasTupleWithID(const tuple_id tuple) const override {
    if ((tuple >= 0) && (static_cast<PtrVector<Tuple, true>::size_type>(tuple) < tuples_.size())) {
      return !tuples_.elementIsNull(tuple);
    } else {
      return false;
    }
  }

  /**
   * @brief Add a Tuple to this MockTupleStorageSubBlock, taking ownership of
   *        the Tuple in the process. Use this instead of insertTuple() when
   *        testing.
   *
   * @param tuple A tuple which this MockTupleStorageSubBlock takes ownership
   *        of.
   * @return tuple's ID in this MockTupleStorageSubBlock.
   **/
  tuple_id addTupleMock(Tuple *tuple) {
    paranoidInsertTypeCheck(*tuple);
    tuples_.push_back(tuple);
    return tuples_.size() - 1;
  }

  InsertResult insertTuple(const Tuple &tuple) override {
    FATAL_ERROR("insertTuple() is unimplemented in MockTupleStorageSubBlock.");
  }

  bool insertTupleInBatch(const Tuple &tuple) override {
    FATAL_ERROR("insertTupleInBatch() is unimplemented in MockTupleStorageSubBlock.");
  }

  tuple_id bulkInsertTuples(ValueAccessor *accessor) override {
    FATAL_ERROR("bulkInsertTuples() is unimplemented in MockTupleStorageSubBlock.");
  }

  tuple_id bulkInsertTuplesWithRemappedAttributes(
      const std::vector<attribute_id> &attribute_map,
      ValueAccessor *accessor) override {
    FATAL_ERROR("bulkInsertTuplesWithRemappedAttributes() is unimplemented in MockTupleStorageSubBlock.");
  }

  const void* getAttributeValue(const tuple_id tuple, const attribute_id attr) const override {
    DEBUG_ASSERT(hasTupleWithID(tuple));
    DEBUG_ASSERT(relation_.hasAttributeWithId(attr));
    if (tuples_.at(tuple).getAttributeValue(attr).isNull()) {
      return NULL;
    } else {
      return tuples_.at(tuple).getAttributeValue(attr).getDataPtr();
    }
  }

  TypedValue getAttributeValueTyped(const tuple_id tuple, const attribute_id attr) const override {
    DEBUG_ASSERT(hasTupleWithID(tuple));
    DEBUG_ASSERT(relation_.hasAttributeWithId(attr));
    return tuples_.at(tuple).getAttributeValue(attr).makeReferenceToThis();
  }

  ValueAccessor* createValueAccessor(const TupleIdSequence *sequence = nullptr) const override {
    FATAL_ERROR("createValueAccessor() is unimplemented in MockTupleStorageSubBlock.");
  }

  bool canSetAttributeValuesInPlaceTyped(
      const tuple_id tuple,
      const std::unordered_map<attribute_id, TypedValue> &new_values) const override {
    return false;
  }

  void setAttributeValueInPlaceTyped(const tuple_id tuple,
                                     const attribute_id attr,
                                     const TypedValue &value) override {
    FATAL_ERROR("setAttributeValueInPlaceTyped() is unimplemented in MockTupleStorageSubBlock.");
  }

  bool deleteTuple(const tuple_id tuple) override {
    DEBUG_ASSERT(hasTupleWithID(tuple));
    tuples_.deleteElement(tuple);
    packed_ = false;
    return false;
  }

  bool bulkDeleteTuples(TupleIdSequence *tuples) override {
    for (TupleIdSequence::const_iterator it = tuples->begin();
         it != tuples->end();
         ++it) {
      deleteTuple(*it);
    }
    return false;
  }

  predicate_cost_t estimatePredicateEvaluationCost(
      const ComparisonPredicate &predicate) const override {
    return predicate_cost::kRowScan;
  }

  void rebuild() override {
  }

  bool isInsertOrderPreserving() const override {
    return false;
  }

 private:
  PtrVector<Tuple, true> tuples_;
  bool packed_;

  DISALLOW_COPY_AND_ASSIGN(MockTupleStorageSubBlock);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_TESTS_MOCK_TUPLE_STORAGE_SUB_BLOCK_HPP_
