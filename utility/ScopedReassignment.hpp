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

#ifndef QUICKSTEP_UTILITY_SCOPED_REASSIGNMENT_HPP_
#define QUICKSTEP_UTILITY_SCOPED_REASSIGNMENT_HPP_

#include <utility>
#include <type_traits>

#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief RAII helper object that assigns a new value to a variable and restores
 *        the old value when the helper object goes out of scope.
 **/
template <typename T>
class ScopedReassignment {
 public:
  /**
   * @brief Constructor.
   *
   * @param var The variable.
   * @param new_value The new value to be assigned to \p var.
   **/
  template <typename U>
  ScopedReassignment(T *var, U &&new_value)
      : var_(*var),
        old_value_(std::move(*var)) {
    *var = std::forward<U>(new_value);
  }

  /**
   * @brief Destructor. Restores the old value to \p var_.
   **/
  ~ScopedReassignment() {
    var_ = std::move(old_value_);
  }

  /**
   * @brief Get the old value.
   *
   * @return A const reference to the old value.
   */
  inline const T& old_value() const {
    return old_value_;
  }

 private:
  T &var_;
  T old_value_;

  DISALLOW_COPY_AND_ASSIGN(ScopedReassignment);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_SCOPED_REASSIGNMENT_HPP_
