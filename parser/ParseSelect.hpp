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

#ifndef QUICKSTEP_PARSER_PARSE_SELECT_HPP_
#define QUICKSTEP_PARSER_PARSE_SELECT_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseGroupBy.hpp"
#include "parser/ParseHaving.hpp"
#include "parser/ParseLimit.hpp"
#include "parser/ParseOrderBy.hpp"
#include "parser/ParsePredicate.hpp"
#include "parser/ParseSelectionClause.hpp"
#include "parser/ParseTableReference.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"

#include "glog/logging.h"

namespace quickstep {

/**
 * @brief The parsed representation of a (sub-query) SELECT statement.
 **/
class ParseSelect : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   * @note Takes ownership of all pointers.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param selection The parsed selection, which becomes owned by this
   *        ParseStatementSelect.
   * @param from_list The parsed list of table references in the FROM clause,
   *        which becomes owned by this ParseStatementSelect.
   * @param where_predicate An optional predicate representing the parsed
   *        WHERE clause (may be NULL if no predicate). Becomes owned by this
   *        ParseStatementSelect if non-NULL.
   * @param group_by Optional parsed GROUP BY clause. Becomes owned by this ParseStatementSelect.
   * @param having_predicate Optional parsed HAVING clause. Becomes owned by this ParseStatementSelect.
   * @param order_by Optional parsed ORDER BY clause. Becomes owned by this ParseStatementSelect.
   * @param limit Optional parsed LIMIT clause. Becomes owned by this ParseStatementSelect.
   **/
  ParseSelect(const int line_number,
              const int column_number,
              ParseSelectionClause *selection,
              PtrList<ParseTableReference> *from_list,
              ParsePredicate *where_predicate,
              ParseGroupBy *group_by,
              ParseHaving *having,
              ParseOrderBy *order_by,
              ParseLimit *limit)
      : ParseTreeNode(line_number, column_number),
        selection_(selection),
        from_list_(from_list),
        where_predicate_(where_predicate),
        group_by_(group_by),
        having_(having),
        order_by_(order_by),
        limit_(limit) {
  }

  ~ParseSelect() override {
  }

  std::string getName() const override {
    return "Select";
  }

  /**
   * @brief Gets the selection.
   *
   * @return The selection.
   **/
  const ParseSelectionClause& selection() const {
    return *selection_;
  }

  /**
   * @brief Gets the FROM list.
   *
   * @return The list of table references in the FROM clause.
   **/
  const PtrList<ParseTableReference>& from_list() const {
    return *from_list_;
  }

  /**
   * @brief Determines whether this select statement has a WHERE predicate.
   *
   * @return Whether there is a WHERE predicate in this statement.
   **/
  bool hasWherePredicate() const {
    return where_predicate_.get() != nullptr;
  }

  /**
   * @brief Gets the WHERE predicate.
   * @warning Always call hasWherePredicate() first.
   *
   * @return The parsed WHERE predicate.
   **/
  const ParsePredicate& where_predicate() const {
    DCHECK(hasWherePredicate());
    return *where_predicate_;
  }

  /**
   * @brief Gets the parsed GROUP BY.
   *
   * @return The parsed GROUP BY.
   */
  const ParseGroupBy* group_by() const { return group_by_.get(); }

  /**
   * @brief Gets the parsed HAVING.
   *
   * @return The parsed HAVNING.
   */
  const ParseHaving* having() const { return having_.get(); }

  /**
   * @brief Gets the parsed ORDER BY.
   *
   * @return The parsed ORDER BY.
   */
  const ParseOrderBy* order_by() const { return order_by_.get(); }

  /**
   * @brief Gets the parsed LIMIT.
   *
   * @return The parsed LIMIT.
   */
  const ParseLimit* limit() const { return limit_.get(); }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    non_container_child_field_names->push_back("select_clause");
    non_container_child_fields->push_back(selection_.get());

    if (where_predicate_ != nullptr) {
      non_container_child_field_names->push_back("where_clause");
      non_container_child_fields->push_back(where_predicate_.get());
    }

    if (from_list_ != nullptr) {
      container_child_field_names->push_back("from_clause");
      container_child_fields->emplace_back();
      for (const ParseTableReference& from_item : *from_list_) {
        container_child_fields->back().push_back(&from_item);
      }
    }

    if (group_by_ != nullptr) {
      non_container_child_field_names->push_back("group_by");
      non_container_child_fields->push_back(group_by_.get());
    }

    if (having_ != nullptr) {
      non_container_child_field_names->push_back("having");
      non_container_child_fields->push_back(having_.get());
    }

    if (order_by_ != nullptr) {
      non_container_child_field_names->push_back("order_by");
      non_container_child_fields->push_back(order_by_.get());
    }

    if (limit_ != nullptr) {
      non_container_child_field_names->push_back("limit");
      non_container_child_fields->push_back(limit_.get());
    }
  }

 private:
  std::unique_ptr<ParseSelectionClause> selection_;
  std::unique_ptr<PtrList<ParseTableReference> > from_list_;
  std::unique_ptr<ParsePredicate> where_predicate_;
  std::unique_ptr<ParseGroupBy> group_by_;
  std::unique_ptr<ParseHaving> having_;
  std::unique_ptr<ParseOrderBy> order_by_;
  std::unique_ptr<ParseLimit> limit_;

  DISALLOW_COPY_AND_ASSIGN(ParseSelect);
};

}  // namespace quickstep

#endif /* QUICKSTEP_PARSER_PARSESELECT_HPP_ */
