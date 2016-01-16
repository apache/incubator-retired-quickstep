/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_PARSER_PARSE_HAVING_HPP_
#define QUICKSTEP_PARSER_PARSE_HAVING_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParsePredicate.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief The parsed representation of HAVING.
 */
class ParseHaving : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number The line number of "HAVING" in the SQL statement.
   * @param column_number The column number of "HAVING" in the SQL statement.
   * @param having_predicate The HAVING predicate.
   */
  ParseHaving(const int line_number, const int column_number, ParsePredicate *having_predicate)
      : ParseTreeNode(line_number, column_number), having_predicate_(having_predicate) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseHaving() override {}

  /**
   * @brief Gets the HAVING predicate.
   *
   * @return HAVING predicate
   */
  const ParsePredicate *having_predicate() const {
    return having_predicate_.get();
  }

  std::string getName() const override {
    return "HAVING";
  }

 protected:
  void getFieldStringItems(std::vector<std::string> *inline_field_names,
                           std::vector<std::string> *inline_field_values,
                           std::vector<std::string> *non_container_child_field_names,
                           std::vector<const ParseTreeNode*> *non_container_child_fields,
                           std::vector<std::string> *container_child_field_names,
                           std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<ParsePredicate> having_predicate_;

  DISALLOW_COPY_AND_ASSIGN(ParseHaving);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_PARSER_PARSEHAVING_HPP_ */
