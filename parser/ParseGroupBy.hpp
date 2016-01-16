/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_PARSER_PARSE_GROUP_BY_HPP_
#define QUICKSTEP_PARSER_PARSE_GROUP_BY_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseExpression.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief The parsed representaiton of a GROUP BY clause.
 */
class ParseGroupBy : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number The line number of "GROUP" in the SQL statement.
   * @param column_number The column number of "GROUP" in the SQL statement.
   * @param group_by_expressions The group by expressions.
   */
  ParseGroupBy(const int line_number, const int column_number, PtrList<ParseExpression> *grouping_expressions)
      : ParseTreeNode(line_number, column_number),
        grouping_expressions_(grouping_expressions) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseGroupBy() override {}

  /**
   * @brief Gets the GROUP BY expressions.
   *
   * @return GROUP BY expressions.
   */
  const PtrList<ParseExpression>* grouping_expressions() const {
    return grouping_expressions_.get();
  }

  std::string getName() const override {
    return "GroupBy";
  }

 protected:
  void getFieldStringItems(std::vector<std::string> *inline_field_names,
                           std::vector<std::string> *inline_field_values,
                           std::vector<std::string> *non_container_child_field_names,
                           std::vector<const ParseTreeNode*> *non_container_child_fields,
                           std::vector<std::string> *container_child_field_names,
                           std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<PtrList<ParseExpression>> grouping_expressions_;

  DISALLOW_COPY_AND_ASSIGN(ParseGroupBy);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_PARSER_PARSEGROUPBY_HPP_ */
