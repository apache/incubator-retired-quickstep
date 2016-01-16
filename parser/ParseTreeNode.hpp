/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
