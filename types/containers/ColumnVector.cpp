/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "types/containers/ColumnVector.hpp"

#include <cstddef>

namespace quickstep {

class Type;
class TypedValue;

ColumnVector* ColumnVector::MakeVectorOfValue(
    const Type &value_type,
    const TypedValue &value,
    const std::size_t num_copies) {
  if (NativeColumnVector::UsableForType(value_type)) {
    NativeColumnVector *result = new NativeColumnVector(value_type, num_copies);
    result->fillWithValue(value);
    return result;
  } else {
    IndirectColumnVector *result = new IndirectColumnVector(value_type, num_copies);
    result->fillWithValue(value);
    return result;
  }
}

}  // namespace quickstep
