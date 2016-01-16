/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
