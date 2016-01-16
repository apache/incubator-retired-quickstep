/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_PARSER_PARSE_ORDERBY_HPP_
#define QUICKSTEP_PARSER_PARSE_ORDERBY_HPP_

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
 * @brief The parsed representation of an ORDER BY item consisting of
 *        an ordering expression and its ordering direction.
 */
class ParseOrderByItem : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number The line number of the first token of this ORDER BY item
   *                    in the SQL statement.
   * @param column_number The column number of the first token of this ORDER BY
   *                      item in the SQL statement.
   * @param ordering_expression The ordering expression.
   * @param is_ascending The optional ordering direction. The default value is
   *                     true.
   * @param nulls_first True if nulls are sorted before non-nulls.
   *                    The default value is true when is_ascending is false,
   *                    false otherwise.
   */
  ParseOrderByItem(const int line_number,
                   const int column_number,
                   ParseExpression *ordering_expression,
                   const bool *is_ascending,
                   const bool *nulls_first)
      : ParseTreeNode(line_number, column_number),
        ordering_expression_(ordering_expression),
        is_ascending_(is_ascending == nullptr ? true : *is_ascending),
        nulls_first_(nulls_first == nullptr ? !is_ascending_ : *nulls_first) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseOrderByItem() override {}

  std::string getName() const override {
    return "OrderByItem";
  }

  /**
   * @return Ordering expression.
   */
  const ParseExpression* ordering_expression() const {
    return ordering_expression_.get();
  }

  /**
   * @return True if the ordering direction is ascending.
   */
  bool is_ascending() const {
    return is_ascending_;
  }

  /**
   * @return True if nulls are sorted before other values.
   */
  bool nulls_first() const {
    return nulls_first_;
  }

 protected:
  void getFieldStringItems(std::vector<std::string> *inline_field_names,
                           std::vector<std::string> *inline_field_values,
                           std::vector<std::string> *non_container_child_field_names,
                           std::vector<const ParseTreeNode*> *non_container_child_fields,
                           std::vector<std::string> *container_child_field_names,
                           std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<ParseExpression> ordering_expression_;
  bool is_ascending_;
  bool nulls_first_;

  DISALLOW_COPY_AND_ASSIGN(ParseOrderByItem);
};

/**
 * @brief The parsed representation of ORDER BY.
 */
class ParseOrderBy : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number The line number of "ORDER" in the SQL statement.
   * @param column_number The column number of "ORDER" in the SQL statement.
   * @param order_by_items The ORDER BY expression and the ordering direction.
   */
  ParseOrderBy(const int line_number, const int column_number, PtrList<ParseOrderByItem> *order_by_items)
      : ParseTreeNode(line_number, column_number), order_by_items_(order_by_items) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseOrderBy() override {}

  /**
   * @brief Gets the ORDER BY expressions.
   *
   * @return ORDER BY expressions.
   */
  const PtrList<ParseOrderByItem>* order_by_items() const {
    return order_by_items_.get();
  }

  std::string getName() const override {
    return "OrderBy";
  }

 protected:
  void getFieldStringItems(std::vector<std::string> *inline_field_names,
                           std::vector<std::string> *inline_field_values,
                           std::vector<std::string> *non_container_child_field_names,
                           std::vector<const ParseTreeNode*> *non_container_child_fields,
                           std::vector<std::string> *container_child_field_names,
                           std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<PtrList<ParseOrderByItem>> order_by_items_;

  DISALLOW_COPY_AND_ASSIGN(ParseOrderBy);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_PARSER_PARSEORDERBY_HPP_ */
