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

#ifndef QUICKSTEP_PARSER_PARSE_TREE_NODE_HPP_
#define QUICKSTEP_PARSER_PARSE_TREE_NODE_HPP_

#include "utility/Macros.hpp"
#include "utility/TreeStringSerializable.hpp"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief Base class for a parse tree node.
 */
class ParseTreeNode : public TreeStringSerializable<const ParseTreeNode*> {
 public:
  /**
   * @brief Destructor.
   */
  ~ParseTreeNode() override {}

  /**
   * @brief Returns the line number.
   *
   * @return The line number of this node in the SQL statement.
   */
  int line_number() const { return line_number_; }

  /**
   * @brief Returns the column number.
   *
   * @return The column number of this node in the SQL statement.
   */
  int column_number() const { return column_number_; }

 protected:
  ParseTreeNode(const int line_number, const int column_number)
      : line_number_(line_number), column_number_(column_number) {
  }

 private:
  // 0-based line number of the first token of this parse tree node in the
  // query string.
  const int line_number_;

  // 0-based column number of the first token of this parse tree node in the
  // query string.
  const int column_number_;

  DISALLOW_COPY_AND_ASSIGN(ParseTreeNode);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_PARSER_PARSE_TREE_NODE_HPP_ */
