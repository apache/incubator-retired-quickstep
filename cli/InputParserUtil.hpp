/**
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

#ifndef QUICKSTEP_CLI_INPUT_PARSER_UTIL_HPP_
#define QUICKSTEP_CLI_INPUT_PARSER_UTIL_HPP_

#include <string>
#include <vector>

#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup CLI
 *  @{
 */

/**
 * @brief A static utility class for parsing input to Quickstep CLI.
 **/
class InputParserUtil {
 public:
  /**
   * @brief Parse an input string consisting of worker thread's CPU core
   *        affinities.
   *
   * @param num_workers The number of worker threads.
   * @param affinity_string A string consisting of the worker thread affinities.
   *
   * @note Check cli/QuickstepCli.cpp for the description on the format of the
   *       affinity_string.
   *
   * @return A vector of CPU cores to which workers are affinitized. If no
   *         information is available for a worker, we assign -1.
   **/
  static std::vector<int> ParseWorkerAffinities(
      const int num_workers,
      const std::string &affinity_string);

  /**
   * @brief Get NUMA nodes of all the CPUs.
   *
   * @note This function is only relevant when libnuma is present.
   *
   * @return A vector where ith element is the NUMA node of ith CPU, starting
   *         from 0.
   **/
  static std::vector<int> GetNUMANodesForCPUs();

 private:
  /**
   * @brief Private constructor to disable instantiation of the class.
   **/
  InputParserUtil();

  DISALLOW_COPY_AND_ASSIGN(InputParserUtil);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_INPUT_PARSER_UTIL_HPP_
