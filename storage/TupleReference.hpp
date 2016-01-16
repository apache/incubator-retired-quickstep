/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_STORAGE_TUPLE_REFERENCE_HPP_
#define QUICKSTEP_STORAGE_TUPLE_REFERENCE_HPP_

#include "storage/StorageBlockInfo.hpp"

namespace quickstep {

class TupleStorageSubBlock;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A reference to a particular tuple in a StorageBlock.
 **/
struct TupleReference {
  TupleReference()
      : block(0), tuple(-1) {
  }

  TupleReference(block_id block_in, tuple_id tuple_in)
      : block(block_in), tuple(tuple_in) {
  }

  block_id block;
  tuple_id tuple;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_TUPLE_REFERENCE_HPP_
