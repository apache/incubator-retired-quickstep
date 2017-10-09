/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_TREE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_TREE_HPP_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "storage/StorageBlockLayout.pb.h"
#include "utility/Macros.hpp"
#include "utility/TreeStringSerializable.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief Base class of OptimizerTree. This class is needed so that we may
 *        attach non-child type nodes to a tree in the string representation
 *        as long as the non-child type is a subclass of OptimizerTreeBase.
 *        For example, a Logical node may have a child tree node of an
 *        Expression type.
 */
class OptimizerTreeBase
    : public TreeStringSerializable<std::shared_ptr<const OptimizerTreeBase>> {
 public:
  typedef std::shared_ptr<const OptimizerTreeBase> OptimizerTreeBaseNodePtr;

  /**
   * @brief Destructor.
   */
  ~OptimizerTreeBase() override {}

 protected:
  /**
   * @brief Constructor.
   */
  OptimizerTreeBase() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(OptimizerTreeBase);
};

/**
 * @brief Base class for expressions, logical/physical trees.
 */
template <class NodeType>
class OptimizerTree : public OptimizerTreeBase {
 public:
  typedef std::shared_ptr<const NodeType> OptimizerTreeNodePtr;

  /**
   * @brief Destructor.
   */
  ~OptimizerTree() override {}

  /**
   * @brief Returns the child node list.
   *
   * @return A vector of children.
   */
  const std::vector<OptimizerTreeNodePtr>& children() const {
    return children_;
  }

  /**
   * @brief Returns the number of child nodes.
   *
   * @return The number of child nodes.
   */
  std::size_t getNumChildren() const { return children_.size(); }

  /**
   * @brief Creates a copy with the child nodes replaced by \p new_children.
   *
   * @param new_children The children to be substituted for the existing ones.
   * @return A copy with \p new_children as child nodes.
   */
  virtual OptimizerTreeNodePtr copyWithNewChildren(
      const std::vector<OptimizerTreeNodePtr> &new_children) const = 0;

 protected:
  /**
   * @brief Constructor.
   */
  OptimizerTree() {}

  /**
   * @brief Adds a new child node to this tree node->
   *
   * @param child The node to add as a new child.
   */
  void addChild(const OptimizerTreeNodePtr &child) {
    children_.push_back(child);
  }

 private:
  std::vector<OptimizerTreeNodePtr> children_;

  DISALLOW_COPY_AND_ASSIGN(OptimizerTree);
};

/**
 * @brief A helper class for printing Protobuf messages in a simple flat-format.
 * @details Holds key-value pairs of a proto message. Key values are added as strings.
 *          To use, create a representation and then add properties:
 *             auto foo = new OptimizerProtoRepresentation<OptimizerPtrType>();
 *             foo.addProperty("friendly_name", proto.GetExtension(ProtoType::friendly_name));
 */
template<class TreeNodeType>
class OptimizerProtoRepresentation : public OptimizerTreeBase {
  /**
   * @brief Inner class which represents key-value properties of the given proto.
   */
  class OptimizerProtoPropertyRepresentation : public OptimizerTreeBase {
   public:
    OptimizerProtoPropertyRepresentation(std::string key, std::string value)
        : key_(key), value_(value) {  }

    std::string getName() const {
      return "ProtoProperty";
    }

    void getFieldStringItems(std::vector<std::string> *inline_field_names,
                             std::vector<std::string> *inline_field_values,
                             std::vector<std::string> *non_container_child_field_names,
                             std::vector<TreeNodeType> *non_container_child_fields,
                             std::vector<std::string> *container_child_field_names,
                             std::vector<std::vector<TreeNodeType>> *container_child_fields) const {
      inline_field_names->push_back(std::string("Property"));
      inline_field_values->push_back(key_);

      inline_field_names->push_back(std::string("Value"));
      inline_field_values->push_back(value_);
    }

   private:
    std::string key_;
    std::string value_;
  };  // class OptimizerProtoRepresentation

 public:
  OptimizerProtoRepresentation() : properties_() { }

  std::string getName() const {
    return "ProtoDescription";
  }

  /**
   * @brief Add a key-value type property to this proto representation.
   * @details Internally, the instance creates another node to represent the
   *          given values.
   *
   * @param key A string key.
   * @param value A string value.
   */
  void addProperty(std::string key, std::string value) {
    std::shared_ptr<const OptimizerProtoPropertyRepresentation> property(
        new OptimizerProtoPropertyRepresentation(key, value));
    properties_.push_back(property);
  }

