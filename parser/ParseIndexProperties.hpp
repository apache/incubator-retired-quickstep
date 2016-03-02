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
#include <unordered_map>

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
  typedef std::unordered_map<std::string, ParseKeyValue::KeyValueType> valid_property_map_t;

  bool hasValidIndexDescription() const {
    return is_index_description_valid_;
  }

  const ParseKeyValue* getInvalidPropertyNode() const {
    return invalid_property_node_;
  }

  std::string getReasonForInvalidIndexDescription() const {
    return reason_for_invalid_description_;
  }

  const IndexSubBlockDescription* getIndexDescription() const {
    return index_sub_block_description_.get();
  }

  void setIndexDescriptionAsInvalid(std::string reason) {
    is_index_description_valid_ = false;
    index_sub_block_description_.reset();
    reason_for_invalid_description_ = reason;
    invalid_property_node_ = nullptr;
  }

  void setIndexDescriptionAsInvalid(const ParseKeyValue *invalid_node, std::string reason) {
    is_index_description_valid_ = false;
    index_sub_block_description_.reset();
    reason_for_invalid_description_ = reason;
    invalid_property_node_ = invalid_node;
  }

  virtual bool addCustomProperties(const PtrList<ParseKeyValue> *key_value_list) = 0;

  bool hasDuplicateCustomProperty(const PtrList<ParseKeyValue> *key_value_list) {
    // define an existence map, which checks for duplicate properties in the key_value_list
    std::unordered_map<std::string, bool> is_duplicate_property;
    for (const ParseKeyValue &key_value : *key_value_list) {
      std::string key = ToLower(key_value.key()->value());
      if (is_duplicate_property.find(key) != is_duplicate_property.end()) {
        setIndexDescriptionAsInvalid(&key_value, key + " is specified more than once");
        return true;  // duplicate found, return true
      } else {
        is_duplicate_property[key] = true;
      }
    }
    return false;  // no duplicates, return false
  }

  bool hasInvalidCustomProperty(const PtrList<ParseKeyValue> *key_value_list) {
    for (const ParseKeyValue &key_value : *key_value_list) {
      std::string key = ToLower(key_value.key()->value());
      if (valid_property_map_.find(key) == valid_property_map_.end()) {
        setIndexDescriptionAsInvalid(&key_value, key + " is an invalid property for this index");
        return true;  // this is not one of the valid keys, return true
      } else {
        // validate the type
        if (key_value.getKeyValueType() != valid_property_map_[key]) {
          setIndexDescriptionAsInvalid(&key_value, key + " property has an invalid type for this index");
          return true;  // this key has an unexpected type, return true
        }
      }
    }
    return false;  // all valid keys, return false
  }

 protected:
  valid_property_map_t valid_property_map_;  // a map of index property constants and their associated type
  bool is_index_description_valid_;
  const ParseKeyValue* invalid_property_node_;  // referred object owned by ParseIndexProperties class
  std::string reason_for_invalid_description_;
  std::unique_ptr<IndexSubBlockDescription> index_sub_block_description_;
};

/**
 * TODO (@ssaurabh): Improve Documentation
 */
class BloomFilterIndexProperties : public IndexProperties {
 public:
  // index properties associated with this index
  static const std::string kBloomFilterSizeInBytes;    // is of type integer
  static const std::string kBloomFilterNumHashes;      // is of type integer
  static const std::string kBloomFilterProjectElementCount;  // is of type integer

  BloomFilterIndexProperties() {
    // initialize the valid_property_map_ for this index with appropriate type for each property
    valid_property_map_[kBloomFilterSizeInBytes] = ParseKeyValue::KeyValueType::kStringInteger;
    valid_property_map_[kBloomFilterNumHashes] = ParseKeyValue::KeyValueType::kStringInteger;
    valid_property_map_[kBloomFilterProjectElementCount] = ParseKeyValue::KeyValueType::kStringInteger;

    is_index_description_valid_ = true;
    invalid_property_node_ = nullptr;

    index_sub_block_description_.reset(new IndexSubBlockDescription());
    index_sub_block_description_->set_sub_block_type(IndexSubBlockDescription::BLOOM_FILTER);
  }

  bool addCustomProperties(const PtrList<ParseKeyValue> *key_value_list) override {
    // verify for duplicates and invalid types, if any
    if (hasDuplicateCustomProperty(key_value_list) || hasInvalidCustomProperty(key_value_list)) {
      return false;
    }

    for (const ParseKeyValue &key_value : *key_value_list) {
      std::string key = ToLower(key_value.key()->value());
      if (key.compare(kBloomFilterSizeInBytes) == 0) {
        const ParseKeyIntegerValue *key_integer_value = static_cast<const ParseKeyIntegerValue*>(&key_value);
        if (key_integer_value->value()->float_like()) {
          setIndexDescriptionAsInvalid(&key_value, "size cannot be specified as a float");
          return false;
        }
        const int64_t bloom_filter_size = key_integer_value->value()->long_value();
        if (bloom_filter_size < 0) {
          setIndexDescriptionAsInvalid(&key_value, "size cannot be negative");
          return false;
        }
        // all good! allow the default bloom filter size to be modified
        index_sub_block_description_->SetExtension(BloomFilterIndexSubBlockDescription::bloom_filter_size,
                                                   bloom_filter_size);
      } else if (key.compare(kBloomFilterNumHashes) == 0) {
        const ParseKeyIntegerValue *key_integer_value = static_cast<const ParseKeyIntegerValue*>(&key_value);
        if (key_integer_value->value()->float_like()) {
          setIndexDescriptionAsInvalid(&key_value, "num_hashes cannot be specified as a float");
          return false;
        }
        const int64_t number_of_hashes = key_integer_value->value()->long_value();
        if (number_of_hashes < 0) {
          setIndexDescriptionAsInvalid(&key_value, "num_hashes cannot be negative");
          return false;
        }
        // all good! allow the default num_hashes to be modified
        index_sub_block_description_->SetExtension(BloomFilterIndexSubBlockDescription::number_of_hashes,
                                                   number_of_hashes);
      } else if (key.compare(kBloomFilterProjectElementCount) == 0) {
        const ParseKeyIntegerValue *key_integer_value = static_cast<const ParseKeyIntegerValue*>(&key_value);
        if (key_integer_value->value()->float_like()) {
          setIndexDescriptionAsInvalid(&key_value, "projected_element_count cannot be specified as a float");
          return false;
        }
        const int64_t projected_element_count = key_integer_value->value()->long_value();
        if (projected_element_count < 0) {
          setIndexDescriptionAsInvalid(&key_value, "projected_element_count cannot be negative");
          return false;
        }
        // all good! allow the default projected_element_count to be modified
        index_sub_block_description_->SetExtension(BloomFilterIndexSubBlockDescription::projected_element_count,
                                                   projected_element_count);
      }
    }
    return true;
  }
};

/**
 * TODO (@ssaurabh): Improve Documentation
 */
class CSBTreeIndexProperties : public IndexProperties {
 public:
  CSBTreeIndexProperties() {
    is_index_description_valid_ = false;
    reason_for_invalid_description_ = "CSBTree index is not yet implemented";
    invalid_property_node_ = nullptr;
    index_sub_block_description_.reset(nullptr);
  }

  bool addCustomProperties(const PtrList<ParseKeyValue> *key_value_list) override {
    return false;
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
                       PtrList<ParseKeyValue> *key_value_list)
  : ParseTreeNode(line_number, column_number),
    key_value_list_(key_value_list) {
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
