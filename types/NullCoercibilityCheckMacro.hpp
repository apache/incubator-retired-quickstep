/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_NULL_COERCIBILITY_CHECK_MACRO_HPP_
#define QUICKSTEP_TYPES_NULL_COERCIBILITY_CHECK_MACRO_HPP_

#include "types/Type.hpp"
#include "types/TypeID.hpp"

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A code-snippet for use in implementations of Type::isCoercibleFrom()
 *        and Type::isSafelyCoercibleFrom() that does common checks for
 *        nullability of types.
 **/
#define QUICKSTEP_NULL_COERCIBILITY_CHECK()               \
  do {                                                    \
    if (original_type.isNullable() && !nullable_) {       \
      return false;                                       \
    } else if (original_type.getTypeID() == kNullType) {  \
      return true;                                        \
    }                                                     \
  } while (false)

/** @} */

#endif  // QUICKSTEP_TYPES_NULL_COERCIBILITY_CHECK_MACRO_HPP_
