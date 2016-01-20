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
