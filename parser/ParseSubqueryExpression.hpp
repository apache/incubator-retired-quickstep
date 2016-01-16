/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_PARSER_PARSE_SUBQUERY_EXPRESSION_HPP_
#define QUICKSTEP_PARSER_PARSE_SUBQUERY_EXPRESSION_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseExpression.hpp"
#include "parser/ParseSelect.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ParseTreeNode;

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief Parsed representation of a subquery expression.
 */
class ParseSubqueryExpression : public ParseExpression {
 public:
  /**
   * @brief Constructor. Takes ownership of \p query.
   *
   * @param line_number The line number of the beginning of the subquery expression.
   * @param column_number The column number of the beginning of the subquery expression.
   * @param query The SELECT subquery.
   */
  ParseSubqueryExpression(const int line_number,
                          const int column_number,
                          ParseSelect *query)
      : ParseExpression(line_number, column_number),
        query_(query) {
  }

  ~ParseSubqueryExpression() override {
  }

  ExpressionType getExpressionType() const override {
    return kSubqueryExpression;
  }

  /**
   * @return The SELECT subquery.
   */
  const ParseSelect* query() const { return query_.get(); }

  std::string getName() const override { return "SubqueryExpression"; }

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
  std::unique_ptr<ParseSelect> query_;
  DISALLOW_COPY_AND_ASSIGN(ParseSubqueryExpression);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_PARSER_PARSE_SUBQUERY_EXPRESSION_HPP_ */
