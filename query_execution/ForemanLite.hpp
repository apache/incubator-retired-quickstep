/**
 *   Copyright 2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_QUERY_EXECUTION_FOREMAN_LITE_HPP_
#define QUICKSTEP_QUERY_EXECUTION_FOREMAN_LITE_HPP_

#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"

namespace quickstep {

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief A base class that Foreman implements. This class is used to derive
 *        for implementations for both the single-node and distributed versions.
 **/
class ForemanLite : public Thread {
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
  ForemanLite(tmb::MessageBus *bus,
              const int cpu_id)
      : bus_(DCHECK_NOTNULL(bus)),
        cpu_id_(cpu_id) {
    foreman_client_id_ = bus_->Connect();
  }

  ~ForemanLite() override {}

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

 private:
  DISALLOW_COPY_AND_ASSIGN(ForemanLite);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_FOREMAN_LITE_HPP_
