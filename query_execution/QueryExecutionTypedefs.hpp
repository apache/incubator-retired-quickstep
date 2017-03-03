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

#ifndef QUICKSTEP_QUERY_EXECUTION_QUERY_EXECUTION_TYPEDEFS_HPP_
#define QUICKSTEP_QUERY_EXECUTION_QUERY_EXECUTION_TYPEDEFS_HPP_

#include <cstddef>
#include <unordered_map>
#include <vector>

#include "query_optimizer/QueryOptimizerConfig.h"  // For QUICKSTEP_DISTRIBUTED
#include "threading/ThreadIDBasedMap.hpp"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_style.h"
#include "tmb/pure_memory_message_bus.h"
#include "tmb/tagged_message.h"

namespace quickstep {

/** \addtogroup QueryExecution
 *  @{
 */

typedef tmb::Address Address;
typedef tmb::AnnotatedMessage AnnotatedMessage;
typedef tmb::MessageBus MessageBus;
typedef tmb::MessageStyle MessageStyle;
typedef tmb::Priority Priority;
typedef tmb::PureMemoryMessageBus<false> MessageBusImpl;
typedef tmb::TaggedMessage TaggedMessage;
typedef tmb::client_id client_id;
typedef tmb::message_type_id message_type_id;

using ClientIDMap = ThreadIDBasedMap<client_id,
                                     'C',
                                     'l',
                                     'i',
                                     'e',
                                     'n',
                                     't',
                                     'I',
                                     'D',
                                     'M',
                                     'a',
                                     'p'>;

#ifdef QUICKSTEP_DISTRIBUTED

constexpr std::size_t kInvalidShiftbossIndex = static_cast<std::size_t>(-1);

#endif  // QUICKSTEP_DISTRIBUTED

// We sort the following message types in the order of a life cycle of a query.
enum QueryExecutionMessageType : message_type_id {
  kAdmitRequestMessage = 0,  // Requesting a query (or queries) to be admitted, from
                             // the main thread to Foreman.
  kWorkOrderMessage,  // From Foreman to Worker.
  kWorkOrderCompleteMessage,  // From Worker to Foreman.
  kCatalogRelationNewBlockMessage,  // From InsertDestination to Foreman.
  kDataPipelineMessage,  // From InsertDestination or some WorkOrders to Foreman.
  kWorkOrderFeedbackMessage,  // From some WorkOrders to Foreman on behalf of
                              // their corresponding RelationalOperators.
  kRebuildWorkOrderMessage,  // From Foreman to Worker.
  kRebuildWorkOrderCompleteMessage,  // From Worker to Foreman.
  kWorkloadCompletionMessage,  // From Foreman to main thread.
  kPoisonMessage,  // From the main thread to Foreman and Workers.

#ifdef QUICKSTEP_DISTRIBUTED
  kShiftbossRegistrationMessage,  // From Shiftboss to Foreman.
  kShiftbossRegistrationResponseMessage,  // From Foreman to Shiftboss, or from
                                          // Shiftboss to Worker.
  kDistributedCliRegistrationMessage,  // From CLI to Conductor.
  kDistributedCliRegistrationResponseMessage,  // From Conductor to CLI.

  kSqlQueryMessage,  // From CLI to Conductor.

  kQueryInitiateMessage,  // From Foreman to Shiftboss.
  kQueryInitiateResponseMessage,  // From Shiftboss to Foreman.

  kInitiateRebuildMessage,  // From Foreman to Shiftboss.
  kInitiateRebuildResponseMessage,  // From Shiftboss to Foreman.

  kQueryTeardownMessage,  // From Foreman to Shiftboss.

  kSaveQueryResultMessage,  // From Foreman to Shiftboss.
  kSaveQueryResultResponseMessage,  // From Shiftboss to Foreman.

  kQueryExecutionSuccessMessage,  // From Foreman to CLI.

  // From Foreman / Conductor to CLI.
  kCommandResponseMessage,
  kQueryExecutionErrorMessage,

  kQueryResultTeardownMessage,  // From CLI to Conductor.

  // BlockLocator related messages, sorted in a life cycle of StorageManager
  // with a unique block domain.
  kBlockDomainRegistrationMessage,  // From Worker to BlockLocator.
  kBlockDomainRegistrationResponseMessage,  // From BlockLocator to Worker.
  kBlockDomainToShiftbossIndexMessage,  // From StorageManager to BlockLocator.
  kAddBlockLocationMessage,  // From StorageManager to BlockLocator.
  kDeleteBlockLocationMessage,  // From StorageManager to BlockLocator.
  kLocateBlockMessage,  // From StorageManager to BlockLocator.
  kLocateBlockResponseMessage,  // From BlockLocator to StorageManager.
  kGetPeerDomainNetworkAddressesMessage,  // From StorageManager to BlockLocator.
  kGetPeerDomainNetworkAddressesResponseMessage,  // From BlockLocator to StorageManager.
  kBlockDomainUnregistrationMessage,  // From StorageManager to BlockLocator.
#endif
};

// WorkOrder profiling data structures.
// Profiling record for an individual work order.
struct WorkOrderTimeEntry {
  std::size_t worker_id;
  std::size_t operator_id;
  std::size_t start_time;  // Epoch time measured in microseconds
  std::size_t end_time;  // Epoch time measured in microseconds
};
// Key = query ID.
// Value = vector of work order profiling records.
typedef std::unordered_map<std::size_t, std::vector<WorkOrderTimeEntry>> WorkOrderTimeRecorder;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_QUERY_EXECUTION_TYPEDEFS_HPP_
