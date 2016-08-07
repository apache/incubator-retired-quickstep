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

#ifndef QUICKSTEP_STORAGE_INDEX_SUB_BLOCK_DESCRIPTION_FACTORY_HPP_
#define QUICKSTEP_STORAGE_INDEX_SUB_BLOCK_DESCRIPTION_FACTORY_HPP_

#include "storage/StorageBlockLayout.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A class that describes factories of IndexSubBlockDescriptionFactory.
 */
class IndexSubBlockDescriptionFactory {
 public:
  /**
   * @brief A static method that verifies the proto description for different indices.
   *
   * @param index_description Index Description to be verified.
   * @return True if the proto is valid, false otherwise.
   */
  static bool ProtoIsValid(const IndexSubBlockDescription &index_description) {
    if (!index_description.IsInitialized()) {
      return false;
    }

    // Make sure at least one key attribute is specified.
    if (index_description.indexed_attribute_ids_size() == 0) {
      return false;
    }

    // Different types of indexes can specify their own validation checks here.
    // Currently these are trivial for CSBTree and SMAIndex.
    switch (index_description.sub_block_type()) {
      case IndexSubBlockDescription_IndexSubBlockType_BITWEAVING_H:  // Fall through.
      case IndexSubBlockDescription_IndexSubBlockType_BITWEAVING_V:
        // Only singular keys are allowed.
        if (index_description.indexed_attribute_ids_size() == 1) {
          return true;
        }
        return false;
      case IndexSubBlockDescription_IndexSubBlockType_CSB_TREE:
        return true;
      case IndexSubBlockDescription_IndexSubBlockType_SMA:
        return true;
      case IndexSubBlockDescription_IndexSubBlockType_BLOOM_FILTER:
        // TODO(@ssaurabh): Implement validation for bloom filters, once they are implemented.
        return true;
      default:
        return false;
    }
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(IndexSubBlockDescriptionFactory);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_INDEX_SUB_BLOCK_DESCRIPTION_FACTORY_HPP_
