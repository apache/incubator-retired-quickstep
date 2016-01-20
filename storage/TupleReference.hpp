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
