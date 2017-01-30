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

#include "expressions/aggregation/AggregationConcreteHandle.hpp"

#include <cstddef>
#include <vector>

#include "storage/HashTableFactory.hpp"
#include "storage/PackedPayloadHashTable.hpp"
#include "storage/ValueAccessorMultiplexer.hpp"

namespace quickstep {

class StorageManager;
class Type;

AggregationStateHashTableBase* AggregationConcreteHandle::createDistinctifyHashTable(
    const HashTableImplType hash_table_impl,
    const std::vector<const Type*> &key_types,
    const std::size_t estimated_num_distinct_keys,
    StorageManager *storage_manager) const {
  // Create a hash table with key types as key_types and value type as bool.
  return AggregationStateHashTableFactory::CreateResizable(
      hash_table_impl,
      key_types,
      estimated_num_distinct_keys,
      {},
      storage_manager);
}

void AggregationConcreteHandle::insertValueAccessorIntoDistinctifyHashTable(
    const std::vector<MultiSourceAttributeId> &argument_ids,
    const std::vector<MultiSourceAttributeId> &key_ids,
    const ValueAccessorMultiplexer &accessor_mux,
    AggregationStateHashTableBase *distinctify_hash_table) const {
  std::vector<MultiSourceAttributeId> concatenated_ids(key_ids);
  for (const MultiSourceAttributeId &arg_id : argument_ids) {
    concatenated_ids.emplace_back(arg_id);
  }

  static_cast<PackedPayloadHashTable *>(distinctify_hash_table)
      ->upsertValueAccessorCompositeKey({}, concatenated_ids, accessor_mux);
}

}  // namespace quickstep
