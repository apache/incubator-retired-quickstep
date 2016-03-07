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
 * @brief Base class for different types of indices and their associated properties
 */
class IndexProperties {
 public:
  typedef std::unordered_map<std::string, ParseKeyValue::KeyValueType> valid_property_map_type;

  /**
   * @brief Constructor.
   *
   * @param is_index_description_valid Boolean flag that indicates whether
   *                                   an index is valid or not.
   * @param reason_for_invalid_description Printable reason for invalid index.
   * @param invalid_property_node Parse node that may be caused the index
   *                              to become valid.
   **/
  IndexProperties(const bool is_index_description_valid,
                  const std::string &reason_for_invalid_description,
                  const ParseKeyValue* invalid_property_node = nullptr)
      : is_index_description_valid_(is_index_description_valid),
        reason_for_invalid_description_(reason_for_invalid_description),
        invalid_property_node_(invalid_property_node) {
  }

  /**
   * @brief Checks whether this instance describes a valid index or not.
   *
   * @return True if index description is valid, false otherwise.
   **/
  bool isIndexDescriptionValid() const {
    return is_index_description_valid_;
  }

  /**
   * @brief Returns the parse node corresponding for invalid index instance, if any.
   *
   * @return The parse node.
   **/
  const ParseKeyValue* getInvalidPropertyNode() const {
    return invalid_property_node_;
  }

  /**
   * @brief Returns printable string that specifies reason for invalid index instance.
   *
   * @return The string describing the reason for invalid index
   **/
  const std::string& getReasonForInvalidIndexDescription() const {
    return reason_for_invalid_description_;
  }

  /**
   * @brief Returns a protobuf object representing the index description.
   *
   * @return An IndexSubBlockDescription object.
   **/
  const IndexSubBlockDescription* getIndexDescription() const {
    return index_sub_block_description_.get();
  }

  /**
   * @brief Marks the index description as invalid, given a reason.
   *
   * @param reason A printable string that describes the reason for invalidity.
   * @param invalid_node A parse node that may have caused this index to become invalid.
   **/
  void setIndexDescriptionAsInvalid(const std::string &reason, const ParseKeyValue *invalid_node = nullptr) {
    is_index_description_valid_ = false;
    index_sub_block_description_.reset();
    reason_for_invalid_description_ = reason;
    invalid_property_node_ = invalid_node;
  }

  /**
   * @brief Modifies the index description given a set of custom properties
   *        specified as a key-value list of properties.
   *
   * @param key_value_list Key Value list of custom properties.
   * @return Returns true if the given set of custom properties was valid
   *         and was applied correctly, otherwise returns false.
   **/
  virtual bool addCustomProperties(const PtrList<ParseKeyValue> *key_value_list) = 0;

  /**
   * @brief A helper function that checks whether the given list of custom properties
   *        contains duplicate keys or not.
   *
   * @param key_value_list Key Value list of custom properties.
   * @return Returns true if the given set of custom properties contains duplicate
   *         keys, otherwise returns false.
   **/
  bool hasDuplicateCustomProperty(const PtrList<ParseKeyValue> *key_value_list) {
    // Define an existence map, which checks for duplicate properties in the key_value_list.
    std::unordered_map<std::string, bool> is_duplicate_property;
    for (const ParseKeyValue &key_value : *key_value_list) {
      const std::string key = ToLower(key_value.key()->value());
      if (is_duplicate_property.find(key) != is_duplicate_property.end()) {
        setIndexDescriptionAsInvalid(key + " is specified more than once", &key_value);
        return true;  // duplicate found, return true.
      } else {
        is_duplicate_property[key] = true;
      }
    }
    return false;  // no duplicates, return false.
  }

  /**
   * @brief A helper function that checks whether the given list of custom properties
   *        defines properties which are valid for this index or not.
   *
   * @param key_value_list Key Value list of custom properties.
   * @return Returns true if the given set of custom properties contains properties
   *         not applicable to a given type of index.
   **/
  bool hasInvalidCustomProperty(const PtrList<ParseKeyValue> *key_value_list) {
    for (const ParseKeyValue &key_value : *key_value_list) {
      const std::string key = ToLower(key_value.key()->value());
      if (valid_property_map_.find(key) == valid_property_map_.end()) {
        setIndexDescriptionAsInvalid(key + " is an invalid property for this index", &key_value);
        return true;  // this is not one of the valid keys, return true.
      } else {
        // Validate the type.
        if (key_value.getKeyValueType() != valid_property_map_[key]) {
          setIndexDescriptionAsInvalid(key + " property has an invalid type for this index", &key_value);
          return true;  // this key has an unexpected type, return true.
        }
      }
    }
    return false;  // all valid keys, return false.
  }

