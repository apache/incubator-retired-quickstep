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
#include <cstdio>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "types/GenericValue.hpp"
#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypeID.hpp"
#include "types/TypeRegistrar.hpp"
#include "types/TypedValue.hpp"
#include "utility/HashPair.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <TypeID type_id, typename Enable = void>
class TypeInstancePolicy;


template <TypeID type_id>
class TypeSynthesizer
    : public Type,
      public TypeInstancePolicy<type_id> {
 private:
  using Trait = TypeIDTrait<type_id>;
  using InstancePolicy = TypeInstancePolicy<type_id>;

 public:
  static constexpr SuperTypeID kStaticSuperTypeID = Trait::kStaticSuperTypeID;
  static constexpr TypeID kStaticTypeID = Trait::kStaticTypeID;
  static constexpr bool kIsParPod = Trait::kIsParPod;
  static constexpr MemoryLayout kMemoryLayout = Trait::kMemoryLayout;

  using TypeClass = typename Trait::TypeClass;
  using cpptype = typename Trait::cpptype;

  serialization::Type getProto() const override {
    serialization::Type proto;

    proto.mutable_type_id()->CopyFrom(TypeIDFactory::GetProto(type_id_));
    proto.set_nullable(nullable_);

    InstancePolicy::fillProto(&proto);

    return proto;
  }

  const Type& getNullableVersion() const override {
    return InstancePolicy::getInstance(true);
  }

  const Type& getNonNullableVersion() const override {
    return InstancePolicy::getInstance(false);
  }

  const cpptype& castValueToLiteral(const UntypedLiteral *value) const {
    return *static_cast<const cpptype*>(value);
  }

  cpptype& castValueToLiteral(UntypedLiteral *value) const {
    return *static_cast<cpptype*>(value);
  }

  UntypedLiteral* unmarshallValue(const TypedValue &value) const override {
    return unmarshallInternal<kMemoryLayout>(value);
  }

  UntypedLiteral* unmarshallValue(TypedValue &&value) const override {
    return unmarshallInternal<kMemoryLayout>(std::move(value));
  }

  std::string printTypedValueToString(const TypedValue &value) const override {
    return invokeOnUnmarshalledValue<kMemoryLayout>(
        value,
        [&](const UntypedLiteral *value) -> std::string {
      return this->printValueToString(value);
    });
  }

  void printTypedValueToFile(const TypedValue &value,
                             FILE *file,
                             const int padding = 0) const override {
    invokeOnUnmarshalledValue<kMemoryLayout>(
        value,
        [&](const UntypedLiteral *value) -> void {
      this->printValueToFile(value, file, padding);
    });
  }

 protected:
  template <MemoryLayout layout = kMemoryLayout>
  explicit TypeSynthesizer(const bool nullable,
                           std::enable_if_t<layout == kCxxInlinePod>* = 0)
      : Type(kStaticSuperTypeID, kStaticTypeID, nullable,
             sizeof(cpptype), sizeof(cpptype)),
        TypeInstancePolicy<type_id>() {
  }

  template <MemoryLayout layout = kMemoryLayout>
  TypeSynthesizer(const bool nullable,
                  const std::size_t minimum_byte_length,
                  const std::size_t maximum_byte_length,
                  const std::size_t parameter,
                  std::enable_if_t<layout == kParInlinePod || layout == kParOutOfLinePod>* = 0)
      : Type(kStaticSuperTypeID, kStaticTypeID, nullable,
             minimum_byte_length, maximum_byte_length),
        TypeInstancePolicy<type_id>(parameter) {
  }

  template <MemoryLayout layout = kMemoryLayout>
  TypeSynthesizer(const bool nullable,
                  const std::size_t minimum_byte_length,
                  const std::size_t maximum_byte_length,
                  const std::vector<GenericValue> &parameters = {},
                  std::enable_if_t<layout == kCxxGeneric>* = 0)
      : Type(kStaticSuperTypeID, kStaticTypeID, nullable,
             minimum_byte_length, maximum_byte_length),
        TypeInstancePolicy<type_id>(parameters) {
  }

 private:
  template <MemoryLayout layout>
  inline UntypedLiteral* unmarshallInternal(
      const TypedValue &value,
      std::enable_if_t<layout == kCxxInlinePod> * = 0) const {
    return cloneValue(value.getDataPtr());
  }

  template <MemoryLayout layout>
  inline UntypedLiteral* unmarshallInternal(
      const TypedValue &value,
      std::enable_if_t<layout == kParInlinePod ||
                       layout == kParOutOfLinePod> * = 0) const {
    return cloneValue(&value);
  }

  template <MemoryLayout layout>
  inline UntypedLiteral* unmarshallInternal(
      TypedValue &&value,
      std::enable_if_t<layout == kParInlinePod ||
                       layout == kParOutOfLinePod> * = 0) const {
    return new TypedValue(std::move(value));
  }

  template <MemoryLayout layout>
  inline UntypedLiteral* unmarshallInternal(
      const TypedValue &value,
      std::enable_if_t<layout == kCxxGeneric> * = 0) const {
    return Type::unmarshallValue(value.getOutOfLineData(), value.getDataSize());
  }


  template <MemoryLayout layout, typename Functor>
  inline auto invokeOnUnmarshalledValue(
      const TypedValue &value,
      const Functor &functor,
      std::enable_if_t<layout == kCxxInlinePod> * = 0) const {
    return functor(value.getDataPtr());
  }

  template <MemoryLayout layout, typename Functor>
  inline auto invokeOnUnmarshalledValue(
      const TypedValue &value,
      const Functor &functor,
      std::enable_if_t<layout == kParInlinePod ||
                       layout == kParOutOfLinePod> * = 0) const {
    return functor(&value);
  }

  template <MemoryLayout layout, typename Functor>
  inline auto invokeOnUnmarshalledValue(
      const TypedValue &value,
      const Functor &functor,
      std::enable_if_t<layout == kCxxGeneric> * = 0) const {
    std::unique_ptr<cpptype> literal(
        static_cast<cpptype*>(Type::unmarshallValue(value.getOutOfLineData(),
                                                    value.getDataSize())));
    return functor(literal.get());
  }

  template <TypeID, typename> friend class TypeInstancePolicy;

  DISALLOW_COPY_AND_ASSIGN(TypeSynthesizer);
};

