/**
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

#ifndef QUICKSTEP_PARSER_PARSE_PREDICATE_EXISTS_HPP_
#define QUICKSTEP_PARSER_PARSE_PREDICATE_EXISTS_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParsePredicate.hpp"
#include "parser/ParseSubqueryExpression.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ParseTreeNode;

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief The parsed representation of an EXISTS predicate.
 */
class ParsePredicateExists : public ParsePredicate {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number The line number of the EXISTS token in the statement.
   * @param column_number The column number of the EXISTS token in the statement.
   * @param subquery The subquery expression for this EXISTS predicate.
   */
  ParsePredicateExists(const int line_number,
                       const int column_number,
                       ParseSubqueryExpression *subquery)
      : ParsePredicate(line_number, column_number),
        subquery_(subquery) {
  }

  ParsePredicateType getParsePredicateType() const override {
    return kExists;
  }

  std::string getName() const override {
    return "Exists";
  }

  /**
   * @return The subquery expression for this EXISTS predicate.
   */
  const ParseSubqueryExpression* subquery() const {
    return subquery_.get();
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    non_container_child_field_names->push_back("subquery");
    non_container_child_fields->push_back(subquery_.get());
  }

 private:
  std::unique_ptr<ParseSubqueryExpression> subquery_;

  DISALLOW_COPY_AND_ASSIGN(ParsePredicateExists);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_PREDICATE_EXISTS_HPP_
