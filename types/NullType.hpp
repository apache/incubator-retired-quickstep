/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_NULLTYPE_HPP_
#define QUICKSTEP_TYPES_NULLTYPE_HPP_

#include <cstddef>
#include <cstdio>
#include <string>

#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class TypedValue;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A special type that can ONLY represent NULL values and can be coerced
 *        to any other nullable Type. It is used in the expression system and
 *        optimizer to represent NULL literals that appear in queries where
 *        a Type hint can not be immediately deduced from context and schema.
 * @note NullType can not be used as an argument type for any operations or
 *       comparisons, however it can be implicitly cast to whatever input type
 *       a particular operation may accept. It is also assumed that applying
 *       any operation to an argument of NullType always yields NULL values.
 **/
class NullType : public Type {
 public:
  static const TypeID kStaticTypeID = kNullType;

  /**
   * @brief Get a reference to the nullable singleton instance of this Type.
   * @note Unlike other Types, there is no corresponding method to get a
   *       non-nullable version of NullType. NullType is ALWAYS nullable.
   *
   * @return A reference to the nullable singleton instance of this Type.
   **/
  static const NullType& InstanceNullable() {
    static NullType instance;
    return instance;
  }

  const Type& getNullableVersion() const override {
    return InstanceNullable();
  }

  const Type& getNonNullableVersion() const override {
    LOG(FATAL) << "Called NullType::getNonNullableVersion(), which is not allowed.";
  }

  std::size_t estimateAverageByteLength() const override {
    return 0;
  }

  bool isCoercibleFrom(const Type &original_type) const override {
    return original_type.getTypeID() == kNullType;
  }

  bool isSafelyCoercibleFrom(const Type &original_type) const override {
    return original_type.getTypeID() == kNullType;
  }

  int getPrintWidth() const override {
    return 0;
  }

  std::string printValueToString(const TypedValue &value) const override {
    LOG(FATAL) << "NullType is not printable";
  }

  void printValueToFile(const TypedValue &value,
                        FILE *file,
                        const int padding = 0) const override {
    LOG(FATAL) << "NullType is not printable";
  }

  bool parseValueFromString(const std::string &value_string,
                            TypedValue *value) const override {
    return false;
  }

 private:
  // NOTE(chasseur): NullType requires 0 bytes of inherent storage. It does,
  // however, require a bit in NULL bitmaps.
  NullType() : Type(Type::kOther, kNullType, true, 0, 0) {
  }

  DISALLOW_COPY_AND_ASSIGN(NullType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_NULLTYPE_HPP_
