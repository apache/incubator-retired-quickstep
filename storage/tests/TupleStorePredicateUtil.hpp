/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_STORAGE_TESTS_TUPLE_STORE_PREDICATE_UTIL_HPP_
#define QUICKSTEP_STORAGE_TESTS_TUPLE_STORE_PREDICATE_UTIL_HPP_

#include <memory>
#include <vector>

#include "expressions/predicate/Predicate.hpp"
#include "storage/IndexSubBlock.hpp"
#include "storage/SubBlocksReference.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

namespace quickstep {

class TupleStorageSubBlock;

class TupleStorePredicateUtil {
 public:
  static TupleIdSequence* GetMatchesForPredicateOnTupleStore(
      const Predicate &pred,
      const TupleStorageSubBlock &tuple_store) {
    PtrVector<IndexSubBlock> indices_dummy;
    std::vector<bool> indices_consistent_dummy;
    SubBlocksReference sub_blocks_ref(tuple_store,
                                      indices_dummy,
                                      indices_consistent_dummy);

    std::unique_ptr<ValueAccessor> value_accessor(tuple_store.createValueAccessor());

    std::unique_ptr<TupleIdSequence> existence_map;
    if (!tuple_store.isPacked()) {
      existence_map.reset(tuple_store.getExistenceMap());
    }

    return pred.getAllMatches(value_accessor.get(),
                              &sub_blocks_ref,
                              nullptr,
                              existence_map.get());
  }

 private:
  TupleStorePredicateUtil();

  DISALLOW_COPY_AND_ASSIGN(TupleStorePredicateUtil);
};

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_TESTS_TUPLE_STORE_PREDICATE_UTIL_HPP_