template <TypeID type_id>
constexpr SuperTypeID TypeSynthesizer<type_id>::kStaticSuperTypeID;

template <TypeID type_id>
constexpr TypeID TypeSynthesizer<type_id>::kStaticTypeID;

template <TypeID type_id>
constexpr bool TypeSynthesizer<type_id>::kIsParPod;

template <TypeID type_id>
constexpr MemoryLayout TypeSynthesizer<type_id>::kMemoryLayout;


template <TypeID type_id>
class TypeInstancePolicy<
    type_id,
    std::enable_if_t<TypeIDTrait<type_id>::kMemoryLayout == kCxxInlinePod>> {
 private:
  using Trait = TypeIDTrait<type_id>;
  using TypeClass = typename Trait::TypeClass;

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

 protected:
  TypeInstancePolicy() {}

  inline const Type& getInstance(const bool nullable) const {
    return nullable ? InstanceNullable() : InstanceNonNullable();
  }

  inline void fillProto(serialization::Type *proto) const {}

 private:
  template <bool nullable>
  inline static const TypeClass& InstanceInternal() {
    static TypeClass instance(nullable);
    return instance;
  }
};

template <TypeID type_id>
class TypeInstancePolicy<
    type_id,
    std::enable_if_t<TypeIDTrait<type_id>::kMemoryLayout == kParInlinePod ||
                     TypeIDTrait<type_id>::kMemoryLayout == kParOutOfLinePod>> {
 private:
  using Trait = TypeIDTrait<type_id>;
  using TypeClass = typename Trait::TypeClass;

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

  inline std::size_t length() const {
    return length_;
  }

 protected:
  TypeInstancePolicy(const std::size_t length)
      : length_(length) {}

  const std::size_t length_;

  inline const Type& getInstance(const bool nullable) const {
    return nullable ? InstanceNullable(length_) : InstanceNonNullable(length_);
  }

  inline void fillProto(serialization::Type *proto) const {
    proto->set_length(length_);
  }

 private:
  template <bool nullable>
  inline static const TypeClass& InstanceInternal(const std::size_t length) {
    static std::unordered_map<size_t, std::unique_ptr<TypeClass>> instance_map;
    auto imit = instance_map.find(length);
    if (imit == instance_map.end()) {
      std::unique_ptr<TypeClass> instance(new TypeClass(nullable, length));
      imit = instance_map.emplace(length, std::move(instance)).first;
    }
    return *(imit->second);
  }
};

