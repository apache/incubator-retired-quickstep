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

#ifndef QUICKSTEP_PARSER_PARSE_LITERAL_VALUE_HPP_
#define QUICKSTEP_PARSER_PARSE_LITERAL_VALUE_HPP_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "parser/ParseString.hpp"
#include "parser/ParseTreeNode.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class Type;

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief The parsed representation of a literal data value.
 **/
class ParseLiteralValue : public ParseTreeNode {
 public:
  /**
   * @brief Virtual destructor.
   **/
  ~ParseLiteralValue() override {
  }

  /**
   * @brief Obtain this value as a literal TypedValue in the quickstep type
   *        system.
   *
   * @param type_hint A hint of what Type is expected. This may be used to give
   *        NULL values the correct Type, or to disambiguate between types that
   *        have similar human-readable text formats. type_hint may be NULL, in
   *        which case the most sensible guess for the value's Type is used
   *        (for a NULL literal, the special type NullType is used).
   * @param concretized_type After the call, *concretized_type will point to
   *        the actual Type that the returned TypedValue belongs to.
   * @return The concrete version of this literal value.
   **/
  virtual TypedValue concretize(const Type *type_hint,
                                const Type **concretized_type) const = 0;

  /**
   * @brief Obtain an exact, deep copy of this ParseLiteralValue.
   *
   * @return A copy of this ParseLiteralValue.
   **/
  virtual ParseLiteralValue* clone() const = 0;

  /**
   * @brief Get a human-readable representation of this value.
   *
   * @return The human-readable form of this value.
   **/
  virtual std::string generateName() const = 0;

 protected:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   **/
  ParseLiteralValue(const int line_number, const int column_number)
      : ParseTreeNode(line_number, column_number) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ParseLiteralValue);
};

/**
 * @brief The parsed representation of a NULL literal.
 **/
class NullParseLiteralValue: public ParseLiteralValue {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   **/
  NullParseLiteralValue(const int line_number, const int column_number)
      : ParseLiteralValue(line_number, column_number) {
  }

  std::string getName() const override {
    return "NullLiteral";
  }

  TypedValue concretize(const Type *type_hint,
                        const Type **concretized_type) const override;

  ParseLiteralValue* clone() const override {
    return new NullParseLiteralValue(line_number(), column_number());
  }

  std::string generateName() const override {
    return "NULL";
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(NullParseLiteralValue);
};

/**
 * @brief The parsed representation of an unquoted numeric literal.
 **/
class NumericParseLiteralValue : public ParseLiteralValue {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param numstring The C-string form of the numeric value.
   **/
  NumericParseLiteralValue(const int line_number,
                           const int column_number,
                           const char *numstring);

  /**
   * @brief Destructor.
   **/
  ~NumericParseLiteralValue() override {
  }

  std::string getName() const override {
    return "NumericLiteral";
  }

  /**
   * @brief Prepend a minus sign to this numeric value, negating it.
   * @note This exists to work around a quirk in our Bison SQL grammar, since
   *       a minus sign can be the leading character in a negative number OR
   *       can represent the unary negation operation applied to a numeric
   *       value. Our solution is to have the Lexer scan all numeric literals
   *       as unsigned values, while the parser has a rule that calls this
   *       method when a minus-sign token occurs in front of a numeric literal.
   **/
  void prependMinus();

  /**
   * @brief Determine whether this numeric value looks like a floating-point
   *        value.
   *
   * @return True if the value is "float-like", false otherwise.
   **/
  bool float_like() const {
    return float_like_;
  }

  /**
   * @brief Get this numeric value as a long.
   *
   * @return This numeric value as a long.
   **/
  std::int64_t long_value() const {
    DCHECK(!float_like_)
        << "Attempted to read integer value from float-like "
        << "NumericParseLiteralValue";
    return long_value_;
  }

  /**
   * @note This implementation of concretize() always tries to honor the given
   *       type_hint first. If resolution with the supplied type_hint fails, or
   *       if no type_hint is given, then the following rules apply:
   *         1. If the literal contains a decimal point or exponent, resolve as
   *            DoubleType (unless FloatType is hinted, we always use
   *            DoubleType for more precision).
   *         2. Otherwise, if the literal is in-range for a 32-bit signed
   *            integer, resolve as IntType.
   *         3. Otherwise, resolve as LongType.
   **/
  TypedValue concretize(const Type *type_hint,
                        const Type **concretized_type) const override;

