/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_PARSER_PARSE_SAMPLE_HPP_
#define QUICKSTEP_PARSER_PARSE_SAMPLE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseLiteralValue.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief A parsed representation of BLOCK SAMPLE.
 */
class ParseSample : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number The line number of " SAMPLE" in the SQL statement.
   * @param column_number The column number of " SAMPLE" in the SQL statement.
   * @param limit_expression The  SAMPLE value expression.
   */
  ParseSample(int line_number, int column_number, bool is_block_sample,NumericParseLiteralValue *percentage)
      : ParseTreeNode(line_number, column_number),
        percentage_(percentage),is_block_sample_(is_block_sample) {
  
 }

  /**
   * @brief Destructor.
   */
  ~ParseSample() override {}

  /**
   * @brief Gets the SAMPLE SAMPLE expression.
   *
   * @return SAMPLE expression
   */
  const NumericParseLiteralValue* getPercentage() const {
    return percentage_.get();
  }
  
  const bool getIsBlockSample() const {
    return is_block_sample_;
  }
  std::string getName() const override {
    return "SAMPLE";
  }
  
 protected:
  void getFieldStringItems(std::vector<std::string> *inline_field_names,
                           std::vector<std::string> *inline_field_values,
                           std::vector<std::string> *non_container_child_field_names,
                           std::vector<const ParseTreeNode*> *non_container_child_fields,
                           std::vector<std::string> *container_child_field_names,
                           std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override;

 private:
  std::unique_ptr<NumericParseLiteralValue> percentage_;
  bool is_block_sample_;

  DISALLOW_COPY_AND_ASSIGN(ParseSample);
};

/** @} */

}  // namespace quickstep

#endif // QUICKSTEP_PARSER_PARSE_SAMPLE_HPP_
