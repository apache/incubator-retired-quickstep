/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
