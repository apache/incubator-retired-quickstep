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

#ifndef QUICKSTEP_UTILITY_PRIME_NUMBER_HPP_
#define QUICKSTEP_UTILITY_PRIME_NUMBER_HPP_

#include <cstddef>

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Find the least prime number that is greater than or equal to the
 *        argument.
 *
 * @param number The number to compute.
 * @return The least prime number greater than or equal to the input number.
 **/
std::size_t get_next_prime_number(std::size_t number);

/**
 * @brief Find the greatest prime number that is less than or equal to the
 *        argument.
 *
 * @param number The number to compute.
 * @return The greatest prime number less than or equal the input number, or 0
 *         if no such prime exists (i.e. if the input is 0 or 1).
 **/
std::size_t get_previous_prime_number(std::size_t number);

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_PRIME_NUMBER_HPP_
