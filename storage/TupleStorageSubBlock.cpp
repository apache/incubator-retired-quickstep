/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "storage/TupleStorageSubBlock.hpp"

#ifdef QUICKSTEP_DEBUG
#include <cassert>
#endif

#include "storage/TupleIdSequence.hpp"
#include "storage/ValueAccessor.hpp"
#include "utility/Macros.hpp"

#ifdef QUICKSTEP_DEBUG
#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#endif

namespace quickstep {

tuple_id TupleStorageSubBlock::numTuples() const {
  if (isEmpty()) {
    return 0;
  } else if (isPacked()) {
    return getMaxTupleID() + 1;
  } else {
    // WARNING: This branch is O(N). Subclasses should override wherever possible.
    tuple_id count = 0;
    for (tuple_id tid = 0; tid <= getMaxTupleID(); ++tid) {
      if (hasTupleWithID(tid)) {
        ++count;
      }
    }
    // Should have at least one tuple, otherwise isEmpty() would have been true.
    DEBUG_ASSERT(count > 0);
    return count;
  }
}

TupleIdSequence* TupleStorageSubBlock::getExistenceMap() const {
  const tuple_id max_tid = getMaxTupleID();
  TupleIdSequence *existing_tuples = new TupleIdSequence(max_tid + 1);

  if (isPacked()) {
    existing_tuples->setRange(0, max_tid + 1, true);
  } else {
    for (tuple_id tid = 0; tid <= max_tid; ++tid) {
      if (hasTupleWithID(tid)) {
        existing_tuples->set(tid, true);
      }
    }
  }

  return existing_tuples;
}

OrderedTupleIdSequence* TupleStorageSubBlock::getExistenceList() const {
  const tuple_id max_tid = getMaxTupleID();
  OrderedTupleIdSequence *existence_list = new OrderedTupleIdSequence();
  existence_list->reserve(numTuples());

  if (isPacked()) {
    for (tuple_id tid = 0; tid <= max_tid; ++tid) {
      existence_list->emplace_back(tid);
    }
  } else {
    for (tuple_id tid = 0; tid <= max_tid; ++tid) {
      if (hasTupleWithID(tid)) {
        existence_list->emplace_back(tid);
      }
    }
  }

  return existence_list;
}

void TupleStorageSubBlock::paranoidInsertTypeCheck(const Tuple &tuple) {
#ifdef QUICKSTEP_DEBUG
  assert(relation_.size() == tuple.size());

  Tuple::const_iterator value_it = tuple.begin();
  CatalogRelationSchema::const_iterator attr_it = relation_.begin();

  while (value_it != tuple.end()) {
    assert(value_it->isPlausibleInstanceOf(attr_it->getType().getSignature()));

    ++value_it;
    ++attr_it;
  }
#endif
}

}  // namespace quickstep
