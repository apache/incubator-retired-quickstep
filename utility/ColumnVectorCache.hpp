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

#ifndef QUICKSTEP_UTILITY_COLUMN_VECTOR_CACHE_HPP_
#define QUICKSTEP_UTILITY_COLUMN_VECTOR_CACHE_HPP_

#include <unordered_map>

#include "types/containers/ColumnVector.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief A memoization table for column vectors parameterized on integer IDs.
 **/
class ColumnVectorCache {
 public:
  /**
   * @brief Constructor.
   */
  ColumnVectorCache() {}

  /**
   * @brief Check whether this cache contains the column vector parameterized on
   *        the given ID.
   *
   * @param share_id The integer ID for the column vector.
   * @return True if this cache contains the column vector; false otherwise.
   */
  inline bool contains(const int share_id) const {
    return cv_cache_.find(share_id) != cv_cache_.end();
  }

  /**
   * @brief Get the cached column vector parameterized on the given ID.
   *
   * @param share_id The integer ID for the column vector.
   * @return The cached column vector.
   */
  inline ColumnVectorPtr get(const int share_id) const {
    DCHECK(contains(share_id));
    return cv_cache_.at(share_id);
  }

  /**
   * @brief Cache the column vector with the given ID as parameter.
   *
   * @param share_id The integer ID for the column vector.
   * @param cv The column vector to be cached.
   */
  inline void set(const int share_id, const ColumnVectorPtr &cv) {
    DCHECK(!contains(share_id));
    cv_cache_.emplace(share_id, cv);
  }

 private:
  std::unordered_map<int, ColumnVectorPtr> cv_cache_;

  DISALLOW_COPY_AND_ASSIGN(ColumnVectorCache);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_COLUMN_VECTOR_CACHE_HPP_
