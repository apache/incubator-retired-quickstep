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

#ifndef QUICKSTEP_TYPES_GENERIC_VALUE_HPP_
#define QUICKSTEP_TYPES_GENERIC_VALUE_HPP_

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

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

class GenericValue {
 public:
  static GenericValue CreateNullValue(const Type &type) {
    return GenericValue(type, nullptr, true);
  }

  static GenericValue CreateWithOwnedData(const Type &type,
                                          const UntypedLiteral *value) {
    return GenericValue(type, value, true);
  }

  static GenericValue CreateReference(const Type &type,
                                      const UntypedLiteral *value) {
    return GenericValue(type, value, false);
  }

  static GenericValue CreateWithTypedValue(const Type &type,
                                           const TypedValue &value) {
    return GenericValue(type, type.unmarshallTypedValue(value), true);
  }

  template <typename TypeClass>
  static GenericValue CreateWithLiteral(const TypeClass &type,
                                        const typename TypeClass::cpptype &value) {
    return GenericValue(type, type.cloneValue(&value), true);
  }

  GenericValue(const GenericValue &other)
      : type_(other.type_),
        value_((other.owns_ && !other.isNull()) ? type_.cloneValue(other.value_)
                                                : other.value_),
        owns_(other.owns_) {}

  GenericValue(GenericValue &&other)
      : type_(other.type_),
        value_(other.value_),
        owns_(other.owns_) {
    other.owns_ = false;
  }

  ~GenericValue() {
    if (owns_ && !isNull()) {
      type_.destroyValue(const_cast<void*>(value_));
    }
  }

  serialization::GenericValue getProto() const {
    serialization::GenericValue proto;
    proto.mutable_type()->MergeFrom(type_.getProto());
    if (!isNull()) {
      TypedValue tv = type_.marshallValue(value_);
      proto.set_data(tv.getDataPtr(), tv.getDataSize());
    }
    return proto;
  }

  inline bool isNull() const {
    DCHECK(value_ != nullptr || type_.isNullable());
    return value_ == nullptr;
  }

  inline bool isReference() const {
    return !owns_;
  }

  inline const Type& getType() const {
    return type_;
  }

  inline TypeID getTypeID() const {
    return type_.getTypeID();
  }

  inline const UntypedLiteral* getValue() const {
    return value_;
  }

  template <TypeID type_id>
  inline const typename TypeIDTrait<type_id>::cpptype& getLiteral() const {
    DCHECK_EQ(type_id, type_.getTypeID());
    return *static_cast<const typename TypeIDTrait<type_id>::cpptype*>(value_);
  }

  inline void ensureNotReference() {
    if (isReference()) {
      if (!isNull()) {
        value_ = type_.cloneValue(value_);
      }
      owns_ = true;
    }
  }

  inline GenericValue makeReferenceToThis() const {
    return GenericValue(type_, value_, false);
  }

  inline bool equals(const GenericValue &other) const {
    if (isNull() || other.isNull()) {
      return isNull() && other.isNull();
    }
    return type_.checkValuesEqual(value_, other.value_, other.type_);
  }

  inline bool operator==(const GenericValue &other) const {
    return equals(other);
  }

  inline std::size_t getHash() const {
    return isNull() ? 0  : type_.hashValue(value_);
  }

  inline GenericValue coerce(const Type &other_type) const {
    return GenericValue(other_type,
                        other_type.coerceValue(value_, type_),
                        true /* take_ownership */);
  }

  inline TypedValue toTypedValue() const {
    return isNull() ? type_.makeNullValue() : type_.marshallValue(value_);
  }

  inline std::string toString() const {
    return isNull() ? "NULL" : type_.printValueToString(value_);
  }

 private:
  GenericValue(const Type &type,
               const UntypedLiteral *value,
               const bool take_ownership)
      : type_(type), value_(value), owns_(take_ownership) {}

  const Type &type_;
  const UntypedLiteral *value_;
  bool owns_;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_GENERIC_VALUE_HPP_
