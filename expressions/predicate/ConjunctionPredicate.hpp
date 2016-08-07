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

#ifndef QUICKSTEP_EXPRESSIONS_PREDICATE_CONJUNCTION_PREDICATE_HPP_
#define QUICKSTEP_EXPRESSIONS_PREDICATE_CONJUNCTION_PREDICATE_HPP_

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/predicate/PredicateWithList.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class TupleIdSequence;
class ValueAccessor;

struct SubBlocksReference;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief A conjunction of other Predicates.
 **/
class ConjunctionPredicate : public PredicateWithList {
 public:
  /**
   * @brief Constructor.
   **/
  ConjunctionPredicate()
      : PredicateWithList() {
  }

  serialization::Predicate getProto() const override;

  Predicate *clone() const override;

  PredicateType getPredicateType() const override {
    return kConjunction;
  }

  bool matchesForSingleTuple(const ValueAccessor &accessor,
                             const tuple_id tuple) const override;

  bool matchesForJoinedTuples(
      const ValueAccessor &left_accessor,
      const relation_id left_relation_id,
      const tuple_id left_tuple_id,
      const ValueAccessor &right_accessor,
      const relation_id right_relation_id,
      const tuple_id right_tuple_id) const override;

  TupleIdSequence* getAllMatches(ValueAccessor *accessor,
                                 const SubBlocksReference *sub_blocks_ref,
                                 const TupleIdSequence *filter,
                                 const TupleIdSequence *existence_map) const override;

  /**
   * @brief Add a predicate to this conjunction.
   * @note If operand is another ConjunctionPredicate, it will be flattened
   *       into this Predicate, rather than nested beneath it.
   *
   * @param operand The predicate to add, becomes owned by this Predicate.
   **/
  void addPredicate(Predicate *operand);

 private:
  void processStaticOperand(const Predicate &operand);
  void processDynamicOperand();

  DISALLOW_COPY_AND_ASSIGN(ConjunctionPredicate);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_PREDICATE_CONJUNCTION_PREDICATE_HPP_
