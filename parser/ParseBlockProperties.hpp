/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "parser/ParseKeyValue.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief Encapsulates the BlockProperties key-value list. Makes the job
 *        of resolving BlockProperties easy.
 */
class ParseBlockProperties : public ParseTreeNode {
  // Valid key names for the BlockProperties.
  static const std::string kKeyBlockSizeMB;
  static const std::string kKeyCompress;
  static const std::string kKeySort;
  static const std::string kKeyType;

 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Beginning line number.
   * @param column_number Beginning column number.
   * @param properties PtrList to the KeyValues.
   */
  ParseBlockProperties(int line_number,
                       int column_number,
                       PtrList<ParseKeyValue> *properties)
      : ParseTreeNode(line_number, column_number),
        properties_(properties) { }

  /**
   * @return The name of this entity.
   */
  std::string getName() const override {
    return "BlockProperties";
  }

  /**
   * @brief Returns the first repeated key contained in the key-value list.
   * @details A repeated key is any 2 key-values with case-insensitive matching
   *          key names.
   *
   * @return A pointer to the first repeated key-value or nullptr if there
   *         are no repetitions.
   */
  const ParseKeyValue* getFirstRepeatedKeyValue() const {
    std::set<std::string> seen_keys;
    for (const ParseKeyValue &key_value : *properties_) {
      std::string lower_key = ToLower(key_value.key()->value());
      std::set<std::string>::iterator itr = seen_keys.find(lower_key);
      if (itr != seen_keys.end()) {
        return &key_value;
      }
      seen_keys.insert(lower_key);
    }
    return nullptr;
  }

  /**
   * @brief Returns the first invalid key contained in the key-value list.
   * @details An invalid key-value is a key-value with a key whose case-insensitive
   *          string does not match one of the valid key names.
   *
   * @return A pointer to the first invalid key-value or nullptr if there none.
   */
  const ParseKeyValue* getFirstInvalidKeyValue() const {
    std::set<std::string> valid_names({kKeyCompress,
        kKeyType, kKeySort, kKeyBlockSizeMB});
    for (const ParseKeyValue &key_value : *properties_) {
      std::string lower_key = ToLower(key_value.key()->value());
      std::set<std::string>::iterator itr = valid_names.find(lower_key);
      if (itr == valid_names.end()) {
        return &key_value;
      }
    }
    return nullptr;
  }

  /**
   * @brief Gets the ParseString value of the type property.
   *
   * @return ParseString value of the type property or nullptr if the type
   *         was incorrect ParseKeyValue type or not specified.
   */
  const ParseString* getType() const {
    const ParseKeyValue *type_key_value = getKeyValueByName(kKeyType);
    if (type_key_value == nullptr) {
      return nullptr;
    }
    if (type_key_value->getKeyValueType() !=
        ParseKeyValue::KeyValueType::kStringString) {
      return nullptr;
    }
    return static_cast<const ParseKeyStringValue*>(type_key_value)->value();
  }

  /**
   * @brief Gets the ParseString value of the sort property.
   *
   * @return ParseString value of the sort property or nullptr if the type
   *         was not a ParseKeyStringValue or was not specified.
   */
  const ParseString* getSort() const {
    const ParseKeyValue *sort_key_value = getKeyValueByName(kKeySort);
    if (sort_key_value == nullptr) {
      return nullptr;
    }
    if (sort_key_value->getKeyValueType() !=
        ParseKeyValue::KeyValueType::kStringString) {
      return nullptr;
    }
    return static_cast<const ParseKeyStringValue*>(sort_key_value)->value();
  }

  /**
   * @brief Get the list of compressed column names.
   * @note Check compressAll() first to see if all columns are to be compressed.
   *
   * @return A list of column names to compress.
   */
  const PtrList<ParseString>* getCompressed() const {
    const ParseKeyValue *compress_key_value = getKeyValueByName(kKeyCompress);
    if (compress_key_value == nullptr) {
      return nullptr;
    }
    if (compress_key_value->getKeyValueType() !=
        ParseKeyValue::KeyValueType::kStringStringList) {
      return nullptr;
    }
    return static_cast<const ParseKeyStringList*>(compress_key_value)->value();
  }

  /**
   * @brief True if the user specified to compress all columns.
   */
  bool compressAll() const {
    const ParseKeyValue *compress_key_value = getKeyValueByName(kKeyCompress);
    if (compress_key_value == nullptr) {
      return false;
    }
    if (compress_key_value->getKeyValueType() !=
        ParseKeyValue::KeyValueType::kStringString) {
      return false;
    }
    // The StringString value from the parser should always be, 'ALL'.
    DCHECK(static_cast<const ParseKeyStringValue*>(compress_key_value)
        ->value()->value().compare("ALL") == 0)
            << "BlockProperties got an invalid COMPRESS value.";
    return true;
  }

  /**
   * @return Pointer to the blocksize property, nullptr if not specified.
   */
  const ParseKeyValue* getBlockSizeMb() const {
    return getKeyValueByName(kKeyBlockSizeMB);
  }

  /**
   * @return the blocksizemb property or -1 if not specified or not an int.
   */
  std::int64_t getBlockSizeMbValue() const {
    const ParseKeyValue *size_key_value = getKeyValueByName(kKeyBlockSizeMB);
    if (size_key_value == nullptr) {
      return -1;
    }
    if (size_key_value->getKeyValueType() !=
        ParseKeyValue::KeyValueType::kStringInteger) {
      return -1;
    }
    std::int64_t numeric_value =
        static_cast<const ParseKeyIntegerValue*>(size_key_value)->value()->long_value();
    return numeric_value;
  }

  /**
   * @return True if the user specified the size property. Note that returning
   *         true does not mean the property was valid.
   */
  bool hasBlockSizeMb() const {
    return getKeyValueByName(kKeyBlockSizeMB) != nullptr;
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    for (const ParseKeyValue &block_property : *properties_) {
      non_container_child_field_names->push_back("block_property");
      non_container_child_fields->push_back(&block_property);
    }
  }

 private:
  const ParseKeyValue* getKeyValueByName(const std::string &name) const {
    // Get the first occurance of this name.
    for (const ParseKeyValue &key_value : *properties_) {
      if (ToLower(key_value.key()->value()).compare(name) == 0) {
        return &key_value;
      }
    }
    return nullptr;
  }

  std::unique_ptr<PtrList<ParseKeyValue> > properties_;

  DISALLOW_COPY_AND_ASSIGN(ParseBlockProperties);
};

/** @} */

}  // namespace quickstep

#endif  /* QUICKSTEP_PARSER_PARSE_BLOCK_PROPERTIES_HPP_ */
