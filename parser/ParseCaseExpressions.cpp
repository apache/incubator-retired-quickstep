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

#include "parser/ParseCaseExpressions.hpp"

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "parser/ParseExpression.hpp"
#include "parser/ParsePredicate.hpp"
#include "parser/ParseTreeNode.hpp"

namespace quickstep {

void ParseSimpleWhenClause::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  non_container_child_field_names->push_back("condition_operand");
  non_container_child_fields->push_back(condition_operand_.get());

  non_container_child_field_names->push_back("result_expression");
  non_container_child_fields->push_back(result_expression_.get());
}

void ParseSearchedWhenClause::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  non_container_child_field_names->push_back("condition_predicate");
  non_container_child_fields->push_back(condition_predicate_.get());

  non_container_child_field_names->push_back("result_expression");
  non_container_child_fields->push_back(result_expression_.get());
}

std::string ParseSimpleCaseExpression::generateName() const {
  std::ostringstream out;
  out << "CASE " << case_operand_->generateName();
  for (const ParseSimpleWhenClause &when_clause : *when_clauses_) {
    out << " WHEN " << when_clause.condition_operand()->generateName()
        << " THEN " << when_clause.result_expression()->generateName();
  }
  if (else_result_expression_ != nullptr) {
    out << " ELSE " << else_result_expression_->generateName();
  }
  out << " END";
  return out.str();
}

void ParseSimpleCaseExpression::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  non_container_child_field_names->push_back("case_operand");
  non_container_child_fields->push_back(case_operand_.get());

  container_child_field_names->push_back("when_clauses");
  container_child_fields->emplace_back();
  for (const ParseSimpleWhenClause &when_clause : *when_clauses_) {
    container_child_fields->back().push_back(&when_clause);
  }

  if (else_result_expression_ != nullptr) {
    non_container_child_field_names->push_back("else_result_expression");
    non_container_child_fields->push_back(else_result_expression_.get());
  }
}

std::string ParseSearchedCaseExpression::generateName() const {
  std::ostringstream out;
  out << "CASE";
  for (const ParseSearchedWhenClause &when_clause : *when_clauses_) {
    // TODO(jianqiao): implement generateName() for Predicate.
    out << " WHEN " << when_clause.condition_predicate()->getShortString()
        << " THEN " << when_clause.result_expression()->generateName();
  }
  if (else_result_expression_ != nullptr) {
    out << " ELSE " << else_result_expression_->generateName();
  }
  out << " END";
  return out.str();
}

void ParseSearchedCaseExpression::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const ParseTreeNode*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  container_child_field_names->push_back("when_clauses");
  container_child_fields->emplace_back();
  for (const ParseSearchedWhenClause &when_clause : *when_clauses_) {
    container_child_fields->back().push_back(&when_clause);
  }

  if (else_result_expression_ != nullptr) {
    non_container_child_field_names->push_back("else_result_expression");
    non_container_child_fields->push_back(else_result_expression_.get());
  }
}

}  // namespace quickstep
