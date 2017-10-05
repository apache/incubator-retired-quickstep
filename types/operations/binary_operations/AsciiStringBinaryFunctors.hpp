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

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ASCII_STRING_BINARY_FUNCTORS_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ASCII_STRING_BINARY_FUNCTORS_HPP_

#include <cctype>
#include <cstring>
#include <string>

#include "types/CharType.hpp"
#include "types/IntType.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/VarCharType.hpp"
#include "types/operations/unary_operations/UnaryOperationWrapper.hpp"
#include "types/operations/utility/OperationSynthesizeUtil.hpp"
#include "types/port/strnlen.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <typename LeftT, typename RightT, typename ResultT>
struct AsciiStringConcatFunctor : public BinaryFunctor<LeftT, RightT, ResultT> {
  explicit AsciiStringConcatFunctor(const LeftT &left, const RightT &right)
      : left_max_(left.getStringLength()),
        right_max_(right.getStringLength()),
        result_max_(left_max_ + right_max_) {}
  inline void concat(const void *left, const std::size_t l_len,
                     const void *right, const std::size_t r_len,
                     void *result) const {
    char *result_str = static_cast<char*>(result);
    std::memcpy(result_str, left, l_len);
    std::memcpy(result_str + l_len, right, r_len);

    const std::size_t result_len = l_len + r_len;
    if (ResultT::kStaticTypeID == kVarChar || result_len < result_max_) {
      result_str[result_len] = 0;
    }
  }
  inline void apply(const void *left, const void *right, void *result) const {
    concat(left, strnlen(static_cast<const char*>(left), left_max_),
           right, strnlen(static_cast<const char*>(right), right_max_),
           result);
  }
  inline TypedValue apply(const void *left, const TypedValue &right) const {
    const std::size_t l_len =
        strnlen(static_cast<const char*>(left), left_max_);
    const std::size_t r_len =
        std::strlen(static_cast<const char*>(right.getOutOfLineData()));
    const std::size_t buf_len = l_len + r_len + 1;
    char *buf = static_cast<char*>(std::malloc(buf_len));
    concat(left, l_len, right.getOutOfLineData(), r_len, buf);
    return TypedValue::CreateWithOwnedData(kVarChar, buf, buf_len);
  }
  inline TypedValue apply(const TypedValue &left, const void *right) const {
    const std::size_t l_len =
        std::strlen(static_cast<const char*>(left.getOutOfLineData()));
    const std::size_t r_len =
        strnlen(static_cast<const char*>(right), right_max_);;
    const std::size_t buf_len = l_len + r_len + 1;
    char *buf = static_cast<char*>(std::malloc(buf_len));
    concat(left.getOutOfLineData(), l_len, right, r_len, buf);
    return TypedValue::CreateWithOwnedData(kVarChar, buf, buf_len);
  }
  inline TypedValue apply(const TypedValue &left, const TypedValue &right) const {
    const std::size_t l_len =
        std::strlen(static_cast<const char*>(left.getOutOfLineData()));
    const std::size_t r_len =
        std::strlen(static_cast<const char*>(right.getOutOfLineData()));
    const std::size_t buf_len = l_len + r_len + 1;
    char *buf = static_cast<char*>(std::malloc(buf_len));
    concat(left.getOutOfLineData(), l_len, right.getOutOfLineData(), r_len, buf);
    return TypedValue::CreateWithOwnedData(kVarChar, buf, buf_len);
  }
  inline static std::string GetName() {
    return "+";
  }
  inline static const Type* GetResultType(const Type &left, const Type &right) {
    DCHECK(left.getTypeID() == LeftT::kStaticTypeID);
    DCHECK(right.getTypeID() == RightT::kStaticTypeID);
    const std::size_t result_len =
        static_cast<const LeftT&>(left).getStringLength() +
            static_cast<const RightT&>(right).getStringLength();
    const bool is_nullable = left.isNullable() || right.isNullable();
    if (LeftT::kStaticTypeID == kChar && RightT::kStaticTypeID == kChar) {
      return &TypeFactory::GetType(kChar, result_len, is_nullable);
    } else {
      return &TypeFactory::GetType(kVarChar, result_len, is_nullable);
    }
  }
  const std::size_t left_max_;
  const std::size_t right_max_;
  const std::size_t result_max_;
};


using AsciiStringBinaryFunctorPack = FunctorPack<
// concat
    AsciiStringConcatFunctor<CharType, CharType, CharType>,
    AsciiStringConcatFunctor<CharType, VarCharType, VarCharType>,
    AsciiStringConcatFunctor<VarCharType, CharType, VarCharType>,
    AsciiStringConcatFunctor<VarCharType, VarCharType, VarCharType>
>;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_ASCII_STRING_BINARY_FUNCTORS_HPP_
