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
