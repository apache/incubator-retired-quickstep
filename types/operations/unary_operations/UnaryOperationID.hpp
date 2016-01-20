/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_UNARY_OPERATION_ID_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_UNARY_OPERATION_ID_HPP_

#include <type_traits>

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief Concrete UnaryOperations.
 **/
enum class UnaryOperationID {
  kNegate = 0,
  kCast,
  kDateExtract,
  kNumUnaryOperationIDs  // Not a real UnaryOperationID, exists for counting purposes.
};

/**
 * @brief Names of comparisons in the same order as UnaryOperationID.
 * @note Defined out-of-line in UnaryOperation.cpp
 **/
extern const char *kUnaryOperationNames[
    static_cast<typename std::underlying_type<UnaryOperationID>::type>(
        UnaryOperationID::kNumUnaryOperationIDs)];

/**
 * @brief Short names (i.e. mathematical symbols) of comparisons in the same
 *        order as UnaryOperationID.
 * @note Defined out-of-line in UnaryOperation.cpp
 **/
extern const char *kUnaryOperationShortNames[
    static_cast<typename std::underlying_type<UnaryOperationID>::type>(
        UnaryOperationID::kNumUnaryOperationIDs)];

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_UNARY_OPERATIONS_UNARY_OPERATION_ID_HPP_
