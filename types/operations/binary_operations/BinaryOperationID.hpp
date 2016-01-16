/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_BINARY_OPERATION_ID_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_BINARY_OPERATION_ID_HPP_

#include <type_traits>

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief Concrete BinaryOperations.
 **/
enum class BinaryOperationID {
  kAdd = 0,
  kSubtract,
  kMultiply,
  kDivide,
  kNumBinaryOperationIDs  // Not a real BinaryOperationID, exists for counting purposes.
};

/**
 * @brief Names of comparisons in the same order as BinaryOperationID.
 **/
extern const char *kBinaryOperationNames[
    static_cast<typename std::underlying_type<BinaryOperationID>::type>(
        BinaryOperationID::kNumBinaryOperationIDs)];

/**
 * @brief Short names (i.e. mathematical symbols) of comparisons in the same
 *        order as BinaryOperationID.
 **/
extern const char *kBinaryOperationShortNames[
    static_cast<typename std::underlying_type<BinaryOperationID>::type>(
        BinaryOperationID::kNumBinaryOperationIDs)];

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_BINARY_OPERATION_ID_HPP_
