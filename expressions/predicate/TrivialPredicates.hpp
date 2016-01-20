/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_EXPRESSIONS_PREDICATE_TRIVIAL_PREDICATES_HPP_
#define QUICKSTEP_EXPRESSIONS_PREDICATE_TRIVIAL_PREDICATES_HPP_

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/predicate/Predicate.hpp"
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
 * @brief Base class for trivial predicates which always evaluate to the same value.
 **/
class TrivialPredicate : public Predicate {
 public:
  bool hasStaticResult() const override {
    return true;
  }

 protected:
  TrivialPredicate() {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(TrivialPredicate);
};

/**
 * @brief Predicate which always evaluates to true.
 **/
class TruePredicate : public TrivialPredicate {
 public:
  TruePredicate() {
  }

  serialization::Predicate getProto() const override {
    serialization::Predicate proto;
    proto.set_predicate_type(serialization::Predicate::TRUE);

    return proto;
  }

  Predicate* clone() const override {
    return new TruePredicate();
  }

  PredicateType getPredicateType() const override {
    return kTrue;
  }

  bool matchesForSingleTuple(const ValueAccessor &accessor,
                             const tuple_id tuple) const override {
    return true;
  }

  bool matchesForJoinedTuples(
      const ValueAccessor &left_accessor,
      const relation_id left_relation_id,
      const tuple_id left_tuple_id,
      const ValueAccessor &right_accessor,
      const relation_id right_relation_id,
      const tuple_id right_tuple_id) const override {
    return true;
  }

  TupleIdSequence* getAllMatches(ValueAccessor *accessor,
                                 const SubBlocksReference *sub_blocks_ref,
                                 const TupleIdSequence *filter,
                                 const TupleIdSequence *existence_map) const override {
    return GenerateSequenceForStaticResult(accessor, filter, existence_map, true);
  }

  bool getStaticResult() const override {
    return true;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(TruePredicate);
};

/**
 * @brief Predicate which always evaluates to false.
 **/
class FalsePredicate : public TrivialPredicate {
 public:
  FalsePredicate() {
  }

  serialization::Predicate getProto() const override {
    serialization::Predicate proto;
    proto.set_predicate_type(serialization::Predicate::FALSE);

    return proto;
  }

  Predicate* clone() const override {
    return new FalsePredicate();
  }

  PredicateType getPredicateType() const override {
    return kFalse;
  }

  bool matchesForSingleTuple(const ValueAccessor &accessor,
                             const tuple_id tuple) const override {
    return false;
  }

  bool matchesForJoinedTuples(
      const ValueAccessor &left_accessor,
      const relation_id left_relation_id,
      const tuple_id left_tuple_id,
      const ValueAccessor &right_accessor,
      const relation_id right_relation_id,
      const tuple_id right_tuple_id) const override {
    return false;
  }

  TupleIdSequence* getAllMatches(ValueAccessor *accessor,
                                 const SubBlocksReference *sub_blocks_ref,
                                 const TupleIdSequence *filter,
                                 const TupleIdSequence *existence_map) const override {
    return GenerateSequenceForStaticResult(accessor, filter, existence_map, false);
  }

  bool getStaticResult() const override {
    return false;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(FalsePredicate);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_PREDICATE_TRIVIAL_PREDICATES_HPP_
