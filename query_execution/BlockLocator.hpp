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

#ifndef QUICKSTEP_QUERY_EXECUTION_BLOCK_LOCATOR_HPP_
#define QUICKSTEP_QUERY_EXECUTION_BLOCK_LOCATOR_HPP_

#include <atomic>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "query_execution/QueryExecutionTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
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
 * @brief A class for keeping trace of blocks loaded in a Worker's buffer pool
 *        in the distributed version.
 **/
class BlockLocator : public Thread {
 public:
  /**
   * @brief Constructor.
   *
   * @param bus A pointer to the TMB.
   * @param cpu_id The ID of the CPU to which the BlockLocator thread can be pinned.
   *
   * @note If cpu_id is not specified, BlockLocator thread can be possibly moved
   *       around on different CPUs by the OS.
  **/
  BlockLocator(tmb::MessageBus *bus,
               const int cpu_id = -1)
      : bus_(DCHECK_NOTNULL(bus)),
        cpu_id_(cpu_id),
        block_domain_(0) {
    locator_client_id_ = bus_->Connect();

    bus_->RegisterClientAsReceiver(locator_client_id_, kBlockDomainRegistrationMessage);
    bus_->RegisterClientAsSender(locator_client_id_, kBlockDomainRegistrationResponseMessage);

    bus_->RegisterClientAsReceiver(locator_client_id_, kAddBlockLocationMessage);
    bus_->RegisterClientAsReceiver(locator_client_id_, kDeleteBlockLocationMessage);

    bus_->RegisterClientAsReceiver(locator_client_id_, kLocateBlockMessage);
    bus_->RegisterClientAsSender(locator_client_id_, kLocateBlockResponseMessage);

    bus_->RegisterClientAsReceiver(locator_client_id_, kGetPeerDomainNetworkAddressesMessage);
    bus_->RegisterClientAsSender(locator_client_id_, kGetPeerDomainNetworkAddressesResponseMessage);

    bus_->RegisterClientAsReceiver(locator_client_id_, kBlockDomainUnregistrationMessage);
    bus_->RegisterClientAsReceiver(locator_client_id_, kPoisonMessage);
  }

  ~BlockLocator() override {}

  /**
   * @brief Get the TMB client ID of BlockLocator thread.
   *
   * @return TMB client ID of BlockLocator thread.
   **/
  tmb::client_id getBusClientID() const {
    return locator_client_id_;
  }

 protected:
  void run() override;

 private:
  void processBlockDomainRegistrationMessage(const tmb::client_id receiver, const std::string &network_address);
  void processLocateBlockMessage(const tmb::client_id receiver, const block_id block);
  void processGetPeerDomainNetworkAddressesMessage(const tmb::client_id receiver, const block_id block);

  tmb::MessageBus *bus_;

  // The ID of the CPU that the BlockLocator thread can optionally be pinned to.
  const int cpu_id_;

  alignas(kCacheLineBytes) std::atomic<block_id_domain> block_domain_;

  // From a block domain to its network info in the ip:port format, i.e.,
  // "0.0.0.0:0".
  std::unordered_map<block_id_domain, const std::string> domain_network_addresses_;

  // From a block to its domains.
  std::unordered_map<block_id, std::unordered_set<block_id_domain>> block_locations_;

  // From a block domain to all blocks loaded in its buffer pool.
  std::unordered_map<block_id_domain, std::unordered_set<block_id>> domain_blocks_;

  tmb::client_id locator_client_id_;

  DISALLOW_COPY_AND_ASSIGN(BlockLocator);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_BLOCK_LOCATOR_HPP_
