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

#ifndef QUICKSTEP_PARSER_PARSE_JOINED_TABLE_REFERENCE_HPP_
#define QUICKSTEP_PARSER_PARSE_JOINED_TABLE_REFERENCE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParsePredicate.hpp"
#include "parser/ParseTableReference.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ParseTreeNode;

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief Parsed representation of a joined table.
 */
class ParseJoinedTableReference : public ParseTableReference {
 public:
  enum class JoinType {
    kInnerJoin = 0,
    kLeftOuterJoin,
    kRightOuterJoin,
    kFullOuterJoin
  };

  /**
   * @brief Constructor.
   *
   * @param line_number The line number of the token "JOIN" in the SQL statement.
   * @param column_number The column number of the token "JOIN" in the SQL statement.
   * @param join_type The join type.
   * @param left_table The left-hand side table.
   * @param right_table The right-hand side table.
   * @param join_predicate The join predicate.
   */
  ParseJoinedTableReference(const int line_number,
                            const int column_number,
                            const JoinType join_type,
                            ParseTableReference *left_table,
                            ParseTableReference *right_table,
                            ParsePredicate *join_predicate)
    : ParseTableReference(line_number, column_number),
      join_type_(join_type),
      left_table_(left_table),
      right_table_(right_table),
      join_predicate_(join_predicate) {
  }

  TableReferenceType getTableReferenceType() const override {
    return kJoinedTableReference;
  }

  std::string getName() const override { return "JoinedTable"; }

  /**
   * @return The join type.
   */
  JoinType join_type() const {
    return join_type_;
  }

  /**
   * @return The left-side table.
   */
  const ParseTableReference* left_table() const {
    return left_table_.get();
  }

  /**
   * @return The right-side table.
   */
  const ParseTableReference* right_table() const {
    return right_table_.get();
  }

  /**
   * @return The join predicate.
   */
  const ParsePredicate* join_predicate() const {
    return join_predicate_.get();
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
  const JoinType join_type_;
  std::unique_ptr<ParseTableReference> left_table_;
  std::unique_ptr<ParseTableReference> right_table_;
  std::unique_ptr<ParsePredicate> join_predicate_;

  DISALLOW_COPY_AND_ASSIGN(ParseJoinedTableReference);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_PARSER_PARSE_JOINED_TABLE_REFERENCE_HPP_ */
