/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_UTILITY_CAST_HPP_
#define QUICKSTEP_UTILITY_CAST_HPP_

#include <memory>
#include <vector>

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Casts a vector of pointers stored in shared_ptr objects from const SourceType
 *        to const TargetType.
 *
 * @param source_ptrs The vector of shared_ptr objects to be casted.
 * @return target_ptrs The casted vector of shared_ptr with pointers of type const TargetType.
 */
template<class TargetType, class SourceType>
static std::vector<std::shared_ptr<const TargetType>> CastSharedPtrVector(
    const std::vector<std::shared_ptr<const SourceType>> &source_ptrs) {
  std::vector<std::shared_ptr<const TargetType>> target_ptrs;
  for (const std::shared_ptr<const SourceType> &ptr : source_ptrs) {
    target_ptrs.push_back(std::static_pointer_cast<const TargetType>(ptr));
  }
  return target_ptrs;
}

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_UTILITY_CAST_HPP_ */
