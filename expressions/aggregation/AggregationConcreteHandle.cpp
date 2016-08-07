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

#include "catalog/CatalogTypedefs.hpp"
#include "storage/HashTable.hpp"
#include "storage/HashTableFactory.hpp"

namespace quickstep {

class StorageManager;
class Type;
class ValueAccessor;

AggregationStateHashTableBase* AggregationConcreteHandle::createDistinctifyHashTable(
    const HashTableImplType hash_table_impl,
    const std::vector<const Type*> &key_types,
    const std::size_t estimated_num_distinct_keys,
    StorageManager *storage_manager) const {
  // Create a hash table with key types as key_types and value type as bool.
  return AggregationStateHashTableFactory<bool>::CreateResizable(
      hash_table_impl,
      key_types,
      estimated_num_distinct_keys,
      storage_manager);
}

void AggregationConcreteHandle::insertValueAccessorIntoDistinctifyHashTable(
    ValueAccessor *accessor,
    const std::vector<attribute_id> &key_ids,
    AggregationStateHashTableBase *distinctify_hash_table) const {
  // If the key-value pair is already there, we don't need to update the value,
  // which should always be "true". I.e. the value is just a placeholder.
  const auto noop_upserter = [](const auto &accessor, const bool *value) -> void {};

  AggregationStateHashTable<bool> *hash_table =
      static_cast<AggregationStateHashTable<bool>*>(distinctify_hash_table);
  if (key_ids.size() == 1) {
    hash_table->upsertValueAccessor(accessor,
                                    key_ids[0],
                                    true /* check_for_null_keys */,
                                    true /* initial_value */,
                                    &noop_upserter);
  } else {
    hash_table->upsertValueAccessorCompositeKey(accessor,
                                                key_ids,
                                                true /* check_for_null_keys */,
                                                true /* initial_value */,
                                                &noop_upserter);
  }
}

}  // namespace quickstep
