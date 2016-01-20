/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_STORAGE_STORAGE_BLOCK_LAYOUT_HPP_
#define QUICKSTEP_STORAGE_STORAGE_BLOCK_LAYOUT_HPP_

#include <cstddef>
#include <exception>

#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogRelationSchema;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A physical layout for StorageBlocks. Describes the size of blocks,
 *        the type of TupleStorageSubBlock to use, any IndexSubBlocks, and the
 *        relative sizes of all sub-blocks.
 **/
class StorageBlockLayout {
 public:
  /**
   * @brief Constructor.
   *
   * @param relation The relation that blocks described by this layout belong
   *        to.
   **/
  explicit StorageBlockLayout(const CatalogRelationSchema &relation)
      : relation_(relation),
        estimated_bytes_per_tuple_(0) {
  }

  /**
   * @brief Constructor for deserializing from Protocol Buffer.
   *
   * @param relation The relation that blocks described by this layout belong
   *        to.
   * @param proto The serialized Protocol Buffer form of the StorageBlockLayout.
   **/
  StorageBlockLayout(const CatalogRelationSchema &relation,
                     const StorageBlockLayoutDescription &proto);

  /**
   * @param Destructor.
   **/
  ~StorageBlockLayout() {
  }

  /**
   * @brief Static method to generate a default layout for a particular
   *        relation.
   * @note The current policy is that a default layout takes up one slot, uses
   *       PackedRowStoreTupleStorageSubBlock for fixed-length relations or
   *       SplitRowStoreTupleStorageSubBlock for variable-length relations, and
   *       has no indices.
   *
   * @param relation The relation to generate a layout for.
   * @param relation_variable_length Whether relation is variable-length.
   * @return A new StorageBlockLayout for the relation, according to the
   *         default policies.
   **/
  static StorageBlockLayout* GenerateDefaultLayout(
      const CatalogRelationSchema &relation,
      const bool relation_variable_length);

  /**
   * @brief Static method to check whether a StorageBlockLayoutDescription is
   *        fully-formed and all parts are valid.
   *
   * @param relation The relation a layout belongs to.
   * @param description A description of a StorageBlockLayout.
   * @return Whether description is fully-formed and valid.
   **/
  static bool DescriptionIsValid(const CatalogRelationSchema &relation,
                                 const StorageBlockLayoutDescription &description);

  /**
   * @brief Get the relation this layout applies to.
   *
   * @return The relation this layout applies to.
   **/
  const CatalogRelationSchema& getRelation() const {
    return relation_;
  }

  /**
   * @brief Get this layout's internal StorageBlockLayoutDescription.
   *
   * @return A reference to this layout's internal description.
   **/
  const StorageBlockLayoutDescription& getDescription() const {
    return layout_description_;
  }

  /**
   * @brief Get a mutable pointer to this layout's internal
   *        StorageBlockLayoutDescription.
   * @note This method should be used to access the internal description of
   *       this layout to modify it and build up the layout.
   *
   * @return A mutable pointer to this layout's internal description.
   **/
  StorageBlockLayoutDescription* getDescriptionMutable() {
    return &layout_description_;
  }

  /**
   * @brief Finalize the layout and build the StorageBlockHeader.
   * @note This should be called after constructing the StorageBlockLayout and
   *       building it up by accessing getDescriptionMutable(), but before
   *       using getBlockHeaderSize() or copyHeaderTo().
   **/
  void finalize();

  /**
   * @brief Determine the size, in bytes, of the StorageBlockHeader in blocks
   *        with this layout, plus the 4 bytes at the front which store the
   *        header length.
   * @warning finalize() must be called before using this method.
   *
   * @return The size (in bytes) of the StorageBlockHeader for this layout.
   **/
  std::size_t getBlockHeaderSize() const {
    DEBUG_ASSERT(block_header_.IsInitialized());
    return sizeof(int) + block_header_.ByteSize();
  }

  /**
   * @brief Copy a StorageBlockHeader describing this layout to the target
   *        memory location.
   * @note The data copied to dest is an int (the length of the rest of the
   *       header) followed by the binary-serialized form of block_header_.
   *
   * @param dest A memory location to copy the header to (i.e. the start of a
   *        StorageBlock's memory). MUST be at least as large as the size
   *        reported by getBlockHeaderSize().
   **/
  void copyHeaderTo(void *dest) const;

  /**
   * @brief Estimate the number of bytes used to store each tuple in this
   *        layout.
   * @warning This estimate is a "best guess" only, and may be very inaccurate
   *          in the presence of variable-length attributes, compression, and
   *          various index types.
   * @warning finalize() must be called before using this method.
   *
   * @return The estimated number of bytes used to store each tuple in blocks
   *         of this layout.
   **/
  std::size_t estimateBytesPerTuple() const {
    DEBUG_ASSERT(block_header_.IsInitialized());
    return estimated_bytes_per_tuple_;
  }

  /**
   * @brief Estimate the number of tuples that can be stored in "full" blocks
   *        of this layout.
   * @warning As with estimateBytesPerTuple(), this is a "best guess" only.
   * @warning finalize() must be called before using this method.
   *
   * @return The estimated number of tuples that can be stored in a full block
   *         of this layout.
   **/
  std::size_t estimateTuplesPerBlock() const;

 private:
  const CatalogRelationSchema &relation_;
  StorageBlockLayoutDescription layout_description_;
  StorageBlockHeader block_header_;

  std::size_t estimated_bytes_per_tuple_;

  DISALLOW_COPY_AND_ASSIGN(StorageBlockLayout);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_STORAGE_BLOCK_LAYOUT_HPP_
