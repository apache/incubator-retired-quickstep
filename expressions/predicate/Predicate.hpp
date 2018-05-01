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

#ifndef QUICKSTEP_EXPRESSIONS_PREDICATE_PREDICATE_HPP_
#define QUICKSTEP_EXPRESSIONS_PREDICATE_PREDICATE_HPP_

#include <string>
#include <type_traits>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expression.hpp"
#include "expressions/Expressions.pb.h"
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
 * @brief Base class for all predicates.
 **/
class Predicate : public Expression {
 public:
  /**
   * @brief The possible types of predicates.
   **/
  enum PredicateType {
    kTrue = 0,
    kFalse,
    kComparison,
    kNegation,
    kConjunction,
    kDisjunction,
    kNumPredicateTypes  // Not a real PredicateType, exists for counting purposes.
  };

  /**
   * @brief Strings naming the elements in PredicateType.
   * @note String literals are defined out-of-line in Predicate.cpp
   **/
  static const char *kPredicateTypeNames[kNumPredicateTypes];

  ~Predicate() override {
  }

  std::string getName() const override {
    return kPredicateTypeNames[
        static_cast<std::underlying_type<PredicateType>::type>(getPredicateType())];
  }

  /**
   * @brief Serialize this predicate in Protocol Buffer form.
   *
   * @return The Protocol Buffer representation of this predicate object.
   **/
  virtual serialization::Predicate getProto() const = 0;

  /**
   * @brief Make a deep copy of this Predicate.
   *
   * @return A cloned copy of this Predicate.
   **/
  virtual Predicate* clone() const = 0;

  /**
   * @brief Get the type of this particular Predicate instance.
   *
   * @return The type of this Predicate.
   **/
  virtual PredicateType getPredicateType() const = 0;

  /**
   * @brief Check whether this predicate is a comparison of the form
   *        'attribute comp literal' or 'literal comp attribute'.
   **/
  virtual bool isAttributeLiteralComparisonPredicate() const {
    return false;
  }

  /**
   * @brief Determine whether the given tuple in the given TupleStorageSubBlock
   *        matches this predicate.
   * @note This only works for predicates which can be evaluated on a single
   *       table. Use matchesForJoinedTuples() to evaluate join predicates.
   *
   * @param accessor ValueAccessor that will be used to read the tuple to check
   *        this Predicate on.
   * @param tuple The ID of the tuple in tupleStore to check this Predicate on.
   * @return Whether the specified tuple matches this predicate.
   **/
  virtual bool matchesForSingleTuple(const ValueAccessor &accessor,
                                     const tuple_id tuple) const = 0;

  /**
   * @brief Determine whether this predicate matches for a given pair of joined
   *        tuples.
   *
   * @param left_accessor The ValueAccessor that the first of the joined tuples
   *        will be read from (this does NOT necessarily correspond to the left
   *        operand of a binary operation).
   * @param left_tuple_id The ID of the tuple (the absolute position) from
   *        left_accessor to evaluate this Predicate for.
   * @param right_accessor The ValueAccessor that the second of the joined
   *        tuples will be read from (this does NOT necessarily correspond to
   *        the right operand of a binary operation).
   * @param right_tuple_id The ID of the tuple (the absolute position) from
   *        right_accessor to evaluate this Predicate for.
   * @return Whether this predicate is true for the given tuples.
   **/
  // TODO(chasseur): Add vectorized method for getting/filtering matches across
  // a join all at once. In the general case, this might require materializing
  // an expression across the cross product of all tuples in a pair of blocks
  // (which could incur prohibitively high temporary memory costs), although
  // there are plenty of common cases where this can be avoided (e.g. filtering
  // a smallish set of matches from a hash-join by a residual predicate).
  virtual bool matchesForJoinedTuples(
      const ValueAccessor &left_accessor,
      const tuple_id left_tuple_id,
      const ValueAccessor &right_accessor,
      const tuple_id right_tuple_id) const = 0;

  /**
   * @brief Determine whether this predicate is true for all tuples accessible
   *        via a ValueAccessor.
   *
   * @param accessor A ValueAccessor which will be used to read attribute
   *        values.
   * @param sub_blocks_ref If non-NULL, contains references to the
   *        TupleStorageSubBlock that produced accessor and any IndexSubBlocks
   *        in the same StorageBlock. These sub-blocks may be used for
   *        fast-path (non-scan) evaluation of parts of this predicate. May be
   *        NULL, in which case scan-based evaluation is always used.
   * @param filter If non-NULL, then only tuple IDs which are set in the
   *        filter will be checked (all others will be assumed to be false).
   * @param existence_map If non-NULL, this indicates which tuples actually
   *        exist and are accessible via ValueAccessor (this is intended for
   *        use with ValueAccessors that read values from non-packed
   *        TupleStorageSubBlocks).
   * @return The IDs of tuples accessible via accessor that match this
   *         predicate.
   **/
  virtual TupleIdSequence* getAllMatches(ValueAccessor *accessor,
                                         const SubBlocksReference *sub_blocks_ref,
                                         const TupleIdSequence *filter,
                                         const TupleIdSequence *existence_map) const = 0;

  /**
   * @brief Determine whether this predicate's result is static (i.e. whether
   *        it can be evaluated completely independent of any tuples).
   *
   * @return Whether this predicate's result is static.
   **/
  virtual bool hasStaticResult() const {
    return false;
  }

  /**
   * @brief Determine whether this predicate's static result is true or false.
   * @note hasStaticResult() should be called first to check whether this
   *       Predicate actually has a static result.
   *
   * @return The static result of this predicate.
   **/
  virtual bool getStaticResult() const;

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const Expression*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const Expression*>> *container_child_fields) const override;

  Predicate() {
  }

  static TupleIdSequence* GenerateSequenceForStaticResult(ValueAccessor *accessor,
                                                          const TupleIdSequence *filter,
                                                          const TupleIdSequence *existence_map,
                                                          const bool static_result);

 private:
  DISALLOW_COPY_AND_ASSIGN(Predicate);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_PREDICATE_PREDICATE_HPP_
