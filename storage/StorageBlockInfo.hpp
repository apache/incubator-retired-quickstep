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

#ifndef QUICKSTEP_STORAGE_STORAGE_BLOCK_INFO_HPP_
#define QUICKSTEP_STORAGE_STORAGE_BLOCK_INFO_HPP_

#include <climits>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include "utility/Macros.hpp"

namespace quickstep {

class TupleStorageSubBlock;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A globally unique identifier for a StorageBlock or StorageBlob.
 * @note A block id whose domain is ZERO means an invalid or nonexistent block.
 **/
typedef std::uint64_t block_id;
typedef std::uint16_t block_id_domain;
typedef std::uint64_t block_id_counter;

static constexpr block_id kInvalidBlockId = 0;
static constexpr int kBlockIdDomainLengthInDigits = std::numeric_limits<block_id_domain>::digits10;
static constexpr int kBlockIdCounterLengthInDigits = std::numeric_limits<block_id_counter>::digits10;
static constexpr block_id_domain kMaxDomain = static_cast<block_id_domain>(0xFFFF);

typedef std::vector<block_id> BlocksInPartition;

/**
 * @brief All-static object that provides access to block_id.
 **/
class BlockIdUtil {
 public:
  /**
   * @brief Factory method to get a block id by its domain and counter.
   *
   * @param domain The domain of the desired block id.
   * @param counter The counter of the desired block id.
   * @return The block id set by its domain and counter.
   **/
  static inline block_id GetBlockId(const block_id_domain domain, const block_id_counter counter) {
    block_id block = (static_cast<block_id>(domain) << kDomainShift)
                     | (static_cast<block_id>(counter) & kCounterMask);
    return block;
  }

  /**
   * @brief Get the domain of the given block id.
   *
   * @param block The block id.
   * @return The domain of the block id.
   **/
  static inline block_id_domain Domain(const block_id block) {
    return static_cast<block_id_domain>(block >> kDomainShift);
  }

  /**
   * @brief Get the counter of the given block id.
   *
   * @param block The block id.
   * @return The counter of the block id.
   **/
  static inline block_id_counter Counter(const block_id block) {
    return static_cast<block_id_counter>(block & kCounterMask);
  }

  /**
   * @brief Get the string format of the given block id.
   *
   * @param block The block id.
   * @return The stringified the block id.
   **/
  static std::string ToString(const block_id block);

 private:
  // Undefined default constructor. Class is all-static and should not be
  // instantiated.
  BlockIdUtil();

  static constexpr std::uint64_t kCounterMask = static_cast<std::uint64_t>(0xFFFFFFFFFFFFULL);
  static constexpr unsigned kDomainShift = 48;

  DISALLOW_COPY_AND_ASSIGN(BlockIdUtil);
};

/**
 * @brief Identifies a single tuple within a StorageBlock.
 *
 * @note A negative value indicates an invalid/unset/nonexistent tuple.
 **/
typedef int tuple_id;
const tuple_id kMaxTupleID = INT_MAX;

/**
 * @brief Codes for the different implementations of TupleStorageSubBlock.
 **/
enum TupleStorageSubBlockType {
  kBasicColumnStore,
  kCompressedPackedRowStore,
  kCompressedColumnStore,
  kSplitRowStore,
  kNumTupleStorageSubBlockTypes  // Not an actual TupleStorageSubBlockType, exists for counting purposes.
};

/**
 * @brief Names corresponsing to TupleStorageSubBlockType.
 * @note Defined out-of-line in StorageBlockInfo.cpp
 **/
extern const char *kTupleStorageSubBlockTypeNames[];

/**
 * @brief Codes for the different implementations of IndexSubBlock.
 **/
enum IndexSubBlockType {
  kCSBTree = 0,
  kSMA,
  kBloomFilter,
  kBitWeavingV,
  kBitWeavingH,
  kNumIndexSubBlockTypes  // Not an actual IndexSubBlockType, exists for counting purposes.
};

/**
 * @brief Names corresponsing to IndexSubBlockType.
 * @note Defined out-of-line in StorageBlockInfo.cpp
 **/
extern const char *kIndexSubBlockTypeNames[];

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_STORAGE_BLOCK_INFO_HPP_
