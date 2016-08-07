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
