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

#ifndef QUICKSTEP_UTILITY_VECTOR_UTIL_HPP_
#define QUICKSTEP_UTILITY_VECTOR_UTIL_HPP_

#include <vector>

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Adds a new element \p item to a vector \p list if the element does not exist.
 *
 * @param item The element to be inserted to the vector.
 * @param vec The vector to be appended to.
 * @return True if the element is inserted.
 */
template <class Type>
bool AppendToVectorIfNotPresent(const Type &item, std::vector<Type> *vec) {
  for (const Type &exist_item : *vec) {
    if (exist_item == item) {
      return false;
    }
  }
  vec->push_back(item);
  return true;
}

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_UTILITY_VECTOR_UTIL_HPP_ */
