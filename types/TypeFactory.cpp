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

#include "types/TypeFactory.hpp"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

#include "types/GenericValue.hpp"
#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypeID.hpp"
#include "types/TypeSynthesizer.hpp"
#include "types/TypeUtil.hpp"
#include "utility/Macros.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

namespace quickstep {

bool TypeFactory::TypeNameIsValid(const std::string &type_name) {
  const auto &type_name_map = GetTypeNameMap();
  return type_name_map.find(type_name) != type_name_map.end();
}

TypeID TypeFactory::GetTypeIDForName(const std::string &type_name) {
  DCHECK(TypeNameIsValid(type_name));
  return GetTypeNameMap().at(type_name);
}

bool TypeFactory::TypeRequiresLengthParameter(const TypeID id) {
  return TypeUtil::IsParameterizedPod(id);
}

bool TypeFactory::TypeParametersAreValid(
    const TypeID id, const std::vector<GenericValue> &parameters) {
  if (TypeUtil::GetMemoryLayout(id) != kCxxGeneric) {
    return false;
  }

  return InvokeOnTypeID<TypeIDSelectorMemoryLayout<kCxxGeneric>>(
      id,
      [&](auto id) -> bool {  // NOLINT(build/c++11)
    using TypeClass = typename TypeIDTrait<decltype(id)::value>::TypeClass;
    return TypeClass::TypeParametersAreValid(parameters);
  });
}


const Type& TypeFactory::GetType(const TypeID id,
                                 const bool nullable) {
  DCHECK(TypeUtil::GetMemoryLayout(id) == kCxxInlinePod)
      << "Called TypeFactory::GetType() with incorrect parameters.";

  return *InvokeOnTypeID<TypeIDSelectorMemoryLayout<kCxxInlinePod>>(
      id,
      [&](auto id) -> const Type* {  // NOLINT(build/c++11)
    return &TypeIDTrait<decltype(id)::value>::TypeClass::Instance(nullable);
  });
}

const Type& TypeFactory::GetType(const TypeID id,
                                 const std::size_t length,
                                 const bool nullable) {
  DCHECK(TypeRequiresLengthParameter(id))
      << "Called TypeFactory::GetType() with incorrect parameters.";

  return *InvokeOnTypeID<TypeIDSelectorMemoryLayout<kParInlinePod, kParOutOfLinePod>>(
      id,
      [&](auto id) -> const Type* {  // NOLINT(build/c++11)
    return &TypeIDTrait<decltype(id)::value>::TypeClass::Instance(nullable, length);
  });
}

const Type& TypeFactory::GetType(const TypeID id,
                                 const std::vector<GenericValue> &parameters,
                                 const bool nullable) {
  DCHECK(TypeUtil::GetMemoryLayout(id) == kCxxGeneric)
      << "Called TypeFactory::GetType() with incorrect parameters.";

  return *InvokeOnTypeID<TypeIDSelectorMemoryLayout<kCxxGeneric>>(
      id,
      [&](auto id) -> const Type* {  // NOLINT(build/c++11)
    return &TypeIDTrait<decltype(id)::value>::TypeClass::Instance(nullable, parameters);
  });
}

bool TypeFactory::ProtoIsValid(const serialization::Type &proto) {
  // Check that proto is fully initialized.
  if (!proto.IsInitialized()) {
    return false;
  }

  // Check that the type_id is valid, and has length if parameterized.
  const TypeID type_id = TypeIDFactory::ReconstructFromProto(proto.type_id());

  if (type_id == kNullType) {
    return proto.nullable();
  }

  if (TypeRequiresLengthParameter(type_id)) {
    return proto.has_length();
  }

  return true;
}

const Type& TypeFactory::ReconstructFromProto(const serialization::Type &proto) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create Type from an invalid proto description:\n"
      << proto.DebugString();

  const TypeID type_id = TypeIDFactory::ReconstructFromProto(proto.type_id());

  switch (TypeUtil::GetMemoryLayout(type_id)) {
    case kCxxInlinePod:
      return GetType(type_id, proto.nullable());
    case kParInlinePod:  // Fall through
    case kParOutOfLinePod:
      return GetType(type_id, proto.length(), proto.nullable());
    case kCxxGeneric: {
      std::vector<GenericValue> parameters;
      for (int i = 0; i < proto.parameters_size(); ++i) {
        parameters.emplace_back(ReconstructValueFromProto(proto.parameters(i)));
      }
      return GetType(type_id, parameters, proto.nullable());
    }
  }
}

GenericValue TypeFactory::ReconstructValueFromProto(
    const serialization::GenericValue &proto) {
  const Type &type = ReconstructFromProto(proto.type());
  if (proto.has_data()) {
    return GenericValue::CreateWithOwnedData(
        type, type.unmarshallValue(proto.data().c_str(), proto.data().size()));
  } else {
    return GenericValue::CreateNullValue(type);
  }
}

const Type* TypeFactory::GetMostSpecificType(const Type &first, const Type &second) {
  if (first.isSafelyCoercibleFrom(second)) {
    return &first;
  } else if (second.isSafelyCoercibleFrom(first)) {
    return &second;
  } else {
    return nullptr;
  }
}

const Type* TypeFactory::GetUnifyingType(const Type &first, const Type &second) {
  // TODO: cache
  const Type *unifier = nullptr;
  if (first.isNullable() || second.isNullable()) {
    unifier = GetMostSpecificType(first.getNullableVersion(),
                                  second.getNullableVersion());
    if (unifier == nullptr) {
      if (((first.getTypeID() == kLong) && (second.getTypeID() == kFloat))
            || ((first.getTypeID() == kFloat) && (second.getTypeID() == kLong))) {
        unifier = &(DoubleType::Instance(true));
      }
    }
  } else {
    unifier = GetMostSpecificType(first, second);
    if (unifier == nullptr) {
      if (((first.getTypeID() == kLong) && (second.getTypeID() == kFloat))
            || ((first.getTypeID() == kFloat) && (second.getTypeID() == kLong))) {
        unifier = &(DoubleType::Instance(false));
      }
    }
  }

  return unifier;
}

const std::unordered_map<std::string, TypeID>& TypeFactory::GetTypeNameMap() {
  static std::unordered_map<std::string, TypeID> type_name_map;
  if (type_name_map.empty()) {
    for (std::size_t i = 0; i < static_cast<std::size_t>(kNumTypeIDs); ++i) {
      type_name_map.emplace(ToLower(kTypeNames[i]), static_cast<TypeID>(i));
    }
  }
  return type_name_map;
}

}  // namespace quickstep
