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

#ifndef QUICKSTEP_TYPES_ASCII_STRING_SUPER_TYPE_HPP_
#define QUICKSTEP_TYPES_ASCII_STRING_SUPER_TYPE_HPP_

#include <cstddef>

#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypeSynthesizer.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A superclass for ASCII string types.
 **/
template <TypeID type_id>
class AsciiStringSuperType : public TypeSynthesizer<type_id> {
 public:
  /**
   * @brief Get the character-length of this string type.
   *
   * @return The maximum length of a string of this type.
   **/
  inline std::size_t getStringLength() const {
    return TypeSynthesizer<type_id>::length_;
  }

 protected:
  AsciiStringSuperType(const bool nullable,
                       const std::size_t minimum_byte_length,
                       const std::size_t maximum_byte_length,
                       const std::size_t string_length)
      : TypeSynthesizer<type_id>(
            nullable, minimum_byte_length, maximum_byte_length, string_length) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(AsciiStringSuperType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_ASCII_STRING_SUPER_TYPE_HPP_
