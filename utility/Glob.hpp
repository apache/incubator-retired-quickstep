/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
