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

#ifndef QUICKSTEP_QUERY_EXECUTION_FOREMAN_BASE_HPP_
#define QUICKSTEP_QUERY_EXECUTION_FOREMAN_BASE_HPP_

#include <cstdio>
#include <memory>
#include <vector>

#include "query_execution/PolicyEnforcerBase.hpp"
#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"

namespace quickstep {

struct WorkOrderTimeEntry;

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief A base class that Foreman implements. This class is used to derive
 *        for implementations for both the single-node and distributed versions.
 **/
class ForemanBase : public Thread {
 public:
  /**
   * @brief Constructor.
   *
   * @param bus A pointer to the TMB.
   * @param cpu_id The ID of the CPU to which the Foreman thread can be pinned.
   *
   * @note If cpu_id is not specified, Foreman thread can be possibly moved
   *       around on different CPUs by the OS.
  **/
  ForemanBase(tmb::MessageBus *bus,
              const int cpu_id)
      : bus_(DCHECK_NOTNULL(bus)),
        cpu_id_(cpu_id) {
    foreman_client_id_ = bus_->Connect();
  }

  ~ForemanBase() override {}

  /**
   * @brief Print the results of profiling individual work orders for a given
   *        query.
   *
   * TODO(harshad) - Add the name of the operator to the output.
   *
   * @param query_id The ID of the query for which the results are to be printed.
   * @param out The file stream.
   **/
  virtual void printWorkOrderProfilingResults(const std::size_t query_id,
                                              std::FILE *out) const = 0;

  /**
   * @brief Get the results of profiling individual work orders for a given
   *        query.
   *
   * @param query_id The ID of the query for which the results are to be printed.
   * @return A vector of records, each being a single profiling entry.
   **/
  const std::vector<WorkOrderTimeEntry>& getWorkOrderProfilingResults(
      const std::size_t query_id) const {
    return policy_enforcer_->getProfilingResults(query_id);
  }

  /**
   * @brief Get the TMB client ID of Foreman thread.
   *
   * @return TMB client ID of foreman thread.
   **/
  tmb::client_id getBusClientID() const {
    return foreman_client_id_;
  }

 protected:
  void run() override = 0;

  tmb::MessageBus *bus_;

  tmb::client_id foreman_client_id_;

  // The ID of the CPU that the Foreman thread can optionally be pinned to.
  const int cpu_id_;

  std::unique_ptr<PolicyEnforcerBase> policy_enforcer_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ForemanBase);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_FOREMAN_BASE_HPP_
