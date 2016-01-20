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

#ifndef QUICKSTEP_PARSER_PARSE_PREDICATE_HPP_
#define QUICKSTEP_PARSER_PARSE_PREDICATE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseExpression.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"

namespace quickstep {

class Comparison;

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief The parsed representation of a predicate.
 **/
class ParsePredicate : public ParseTreeNode {
 public:
  /**
   * @brief The possible types of ParsePredicates.
   **/
  enum ParsePredicateType {
    kBetween,
    kComparison,
    kNegation,
    kConjunction,
    kDisjunction
  };

  /**
   * @brief Virtual destructor.
   **/
  ~ParsePredicate() override {}

  /**
   * @brief Identify the type of this ParsePredicate.
   *
   * @return The type of this ParsePredicate.
   **/
  virtual ParsePredicateType getParsePredicateType() const = 0;

 protected:
  ParsePredicate(const int line_number, const int column_number)
      : ParseTreeNode(line_number, column_number) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ParsePredicate);
};

/**
 * @brief The parsed representation of a simple comparison predicate.
 **/
class ParsePredicateComparison : public ParsePredicate {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the comparison operator token in the SQL statement.
   * @param column_number Column number of the comparison operator token in the SQL statement.
   * @param op The comparison (from the quickstep type system).
   * @param left_operand The left operand of the comparison, which becomes
   *        owned by this ParsePredicateComparison.
   * @param right_operand The right operand of the comparison, which becomes
   *        owned by this ParsePredicateComparison.
   **/
  ParsePredicateComparison(const int line_number,
                           const int column_number,
                           const Comparison &op,
                           ParseExpression *left_operand,
                           ParseExpression *right_operand)
      : ParsePredicate(line_number, column_number),
        op_(op),
        left_operand_(left_operand),
        right_operand_(right_operand) {
  }

  /**
   * @brief Destructor.
   */
  ~ParsePredicateComparison() override {}

  /**
   * @return The comparison operator.
   */
  const Comparison& op() const {
    return op_;
  }

  /**
   * @return The left operand.
   */
  const ParseExpression* left_operand() const {
    return left_operand_.get();
  }

  /**
   * @return The right operand.
   */
  const ParseExpression* right_operand() const {
    return right_operand_.get();
  }

  std::string getName() const override;

  ParsePredicateType getParsePredicateType() const override {
    return kComparison;
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  const Comparison &op_;
  std::unique_ptr<ParseExpression> left_operand_;
  std::unique_ptr<ParseExpression> right_operand_;

  DISALLOW_COPY_AND_ASSIGN(ParsePredicateComparison);
};

/**
 * @brief The parsed representation of a NOT predicate.
 **/
class ParsePredicateNegation : public ParsePredicate {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param operand The inner predicate to be negated, becomes owned by this
   *        ParsePredicateNegation.
   **/
  ParsePredicateNegation(const int line_number, const int column_number, ParsePredicate *operand)
      : ParsePredicate(line_number, column_number), operand_(operand) {
  }

  /**
   * @brief Destructor.
   */
  ~ParsePredicateNegation() override {}

  /**
   * @return The operand.
   */
  const ParsePredicate* operand() const {
    return operand_.get();
  }

  ParsePredicateType getParsePredicateType() const override {
    return kNegation;
  }

  std::string getName() const override {
    return "Not";
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<ParsePredicate> operand_;

  DISALLOW_COPY_AND_ASSIGN(ParsePredicateNegation);
};

/**
 * @brief The parsed representation of a BETWEEN predicate.
 *        The parser does not convert it to two comparison
 *        predicates to avoid cloning and evaluating \p middle_operand_
 *        twice for complex expressions (subqueries or
 *        aggregations). The query optimizer is responsible for the conversion.
 **/
class ParsePredicateBetween : public ParsePredicate {
 public:
  /**
   * @brief Construction.
   *
   * @param line_number The line number of 'BETWEEN' in the SQL statement.
   * @param column_number The column number of 'BETWEEN' in the SQL statement.
   * @param lower_bound_operand The operand on the left side of BETWEEN.
   * @param check_operand The operand in between BETWEEN and AND.
   * @param upper_bound_operand The operand after AND.
   */
  ParsePredicateBetween(const int line_number,
                        const int column_number,
                        ParseExpression *check_operand,
                        ParseExpression *lower_bound_operand,
                        ParseExpression *upper_bound_operand)
      : ParsePredicate(line_number, column_number),
        check_operand_(check_operand),
        lower_bound_operand_(lower_bound_operand),
        upper_bound_operand_(upper_bound_operand) {
  }

  /**
   * @brief Destructor.
   */
  ~ParsePredicateBetween() override {}

  /**
   * @return The operand to be compared with the lower bound and
   *         the upper bound.
   */
  const ParseExpression* check_operand() const {
    return check_operand_.get();
  }

  /**
   * @return Lower bound operand.
   */
  const ParseExpression* lower_bound_operand() const {
    return lower_bound_operand_.get();
  }

  /**
   * @return Upper bound operand.
   */
  const ParseExpression* upper_bound_operand() const {
    return upper_bound_operand_.get();
  }

  ParsePredicateType getParsePredicateType() const override {
    return kBetween;
  }

  std::string getName() const override {
    return "Between";
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<ParseExpression> check_operand_;
  std::unique_ptr<ParseExpression> lower_bound_operand_;
  std::unique_ptr<ParseExpression> upper_bound_operand_;

  DISALLOW_COPY_AND_ASSIGN(ParsePredicateBetween);
};

/**
 * @brief Abstract base class of conjunction and disjunction predicates.
 **/
class ParsePredicateWithList : public ParsePredicate {
 public:
  ~ParsePredicateWithList() override {}

  const PtrList<ParsePredicate>& operands() const {
    return operands_;
  }

  /**
   * @brief Add a child predicate to this conjunction or disjunction.
   *
   * @param operand The child predicate to add to this one, which becomes
   *        owned by this predicate.
   **/
  void addPredicate(ParsePredicate *operand) {
    operands_.push_back(operand);
  }

 protected:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   **/
  ParsePredicateWithList(const int line_number, const int column_number)
      : ParsePredicate(line_number, column_number) {
  }

  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

  PtrList<ParsePredicate> operands_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ParsePredicateWithList);
};

/**
 * @brief An AND over other predicates.
 **/
class ParsePredicateConjunction : public ParsePredicateWithList {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   **/
  ParsePredicateConjunction(const int line_number, const int column_number)
      : ParsePredicateWithList(line_number, column_number) {
  }

  ParsePredicateType getParsePredicateType() const override {
    return kConjunction;
  }

  std::string getName() const override {
    return "And";
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ParsePredicateConjunction);
};

/**
 * @brief An OR over other predicates.
 **/
class ParsePredicateDisjunction : public ParsePredicateWithList {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   **/
  ParsePredicateDisjunction(const int line_number, const int column_number)
      : ParsePredicateWithList(line_number, column_number) {
  }

  ParsePredicateType getParsePredicateType() const override {
    return kDisjunction;
  }

  std::string getName() const override {
    return "Or";
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ParsePredicateDisjunction);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_PREDICATE_HPP_
