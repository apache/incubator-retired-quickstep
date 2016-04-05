/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#ifndef QUICKSTEP_PARSER_PARSE_BASIC_EXPRESSIONS_HPP_
#define QUICKSTEP_PARSER_PARSE_BASIC_EXPRESSIONS_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseExpression.hpp"
#include "parser/ParseLiteralValue.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"

namespace quickstep {

class BinaryOperation;
class UnaryOperation;

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief The parsed representation of a literal value.
 **/
class ParseScalarLiteral : public ParseExpression {
 public:
  /**
   * @brief Constructor.
   *
   * @param literal_value The parsed literal value to represent, which becomes
   *        owned by this ParseScalarLiteral.
   **/
  explicit ParseScalarLiteral(ParseLiteralValue *literal_value)
      : ParseExpression(literal_value->line_number(),
                        literal_value->column_number()),
                        literal_value_(literal_value) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseScalarLiteral() override {
  }

  /**
   * @brief Gets the parsed literal value.
   *
   * @return The parsed literal value.
   */
  const ParseLiteralValue* literal_value() const {
    return literal_value_.get();
  }

  ExpressionType getExpressionType() const override {
    return kScalarLiteral;
  }

  std::string getName() const override {
    return "Literal";
  }

  std::string generateName() const override {
    return literal_value_->generateName();
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
  std::unique_ptr<ParseLiteralValue> literal_value_;

  DISALLOW_COPY_AND_ASSIGN(ParseScalarLiteral);
};


/**
 * @brief The parsed representation of an attribute reference.
 **/
class ParseAttribute : public ParseExpression {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param attr_name The name of the attribute.
   * @param rel_name An optional relation name to qualify the attribute name
   *                 (by default, all relations in the FROM list will be searched for
   *                 the attribute).
   **/
  ParseAttribute(const int line_number,
                 const int column_number,
                 ParseString *attr_name,
                 ParseString *rel_name = nullptr)
      : ParseExpression(line_number, column_number),
        attr_name_(attr_name),
        rel_name_(rel_name) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseAttribute() override {}

  ExpressionType getExpressionType() const override {
    return kAttribute;
  }

  std::string getName() const override {
    return "AttributeReference";
  }

  /**
   * @return Attribute name.
   */
  const ParseString* attr_name() const {
    return attr_name_.get();
  }

  /**
   * @return Relation name.
   */
  const ParseString* rel_name() const {
    return rel_name_.get();
  }

  std::string generateName() const override;

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<ParseString> attr_name_, rel_name_;

  DISALLOW_COPY_AND_ASSIGN(ParseAttribute);
};


/**
 * @brief The parsed representation of an unary operation applied to an expression.
 **/
class ParseUnaryExpression : public ParseExpression {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param op The UnaryOperation from the quickstep type system to apply.
   * @param operand The parsed scalar representation of the unary operation's
   *        argument, which becomes owned by this ParseScalarUnaryExpression.
   **/
  ParseUnaryExpression(const int line_number,
                       const int column_number,
                       const UnaryOperation &op,
                       ParseExpression *operand)
      : ParseExpression(line_number, column_number),
        op_(op),
        operand_(operand) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseUnaryExpression() override {
  }

  ExpressionType getExpressionType() const override {
    return kUnaryExpression;
  }

  std::string getName() const override;

  /**
   * @return The unary operation.
   */
  const UnaryOperation& op() const {
    return op_;
  }

  /**
   * @return The operand expression.
   */
  const ParseExpression* operand() const {
    return operand_.get();
  }

  std::string generateName() const override;

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  const UnaryOperation &op_;
  std::unique_ptr<ParseExpression> operand_;

