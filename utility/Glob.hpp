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

#ifndef QUICKSTEP_UTILITY_GLOB_HPP_
#define QUICKSTEP_UTILITY_GLOB_HPP_

#include <cstddef>
#include <functional>
#include <string>
#include <vector>

namespace quickstep {
namespace utility {
namespace file {

/**
 * @brief Execute functor for each file/directory that matches the glob pattern.
 * @param pattern Glob pattern to use.
 * @param functor Functor to execute for each file/directory matching the
 *                pattern. Functor takes constant char pointer to null
 *                terminated string.
 * @return Number of files/directories matching the glob pattern.
 */
std::size_t GlobForEach(const std::string &pattern,
                        std::function<void(const char *)> functor);

/**
 * @brief List files that match file pattern using glob-like utility.
 * @param pattern File pattern to expand.
 * @return Vector of files/folders that match the specifed pattern.
 */
std::vector<std::string> GlobExpand(const std::string &pattern);

}  // namespace file
}  // namespace utility
}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_GLOB_HPP_
