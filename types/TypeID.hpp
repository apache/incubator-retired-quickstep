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
#include <type_traits>

#include "types/Type.pb.h"
#include "utility/Macros.hpp"

namespace quickstep {

/**
 * @brief Concrete Types.
 *
 * @note TypedValue assumes that this doesn't exceed 64 TypeIDs.
 **/
enum TypeID : int {
  kBool = 0,
  kInt,
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

enum MemoryLayout {
  kCxxNativePod,
  kParNativePod,
  kParIndirectPod,
  kGeneric
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

class TypeIDFactory {
 public:
  inline static serialization::TypeID GetProto(const TypeID type_id) {
    serialization::TypeID proto;
    proto.set_id(static_cast<std::underlying_type_t<TypeID>>(type_id));
    return proto;
  }

  inline static TypeID ReconstructFromProto(const serialization::TypeID &proto) {
    return static_cast<TypeID>(proto.id());
  }

  inline static bool ProtoIsValid(const serialization::TypeID &proto) {
    return proto.id() < static_cast<std::underlying_type_t<TypeID>>(kNumTypeIDs);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(TypeIDFactory);
};

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPE_ID_HPP_
