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

#ifndef QUICKSTEP_UTILITY_SYSTEM_CALCULATE_INSTALLED_MEMORY_HPP_
#define QUICKSTEP_UTILITY_SYSTEM_CALCULATE_INSTALLED_MEMORY_HPP_

#include <cstdint>

namespace quickstep {
namespace utility {
namespace system {

/**
 * @brief Compute the amount of (DRAM) memory installed on the machine.
 * @param total_memory Returns the total installed system memory in bytes.
 * @return true on success, false otherwise.
 */
bool calculateTotalMemoryInBytes(std::uint64_t *total_memory);

}  // namespace system
}  // namespace utility
}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_SYSTEM_CALCULATE_INSTALLED_MEMORY_HPP_
