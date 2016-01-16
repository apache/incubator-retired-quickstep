/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_EXPRESSIONS_PREDICATE_PREDICATE_WITH_LIST_HPP_
#define QUICKSTEP_EXPRESSIONS_PREDICATE_PREDICATE_WITH_LIST_HPP_

#include "expressions/predicate/Predicate.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"

namespace quickstep {

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Abstract base class for conjunctions and disjunctions.
 **/
class PredicateWithList : public Predicate {
 public:
  PredicateWithList()
      : has_static_result_(true), static_result_(true) {
  }

  /**
   * @brief Destructor which also deletes all operands.
   **/
  virtual ~PredicateWithList() {
  }

  bool hasStaticResult() const override {
    return has_static_result_;
  }

  bool getStaticResult() const override {
    if (has_static_result_) {
      return static_result_;
    } else {
      FATAL_ERROR("Called getStaticResult() on a predicate which has no static result");
    }
  }

 protected:
  PtrList<Predicate> static_operand_list_;
  PtrList<Predicate> dynamic_operand_list_;

  bool has_static_result_;
  bool static_result_;

 private:
  friend class PredicateTest;

  DISALLOW_COPY_AND_ASSIGN(PredicateWithList);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_PREDICATE_PREDICATE_WITH_LIST_HPP_
