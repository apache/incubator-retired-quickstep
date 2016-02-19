/**
 *   Copyright 2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_PARSER_PARSE_BLOCK_PROPERTIES_HPP_
#define QUICKSTEP_PARSER_PARSE_BLOCK_PROPERTIES_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseString.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief Parsed BlockPropertyItem which simply a key (property) and the
 *        corresponding value.
 * @details The value may actually be a list of Parse strings. This is to cover
 *          the case where a user specifies a subset of columns to compress
 *          rather than using the keyword 'ALL'.
 *
 * @tparam The type of the Value which the item holds (ParseString for all
 *         cases except for BlockSize which is a ParseNumericLiteral). T must
 *         inheret from ParseTreeNode.
 */
template<class T>
class ParseBlockPropertyItem : public ParseTreeNode {
 public:
  enum class Property {
    kBlockSize,
    kCompress,
    kSort,
    kType
  };

  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param property Describes which type of block property this is. Corresponds
   *                 to a field of the StorageBlockLayoutDescription message.
   * @param value A parse string representing the assignment of the property.
   *              ParseBlockPropertyItem takes ownership of the pointer.
   **/
  ParseBlockPropertyItem(int line_number,
                         int column_number,
                         Property property,
                         T *value)
      : ParseTreeNode(line_number, column_number),
        property_(property),
        compress_all_(false) {
    values_.reset(new PtrList<T>);
    values_->push_back(value);
  }

  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param property Describes which type of block property this is. Corresponds
   *                 to a field of the StorageBlockLayoutDescription message.
   * @param values A list of parse strings representing the assignment of the
   *               property. The ParseBlockPropertyItem takes ownership.
   **/
  ParseBlockPropertyItem(int line_number,
                         int column_number,
                         Property property,
                         PtrList<T> *values)
      : ParseTreeNode(line_number, column_number),
        property_(property),
        values_(values),
        compress_all_(false) { }

  /**
   * @brief Useful in the case where the parser finds the terms, 'COMPRESS ALL'.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @return A caller-managed BlockPropertyItem representing the property of
   *         COMPRESS ALL for compressing all attributes in a table.
   */
  static ParseBlockPropertyItem<ParseString>* GetCompressAllItem(int line_number, int column_number,
                                                                 int line_number_all, int column_number_all) {
    ParseBlockPropertyItem<ParseString> *compress_all =
        new ParseBlockPropertyItem(line_number, column_number, Property::kCompress, new PtrList<ParseString>());
    compress_all->compress_all_ = true;
    compress_all->values_->push_back(new ParseString(line_number_all, column_number_all, "ALL"));
    return compress_all;
  }

  /**
   * @return Name of the parser construct represented by this class.
   */
  std::string getName() const override {
    return std::string("BlockPropertyItem");
  }

  /**
   * @brief The type of property being described.
   *
   * @return The specific block property which this instance is describing.
   */
  Property property() const {
    return property_;
  }

  /**
   * @return A list of T values which the parser has captured to
   *         describe this property. This may be empty in the case of 'COMPRESS ALL'.
   */
  const PtrList<T>& values() const {
    return *(values_.get());
  }

  /**
   * @brief Gets a string version of the property which this pair describes.
   * @details For example, if this the property type of 'kCompress' will return
   *          a string object with the value 'compress'.
   *
   * @return A string description of the property.
   */
  std::string getPropertyString() const {
    switch (property_) {
      case Property::kBlockSize:
        return "blocksize";
      case Property::kCompress:
        return "compress";
      case Property::kSort:
        return "sort";
      case Property::kType:
        return "type";
      default:
        return "unknown";
    }
  }

  /**
   * @brief Returns if the user specified to compress all attributes (COMPRESS
   *  ALL) attributes using a special keyword.
   *
   * @return \ctrue if this is a compression property and the value is 'ALL'.
   */
  bool compressAll() const {
    return property_ == Property::kCompress && compress_all_;
  }

  bool containsParseStrings() const {
    return !(containsParseNumericLiterals());
  }

  bool containsParseNumericLiterals() const {
    return property_ == Property::kBlockSize;
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("property");
    inline_field_values->push_back(getPropertyString());

    container_child_field_names->push_back((values_->size() == 1) ? "value" : "values");
    container_child_fields->emplace_back();
    for (const T& item_value : *values_) {
      container_child_fields->back().push_back(&item_value);
    }
  }

 private:
  Property property_;
  std::unique_ptr<PtrList<T>> values_;
  bool compress_all_;

  DISALLOW_COPY_AND_ASSIGN(ParseBlockPropertyItem);
};

/**
 * @brief Contains user-specified physical properties of a table's blocks as BlockPropertyItems.
 */
class ParseBlockProperties : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   **/
  ParseBlockProperties(int line_number,
                       int column_number,
                       PtrList<ParseBlockPropertyItem>* properties)
                           : ParseTreeNode(line_number, column_number),
                             properties_(properties) { }

  /**
   * @return Name of the parser construct represented by this class.
   */
  std::string getName() const override {
    return "BlockProperties";
  }

  /**
   * @brief Gets the specified BlockPropertyItem.
   *
   * @param property The property, one of kSort, kCompress, or kType.
   * @return Pointer to the property or nullptr if not found.
   */
  const ParseBlockPropertyItem* getPropertyItem(ParseBlockPropertyItem::Property property) const {
    for (const ParseBlockPropertyItem &item : *properties_) {
      if (item.property() == property) {
        return &item;
      }
    }
    return nullptr;
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    for (const ParseBlockPropertyItem &property_item : *properties_) {
      non_container_child_field_names->push_back(property_item.getName());
      non_container_child_fields->push_back(&property_item);
    }
  }

 private:
  std::unique_ptr<PtrList<ParseBlockPropertyItem> > properties_;

  DISALLOW_COPY_AND_ASSIGN(ParseBlockProperties);
};

/** @} */

}  // namespace quickstep

#endif  /* QUICKSTEP_PARSER_PARSE_BLOCK_PROPERTIES_HPP_ */
