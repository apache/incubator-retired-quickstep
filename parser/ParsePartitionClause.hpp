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

#ifndef QUICKSTEP_PARSER_PARSE_PARTITION_CLAUSE_HPP_
#define QUICKSTEP_PARSER_PARSE_PARTITION_CLAUSE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseLiteralValue.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief A parsed representation of partition clause.
 */
class ParsePartitionClause : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number The line number of "PARTITION CLAUSE" in the SQL statement.
   * @param column_number The column number of "PARTITION CLAUSE" in the SQL statement.
   * @param partition_type The type of partitioning to be made - Hash or Range
   * @param attribute_name_list A list of attributes of the relation based on which the partitioning has to be done.
   * @param num_partitions The number of partitions to be created.
   */
  ParsePartitionClause(const int line_number,
                       const int column_number,
                       ParseString *partition_type,
                       PtrList<ParseString> *attribute_name_list,
                       NumericParseLiteralValue *num_partitions)
      : ParseTreeNode(line_number, column_number),
        partition_type_(partition_type),
        attribute_name_list_(attribute_name_list),
        num_partitions_(num_partitions) {
  }

  /**
   * @brief Destructor.
   */
  ~ParsePartitionClause() override {}

  std::string getName() const override {
    return "PartitionClause";
  }

  /**
   * @brief Get the type of the partitioning to be created.
   *
   * @return The type of partitioning.
   **/
  const ParseString* partition_type() const {
    return partition_type_.get();
  }

  /**
   * @brief Get the list of attributes on which partitioning is supposed to be defined.
   *
   * @return The list of attributes on which partitioning is to be based on.
   **/
  const PtrList<ParseString>& attribute_name_list() const {
    return *attribute_name_list_;
  }

  /**
   * @brief Get the number of partitions.
   *
   * @return The number of partitions.
   */
  const NumericParseLiteralValue* num_partitions() const {
    return num_partitions_.get();
  }

 protected:
  void getFieldStringItems(std::vector<std::string> *inline_field_names,
                           std::vector<std::string> *inline_field_values,
                           std::vector<std::string> *non_container_child_field_names,
                           std::vector<const ParseTreeNode*> *non_container_child_fields,
                           std::vector<std::string> *container_child_field_names,
                           std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("partition_type");
    inline_field_values->push_back(partition_type_->value());

    if (attribute_name_list_.get() != nullptr) {
      container_child_field_names->push_back("attribute_name_list");
      container_child_fields->emplace_back();
      for (const ParseString& attribute_name : *attribute_name_list_) {
        container_child_fields->back().push_back(&attribute_name);
      }
    }

    non_container_child_field_names->push_back("Number of Partitions");
    non_container_child_fields->push_back(num_partitions_.get());
  }

 private:
  std::unique_ptr<ParseString> partition_type_;
  std::unique_ptr<PtrList<ParseString>> attribute_name_list_;
  std::unique_ptr<NumericParseLiteralValue> num_partitions_;

  DISALLOW_COPY_AND_ASSIGN(ParsePartitionClause);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_PARTITION_CLAUSE_HPP_
