/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *   University of Wisconsin—Madison.
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
 * @brief A parsed representation of block/tuple sample.
 */
class ParseSample : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   * @param line_number The line number of "SAMPLE" in the SQL statement.
   * @param column_number The column number of "SAMPLE" in the SQL statement.
   * @param is_block_sample The flag indicating whether this is block sample or tuple sample.
   * @param percentage  The percentage of data to sample.
   */
  ParseSample(const int line_number,
              const int column_number,
              const bool is_block_sample,
              NumericParseLiteralValue *percentage)
      : ParseTreeNode(line_number, column_number),
        percentage_(percentage),
        is_block_sample_(is_block_sample) {}

  /**
   * @brief Destructor.
   */
  ~ParseSample() override {}

  /**
   * @brief Get the sample percentage.
   *
   * @return The sample percentage.
   */
  const NumericParseLiteralValue* percentage() const {
    return percentage_.get();
  }

  /**
   * @brief Get the sample type indicating flag.
   *
   * @return True if this is a block sample. False if this is a tuple sample.
   */
  const bool is_block_sample() const {
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
  const bool is_block_sample_;

  DISALLOW_COPY_AND_ASSIGN(ParseSample);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_SAMPLE_HPP_
