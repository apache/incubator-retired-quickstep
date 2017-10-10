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

#include "types/containers/ColumnVector.hpp"

#include <cstddef>

#include "types/TypeID.hpp"
#include "types/TypeUtil.hpp"
#include "types/TypeRegistrar.hpp"
#include "types/TypeIDSelectors.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class Type;
class TypedValue;

ColumnVector* ColumnVector::MakeVectorOfValue(
    const Type &value_type,
    const TypedValue &value,
    const std::size_t num_copies) {
  switch (value_type.getMemoryLayout()) {
    case kCxxInlinePod:  // Fall through
    case kParInlinePod: {
      NativeColumnVector *result = new NativeColumnVector(value_type, num_copies);
      result->fillWithValue(value);
      return result;
    }
    case kParOutOfLinePod: {
      IndirectColumnVector *result = new IndirectColumnVector(value_type, num_copies);
      result->fillWithValue(value);
      return result;
    }
    case kCxxGeneric: {
      // TODO(refactor-type): Omit non-supported types.
      return InvokeOnTypeID<TypeIDSelectorMemoryLayout<kCxxGeneric>>(
          value_type.getTypeID(),
          [&](auto tid) -> ColumnVector* {
        using TypeClass = typename TypeIDTrait<decltype(tid)::value>::TypeClass;
        GenericColumnVector<TypeClass> *result =
            new GenericColumnVector<TypeClass>(value_type, num_copies);
        result->fillWithValue(value);
        return result;
      });
    }
  }
  QUICKSTEP_UNREACHABLE();
}

constexpr ColumnVector::Implementation NativeColumnVector::kImplementation;

constexpr ColumnVector::Implementation IndirectColumnVector::kImplementation;

}  // namespace quickstep
