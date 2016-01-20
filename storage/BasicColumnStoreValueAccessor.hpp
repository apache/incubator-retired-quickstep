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

#ifndef QUICKSTEP_STORAGE_BASIC_COLUMN_STORE_VALUE_ACCESSOR_HPP_
#define QUICKSTEP_STORAGE_BASIC_COLUMN_STORE_VALUE_ACCESSOR_HPP_

#include <vector>

#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

namespace quickstep {

class BasicColumnStoreTupleStorageSubBlock;

class BasicColumnStoreValueAccessorHelper {
 public:
  BasicColumnStoreValueAccessorHelper(const CatalogRelationSchema &relation,
                                      const tuple_id num_tuples,
                                      const std::vector<void*> &column_stripes,
                                      const PtrVector<BitVector<false>, true> &column_null_bitmaps)
      : relation_(relation),
        num_tuples_(num_tuples),
        column_stripes_(column_stripes),
        column_null_bitmaps_(column_null_bitmaps) {
  }

  inline tuple_id numPackedTuples() const {
    return num_tuples_;
  }

  template <bool check_null>
  inline const void* getAttributeValue(const tuple_id tuple,
                                       const attribute_id attr) const {
    DEBUG_ASSERT(tuple < num_tuples_);
    DEBUG_ASSERT(relation_.hasAttributeWithId(attr));
    if (check_null
        && (!column_null_bitmaps_.elementIsNull(attr))
        && column_null_bitmaps_[attr].getBit(tuple)) {
      return nullptr;
    }

    // TODO(chasseur): Consider cacheing the byte lengths of attributes.
    return static_cast<const char*>(column_stripes_[attr])
           + (tuple * relation_.getAttributeById(attr)->getType().maximumByteLength());
  }

  inline TypedValue getAttributeValueTyped(const tuple_id tuple,
                                           const attribute_id attr) const {
    const Type &attr_type = relation_.getAttributeById(attr)->getType();
    const void *untyped_value = getAttributeValue<true>(tuple, attr);
    return (untyped_value == nullptr)
        ? attr_type.makeNullValue()
        : attr_type.makeValue(untyped_value, attr_type.maximumByteLength());
  }

 private:
  const CatalogRelationSchema &relation_;
  const tuple_id num_tuples_;
  const std::vector<void*> &column_stripes_;
  const PtrVector<BitVector<false>, true> &column_null_bitmaps_;

  DISALLOW_COPY_AND_ASSIGN(BasicColumnStoreValueAccessorHelper);
};

typedef PackedTupleStorageSubBlockValueAccessor<
    BasicColumnStoreTupleStorageSubBlock,
    BasicColumnStoreValueAccessorHelper,
    ValueAccessor::Implementation::kBasicColumnStore>
        BasicColumnStoreValueAccessor;

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_BASIC_COLUMN_STORE_VALUE_ACCESSOR_HPP_
