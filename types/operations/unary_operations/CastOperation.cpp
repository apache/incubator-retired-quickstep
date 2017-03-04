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

#include "types/operations/unary_operations/CastOperation.hpp"

#include <algorithm>
#include <map>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

#include "types/CharType.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/Type.hpp"
#include "types/TypeUtil.hpp"
#include "types/TypedValue.hpp"
#include "types/VarCharType.hpp"
#include "types/operations/unary_operations/UnaryOperationWrapper.hpp"
#include "types/port/strnlen.hpp"
#include "utility/EqualsAnyConstant.hpp"
#include "utility/StringUtil.hpp"

namespace quickstep {

namespace {

template <typename ArgumentT, typename ResultT>
struct NumericCastToNumericFunctor
    : public UnaryFunctor<ArgumentT, ResultT> {
  inline typename ResultT::cpptype apply(
      const typename ArgumentT::cpptype &argument) const {
    return static_cast<typename ResultT::cpptype>(argument);
  }
};

template <typename ArgumentT, typename ResultT>
class CastToAsciiStringFunctor : public UnaryFunctor<ArgumentT, ResultT> {
 public:
  explicit CastToAsciiStringFunctor(const ArgumentT &argument_type,
                                    const std::size_t max_string_length)
      : argument_type_(argument_type),
        max_string_length_(max_string_length) {}

  inline void apply(const typename ArgumentT::cpptype &argument, void *result) const {
    std::string str = argument_type_.printValueToString(TypedValue(argument));
    const std::size_t str_len = str.length();

    if (str_len < max_string_length_) {
      std::memcpy(result, str.c_str(), str_len);
      static_cast<char *>(result)[str_len] = 0;
    } else {
      std::memcpy(result, str.c_str(), max_string_length_);
    }
  }

  inline TypedValue apply(const typename ArgumentT::cpptype &argument) const {
    std::string str = argument_type_.printValueToString(TypedValue(argument));
    const std::size_t len = std::min(str.length(), max_string_length_);
    const std::size_t buf_len = len + 1;

    char *buf = static_cast<char *>(std::malloc(buf_len));
    std::memcpy(buf, str.c_str(), len);
    buf[len] = 0;
    return TypedValue::CreateWithOwnedData(kVarChar, buf, buf_len);
  }

