/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_NUMERIC_SUPER_TYPE_HPP_
#define QUICKSTEP_TYPES_NUMERIC_SUPER_TYPE_HPP_

#include <cstddef>

#include "types/NullCoercibilityCheckMacro.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief Templatized superclass for Numeric types. Contains code common to all
 *        Numeric types.
 **/
template <typename CppType>
class NumericSuperType : public Type {
 public:
  typedef CppType cpptype;

  std::size_t estimateAverageByteLength() const override {
    return sizeof(CppType);
  }

  bool isCoercibleFrom(const Type &original_type) const override {
    QUICKSTEP_NULL_COERCIBILITY_CHECK();
    return (original_type.getSuperTypeID() == kNumeric);
  }

  TypedValue makeZeroValue() const override {
    return TypedValue(static_cast<CppType>(0));
  }

 protected:
  NumericSuperType(const TypeID type_id, const bool nullable)
      : Type(Type::kNumeric, type_id, nullable, sizeof(CppType), sizeof(CppType)) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(NumericSuperType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_NUMERIC_SUPER_TYPE_HPP_
