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

#ifndef QUICKSTEP_UTILITY_NETWORK_UTIL_HPP_
#define QUICKSTEP_UTILITY_NETWORK_UTIL_HPP_

#include <string>

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

constexpr char kLocalIpv4Address[] = "127.0.0.1";

/**
 * @brief Get the IPv4 network address in the text format, i.e., "127.0.0.1".
 */
extern std::string GetIpv4Address();

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_NETWORK_UTIL_HPP_
