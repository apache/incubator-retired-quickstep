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
#include <cstdlib>
#include <cstring>
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
#include "utility/meta/Common.hpp"

#include "third_party/src/farmhash/farmhash.h"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <TypeID type_id, typename Enable = void>
class TypeSynthesizePolicy;


////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////  CxxInlinePod  ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <TypeID type_id>
class TypeSynthesizePolicy<
    type_id,
    std::enable_if_t<TypeIDTrait<type_id>::kMemoryLayout == kCxxInlinePod>> : public Type {
 private:
  using Trait = TypeIDTrait<type_id>;
  using TypeClass = typename Trait::TypeClass;
  using cpptype = typename Trait::cpptype;

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

  std::size_t getHash() const override {
    return static_cast<std::size_t>(getTypeID());
  }

  bool checkValuesEqual(const UntypedLiteral *lhs,
                        const UntypedLiteral *rhs,
                        const Type &rhs_type) const override {
    // TODO(refactor-type): Operator == overloading.
    if (type_id_ != rhs_type.getTypeID()) {
      return false;
    }
    return !std::memcmp(lhs, rhs, sizeof(cpptype));
  }

  UntypedLiteral* cloneValue(const UntypedLiteral *value) const override {
    DCHECK(value != nullptr);
    UntypedLiteral* clone = std::malloc(sizeof(cpptype));
    std::memcpy(clone, value, sizeof(cpptype));
    return clone;
  }

  void destroyValue(UntypedLiteral *value) const override {
    DCHECK(value != nullptr);
    std::free(value);
  }

  std::size_t hashValue(const UntypedLiteral *value) const override {
    return hashValueInl<sizeof(cpptype)>(value);
  }

  TypedValue marshallValue(const UntypedLiteral *value) const override {
    return makeValue(value, sizeof(cpptype)).ensureNotReference();
  }

  UntypedLiteral* unmarshallValue(const void *data,
                                  const std::size_t length) const override {
    DCHECK_EQ(sizeof(cpptype), length);
    UntypedLiteral *value = std::malloc(sizeof(cpptype));
    std::memcpy(value, data, sizeof(cpptype));
    return value;
  }

 protected:
  explicit TypeSynthesizePolicy(const bool nullable)
      : Type(Trait::kStaticSuperTypeID, type_id, nullable,
             sizeof(cpptype), sizeof(cpptype)) {}

  inline const Type& getInstance(const bool nullable) const {
    return nullable ? InstanceNullable() : InstanceNonNullable();
  }

  inline void mergeIntoProto(serialization::Type *proto) const {}

  inline UntypedLiteral* unmarshallTypedValueInl(const TypedValue &value) const {
    return cloneValue(value.getDataPtr());
  }

  template <typename Functor>
  inline auto invokeOnUnmarshalledTypedValue(const TypedValue &value,
                                             const Functor &functor) const {
    return functor(value.getDataPtr());
  }

 private:
  template <bool nullable>
  inline static const TypeClass& InstanceInternal() {
    static TypeClass instance(nullable);
    return instance;
  }

  template <std::size_t size>
  inline std::size_t hashValueInl(
      const UntypedLiteral *value,
      std::enable_if_t<meta::CxxSupportedIntegerSizes
                           ::template contains<size>::value> * = 0) const {
    using CxxUIntType = typename meta::UnsignedInteger<size>::type;
    CxxUIntType buffer;
    std::memcpy(&buffer, value, size);
    return buffer;
  }

  template <std::size_t size>
  inline std::size_t hashValueInl(
      const UntypedLiteral *value,
      std::enable_if_t<!meta::CxxSupportedIntegerSizes
                            ::template contains<size>::value> * = 0) const {
    return util::Hash(static_cast<const char*>(value), size);
  }
};


