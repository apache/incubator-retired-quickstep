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
  typedef std::unordered_map<const char*, ParseKeyValue::KeyValueType> valid_property_map_type;

  // An enum specifying the reason for an invalid index description, if any.
  enum class InvalidIndexType {
    kNone = 0,
    kUnimplemented,
    kDuplicateKey,
    kInvalidKey,
    kInvalidValue,
    kTypeIsNotString,
    kSizeIsFloat,
    kSizeIsNegative,
    kNumHashesIsFloat,
    kNumHashesIsNegative,
    kProjectedCountIsFloat,
    kProjectedCountIsNegative
  };

  /**
   * @brief Constructor.
   * @note The constructor takes ownership of index_sub_block_description.
   *
   * @param index_sub_block_description A pointer to an IndexSubBlockDescription proto object.
   * @param invalid_index_type An enum describing the validity of index.
   * @param invalid_property_node A pointer to an invalid ParseKeyValue node, if any.
   **/
  IndexProperties(IndexSubBlockDescription *index_sub_block_description,
                  const InvalidIndexType invalid_index_type = InvalidIndexType::kNone,
                  const ParseKeyValue *invalid_property_node = nullptr)
      : index_sub_block_description_(index_sub_block_description),
        invalid_index_type_(invalid_index_type),
        invalid_property_node_(invalid_property_node) {
  }

  /**
   * @brief Virtual Destructor
   */
  virtual ~IndexProperties() {
  }

  /**
   * @brief Returns printable string that specifies reason for invalid index instance.
   *
   * @return The string describing the reason for invalid index
   **/
  virtual std::string getReasonForInvalidIndexDescription() const = 0;

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
   * @brief Checks whether this instance describes a valid index or not.
   *
   * @return True if index description is valid, false otherwise.
   **/
  bool isIndexPropertyValid() const {
    return invalid_index_type_ == InvalidIndexType::kNone;
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
   * @brief Returns a protobuf object representing the index description.
   *
   * @return An IndexSubBlockDescription object.
   **/
  const IndexSubBlockDescription* getIndexDescription() const {
    return index_sub_block_description_.get();
  }

  /**
   * @brief A helper function that checks whether the given list of custom properties
   *        contains duplicate keys or not.
   *
   * @param key_value_list Key Value list of custom properties.
   * @param invalid_node Double pointer to an invalid node that is used to store duplicate node, if any.
   * @return Returns true if the given set of custom properties contains duplicate
   *         keys, otherwise returns false.
   **/
  bool hasDuplicateCustomProperty(const PtrList<ParseKeyValue> *key_value_list, const ParseKeyValue **invalid_node) {
    // Define an existence map, which checks for duplicate properties in the key_value_list.
    std::unordered_map<std::string, bool> is_duplicate_property;
    for (const ParseKeyValue &key_value : *key_value_list) {
      const std::string key = ToLower(key_value.key()->value());
      if (is_duplicate_property.find(key) != is_duplicate_property.end()) {
        *invalid_node = &key_value;
        return true;  // duplicate found, return true.
      } else {
        is_duplicate_property[key] = true;
      }
    }
    return false;  // no duplicates, return false.
  }

 protected:
  std::unique_ptr<IndexSubBlockDescription> index_sub_block_description_;
  InvalidIndexType invalid_index_type_;
  const ParseKeyValue *invalid_property_node_;  // referred object owned by ParseIndexProperties class.

  /**
   * @brief A setter function that be used to invalidate the index.
   *
   * @param invalid_index_type An enum corresponding to the reason, which invalidated the index.
   * @param invalid_property_node Pointer to the node, which may have caused invalidation.
   **/
  void setIndexDescriptionAsInvalid(const InvalidIndexType invalid_index_type,
                                    const ParseKeyValue *invalid_property_node) {
    invalid_index_type_ = invalid_index_type;
    invalid_property_node_ = invalid_property_node;
    index_sub_block_description_.reset();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(IndexProperties);
};

/**
 * @brief Implementation of index properties for a BitWeaving Index (H or V).
 */
class BitWeavingIndexProperties : public IndexProperties {
 public:
  static const char *kBitWeavingType;  // is of type string.

  /**
   * @brief Constructor.
   **/
  BitWeavingIndexProperties()
    : IndexProperties(new IndexSubBlockDescription()) {
    // Default to BITWEAVING_V, custom properties can change this to H.
    index_sub_block_description_->set_sub_block_type(IndexSubBlockDescription::BITWEAVING_V);
  }

  ~BitWeavingIndexProperties() override {
  }

  std::string getReasonForInvalidIndexDescription() const override {
    switch (invalid_index_type_) {
      case InvalidIndexType::kNone:
        return "";
      case InvalidIndexType::kDuplicateKey:  // Fall through.
      case InvalidIndexType::kInvalidKey:
        return "The only valid property for BitWeaving is TYPE.";
      case InvalidIndexType::kTypeIsNotString:  // Fall through.
      case InvalidIndexType::kInvalidValue:
        return "The only valid values for TYPE are V or H.";
      default:
        return "Unknown reason";
    }
  }

  bool addCustomProperties(const PtrList<ParseKeyValue> *key_value_list) override {
    if (key_value_list->size() == 0u) {
      // No properties specified.
      return true;
    } else if (key_value_list->size() == 1u) {
      const ParseKeyValue &key_value = *key_value_list->begin();
      if (key_value.getKeyValueType() != ParseKeyValue::KeyValueType::kStringString) {
        setIndexDescriptionAsInvalid(InvalidIndexType::kTypeIsNotString, &key_value);
        return false;
      }
      const std::string key = ToLower(key_value.key()->value());
      const std::string value = ToLower(
          static_cast<const ParseKeyStringValue&>(key_value).key()->value());
      if (key.compare(kBitWeavingType) == 0) {
        if (value.compare("h") == 0) {
          index_sub_block_description_->set_sub_block_type(IndexSubBlockDescription::BITWEAVING_H);
          return true;
        } else if (value.compare("v") != 0) {
          setIndexDescriptionAsInvalid(InvalidIndexType::kInvalidValue, &key_value);
          return false;
        } else {
          // If V was specified, then we do nothing because it's set to V by default.
          return true;
        }
      } else {
        // Incorrect key specified.
        setIndexDescriptionAsInvalid(InvalidIndexType::kInvalidKey, &key_value);
        return false;
      }
    } else {
      // More than one key. This must be an error.
      invalid_index_type_ = InvalidIndexType::kDuplicateKey;
      return false;
    }
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(BitWeavingIndexProperties);
};

/**
 * @brief Implementation of index properties for Bloom Filter Index
 */
class BloomFilterIndexProperties : public IndexProperties {
 public:
  // Index properties associated with this index.

  static const char *kBloomFilterSizeInBytes;    // is of type integer
  static const char *kBloomFilterNumHashes;      // is of type integer
  static const char *kBloomFilterProjectElementCount;  // is of type integer

  /**
   * @brief Constructor.
   **/
  BloomFilterIndexProperties()
      : IndexProperties(new IndexSubBlockDescription()) {
    index_sub_block_description_->set_sub_block_type(IndexSubBlockDescription::BLOOM_FILTER);

    // Initialize the valid_property_map_ for this index with appropriate type for each property.
    valid_property_map_[kBloomFilterSizeInBytes] = ParseKeyValue::KeyValueType::kStringInteger;
    valid_property_map_[kBloomFilterNumHashes] = ParseKeyValue::KeyValueType::kStringInteger;
    valid_property_map_[kBloomFilterProjectElementCount] = ParseKeyValue::KeyValueType::kStringInteger;
  }

  ~BloomFilterIndexProperties() override {
  }

  std::string getReasonForInvalidIndexDescription() const override {
    switch (invalid_index_type_) {
      case InvalidIndexType::kNone:
        return "";
      case InvalidIndexType::kUnimplemented:
        return "Bloom Filter index is not yet implemented";
      case InvalidIndexType::kDuplicateKey:
        return "Index property is specified more than once";
      case InvalidIndexType::kInvalidKey:
        return "Invalid property for this index";
      case InvalidIndexType::kSizeIsFloat:
        return "Size cannot be specified as a float";
      case InvalidIndexType::kSizeIsNegative:
        return "Size cannot be negative";
      case InvalidIndexType::kNumHashesIsFloat:
        return "Num_hashes cannot be specified as a float";
      case InvalidIndexType::kNumHashesIsNegative:
        return "Num_hashes cannot be negative";
      case InvalidIndexType::kProjectedCountIsFloat:
        return "Projected_element_count cannot be specified as a float";
      case InvalidIndexType::kProjectedCountIsNegative:
        return "Projected_element_count cannot be negative";
      default:
        return "Unknown reason";
    }
  }

  bool addCustomProperties(const PtrList<ParseKeyValue> *key_value_list) override {
    // If the index is unimplemented, skip adding the custom properties.
    if (invalid_index_type_ == InvalidIndexType::kUnimplemented) {
      return false;
    }

    // Verify for duplicates, if any.
    if (hasDuplicateCustomProperty(key_value_list, &invalid_property_node_)) {
      invalid_index_type_ = InvalidIndexType::kDuplicateKey;
      return false;
    }

    for (const ParseKeyValue &key_value : *key_value_list) {
      const std::string key = ToLower(key_value.key()->value());
      if (key.compare(kBloomFilterSizeInBytes) == 0) {
        const ParseKeyIntegerValue *key_integer_value = static_cast<const ParseKeyIntegerValue*>(&key_value);
        if (key_integer_value->value()->float_like()) {
          setIndexDescriptionAsInvalid(InvalidIndexType::kSizeIsFloat, &key_value);
          return false;
        }
        const std::int64_t bloom_filter_size = key_integer_value->value()->long_value();
        if (bloom_filter_size < 0) {
          setIndexDescriptionAsInvalid(InvalidIndexType::kSizeIsNegative, &key_value);
          return false;
        }
        // All good! allow the default bloom filter size to be modified.
        index_sub_block_description_->SetExtension(BloomFilterIndexSubBlockDescription::bloom_filter_size,
                                                   bloom_filter_size);
      } else if (key.compare(kBloomFilterNumHashes) == 0) {
        const ParseKeyIntegerValue *key_integer_value = static_cast<const ParseKeyIntegerValue*>(&key_value);
        if (key_integer_value->value()->float_like()) {
          setIndexDescriptionAsInvalid(InvalidIndexType::kNumHashesIsFloat, &key_value);
          return false;
        }
        const std::int64_t number_of_hashes = key_integer_value->value()->long_value();
        if (number_of_hashes < 0) {
          setIndexDescriptionAsInvalid(InvalidIndexType::kNumHashesIsNegative, &key_value);
          return false;
        }
        // All good! allow the default num_hashes to be modified.
        index_sub_block_description_->SetExtension(BloomFilterIndexSubBlockDescription::number_of_hashes,
                                                   number_of_hashes);
      } else if (key.compare(kBloomFilterProjectElementCount) == 0) {
        const ParseKeyIntegerValue *key_integer_value = static_cast<const ParseKeyIntegerValue*>(&key_value);
        if (key_integer_value->value()->float_like()) {
          setIndexDescriptionAsInvalid(InvalidIndexType::kProjectedCountIsFloat, &key_value);
          return false;
        }
        const std::int64_t projected_element_count = key_integer_value->value()->long_value();
        if (projected_element_count < 0) {
          setIndexDescriptionAsInvalid(InvalidIndexType::kProjectedCountIsNegative, &key_value);
          return false;
        }
        // All good! allow the default projected_element_count to be modified.
        index_sub_block_description_->SetExtension(BloomFilterIndexSubBlockDescription::projected_element_count,
                                                   projected_element_count);
      } else {
        // None of the defined properties matched, this is an invalid property for this index.
        setIndexDescriptionAsInvalid(InvalidIndexType::kInvalidKey, &key_value);
        return false;
      }
    }
    invalid_index_type_ = InvalidIndexType::kNone;
    invalid_property_node_ = nullptr;
    return true;
  }

 private:
  // A map of index property constants and their associated type.
  valid_property_map_type valid_property_map_;

  DISALLOW_COPY_AND_ASSIGN(BloomFilterIndexProperties);
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
    : IndexProperties(new IndexSubBlockDescription()) {
    index_sub_block_description_->set_sub_block_type(IndexSubBlockDescription::CSB_TREE);
  }

  ~CSBTreeIndexProperties() override {
  }

  std::string getReasonForInvalidIndexDescription() const override {
    switch (invalid_index_type_) {
      case InvalidIndexType::kNone:
        return "";
      case InvalidIndexType::kUnimplemented:
        return "CSB Tree index is not yet implemented";
      case InvalidIndexType::kInvalidKey:
        return "CSB Tree index does not define index properties";
      default:
        return "Unknown reason";
    }
  }

  bool addCustomProperties(const PtrList<ParseKeyValue> *key_value_list) override {
    // CSBTreeIndex does not define any additional index property,
    // so a call to this function will invalidate the index.
    invalid_index_type_ = InvalidIndexType::kInvalidKey;
    invalid_property_node_ = nullptr;
    index_sub_block_description_.reset();
    return false;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CSBTreeIndexProperties);
};

/**
 * @brief Implementation of index properties for SMA Index.
 */
class SMAIndexProperties : public IndexProperties {
 public:
  /**
   * @brief Constructor.
   */
  SMAIndexProperties() : IndexProperties(new IndexSubBlockDescription()) {
    index_sub_block_description_->set_sub_block_type(IndexSubBlockDescription::SMA);
  }

  ~SMAIndexProperties() override {
  }

  std::string getReasonForInvalidIndexDescription() const override {
    switch (invalid_index_type_) {
      case InvalidIndexType::kNone:
        return "";
      case InvalidIndexType::kInvalidKey:
        return "SMA index does not define index properties";
      default:
        return "Unknown reason";
    }
  }

  bool addCustomProperties(const PtrList<ParseKeyValue> *key_value_list) override {
    // SMA does not define any index properties, so calling this function
    // will invalidate the index.
    invalid_index_type_ = InvalidIndexType::kInvalidKey;
    invalid_property_node_ = nullptr;
    index_sub_block_description_.reset();
    return false;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(SMAIndexProperties);
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
