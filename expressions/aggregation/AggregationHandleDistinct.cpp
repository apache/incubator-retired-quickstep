/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "expressions/aggregation/AggregationHandleDistinct.hpp"

#include <cstddef>
#include <memory>
#include <vector>
#include <utility>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/HashTable.hpp"

#include "types/TypedValue.hpp"

#include "glog/logging.h"

namespace quickstep {

class ColumnVector;
class StorageManager;
class Type;
class ValueAccessor;

AggregationStateHashTableBase* AggregationHandleDistinct::createGroupByHashTable(
    const HashTableImplType hash_table_impl,
    const std::vector<const Type*> &group_by_types,
    const std::size_t estimated_num_groups,
    StorageManager *storage_manager) const {
  return createDistinctifyHashTable(
      hash_table_impl,
      group_by_types,
      estimated_num_groups,
      storage_manager);
}

void AggregationHandleDistinct::aggregateValueAccessorIntoHashTable(
    ValueAccessor *accessor,
    const std::vector<attribute_id> &argument_ids,
    const std::vector<attribute_id> &group_by_key_ids,
    AggregationStateHashTableBase *hash_table) const {
  DCHECK_EQ(argument_ids.size(), 0u);

  insertValueAccessorIntoDistinctifyHashTable(
      accessor,
      group_by_key_ids,
      hash_table);
}

ColumnVector* AggregationHandleDistinct::finalizeHashTable(
    const AggregationStateHashTableBase &hash_table,
    std::vector<std::vector<TypedValue>> *group_by_keys) const {
  DCHECK(group_by_keys->empty());

  const auto keys_retriever = [&group_by_keys](std::vector<TypedValue> &group_by_key,
                                               const bool &dumb_placeholder) -> void {
    group_by_keys->emplace_back(std::move(group_by_key));
  };
  static_cast<const AggregationStateHashTable<bool>&>(hash_table).forEachCompositeKey(&keys_retriever);

  return nullptr;
}

}  // namespace quickstep
