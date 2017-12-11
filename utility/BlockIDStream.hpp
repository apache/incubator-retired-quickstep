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

#ifndef QUICKSTEP_UTILITY_BLOCK_ID_STREAM_HPP_
#define QUICKSTEP_UTILITY_BLOCK_ID_STREAM_HPP_

#include <cstddef>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

class BlockIDStream {
 public:
  BlockIDStream(const CatalogRelation &relation,
                const bool relation_is_stored,
                const bool &done_feeding_blocks)
      : relation_is_stored_(relation_is_stored),
        done_feeding_blocks_(done_feeding_blocks),
        current_position_(0) {
    if (relation_is_stored_) {
      block_ids_ = relation.getBlocksSnapshot();
    }
  }

  inline void append(const block_id block) {
    block_ids_.emplace_back(block);
  }

  inline bool hasNext() const {
    return current_position_ < block_ids_.size();
  }

  inline block_id getNext() {
    DCHECK(current_position_ < block_ids_.size());
    return block_ids_[current_position_++];
  }

  bool isEndOfStream() const {
    return (!hasNext()) && (relation_is_stored_ || done_feeding_blocks_);
  }

 private:
  const bool relation_is_stored_;
  const bool &done_feeding_blocks_;

  std::vector<block_id> block_ids_;
  std::size_t current_position_;

  DISALLOW_COPY_AND_ASSIGN(BlockIDStream);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_BLOCK_ID_STREAM_HPP_

