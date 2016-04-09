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

#ifndef QUICKSTEP_TRANSACTION_RESOURCE_ID_HPP_
#define QUICKSTEP_TRANSACTION_RESOURCE_ID_HPP_

#include <cstddef>
#include <limits>
#include <string>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"

namespace quickstep {
namespace transaction {

/** \addtogroup Transaction
 *  @{
 */

/**
 * @brief Class for representing unique id for each database resource
 *        e.g Tuple, block, relation, database etc...
 **/
class ResourceId {
 public:
  /**
   * @brief Constructor
   *
   * @param db_id Unique id of the database resource.
   * @param rel_id Unique id of the relation resource in the database.
   * @param block_id Unique id of the block resource in the relation.
   * @param tuple_id Unique id of the tuple resource in the block.
   **/
  explicit ResourceId(const database_id db_id = kDatabaseIdPlaceholder,
                      const relation_id rel_id = kRelationIdPlaceholder,
                      const block_id block_id = kBlockIdPlaceholder,
                      const tuple_id tuple_id = kTupleIdPlaceholder)
      : db_id_(db_id),
        rel_id_(rel_id),
        block_id_(block_id),
        tuple_id_(tuple_id) {
  }

  /**
   * @brief Copy constructor.
   *
   * @param other The ResourceId that will be copied.
   **/
  ResourceId(const ResourceId &other)
      : db_id_(other.db_id_),
        rel_id_(other.rel_id_),
        block_id_(other.block_id_),
        tuple_id_(other.tuple_id_) {
  }

  /**
   * @brief Hasher class for ResourceId class to use it in the hash map.
   **/
  struct ResourceIdHasher {
    /**
     * @brief Functor of the class.
     *
     * @param rid Resource id to be hashed.
     * @return Hash of the resource id.
     **/
    std::size_t operator()(const ResourceId &rid) const;
  };

  /**
   * @brief Equality operator for ResourceId class.
   *
   * @param other Resource to be compared.
   * @return True if this and other are the id of the same
   *         resource, false otherwise.
   **/
  bool operator==(const ResourceId &other) const;

   /**
   * @brief Inequality operator for ResourceId class.
   *
   * @param other Resource to be compared.
   * @return True if this and other are the ids of the different
   *         resources, false otherwise.
   **/
  inline bool operator!=(const ResourceId &other) const {
    return !(*this == other);
  }

  /**
   * @brief Checks whether this resource id has a parent in resource hierarchy.
   *
   * @return False if this resource id is database level, true otherwise.
   **/
  inline bool hasParent() const {
    return !isDatabaseAccess();
  }

  /**
   * @brief Getter for this resource id's parent in the hierarchy.
   *
   * @return The resource id of this resource id's parent.
   **/
  ResourceId getParentResourceId() const;

  /**
   * @brief Checks whether this is a database level resource id.
   *
   * @return True if this is a database level access, false otherwise.
   **/
  inline bool isDatabaseAccess() const {
    return !isDatabaseIdPlaceholder()
        && isRelationIdPlaceholder()
        && isBlockIdPlaceholder()
        && isTupleIdPlaceholder();
  }

  /**
   * @brief Checks whether this is a relation level resource id.
   *
   * @return True if this is a relation level access, false otherwise.
   **/
  bool isRelationAccess() const {
    return !isDatabaseIdPlaceholder()
        && !isRelationIdPlaceholder()
        && isBlockIdPlaceholder()
        && isTupleIdPlaceholder();
  }

  /**
   * @brief Checks whether this is a block level resource id.
   *
   * @return True if this is a block level access, false otherwise.
   **/
  bool isBlockAccess() const {
    return !isDatabaseIdPlaceholder()
        && !isRelationIdPlaceholder()
        && !isBlockIdPlaceholder()
        && isTupleIdPlaceholder();
  }

  /**
   * @brief Checks whether this is a tuple level resource id.
   *
   * @return True if this is a tuple level access, false otherwise.
   **/
  bool isTupleAccess() const {
    return !isDatabaseIdPlaceholder()
        && !isRelationIdPlaceholder()
        && !isBlockIdPlaceholder()
        && !isTupleIdPlaceholder();
  }

  /**
   * @brief This is a helper method for string representation
   *        of the resource id.
   *
   * @return String representation of the reosurce id.
   **/
  std::string toString() const;

 private:
  // Negative value is invalid id for database id. Use -1 as placeholder.
  static constexpr database_id kDatabaseIdPlaceholder = kInvalidCatalogId;

  // Negative value is invalid id for relation id. Use -1 as placeholder.
  static constexpr relation_id kRelationIdPlaceholder = kInvalidCatalogId;

  // Zero is invalid id for block id. Use zero as the placeholder.
  static constexpr block_id kBlockIdPlaceholder = kInvalidBlockId;

  // Negative tuple id os invalid, therefore use a negative value.
  static constexpr tuple_id kTupleIdPlaceholder = kInvalidCatalogId;

  inline bool isDatabaseIdPlaceholder() const {
    return db_id_ == kDatabaseIdPlaceholder;
  }

  inline bool isRelationIdPlaceholder() const {
    return rel_id_ == kRelationIdPlaceholder;
  }

  inline bool isBlockIdPlaceholder() const {
    return block_id_ == kBlockIdPlaceholder;
  }

  inline bool isTupleIdPlaceholder() const {
    return tuple_id_ == kTupleIdPlaceholder;
  }

  const database_id db_id_;
  const relation_id rel_id_;
  const block_id block_id_;
  const tuple_id tuple_id_;
};

/** @} */

}  // namespace transaction
}  // namespace quickstep

#endif  // QUICKSTEP_RESOURCE_ID_HPP_
