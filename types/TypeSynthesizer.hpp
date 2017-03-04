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

#ifndef QUICKSTEP_TYPES_TYPE_SYNTHESIZER_HPP_
#define QUICKSTEP_TYPES_TYPE_SYNTHESIZER_HPP_

#include <cstddef>
#include <type_traits>

#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypeID.hpp"
#include "types/TypeRegistrar.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrMap.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <typename TypeClass, bool parameterized>
class TypeInstance;


template <TypeID type_id>
class TypeSynthesizer
    : public Type,
      public TypeInstance<typename TypeIDTrait<type_id>::TypeClass,
                          TypeIDTrait<type_id>::kParameterized> {
 public:
  using Trait = TypeIDTrait<type_id>;
  using TypeClass = typename Trait::TypeClass;

  static constexpr Type::SuperTypeID kStaticSuperTypeID = Trait::kStaticSuperTypeID;
  static constexpr TypeID kStaticTypeID = Trait::kStaticTypeID;
  static constexpr bool kParameterized = Trait::kParameterized;
  static constexpr TypeStorageLayout kLayout = Trait::kLayout;

  typedef typename Trait::cpptype cpptype;

  serialization::Type getProto() const override {
    serialization::Type proto;

    proto.mutable_type_id()->CopyFrom(TypeIDFactory::GetProto(type_id_));
    proto.set_nullable(nullable_);

    if (kParameterized) {
      proto.set_length(parameter_);
    }

    return proto;
  }

  const Type& getNullableVersion() const override {
    return getInstance<kParameterized>(true);
  }

  const Type& getNonNullableVersion() const override {
    return getInstance<kParameterized>(false);
  }

 protected:
  template <TypeStorageLayout layout = kLayout, bool parameterized = kParameterized>
  explicit TypeSynthesizer(const bool nullable,
                           std::enable_if_t<layout == kNativeEmbedded ||
                                            layout == kNativeInline>* = 0)
      : Type(kStaticSuperTypeID, kStaticTypeID, nullable,
             sizeof(cpptype), sizeof(cpptype)) {
    DCHECK(!kParameterized);
  }

  template <TypeStorageLayout layout = kLayout, bool parameterized = kParameterized>
  TypeSynthesizer(const bool nullable,
                  const std::size_t minimum_byte_length,
                  const std::size_t maximum_byte_length,
                  const std::size_t parameter,
                  std::enable_if_t<parameterized &&
                                   (layout == kNonNativeInline ||
                                    layout == kOutOfLine)>* = 0)
      : Type(kStaticSuperTypeID, kStaticTypeID, nullable,
             minimum_byte_length, maximum_byte_length, parameter) {
    DCHECK(kLayout != kNonNativeInline || minimum_byte_length == maximum_byte_length);
  }

  template <TypeStorageLayout layout = kLayout, bool parameterized = kParameterized>
  TypeSynthesizer(const bool nullable,
                  const std::size_t minimum_byte_length,
                  const std::size_t maximum_byte_length,
                  std::enable_if_t<!parameterized &&
                                   (layout == kNonNativeInline ||
                                    layout == kOutOfLine)>* = 0)
      : Type(kStaticSuperTypeID, kStaticTypeID, nullable,
             minimum_byte_length, maximum_byte_length) {
    DCHECK(kLayout != kNonNativeInline || minimum_byte_length == maximum_byte_length);
  }

 private:
  template <bool has_param>
  inline const Type& getInstance(const bool nullable,
                                 std::enable_if_t<has_param>* = 0) const {
    return TypeInstance<TypeClass, kParameterized>::Instance(parameter_, nullable);
  }

  template <bool has_param>
  inline const Type& getInstance(const bool nullable,
                                 std::enable_if_t<!has_param>* = 0) const {
    return TypeInstance<TypeClass, kParameterized>::Instance(nullable);
  }

  friend class TypeInstance<TypeClass, kParameterized>;

  DISALLOW_COPY_AND_ASSIGN(TypeSynthesizer);
};

template <TypeID type_id>
constexpr Type::SuperTypeID TypeSynthesizer<type_id>::kStaticSuperTypeID;

template <TypeID type_id>
constexpr TypeID TypeSynthesizer<type_id>::kStaticTypeID;

template <TypeID type_id>
constexpr bool TypeSynthesizer<type_id>::kParameterized;

template <TypeID type_id>
constexpr TypeStorageLayout TypeSynthesizer<type_id>::kLayout;


template <typename TypeClass>
class TypeInstance<TypeClass, false> {
 public:
  static const TypeClass& InstanceNonNullable() {
    return InstanceInternal<false>();
  }

  static const TypeClass& InstanceNullable() {
    return InstanceInternal<true>();
  }

  static const TypeClass& Instance(const bool nullable) {
    if (nullable) {
      return InstanceNullable();
    } else {
      return InstanceNonNullable();
    }
  }

 private:
  template <bool nullable>
  inline static const TypeClass& InstanceInternal() {
    static TypeClass instance(nullable);
    return instance;
  }
};

template <typename TypeClass>
class TypeInstance<TypeClass, true> {
 public:
  static const TypeClass& InstanceNonNullable(const std::size_t length) {
    return InstanceInternal<false>(length);
  }

  static const TypeClass& InstanceNullable(const std::size_t length) {
    return InstanceInternal<true>(length);
  }

  static const TypeClass& Instance(const bool nullable, const std::size_t length) {
    if (nullable) {
      return InstanceNullable(length);
    } else {
      return InstanceNonNullable(length);
    }
  }

 private:
  template <bool nullable>
  inline static const TypeClass& InstanceInternal(const std::size_t length) {
    static PtrMap<size_t, TypeClass> instance_map;
    auto imit = instance_map.find(length);
    if (imit == instance_map.end()) {
      imit = instance_map.insert(length, new TypeClass(length, nullable)).first;
    }
    return *(imit->second);
  }
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPE_SYNTHESIZER_HPP_
