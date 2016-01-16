/**
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
