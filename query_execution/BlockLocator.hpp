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
#include <cstddef>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "query_execution/QueryExecutionTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "threading/SpinSharedMutex.hpp"
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

    bus_->RegisterClientAsReceiver(locator_client_id_, kBlockDomainToShiftbossIndexMessage);

    bus_->RegisterClientAsReceiver(locator_client_id_, kAddBlockLocationMessage);
    bus_->RegisterClientAsReceiver(locator_client_id_, kDeleteBlockLocationMessage);

    bus_->RegisterClientAsReceiver(locator_client_id_, kGetAllDomainNetworkAddressesMessage);
    bus_->RegisterClientAsSender(locator_client_id_, kGetAllDomainNetworkAddressesResponseMessage);

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

  /**
   * @brief Get the block domain info for a given block.
   *
   * @param block The given block.
   *
   * @return The block domain info for a given block.
   **/
  std::unordered_set<block_id_domain> getBlockDomains(const block_id block) const {
    {
      // Lock 'block_locations_shared_mutex_' as briefly as possible as a
      // reader.
      SpinSharedMutexSharedLock<false> read_lock(block_locations_shared_mutex_);
      const auto cit = block_locations_.find(block);
      if (cit != block_locations_.end()) {
        return cit->second;
      }
    }

    return std::unordered_set<block_id_domain>();
  }

  /**
   * @brief Get the block locality info for scheduling in ForemanDistributed.
   *
   * @param block The given block.
   * @param shiftboss_index_for_block The index of Shiftboss that has loaded the
   *        block in the buffer pool.
   *
   * @return Whether the block locality info has found.
   **/
  bool getBlockLocalityInfo(const block_id block, std::size_t *shiftboss_index_for_block) const {
    const std::unordered_set<block_id_domain> block_domains = getBlockDomains(block);
    if (!block_domains.empty()) {
      // NOTE(zuyu): This lock is held for the rest duration of this call, as the
      // exclusive case is rare.
      SpinSharedMutexSharedLock<false> read_lock(block_domain_to_shiftboss_index_shared_mutex_);
      for (const block_id_domain block_domain : block_domains) {
        // TODO(quickstep-team): choose the best node, instead of the first.
        const auto cit = block_domain_to_shiftboss_index_.find(block_domain);
        if (cit != block_domain_to_shiftboss_index_.end()) {
          *shiftboss_index_for_block = cit->second;
          return true;
        }
      }
    }

    return false;
  }

 protected:
  void run() override;

 private:
  void processBlockDomainRegistrationMessage(const tmb::client_id receiver, const std::string &network_address);
  void processGetAllDomainNetworkAddressesMessage(const tmb::client_id receiver);

  tmb::MessageBus *bus_;

  // The ID of the CPU that the BlockLocator thread can optionally be pinned to.
  const int cpu_id_;

  alignas(kCacheLineBytes) std::atomic<block_id_domain> block_domain_;

  // From a block domain to its network info in the ip:port format, i.e.,
  // "0.0.0.0:0".
  std::unordered_map<block_id_domain, const std::string> domain_network_addresses_;

  // From a block domain to its Shiftboss index, used by ForemanDistributed
  // to schedule based on the data-locality info.
  // Note that not every 'block_id_domain' has a Shiftboss index. For example,
  // DistributedCli has StorageManager with a 'block_id_domain', which is not
  // a part of Shiftboss.
  std::unordered_map<block_id_domain, std::size_t> block_domain_to_shiftboss_index_;
  alignas(kCacheLineBytes) mutable SpinSharedMutex<false> block_domain_to_shiftboss_index_shared_mutex_;

  // From a block to its domains.
  std::unordered_map<block_id, std::unordered_set<block_id_domain>> block_locations_;
  alignas(kCacheLineBytes) mutable SpinSharedMutex<false> block_locations_shared_mutex_;

  // From a block domain to all blocks loaded in its buffer pool.
  std::unordered_map<block_id_domain, std::unordered_set<block_id>> domain_blocks_;

  tmb::client_id locator_client_id_;

  DISALLOW_COPY_AND_ASSIGN(BlockLocator);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_BLOCK_LOCATOR_HPP_