  DISALLOW_COPY_AND_ASSIGN(ParseUnaryExpression);
};

/**
 * @brief The parsed representation of a binary operation applied to two
 *        expressions.
 **/
class ParseBinaryExpression : public ParseExpression {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the binary operator token in the SQL statement.
   * @param column_number Column number of the binary operator token in the SQL statement.
   * @param op The BinaryOperation from the quickstep type system to apply.
   * @param left_operand The parsed scalar representation of the binary
   *        operation's left argument, which becomes owned by this
   *        ParseScalarBinaryExpression.
   * @param right_operand The parsed scalar representation of the binary
   *        operation's right argument, which becomes owned by this
   *        ParseScalarBinaryExpression.
   **/
  ParseBinaryExpression(const int line_number,
                        const int column_number,
                        const BinaryOperation &op,
                        ParseExpression *left_operand,
                        ParseExpression *right_operand)
      : ParseExpression(line_number, column_number),
        op_(op),
        left_operand_(left_operand),
        right_operand_(right_operand) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseBinaryExpression() override {
  }

  ExpressionType getExpressionType() const override {
    return kBinaryExpression;
  }

  std::string getName() const override;

  /**
   * @return The binary operation.
   */
  const BinaryOperation& op() const {
    return op_;
  }

  /**
   * @return The left operand expression.
   */
  const ParseExpression* left_operand() const {
    return left_operand_.get();
  }

  /**
   * @return The right operand expression.
   */
  const ParseExpression* right_operand() const {
    return right_operand_.get();
  }

  std::string generateName() const override;

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  const BinaryOperation &op_;
  std::unique_ptr<ParseExpression> left_operand_;
  std::unique_ptr<ParseExpression> right_operand_;

  DISALLOW_COPY_AND_ASSIGN(ParseBinaryExpression);
};

/**
 * @brief The parsed representation of '*' as a function argument.
 */
class ParseStar : public ParseTreeNode {
 public:
  ParseStar(const int line_number, const int column_number)
      : ParseTreeNode(line_number, column_number) {}

  std::string getName() const override {
    return "Star";
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {}

 private:
  DISALLOW_COPY_AND_ASSIGN(ParseStar);
};

/**
 * @brief Parsed function call in the form of a name with a list of arguments in parentheses.
 */
class ParseFunctionCall : public ParseExpression {
 public:
  /**
   * @brief Constructor.
   * @note Takes ownership of all pointers.
   *
   * @param line_number The line number of the first token of the function call.
   * @param column_number The column number of the first token of the first call.
   * @param is_distinct Whether this function call contains the DISTINCT keyword.
   * @param name The function name.
   * @param arguments The function argument list.
   */
  ParseFunctionCall(const int line_number,
                    const int column_number,
                    const bool is_distinct,
                    ParseString *name,
                    PtrList<ParseExpression> *arguments)
      : ParseExpression(line_number, column_number),
        is_distinct_(is_distinct),
        name_(name),
        arguments_(arguments) {
  }

  /**
   * @brief Constructor for a function call that has "*" as the argument.
   * @note Takes ownership of all pointers.
   *
   * @param line_number The line number of the first token of the function call.
   * @param column_number The column number of the first token of the function call.
   * @param name The function name.
   * @param star The parsed star.
   */
  ParseFunctionCall(const int line_number, const int column_number, ParseString *name, ParseStar *star)
      : ParseExpression(line_number, column_number),
        is_distinct_(false),
        name_(name),
        star_(star) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseFunctionCall() override {
  }

  ExpressionType getExpressionType() const override {
    return kFunctionCall;
  }

  std::string getName() const override {
    return "FunctionCall";
  }

  /**
   * @return Whether this function call contains the DISTINCT keyword.
   */
  bool is_distinct() const {
    return is_distinct_;
  }

  /**
   * @return The function name.
   */
  const ParseString* name() const {
    return name_.get();
  }

  /**
   * @return The argument list.
   */
  const PtrList<ParseExpression>* arguments() const {
    return arguments_.get();
  }

  /**
   * @return The parsed star.
   */
  const ParseStar* star() const {
    return star_.get();
  }

  std::string generateName() const override;

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  const bool is_distinct_;
  std::unique_ptr<ParseString> name_;
  // Either <arguments_> or <star_> is NULL.
  std::unique_ptr<PtrList<ParseExpression>> arguments_;
  std::unique_ptr<ParseStar> star_;

  DISALLOW_COPY_AND_ASSIGN(ParseFunctionCall);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_BASIC_EXPRESSIONS_HPP_
