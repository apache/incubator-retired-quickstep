/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
