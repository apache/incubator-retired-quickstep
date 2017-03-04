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

#include "types/operations/unary_operations/SubstringOperation.hpp"

#include <cstddef>
#include <vector>

#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/meta/Dispatchers.hpp"

#include "glog/logging.h"

namespace quickstep {

UncheckedUnaryOperator* SubstringOperation::makeUncheckedUnaryOperator(
    const Type &type,
    const std::vector<TypedValue> &static_arguments) const {
  DCHECK(UnaryOperation::canApplyTo(type, static_arguments));

  std::size_t start_position;
  std::size_t substring_length;
  ExtractStaticArguments(static_arguments, &start_position, &substring_length);

  const std::size_t input_maximum_length =
      type.getTypeID() == kChar
          ? static_cast<const CharType&>(type).getStringLength()
          : static_cast<const VarCharType&>(type).getStringLength();
  const bool input_null_terminated = (type.getTypeID() == TypeID::kVarChar);

  const Type *result_type = getResultType(type, static_arguments);
  DCHECK(result_type != nullptr);

  return meta::InvokeOnBools(
      input_null_terminated, type.isNullable(),
      [&](auto is_null_terminated,  // NOLINT(build/c++11)
          auto is_nullable) -> UncheckedUnaryOperator* {
    return new SubstringUncheckedOperator<
        decltype(is_null_terminated)::value,
        decltype(is_nullable)::value>(
            start_position,
            ComputeMaximumSubstringLength(type, start_position, substring_length),
            input_maximum_length,
            *result_type);
  });
}

}  // namespace quickstep
