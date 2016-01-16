/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_PARSER_PARSE_SELECTION_CLAUSE_HPP_
#define QUICKSTEP_PARSER_PARSE_SELECTION_CLAUSE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseExpression.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"

namespace quickstep {

class ParseSelectionItem;

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief The complete selection in a SELECT statement.
 **/
class ParseSelectionClause : public ParseTreeNode {
 public:
  /**
   * @brief The type of a SELECT clause.
   */
  enum SelectionType {
    kStar,     //!< kStar Represents a SELECT clause with * as its only SELECT-list item.
    kNonStar,  //!< kNonStar Represents a SELECT clause with expressions.
  };

  /**
   * @return The type of this SELECT clause.
   */
  virtual SelectionType getSelectionType() const = 0;

 protected:
  ParseSelectionClause(const int line_number, const int column_number)
      : ParseTreeNode(line_number, column_number) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ParseSelectionClause);
};

/**
 * @brief A "star" (*) selection, representing all attributes from all
 *        relations in the FROM list.
 **/
class ParseSelectionStar : public ParseSelectionClause {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number The line number of "*" in the SQL statement string.
   * @param column_number The column number of "*" in the SQL statement string.
   */
  ParseSelectionStar(const int line_number, const int column_number)
      : ParseSelectionClause(line_number, column_number) {
  }

  SelectionType getSelectionType() const override {
    return kStar;
  }

  std::string getName() const override { return "SelectStar"; }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {}

 private:
  DISALLOW_COPY_AND_ASSIGN(ParseSelectionStar);
};

/**
 * @brief A list of comma-seperated ParseSelectionItems.
 **/
class ParseSelectionList : public ParseSelectionClause {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number The line number of the first token of the SELECT list in the SQL statement string.
   * @param column_number The column number of the first token of the SELECT list in the SQL statement string.
   */
  ParseSelectionList(const int line_number, const int column_number)
      : ParseSelectionClause(line_number, column_number) {
  }

  SelectionType getSelectionType() const override {
    return kNonStar;
  }

  std::string getName() const override { return "SelectList"; }

  /**
   * @return The list of SELECT-list items.
   */
  const PtrList<ParseSelectionItem>& select_item_list() const { return select_item_list_; }

  /**
   * @brief Append an item to the selection list.
   *
   * @param item The selection item to add to the end of this selection list,
   *        which becomes owned by this ParseSelectionList.
   **/
  void add(ParseSelectionItem *item) {
    select_item_list_.push_back(item);
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
  PtrList<ParseSelectionItem> select_item_list_;

  DISALLOW_COPY_AND_ASSIGN(ParseSelectionList);
};

/**
 * @brief A SELECT-list item that consists of an expression and optionally an alias.
 */
class ParseSelectionItem : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number The line number of the first token of this item in the SQL statement string.
   * @param column_number The column number of the first token of this item in the SQL statement string.
   * @param expression The SELECT-list expression.
   * @param alias The expression alias name.
   */
  ParseSelectionItem(const int line_number,
                     const int column_number,
                     ParseExpression *expression,
                     ParseString *alias = nullptr)
     : ParseTreeNode(line_number, column_number),
       expression_(expression),
       alias_(alias) {}

  /**
   * @brief Destructor.
   */
  ~ParseSelectionItem() override {}

  std::string getName() const override { return "SelectListItem"; }

  /**
   * @return The SELECT-list expression.
   */
  const ParseExpression* expression() const { return expression_.get(); }

  /**
   * @return The alias of the SELECT-list expression.
   */
  const ParseString* alias() const { return alias_.get(); }

  /**
   * @return Generates a human-readable name for the current expression.
   */
  std::string generateName() const {
    return expression_->generateName();
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
  std::unique_ptr<ParseExpression> expression_;
  std::unique_ptr<ParseString> alias_;

  DISALLOW_COPY_AND_ASSIGN(ParseSelectionItem);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_SELECTION_CLAUSE_HPP_
