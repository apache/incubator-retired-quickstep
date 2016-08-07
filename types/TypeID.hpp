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

#ifndef QUICKSTEP_TYPES_TYPE_ID_HPP_
#define QUICKSTEP_TYPES_TYPE_ID_HPP_

#include <cstddef>

namespace quickstep {

/**
 * @brief Concrete Types.
 *
 * @note TypedValue assumes that this doesn't exceed 64 TypeIDs.
 **/
enum TypeID {
  kInt = 0,
  kLong,
  kFloat,
  kDouble,
  kChar,
  kVarChar,
  kDate,
  kDatetime,
  kDatetimeInterval,
  kYearMonthInterval,
  kNullType,
  kNumTypeIDs  // Not a real TypeID, exists for counting purposes.
};

/**
 * @brief Provides basic information about a Type in the Quickstep type system.
 *
 * @note This does not necessarily uniquely identify a Type. For instance, a
 *       DECIMAL type may have a precision that it is not captured by this
 *       struct.
 * @note This is used only to provide basic Type information to
 *       TypedValue::isPlausibleInstanceOf() for debugging purposes.
 **/
struct TypeSignature {
  TypeID id;
  bool nullable;
  std::size_t length;
};

/**
 * @brief Names of types in the same order as TypeID.
 * @note Defined out-of-line in TypeID.cpp
 **/
extern const char *kTypeNames[kNumTypeIDs];

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPE_ID_HPP_
