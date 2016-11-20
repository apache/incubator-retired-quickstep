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

#ifndef QUICKSTEP_QUERY_EXECUTION_BLOCK_LOCATOR_UTIL_HPP_
#define QUICKSTEP_QUERY_EXECUTION_BLOCK_LOCATOR_UTIL_HPP_

#include <string>

#include "storage/StorageBlockInfo.hpp"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {
namespace block_locator {

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief Broadcast to find BlockLocator to get a block domain for
 * StorageManager with the given network address.
 *
 * @param network_address The network address of the StorageManager.
 * @param cli_id The client ID of the block domain requester.
 * @param locator_client_id The client ID of BlockLocator to set.
 * @param bus A pointer to the TMB.
 *
 * @return The requested block domain.
 **/
block_id_domain getBlockDomain(const std::string &network_address,
                               const tmb::client_id cli_id,
                               tmb::client_id *locator_client_id,
                               tmb::MessageBus *bus);

/** @} */

}  // namespace block_locator
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_BLOCK_LOCATOR_UTIL_HPP_
