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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_NUMERIC_CAST_OPERATION_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_NUMERIC_CAST_OPERATION_HPP_

#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "types/IntType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "utility/Macros.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

#include "re2/stringpiece.h"
#include "re2/re2.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief UnaryOperation for CAST.
 */
class CastOperation : public UnaryOperation {
 public:
  CastOperation() {}

  std::string getName() const override {
    return "Cast";
  }

  std::string getShortName() const override {
    return "Cast";
  }

  std::vector<OperationSignaturePtr> getSignatures() const override {
    const std::vector<TypeID> source_type_ids =
        { kBool, kInt, kLong, kFloat, kDouble, kChar, kVarChar };
    const std::vector<TypeID> target_type_carrier = { kMetaType };

    std::vector<OperationSignaturePtr> signatures;
    for (const TypeID source_type_id : source_type_ids) {
      signatures.emplace_back(
          OperationSignature::Create(getName(), {source_type_id}, target_type_carrier));
    }
    return signatures;
  }

  bool canApplyTo(const Type &type,
                  const std::vector<TypedValue> &static_arguments,
                  std::string *message) const override;

  const Type* getResultType(
      const Type &type,
      const std::vector<TypedValue> &static_arguments) const override;

  UncheckedUnaryOperator* makeUncheckedUnaryOperator(
      const Type &type,
      const std::vector<TypedValue> &static_arguments) const override;

 private:
  static const Type* ExtractTargetType(
      const Type &type,
      const std::vector<TypedValue> &static_arguments);

  DISALLOW_COPY_AND_ASSIGN(CastOperation);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_NUMERIC_CAST_OPERATION_HPP_