 protected:
  valid_property_map_type valid_property_map_;  // a map of index property constants and their associated type.
  bool is_index_description_valid_;
  std::string reason_for_invalid_description_;
  const ParseKeyValue *invalid_property_node_;  // referred object owned by ParseIndexProperties class.
  std::unique_ptr<IndexSubBlockDescription> index_sub_block_description_;
};

/**
 * @brief Implementation of index properties for Bloom Filter Index
 */
class BloomFilterIndexProperties : public IndexProperties {
 public:
  // Index properties associated with this index.

  static const std::string kBloomFilterSizeInBytes;    // is of type integer
  static const std::string kBloomFilterNumHashes;      // is of type integer
  static const std::string kBloomFilterProjectElementCount;  // is of type integer
  /**
   * @brief Constructor.
   **/
  BloomFilterIndexProperties()
      : IndexProperties(false, "Bloom Filter index is not yet implemented", nullptr) {
    // Initialize the valid_property_map_ for this index with appropriate type for each property.
    valid_property_map_[kBloomFilterSizeInBytes] = ParseKeyValue::KeyValueType::kStringInteger;
    valid_property_map_[kBloomFilterNumHashes] = ParseKeyValue::KeyValueType::kStringInteger;
    valid_property_map_[kBloomFilterProjectElementCount] = ParseKeyValue::KeyValueType::kStringInteger;

    index_sub_block_description_.reset(new IndexSubBlockDescription());
    index_sub_block_description_->set_sub_block_type(IndexSubBlockDescription::BLOOM_FILTER);
  }

  bool addCustomProperties(const PtrList<ParseKeyValue> *key_value_list) override {
    // Verify for duplicates and invalid types, if any.
    if (hasDuplicateCustomProperty(key_value_list) || hasInvalidCustomProperty(key_value_list)) {
      return false;
    }

    for (const ParseKeyValue &key_value : *key_value_list) {
      const std::string key = ToLower(key_value.key()->value());
      if (key.compare(kBloomFilterSizeInBytes) == 0) {
        const ParseKeyIntegerValue *key_integer_value = static_cast<const ParseKeyIntegerValue*>(&key_value);
        if (key_integer_value->value()->float_like()) {
          setIndexDescriptionAsInvalid("size cannot be specified as a float", &key_value);
          return false;
        }
        const std::int64_t bloom_filter_size = key_integer_value->value()->long_value();
        if (bloom_filter_size < 0) {
          setIndexDescriptionAsInvalid("size cannot be negative", &key_value);
          return false;
        }
        // All good! allow the default bloom filter size to be modified.
        index_sub_block_description_->SetExtension(BloomFilterIndexSubBlockDescription::bloom_filter_size,
                                                   bloom_filter_size);
      } else if (key.compare(kBloomFilterNumHashes) == 0) {
        const ParseKeyIntegerValue *key_integer_value = static_cast<const ParseKeyIntegerValue*>(&key_value);
        if (key_integer_value->value()->float_like()) {
          setIndexDescriptionAsInvalid("num_hashes cannot be specified as a float", &key_value);
          return false;
        }
        const std::int64_t number_of_hashes = key_integer_value->value()->long_value();
        if (number_of_hashes < 0) {
          setIndexDescriptionAsInvalid("num_hashes cannot be negative", &key_value);
          return false;
        }
        // All good! allow the default num_hashes to be modified.
        index_sub_block_description_->SetExtension(BloomFilterIndexSubBlockDescription::number_of_hashes,
                                                   number_of_hashes);
      } else if (key.compare(kBloomFilterProjectElementCount) == 0) {
        const ParseKeyIntegerValue *key_integer_value = static_cast<const ParseKeyIntegerValue*>(&key_value);
        if (key_integer_value->value()->float_like()) {
          setIndexDescriptionAsInvalid("projected_element_count cannot be specified as a float", &key_value);
          return false;
        }
        const std::int64_t projected_element_count = key_integer_value->value()->long_value();
        if (projected_element_count < 0) {
          setIndexDescriptionAsInvalid("projected_element_count cannot be negative", &key_value);
          return false;
        }
        // All good! allow the default projected_element_count to be modified.
        index_sub_block_description_->SetExtension(BloomFilterIndexSubBlockDescription::projected_element_count,
                                                   projected_element_count);
      }
    }
    return true;
  }
};

/**
 * @brief Implementation of index properties for CSB Tree Index.
 */
class CSBTreeIndexProperties : public IndexProperties {
 public:
 /**
  * @brief Constructor.
  **/
  CSBTreeIndexProperties()
      : IndexProperties(true, "", nullptr) {
    index_sub_block_description_.reset(new IndexSubBlockDescription());
    index_sub_block_description_->set_sub_block_type(IndexSubBlockDescription::CSB_TREE);
  }

  bool addCustomProperties(const PtrList<ParseKeyValue> *key_value_list) override {
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

#endif  // QUICKSTEP_PARSER_PARSE_INDEX_PROPERTIES_HPP_
