/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_EXPRESSIONS_PREDICATE_NEGATION_PREDICATE_HPP_
#define QUICKSTEP_EXPRESSIONS_PREDICATE_NEGATION_PREDICATE_HPP_

#include <memory>

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
 * @brief A negation of another Predicate.
 **/
class NegationPredicate : public Predicate {
 public:
  /**
   * @brief Constructor.
   * @note The static factory method NegatePredicate (which is able to simplify
   *       away double-negations) should be used instead of this constructor in
   *       most cases.
   *
   * @param operand The Predicate which will be negated, becomes owned by this
   *        NegationPredicate.
   **/
  explicit NegationPredicate(Predicate *operand):
    operand_(operand) {
    initHelper();
  }

  /**
   * @brief Destructor which also deletes the operand.
   **/
  ~NegationPredicate() override {
  }

  /**
   * @brief Static method to obtain the negated form of a Predicate. This
   *        should be used instead of invoking NegationPredicate's constructor,
   *        as it allows double-negations to be simplified away.
   *
   * @param operand The Predicate to negate. Caller relinquishes ownership of
   *        operand by calling this method.
   * @return The negative of operand.
   **/
  static Predicate* NegatePredicate(Predicate *operand);

  serialization::Predicate getProto() const override;

  Predicate* clone() const override;

  PredicateType getPredicateType() const override {
    return kNegation;
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

  bool hasStaticResult() const override {
    return has_static_result_;
  }

  bool getStaticResult() const override;

 private:
  void initHelper();

  std::unique_ptr<Predicate> operand_;

  bool has_static_result_;
  bool static_result_;

  friend class PredicateTest;

  DISALLOW_COPY_AND_ASSIGN(NegationPredicate);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_PREDICATE_NEGATION_PREDICATE_HPP_
