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

#ifndef QUICKSTEP_PARSER_PARSE_INDEX_PROPERTIES_HPP_
#define QUICKSTEP_PARSER_PARSE_INDEX_PROPERTIES_HPP_

#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "parser/ParseKeyValue.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseTreeNode.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * TODO (@ssaurabh): Improve Documentation
 */
class IndexProperties {
 public:
  
  virtual bool hasValidIndexDescription() const = 0;
  
  virtual const ParseKeyValue* getInvalidPropertyNode() const = 0;
  
  virtual std::string getReasonForInvalidIndex() const = 0;
  
  virtual const StorageBlockLayoutDescription* getIndexDescription() const = 0;
  
  virtual bool parseCustomProperties(PtrList<ParseKeyValue> *key_value_list) = 0;
  
};

/**
 * TODO (@ssaurabh): Improve Documentation
 */
class DefaultIndexProperties : public IndexProperties {
 public:
  
  bool hasValidIndexDescription() const override {
    return false;
  }
  
  const ParseKeyValue* getInvalidPropertyNode() const override {
    return nullptr;
  }
  
  std::string getReasonForInvalidIndex() const override {
    return "no such index type exists";
  }
  
  const StorageBlockLayoutDescription* getIndexDescription() const override {
    return nullptr;
  }
  
  bool parseCustomProperties(PtrList<ParseKeyValue> *key_value_list) override {
    return true;
  }
};
  
/**
 * TODO (@ssaurabh): Improve Documentation
 */
class BloomFilterIndexProperties : public IndexProperties {
 public:
  
  bool hasValidIndexDescription() const override {
    return false;
  }
  
  const ParseKeyValue* getInvalidPropertyNode() const override {
    return nullptr;
  }
  
  std::string getReasonForInvalidIndex() const override {
    return "";
  }
  
  const StorageBlockLayoutDescription* getIndexDescription() const override {
    return nullptr;
  }
  
  bool parseCustomProperties(PtrList<ParseKeyValue> *key_value_list) override {
    return true;
  }
};
  
/**
 * TODO (@ssaurabh): Improve Documentation
 */
class CSBTreeIndexProperties : public IndexProperties {
public:
  
  bool hasValidIndexDescription() const override {
    return false;
  }
  
  const ParseKeyValue* getInvalidPropertyNode() const override {
    return nullptr;
  }
  
  std::string getReasonForInvalidIndex() const override {
    return "CSB Tree index is not yet implemented";
  }
  
  const StorageBlockLayoutDescription* getIndexDescription() const override {
    return nullptr;
  }
  
  bool parseCustomProperties(PtrList<ParseKeyValue> *key_value_list) override {
    return true;
  }
};
  
  
/**
 * @brief Encapsulates the IndexProperties key-value list. Makes the job
 *        of resolving IndexProperties easy.
 */
class ParseIndexProperties : public ParseTreeNode {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Beginning line number.
   * @param column_number Beginning column number.
   * @param properties PtrList to the KeyValues.
   */
  ParseIndexProperties(const int line_number,
                       const int column_number,
                       IndexProperties *index_properties,
                       PtrList<ParseKeyValue> *key_value_list)
  : ParseTreeNode(line_number, column_number),
    key_value_list_(key_value_list) {
      index_properties->parseCustomProperties(key_value_list);
  }
  
  std::string getName() const override { return "IndexProperties"; }
  
  
  const PtrList<ParseKeyValue>* getKeyValueList() const {
    return key_value_list_.get();
  }
  
 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    for (const ParseKeyValue &index_property : *key_value_list_) {
      non_container_child_field_names->push_back("index_property");
      non_container_child_fields->push_back(&index_property);
    }
  }

 private:
  std::unique_ptr<PtrList<ParseKeyValue>> key_value_list_;

  DISALLOW_COPY_AND_ASSIGN(ParseIndexProperties);
};

/** @} */

}  // namespace quickstep

#endif  /* QUICKSTEP_PARSER_PARSE_INDEX_PROPERTIES_HPP_ */