template <TypeID type_id>
class TypeInstancePolicy<
    type_id,
    std::enable_if_t<TypeIDTrait<type_id>::kMemoryLayout == kCxxGeneric>> {
 private:
  using Trait = TypeIDTrait<type_id>;
  using TypeClass = typename Trait::TypeClass;

 public:
  static const TypeClass& InstanceNonNullable(
      const std::vector<GenericValue> &parameters = {}) {
    return InstanceInternal<false>(parameters);
  }

  static const TypeClass& InstanceNullable(
      const std::vector<GenericValue> &parameters = {}) {
    return InstanceInternal<true>(parameters);
  }

  static const TypeClass& Instance(
      const bool nullable,
      const std::vector<GenericValue> &parameters = {}) {
    if (nullable) {
      return InstanceNullable(parameters);
    } else {
      return InstanceNonNullable(parameters);
    }
  }

  inline const std::vector<GenericValue>& parameters() const {
    return parameters_;
  }

 protected:
  TypeInstancePolicy(const std::vector<GenericValue> &parameters)
      : parameters_(parameters) {}

  const std::vector<GenericValue> parameters_;

  inline const Type& getInstance(const bool nullable) const {
    return nullable ? InstanceNullable(parameters_)
                    : InstanceNonNullable(parameters_);
  }

  inline void fillProto(serialization::Type *proto) const {
    LOG(FATAL) << "TODO";
  }

 private:
  struct ParametersHasher {
    inline std::size_t operator()(const std::vector<GenericValue> &parameters) const {
      std::size_t hash_code = 0;
      for (const GenericValue &value : parameters) {
        hash_code = CombineHashes(hash_code, value.getHash());
      }
      return hash_code;
    }
  };

  template <typename T>
  inline static TypeClass* CreateInstance(
      const bool nullable,
      const std::vector<GenericValue> &parameters,
      decltype(new T(nullable)) * = 0) {
    DCHECK(parameters.empty());
    return new T(nullable);
  }

  template <typename T>
  inline static TypeClass* CreateInstance(
      const bool nullable,
      const std::vector<GenericValue> &parameters,
      decltype(new T(nullable, parameters)) * = 0) {
    return new T(nullable, parameters);
  }

  template <bool nullable>
  inline static const TypeClass& InstanceInternal(
      const std::vector<GenericValue> &parameters) {
    static std::unordered_map<std::vector<GenericValue>,
                              std::unique_ptr<TypeClass>,
                              ParametersHasher> instance_map;
    auto imit = instance_map.find(parameters);
    if (imit == instance_map.end()) {
      std::unique_ptr<TypeClass> instance(
          TypeInstancePolicy<type_id>::template CreateInstance<TypeClass>(
              nullable, parameters));
      imit = instance_map.emplace(parameters, std::move(instance)).first;
    }
    return *(imit->second);
  }
};

#define QUICKSTEP_SYNTHESIZE_TYPE(type) \
  template <TypeID, typename> friend class TypeInstancePolicy; \
  DISALLOW_COPY_AND_ASSIGN(type)

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPE_SYNTHESIZER_HPP_
