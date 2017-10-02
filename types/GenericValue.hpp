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
  GenericValue(const Type &type)
      : type_(type), value_(nullptr), owns_(true) {}

  GenericValue(const Type &type, const UntypedLiteral *value, const bool owns)
      : type_(type), value_(value), owns_(owns) {}

  GenericValue(const Type &type, const TypedValue &value)
      : type_(type), value_(type.unmarshallTypedValue(value)), owns_(true) {}

  template <typename TypeClass>
  GenericValue(const TypeClass &type, const typename TypeClass::cpptype &value)
      : type_(type), value_(type.cloneValue(&value)), owns_(true) {}

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
    LOG(FATAL) << "Not implemented";
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
    LOG(FATAL) << "Not implemented";
  }

  inline TypedValue toTypedValue() const {
    return isNull() ? type_.makeNullValue() : type_.marshallValue(value_);
  }

  inline std::string toString() const {
    return isNull() ? "NULL" : type_.printValueToString(value_);
  }

 private:
  const Type &type_;
  const UntypedLiteral *value_;
  bool owns_;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_GENERIC_VALUE_HPP_