  ParseLiteralValue* clone() const override {
    return new NumericParseLiteralValue(line_number(),
                                        column_number(),
                                        numeric_string_.c_str());
  }

  std::string generateName() const override {
    return numeric_string_;
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
  std::int64_t long_value_;
  bool float_like_;

  std::string numeric_string_;

  DISALLOW_COPY_AND_ASSIGN(NumericParseLiteralValue);
};

/**
 * @brief The parsed representation of a quoted string value. This includes
 *        explicitly typed literals of the form "TYPE 'literal value'".
 **/
class StringParseLiteralValue : public ParseLiteralValue {
 public:
  /**
   * @brief Constructor.
   *
   * @param value The string.
   * @param explicit_type If non-NULL, indicates an explicit Type that appeared
   *        immediately before a quoted string in the SQL syntax. If NULL, then
   *        value appeared by itself as a quoted string without an explicit
   *        Type.
   **/
  explicit StringParseLiteralValue(ParseString *value,
                                   const Type *explicit_type)
      : ParseLiteralValue(value->line_number(), value->column_number()),
        value_(value),
        explicit_type_(explicit_type) {
  }

  ~StringParseLiteralValue() override {
  }

  /**
   * @brief Attempt to parse an interval literal that was specified
   *        in the form "INTERVAL 'value string'" in SQL syntax.
   * @note There are two distinct INTERVAL Types in Quickstep:
   *       DatetimeInterval ("DATETIME INTERVAL" in SQL) and YearMonthInterval
   *       ("YEARMONTH INTERVAL" in SQL). The INTERVAL keyword by itself is
   *       ambiguous and does not name a Type, however the text formats for
   *       literals of the two different interval Types are distinct and
   *       non-overlapping, so if a valid interval literal string follows the
   *       INTERVAL keyword, the Type can be determined unambiguously. This
   *       method does just that, allowing the SQL parser to be more forgiving
   *       when interval literals don't specify the flavor of interval
   *       up-front.
   *
   * @param value A string literal that followed an INTERVAL keyword in SQL
   *        syntax. On successful parse, this becomes owned by the parsed
   *        StringParseLiteralValue. On failure, it is deleted.
   * @param output On successful parse, *output is overwritten to point to a
   *        new heap-allocated StringParseLiteralValue representing the parsed
   *        interval literal.
   * @return true if value was successfully parsed as either of the interval
   *         types, false otherwise.
   **/
  static bool ParseAmbiguousInterval(ParseString *value,
                                     StringParseLiteralValue **output);

  /**
   * @note The rules for what type this literal is resolved as depend on
   *       whether an explicit_type_ from the SQL context exists, and whether
   *       a type_hint is provided. Specifically, they are:
   *         1. If there is an explicit_type_:
   *           a. If there is a type_hint and it is safely coercible from
   *              explicit_type_, do the coercion and resolve as type_hint.
   *           b. Otherwise resolve as explicit_type_.
   *         2. If there is no explicit_type_:
   *           a. If there is a type_hint and it can successfully parse the
   *              string value, resolve as the hinted type.
   *           b. Otherwise, resolve as a non-nullable VarCharType that is
   *              exactly long enough for this string literal.
   **/
  TypedValue concretize(const Type *type_hint,
                        const Type **concretized_type) const override;

  std::string getName() const override {
    return "StringLiteral";
  }

  ParseLiteralValue* clone() const override {
    return new StringParseLiteralValue(value_->clone(), explicit_type_);
  }

  std::string generateName() const override;

  /**
   * @brief Attempt to parse this string literal as an instance of the
   *        explicit Type provided to the constructor. This should ALWAYS be
   *        called by the parser immediately after creating a
   *        StringParseLiteralValue with an explicit Type.
   *
   * @return true if parse was successful, false if the string was not in a
   *         valid format for the specified explicit Type.
   **/
  bool tryExplicitTypeParse();

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<ParseString> value_;
  const Type *explicit_type_;
  TypedValue explicit_parsed_value_;

  DISALLOW_COPY_AND_ASSIGN(StringParseLiteralValue);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_LITERAL_VALUE_HPP_
