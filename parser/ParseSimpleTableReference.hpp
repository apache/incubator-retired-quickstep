/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_PARSER_PARSE_SIMPLE_TABLE_REFERENCE_HPP_
#define QUICKSTEP_PARSER_PARSE_SIMPLE_TABLE_REFERENCE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseString.hpp"
#include "parser/ParseTableReference.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ParseTreeNode;

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief A FROM-list table reference by relation name.
 */
class ParseSimpleTableReference : public ParseTableReference {
 public:
  /**
   * @brief Constructor. Takes ownership of \p table_name.
   *
   * @param line_number The line number of the first token of the table reference.
   * @param column_number The column number of the first token of the table reference.
   * @param table_name The table name.
   */
  ParseSimpleTableReference(const int line_number,
                            const int column_number,
                            ParseString *table_name)
      : ParseTableReference(line_number, column_number),
        table_name_(table_name) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseSimpleTableReference() override {}

  TableReferenceType getTableReferenceType() const override {
    return kSimpleTableReference;
  }

  std::string getName() const override { return "TableReference"; }

  /**
   * @return The table name.
   */
  const ParseString* table_name() const { return table_name_.get(); }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<ParseString> table_name_;

  DISALLOW_COPY_AND_ASSIGN(ParseSimpleTableReference);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_PARSER_PARSE_SIMPLE_TABLE_REFERENCE_HPP_ */
