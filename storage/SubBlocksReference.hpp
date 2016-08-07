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

#ifndef QUICKSTEP_STORAGE_SUB_BLOCKS_REFERENCE_HPP_
#define QUICKSTEP_STORAGE_SUB_BLOCKS_REFERENCE_HPP_

#include <vector>

#include "utility/PtrVector.hpp"

#include "glog/logging.h"

namespace quickstep {

class IndexSubBlock;
class TupleStorageSubBlock;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief Container for const-references to sub-blocks inside a StorageBlock.
 *        Intended for passing into expressions that may take advantage of
 *        fast-path non-scan evaluation of ComparisonPredicates.
 **/
struct SubBlocksReference {
  SubBlocksReference(const TupleStorageSubBlock &tuple_store_in,
                     const PtrVector<IndexSubBlock, false> &indices_in,
                     const std::vector<bool> &indices_consistent_in)
      : tuple_store(tuple_store_in),
        indices(indices_in),
        indices_consistent(indices_consistent_in) {
    DCHECK_EQ(indices.size(), indices_consistent.size());
  }

  const TupleStorageSubBlock &tuple_store;
  const PtrVector<IndexSubBlock, false> &indices;
  const std::vector<bool> &indices_consistent;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_SUB_BLOCKS_REFERENCE_HPP_
