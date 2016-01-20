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