////////////////////////////////////////////////////////////////////////////////
///////////////////////  ParInlinePod & ParOutOfLinePod  ///////////////////////
////////////////////////////////////////////////////////////////////////////////
template <TypeID type_id>
class TypeSynthesizePolicy<
    type_id,
    std::enable_if_t<TypeIDTrait<type_id>::kMemoryLayout == kParInlinePod ||
                     TypeIDTrait<type_id>::kMemoryLayout == kParOutOfLinePod>> : public Type {
 private:
  using Trait = TypeIDTrait<type_id>;
  using TypeClass = typename Trait::TypeClass;
  using cpptype = typename Trait::cpptype;

  static_assert(std::is_same<cpptype, TypedValue>::value,
                "Unexpected cpptype for paramerized PODs.");

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

  std::size_t length() const {
    return length_;
  }

  std::size_t getHash() const override {
    return CombineHashes(static_cast<std::size_t>(type_id), length_);
  }

  std::size_t hashValue(const UntypedLiteral *value) const override {
    // TODO(refactor-type): Better implementation.
    return static_cast<const TypedValue*>(value)->getHash();
  }

  bool checkValuesEqual(const UntypedLiteral *lhs,
                        const UntypedLiteral *rhs,
                        const Type &rhs_type) const override {
    // TODO(refactor-type): Better implementation.
    const TypedValue *lhs_value = static_cast<const TypedValue*>(lhs);
    const TypedValue *rhs_value = static_cast<const TypedValue*>(rhs);
    return lhs_value->fastEqualCheck(*rhs_value);
  }

  UntypedLiteral* cloneValue(const UntypedLiteral *value) const override {
    DCHECK(value != nullptr);
    return new TypedValue(*static_cast<const TypedValue*>(value));
  }

  void destroyValue(UntypedLiteral *value) const override {
    DCHECK(value != nullptr);
    delete static_cast<TypedValue*>(value);
  }

  TypedValue marshallValue(const UntypedLiteral *value) const override {
    return *static_cast<const TypedValue*>(value);
  }

  UntypedLiteral* unmarshallValue(const void *data,
                                  const std::size_t length) const override {
    TypedValue *value = new TypedValue(makeValue(data, length));
    value->ensureNotReference();
    return value;
  }

 protected:
  TypeSynthesizePolicy(const bool nullable,
                       const std::size_t minimum_byte_length,
                       const std::size_t maximum_byte_length,
                       const std::size_t length)
      : Type(Trait::kStaticSuperTypeID, type_id, nullable,
             minimum_byte_length, maximum_byte_length),
        length_(length) {}

  const std::size_t length_;

  inline const Type& getInstance(const bool nullable) const {
    return nullable ? InstanceNullable(length_) : InstanceNonNullable(length_);
  }

  inline void mergeIntoProto(serialization::Type *proto) const {
    proto->set_length(length_);
  }

  inline UntypedLiteral* unmarshallTypedValueInl(const TypedValue &value) const {
    return new TypedValue(value);
  }

  template <typename Functor>
  inline auto invokeOnUnmarshalledTypedValue(const TypedValue &value,
                                             const Functor &functor) const {
    return functor(&value);
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


////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////  CxxGeneric  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <TypeID type_id>
class TypeSynthesizePolicy<
    type_id,
    std::enable_if_t<TypeIDTrait<type_id>::kMemoryLayout == kCxxGeneric>> : public Type {
 private:
  using Trait = TypeIDTrait<type_id>;
  using TypeClass = typename Trait::TypeClass;
  using cpptype = typename Trait::cpptype;

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

  const std::vector<GenericValue>& parameters() const {
    return parameters_;
  }

  std::size_t getHash() const override {
    return CombineHashes(static_cast<std::size_t>(type_id_),
                         ParametersHasher::ComputeHash(parameters_));
  }

  bool checkValuesEqual(const UntypedLiteral *lhs,
                        const UntypedLiteral *rhs,
                        const Type &rhs_type) const override {
    // LOG(FATAL) << "Not implemented";
    // TODO.
    return false;
  }

  UntypedLiteral* cloneValue(const UntypedLiteral *value) const override {
    DCHECK(value != nullptr);
    return new cpptype(*static_cast<const cpptype*>(value));
  }

  void destroyValue(UntypedLiteral *value) const override {
    DCHECK(value != nullptr);
    delete static_cast<cpptype*>(value);
  }

  std::size_t hashValue(const UntypedLiteral *value) const override {
    // TODO(refactor-type): Add note that it is a shallow hash.
    return util::Hash(static_cast<const char*>(value), sizeof(cpptype));
  }

  TypedValue marshallValue(const UntypedLiteral *value) const override {
    LOG(FATAL) << "Not implemented";
  }

  UntypedLiteral* unmarshallValue(const void *data,
                                  const std::size_t length) const override {
    LOG(FATAL) << "Not implemented";
  }

 protected:
  TypeSynthesizePolicy(const bool nullable,
                       const std::size_t minimum_byte_length,
                       const std::size_t maximum_byte_length,
                       const std::vector<GenericValue> &parameters)
      : Type(Trait::kStaticSuperTypeID, type_id, nullable,
             minimum_byte_length, maximum_byte_length),
        parameters_(parameters) {}

  inline const Type& getInstance(const bool nullable) const {
    return nullable ? InstanceNullable(parameters_)
                    : InstanceNonNullable(parameters_);
  }

  inline void mergeIntoProto(serialization::Type *proto) const {
    for (const auto &param : parameters_) {
      proto->add_parameters()->MergeFrom(param.getProto());
    }
  }

  inline UntypedLiteral* unmarshallTypedValueInl(const TypedValue &value) const {
    return unmarshallValue(value.getOutOfLineData(), value.getDataSize());
  }

  template <typename Functor>
  inline auto invokeOnUnmarshalledTypedValue(const TypedValue &value,
                                             const Functor &functor) const {
    std::unique_ptr<cpptype> literal(
        static_cast<cpptype*>(unmarshallValue(value.getOutOfLineData(),
                                              value.getDataSize())));
    return functor(literal.get());
  }

  const std::vector<GenericValue> parameters_;

 private:
  struct ParametersHasher {
    inline static std::size_t ComputeHash(
        const std::vector<GenericValue> &parameters) {
      std::size_t hash_code = 0;
      for (const GenericValue &value : parameters) {
        hash_code = CombineHashes(hash_code, value.getHash());
      }
      return hash_code;
    }
    inline std::size_t operator()(
        const std::vector<GenericValue> &parameters) const {
      return ComputeHash(parameters);
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
          TypeSynthesizePolicy<type_id>::template CreateInstance<TypeClass>(
              nullable, parameters));
      imit = instance_map.emplace(parameters, std::move(instance)).first;
    }
    return *(imit->second);
  }
};


////////////////////////////////////////////////////////////////////////////////
///////////////////////////////  TypeSynthesizer  //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <TypeID type_id>
class TypeSynthesizer : public TypeSynthesizePolicy<type_id> {
 private:
  using Trait = TypeIDTrait<type_id>;
  using SynthesizePolicy = TypeSynthesizePolicy<type_id>;

 public:
  static constexpr SuperTypeID kStaticSuperTypeID = Trait::kStaticSuperTypeID;
  static constexpr TypeID kStaticTypeID = Trait::kStaticTypeID;
  static constexpr bool kIsParPod = Trait::kIsParPod;
  static constexpr MemoryLayout kMemoryLayout = Trait::kMemoryLayout;

  using TypeClass = typename Trait::TypeClass;
  using cpptype = typename Trait::cpptype;

  serialization::Type getProto() const override {
    serialization::Type proto;
    proto.mutable_type_id()->CopyFrom(TypeIDFactory::GetProto(Type::type_id_));
    proto.set_nullable(Type::nullable_);
    SynthesizePolicy::mergeIntoProto(&proto);
    return proto;
  }

  const Type& getNullableVersion() const override {
    return SynthesizePolicy::getInstance(true);
  }

  const Type& getNonNullableVersion() const override {
    return SynthesizePolicy::getInstance(false);
  }

  UntypedLiteral* unmarshallTypedValue(const TypedValue &value) const override {
    return SynthesizePolicy::unmarshallTypedValueInl(value);
  }

  std::string printTypedValueToString(const TypedValue &value) const override {
    return SynthesizePolicy::invokeOnUnmarshalledTypedValue(
        value,
        [&](const UntypedLiteral *value) -> std::string {
      return this->printValueToString(value);
    });
  }

  void printTypedValueToFile(const TypedValue &value,
                             FILE *file,
                             const int padding = 0) const override {
    SynthesizePolicy::invokeOnUnmarshalledTypedValue(
        value,
        [&](const UntypedLiteral *value) -> void {
      this->printValueToFile(value, file, padding);
    });
  }

  const cpptype& castValueToLiteral(const UntypedLiteral *value) const {
    return *static_cast<const cpptype*>(value);
  }

  cpptype& castValueToLiteral(UntypedLiteral *value) const {
    return *static_cast<cpptype*>(value);
  }

 protected:
  template <MemoryLayout layout = kMemoryLayout>
  explicit TypeSynthesizer(const bool nullable,
                           std::enable_if_t<layout == kCxxInlinePod>* = 0)
      : TypeSynthesizePolicy<type_id>(nullable) {
  }

  template <MemoryLayout layout = kMemoryLayout>
  TypeSynthesizer(const bool nullable,
                  const std::size_t minimum_byte_length,
                  const std::size_t maximum_byte_length,
                  const std::size_t parameter,
                  std::enable_if_t<layout == kParInlinePod ||
                                   layout == kParOutOfLinePod>* = 0)
      : TypeSynthesizePolicy<type_id>(nullable, minimum_byte_length,
                                      maximum_byte_length, parameter) {
  }

  template <MemoryLayout layout = kMemoryLayout>
  TypeSynthesizer(const bool nullable,
                  const std::size_t minimum_byte_length,
                  const std::size_t maximum_byte_length,
                  const std::vector<GenericValue> &parameters = {},
                  std::enable_if_t<layout == kCxxGeneric>* = 0)
      : TypeSynthesizePolicy<type_id>(nullable, minimum_byte_length,
                                      maximum_byte_length, parameters) {
  }

 private:
  template <TypeID, typename> friend class TypeSynthesizePolicy;

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


#define QUICKSTEP_SYNTHESIZE_TYPE(type) \
  template <TypeID, typename> friend class TypeSynthesizePolicy; \
  DISALLOW_COPY_AND_ASSIGN(type)

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPE_SYNTHESIZER_HPP_
