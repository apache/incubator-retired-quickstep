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

#include "storage/SubBlockTypeRegistry.hpp"

#include <cstddef>

#include "storage/StorageBlockLayout.pb.h"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

bool SubBlockTypeRegistry::LayoutDescriptionIsValid(
    const CatalogRelationSchema &relation,
    const StorageBlockLayoutDescription &description) {
  // Check that layout is fully initialized.
  if (!description.IsInitialized()) {
    return false;
  }

  // Check that the number of slots is positive.
  if (description.num_slots() == 0) {
    return false;
  }

  // Check that the tuple_store_description is valid.
  const TupleStorageSubBlockDescription &tuple_store_description = description.tuple_store_description();
  if (!tuple_store_description.IsInitialized()) {
    return false;
  }

  std::unordered_map<TupleStoreTypeIntegral,
                     TupleStoreDescriptionIsValidFunction>::const_iterator
      it = Instance()->tuple_store_description_is_valid_functions_.find(
          static_cast<TupleStoreTypeIntegral>(tuple_store_description.sub_block_type()));
  if (it == Instance()->tuple_store_description_is_valid_functions_.end()) {
    return false;
  }
  if (!(*it->second)(relation, tuple_store_description)) {
    return false;
  }

  // Check that each index_description is valid.
  for (int index_description_num = 0;
       index_description_num < description.index_description_size();
       ++index_description_num) {
    const IndexSubBlockDescription &index_description = description.index_description(index_description_num);
    if (!index_description.IsInitialized()) {
      return false;
    }

    std::unordered_map<IndexTypeIntegral,
                       IndexDescriptionIsValidFunction>::const_iterator
        index_it = Instance()->index_description_is_valid_functions_.find(
            static_cast<IndexTypeIntegral>(index_description.sub_block_type()));
    if (index_it == Instance()->index_description_is_valid_functions_.end()) {
      return false;
    }
    if (!(*index_it->second)(relation, index_description)) {
      return false;
    }
  }

  return true;
}

bool SubBlockTypeRegistry::IndexDescriptionIsValid(
      const CatalogRelationSchema &relation,
      const IndexSubBlockDescription &description) {
  if (!description.IsInitialized()) {
    return false;
  }

  std::unordered_map<IndexTypeIntegral,
                     IndexDescriptionIsValidFunction>::const_iterator
      index_it = Instance()->index_description_is_valid_functions_.find(
          static_cast<IndexTypeIntegral>(description.sub_block_type()));
  // No validity function was registered.
  if (index_it == Instance()->index_description_is_valid_functions_.end()) {
    return false;
  }

  if (!(*index_it->second)(relation, description)) {
    return false;
  }
  return true;
}

std::size_t SubBlockTypeRegistry::EstimateBytesPerTupleForTupleStore(
    const CatalogRelationSchema &relation,
    const TupleStorageSubBlockDescription &description) {
  DEBUG_ASSERT(description.IsInitialized());

  std::unordered_map<TupleStoreTypeIntegral,
                     TupleStoreEstimateBytesPerTupleFunction>::const_iterator
      it = Instance()->tuple_store_estimate_bytes_per_tuple_functions_.find(
          static_cast<TupleStoreTypeIntegral>(description.sub_block_type()));
  DEBUG_ASSERT(it != Instance()->tuple_store_estimate_bytes_per_tuple_functions_.end());

  return (*it->second)(relation, description);
}

std::size_t SubBlockTypeRegistry::EstimateBytesPerTupleForIndex(
    const CatalogRelationSchema &relation,
    const IndexSubBlockDescription &description) {
  DEBUG_ASSERT(description.IsInitialized());

  std::unordered_map<IndexTypeIntegral,
                     IndexEstimateBytesPerTupleFunction>::const_iterator
      it = Instance()->index_estimate_bytes_per_tuple_functions_.find(
          static_cast<IndexTypeIntegral>(description.sub_block_type()));
  DEBUG_ASSERT(it != Instance()->index_estimate_bytes_per_tuple_functions_.end());

  return (*it->second)(relation, description);
}

std::size_t SubBlockTypeRegistry::EstimateBytesPerBlockForIndex(const CatalogRelationSchema &relation,
                                                                const IndexSubBlockDescription &description) {
  DCHECK(description.IsInitialized());

  std::unordered_map<IndexTypeIntegral,
  IndexEstimateBytesPerBlockFunction>::const_iterator
  it = Instance()->index_estimate_bytes_per_block_functions_.find(
      static_cast<IndexTypeIntegral>(description.sub_block_type()));
  DCHECK(it != Instance()->index_estimate_bytes_per_block_functions_.end());

  return (*it->second)(relation, description);
}

}  // namespace quickstep
