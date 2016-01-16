/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
