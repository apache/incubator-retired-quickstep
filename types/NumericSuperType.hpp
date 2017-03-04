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

#ifndef QUICKSTEP_TYPES_NUMERIC_SUPER_TYPE_HPP_
#define QUICKSTEP_TYPES_NUMERIC_SUPER_TYPE_HPP_

#include <cstddef>
#include <unordered_set>

#include "types/NullCoercibilityCheckMacro.hpp"
#include "types/NumericTypeSafeCoercibility.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypeRegistrar.hpp"
#include "types/TypeSynthesizer.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"
#include "utility/meta/TMP.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief Templatized superclass for Numeric types. Contains code common to all
 *        Numeric types.
 **/
template <TypeID type_id>
class NumericSuperType : public TypeSynthesizer<type_id> {
 public:
  bool isSafelyCoercibleFrom(const Type &original_type) const override {
    QUICKSTEP_NULL_COERCIBILITY_CHECK();
    const auto it = safe_coerce_cache_.find(original_type.getTypeID());
    return it != safe_coerce_cache_.end();
  }

  bool isCoercibleFrom(const Type &original_type) const override {
    QUICKSTEP_NULL_COERCIBILITY_CHECK();
    return (original_type.getSuperTypeID() == Type::kNumeric);
  }

  TypedValue makeZeroValue() const override {
    return TypedValue(static_cast<typename TypeIDTrait<type_id>::cpptype>(0));
  }

  TypedValue coerceValue(const TypedValue &original_value,
                         const Type &original_type) const override {
    if (original_type.getSuperTypeID() != Type::kNumeric) {
      LOG(FATAL) << "Attempted to coerce Type " << original_type.getName()
                 << " (not recognized as a numeric Type) to " << Type::getName();
    }

    if (original_value.isNull()) {
      return Type::makeNullValue();
    }

    return InvokeOnTypeID<TypeIDSelectorNumeric>(
        original_type.getTypeID(),
        [&](auto orig_tid) -> TypedValue {  // NOLINT(build/c++11)
      using OrigCppType = typename TypeIDTrait<decltype(orig_tid)::value>::cpptype;
      using TargetCppType = typename TypeIDTrait<type_id>::cpptype;

      return TypedValue(
          static_cast<TargetCppType>(original_value.getLiteral<OrigCppType>()));
    });
  }

 protected:
  explicit NumericSuperType(const bool nullable)
      : TypeSynthesizer<type_id>(nullable),
        safe_coerce_cache_(CreateSafeCoercibilityCache()) {}

 private:
  using TargetType = typename TypeIDTrait<type_id>::TypeClass;

  template <typename SourceTypeID>
  struct SafeCoercibilityFilter {
    static constexpr bool value =
        NumericTypeSafeCoercibility<
            typename TypeIDTrait<SourceTypeID::value>::TypeClass,
            TargetType>::value;
  };

  inline static auto CreateSafeCoercibilityCache() {
    using SourceTypeIDs = TypeIDSequenceAll::template bind_to<meta::TypeList>;
    using ResultTypeIDs = SourceTypeIDs::template filter<SafeCoercibilityFilter>;

    return ResultTypeIDs::template as_sequence<TypeID>
        ::template Instantiate<std::unordered_set<TypeID>>();
  };

  const std::unordered_set<TypeID> safe_coerce_cache_;

  DISALLOW_COPY_AND_ASSIGN(NumericSuperType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_NUMERIC_SUPER_TYPE_HPP_