  /**
   * @brief Add a key-value type property to this proto representation.
   * @details Internally, the instance creates another node to represent the
   *          given values.
   *
   * @param key A string key.
   * @param value An int value which is converted into a string.
   */
  void addProperty(std::string key, int value) {
    std::shared_ptr<const OptimizerProtoPropertyRepresentation> property(
        new OptimizerProtoPropertyRepresentation(key, std::to_string(value)));
    properties_.push_back(property);
  }

  void getFieldStringItems(std::vector<std::string> *inline_field_names,
                           std::vector<std::string> *inline_field_values,
                           std::vector<std::string> *non_container_child_field_names,
                           std::vector<TreeNodeType> *non_container_child_fields,
                           std::vector<std::string> *container_child_field_names,
                           std::vector<std::vector<TreeNodeType>> *container_child_fields) const {
    for (auto property : properties_) {
      non_container_child_field_names->push_back("Property");
      non_container_child_fields->push_back(property);
    }
  }
  // A list of managed properties.
  std::vector<std::shared_ptr<const OptimizerProtoPropertyRepresentation> > properties_;
};

/**
 * @brief Returns an optimizer node representation of a LayoutDescription.
 *
 * @param description A valid StorageBlockLayoutDescription.
 * @return A caller-managed optimizer tree node of the proto message.
 */
template<class TreeNodeType>
OptimizerProtoRepresentation<TreeNodeType>* getOptimizerRepresentationForProto(
    const StorageBlockLayoutDescription *description) {
  if (description == nullptr) {
    return nullptr;
  }

  std::unique_ptr<OptimizerProtoRepresentation<TreeNodeType> >
      node(new OptimizerProtoRepresentation<TreeNodeType>());

  // Add properties based on the tuple storage block type.
  const ::quickstep::TupleStorageSubBlockDescription &storage_block_description
      = description->tuple_store_description();
  switch (storage_block_description.sub_block_type()) {
    case TupleStorageSubBlockDescription::SPLIT_ROW_STORE: {
      node->addProperty("blocktype", "split_rowstore");
      break;
    }
    case TupleStorageSubBlockDescription::BASIC_COLUMN_STORE: {
      node->addProperty("blocktype", "columnstore");
      if (storage_block_description.HasExtension(
              quickstep::BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id)) {
        node->addProperty("sort",
            storage_block_description.GetExtension(
                quickstep::BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id));
      }
      break;
    }
    case TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE: {
      node->addProperty("blocktype", "compressed_columnstore");
      node->addProperty("sort",
          storage_block_description.GetExtension(
              quickstep::CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id));
      for (int compressed_attribute = 0;
           compressed_attribute < storage_block_description.ExtensionSize(
               quickstep::CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id);
           ++compressed_attribute) {
        node->addProperty("compress",
            storage_block_description.GetExtension(
                quickstep::CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id,
                compressed_attribute));
      }
      break;
    }
    case TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE: {
      node->addProperty("blocktype", "compressed_packed_rowstore");
      for (int compressed_attribute = 0;
           compressed_attribute < storage_block_description.ExtensionSize(
               quickstep::CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id);
           ++compressed_attribute) {
        node->addProperty("compress",
            storage_block_description.GetExtension(
                quickstep::CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id,
                compressed_attribute));
      }
      break;
    }
    default: {
      LOG(WARNING) << "Unrecognized block type in protobuf message.";
      break;
    }
  }
  // Every case will specify a slots number.
  node->addProperty("slots", description->num_slots());
  return node.release();
}

template<class TreeNodeType>
OptimizerProtoRepresentation<TreeNodeType>* getOptimizerRepresentationForProto(
    const serialization::PartitionSchemeHeader *partition_header) {
  if (partition_header == nullptr) {
    return nullptr;
  }

  auto node = std::make_unique<OptimizerProtoRepresentation<TreeNodeType>>();

  // Add properties based on the partition type.
  switch (partition_header->partition_type()) {
    case serialization::PartitionSchemeHeader::HASH: {
      node->addProperty("partition_type", "hash");
      break;
    }
    case serialization::PartitionSchemeHeader::RANDOM: {
      node->addProperty("partition_type", "random");
      break;
    }
    case serialization::PartitionSchemeHeader::RANGE: {
      node->addProperty("partition_type", "range");
      // TODO(quickstep-team): display the range boundaries.
      node->addProperty("range_boundaries", "TODO");
      break;
    }
    default:
      LOG(FATAL) << "Unrecognized partition type in protobuf message.";
  }
  // Every case will specify a partition number and a partition attributes.
  node->addProperty("num_partitions", partition_header->num_partitions());

  for (int i = 0; i < partition_header->partition_attribute_ids_size(); ++i) {
    node->addProperty("partition_attr_id", partition_header->partition_attribute_ids(i));
  }

  return node.release();
}

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_TREE_HPP_ */