 private:
  const ArgumentT &argument_type_;
  const std::size_t max_string_length_;
};

template <typename ResultCppType>
ResultCppType CastStringToNumericImpl(const char *str);

template <>
bool CastStringToNumericImpl(const char *str) {
  const std::string lo_str = ToLower(str);
  if (lo_str == "true") {
    return true;
  } else {
    return false;
  }
}
template <>
int CastStringToNumericImpl(const char *str) {
  return std::atoi(str);
}
template <>
float CastStringToNumericImpl(const char *str) {
  return static_cast<float>(std::atof(str));
}
template <>
std::int64_t CastStringToNumericImpl(const char *str) {
  return std::atoll(str);
}
template <>
double CastStringToNumericImpl(const char *str) {
  return std::atof(str);
}

template <typename ArgumentT, typename ResultT,
          typename ResultT::cpptype f(const char*)>
struct AsciiStringCastToNumericFunctor
    : public UnaryFunctor<ArgumentT, ResultT> {
  explicit AsciiStringCastToNumericFunctor(const std::size_t max_string_length)
      : max_string_length_(max_string_length) {}

  inline typename ResultT::cpptype apply(const TypedValue &argument) const {
    return f(static_cast<const char*>(argument.getDataPtr()));
  }

  inline typename ResultT::cpptype apply(const void *argument) const {
    const char *str = static_cast<const char*>(argument);
    const std::string value(str, strnlen(str, max_string_length_));
    return f(value.c_str());
  }

 private:
  const std::size_t max_string_length_;
};

template <typename ArgumentT, typename ResultT>
struct AsciiStringCastToAsciiStringFunctor
    : public UnaryFunctor<ArgumentT, ResultT> {
  explicit AsciiStringCastToAsciiStringFunctor(const std::size_t max_string_length)
      : max_string_length_(max_string_length) {}

  inline void apply(const void *argument, void *result) const {
    std::memcpy(result, argument, max_string_length_);
  }

  inline void apply(const TypedValue &argument, void *result) const {
    std::memcpy(result,
                argument.getOutOfLineData(),
                std::min(argument.getDataSize(), max_string_length_));
  }

  inline TypedValue apply(const void *argument) const {
    const std::size_t len =
        strnlen(static_cast<const char*>(argument), max_string_length_);

    char *buf = static_cast<char *>(std::malloc(len+1));
    std::memcpy(buf, argument, len);
    buf[len] = 0;
    return TypedValue::CreateWithOwnedData(kVarChar, buf, len+1);
  }

  inline TypedValue apply(const TypedValue &argument) const {
    const std::size_t len =
        std::min(argument.getDataSize() - 1, max_string_length_);

    char *buf = static_cast<char *>(std::malloc(len+1));
    std::memcpy(buf, argument.getDataPtr(), len);
    buf[len] = 0;
    return TypedValue::CreateWithOwnedData(kVarChar, buf, len+1);
  }

 private:
  const std::size_t max_string_length_;
};

}  // namespace

const re2::RE2 CastOperation::kTypePattern("([a-z]+)(\\(([0-9]+)\\))?");

const std::map<std::string, TypeID> CastOperation::kNameToTypeIDMap = {
    { "bool",    kBool },
    { "int",     kInt },
    { "long",    kLong },
    { "float",   kFloat },
    { "double",  kDouble },
    { "char",    kChar },
    { "varchar", kVarChar }
};

UncheckedUnaryOperator* CastOperation::makeUncheckedUnaryOperator(
    const Type &type,
    const std::vector<TypedValue> &static_arguments) const {
  const Type *result_type = getResultType(type, static_arguments);
  DCHECK(result_type != nullptr);

  const TypeID argument_type_id = type.getTypeID();
  const TypeID result_type_id = result_type->getTypeID();

  if (QUICKSTEP_EQUALS_ANY_CONSTANT(argument_type_id, kBool, kInt, kLong, kFloat, kDouble)) {
    return InvokeOnTypeID<TypeIDSelectorNumeric>(
        argument_type_id,
        [&](auto arg_tid) -> UncheckedUnaryOperator* {  // NOLINT(build/c++11)
      using ArgumentT = typename TypeIDTrait<decltype(arg_tid)::value>::TypeClass;

      switch (result_type_id) {
        case kBool:  // Fall through
        case kInt:
        case kLong:
        case kFloat:
        case kDouble: {
          return InvokeOnTypeID<TypeIDSelectorNumeric>(
              result_type_id,
              [&](auto result_tid) -> UncheckedUnaryOperator* {  // NOLINT(build/c++11)
            using ResultT = typename TypeIDTrait<decltype(result_tid)::value>::TypeClass;

            return new UncheckedUnaryOperatorWrapperCodegen<
                NumericCastToNumericFunctor<ArgumentT, ResultT>>(type, *result_type);
          });
        }
        case kChar:  // Fall through
        case kVarChar: {
          return InvokeOnTypeID<TypeIDSelectorEqualsAny<kChar, kVarChar>>(
              result_type_id,
              [&](auto result_tid) -> UncheckedUnaryOperator* {  // NOLINT(build/c++11)
            using ResultT = typename TypeIDTrait<decltype(result_tid)::value>::TypeClass;

            return new UncheckedUnaryOperatorWrapperCodegen<
                 CastToAsciiStringFunctor<ArgumentT, ResultT>>(
                     type, *result_type,
                     static_cast<const ArgumentT&>(type),
                     static_cast<const ResultT*>(result_type)->getStringLength());
          });
        }
        default:
          LOG(FATAL) << "Unexpected result type " << result_type->getName()
                     << " in CastOperation::makeUncheckedUnaryOperator "
                     << "for argument type " << type.getName();
      }
    });
  } else if (QUICKSTEP_EQUALS_ANY_CONSTANT(argument_type_id, kChar, kVarChar)) {
    return InvokeOnTypeID<TypeIDSelectorEqualsAny<kChar, kVarChar>>(
        argument_type_id,
        [&](auto arg_tid) -> UncheckedUnaryOperator* {  // NOLINT(build/c++11)
      using ArgumentT = typename TypeIDTrait<decltype(arg_tid)::value>::TypeClass;

      switch (result_type_id) {
        case kBool:  // Fall through
        case kInt:
        case kLong:
        case kFloat:
        case kDouble: {
          return InvokeOnTypeID<TypeIDSelectorNumeric>(
              result_type_id,
              [&](auto result_tid) -> UncheckedUnaryOperator* {  // NOLINT(build/c++11)
            using ResultT = typename TypeIDTrait<decltype(result_tid)::value>::TypeClass;

            return new UncheckedUnaryOperatorWrapperCodegen<
                AsciiStringCastToNumericFunctor<
                    ArgumentT, ResultT,
                    CastStringToNumericImpl<typename ResultT::cpptype>>>(
                        type, *result_type,
                        static_cast<const ArgumentT&>(type).getStringLength());
          });
        }
        case kChar:  // Fall through
        case kVarChar: {
          return InvokeOnTypeID<TypeIDSelectorEqualsAny<kChar, kVarChar>>(
              result_type_id,
              [&](auto result_tid) -> UncheckedUnaryOperator* {  // NOLINT(build/c++11)
            using ResultT = typename TypeIDTrait<decltype(result_tid)::value>::TypeClass;

            return new UncheckedUnaryOperatorWrapperCodegen<
                 AsciiStringCastToAsciiStringFunctor<ArgumentT, ResultT>>(
                     type, *result_type,
                     std::min(static_cast<const ArgumentT&>(type).getStringLength(),
                              static_cast<const ResultT*>(result_type)->getStringLength()));
          });
        }
        default:
          LOG(FATAL) << "Unexpected result type " << result_type->getName()
                     << " in CastOperation::makeUncheckedUnaryOperator "
                     << "for argument type " << type.getName();
      }
    });
  }

  LOG(FATAL) << "Unexpected argument type in "
             << "CastOperation::makeUncheckedUnaryOperator: "
             << result_type->getName();
}

}  // namespace quickstep
