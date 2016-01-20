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
