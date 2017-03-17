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

#include "query_execution/BlockLocator.hpp"

#include <cstdlib>
#include <string>
#include <utility>

#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "threading/ThreadUtil.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

using std::free;
using std::malloc;
using std::move;

using tmb::TaggedMessage;
using tmb::client_id;

namespace quickstep {

void BlockLocator::run() {
  if (cpu_id_ >= 0) {
    ThreadUtil::BindToCPU(cpu_id_);
  }

  for (;;) {
    // Receive() is a blocking call, causing this thread to sleep until next
    // message is received.
    const tmb::AnnotatedMessage annotated_message = bus_->Receive(locator_client_id_, 0, true);
    const TaggedMessage &tagged_message = annotated_message.tagged_message;
    const tmb::message_type_id message_type = tagged_message.message_type();
    const client_id sender = annotated_message.sender;
    DLOG(INFO) << "BlockLocator received " << QueryExecutionUtil::MessageTypeToString(message_type)
               << " from Client " << sender;
    switch (message_type) {
      case kBlockDomainRegistrationMessage: {
        serialization::BlockDomainRegistrationMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        processBlockDomainRegistrationMessage(sender, proto.domain_network_address());
        break;
      }
      case kBlockDomainToShiftbossIndexMessage: {
        serialization::BlockDomainToShiftbossIndexMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        {
          // Lock 'block_domain_to_shiftboss_index_shared_mutex_' as briefly as
          // possible to insert an entry for the new Shiftboss index.
          SpinSharedMutexExclusiveLock<false> write_lock(block_domain_to_shiftboss_index_shared_mutex_);
          block_domain_to_shiftboss_index_.emplace(proto.block_domain(), proto.shiftboss_index());
        }
        break;
      }
      case kAddBlockLocationMessage: {
        serialization::BlockLocationMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        const block_id block = proto.block_id();
        const block_id_domain domain = proto.block_domain();

        const auto result_domain_blocks = domain_blocks_[domain].insert(block);

        {
          // Lock 'block_locations_shared_mutex_' as briefly as possible to
          // insert an entry for the new block location.
          SpinSharedMutexExclusiveLock<false> write_lock(block_locations_shared_mutex_);
          const auto result_block_locations = block_locations_[block].insert(domain);
          DCHECK_EQ(result_block_locations.second, result_domain_blocks.second);
        }

        if (result_domain_blocks.second) {
          DLOG(INFO) << "Block " << BlockIdUtil::ToString(block) << " loaded in Domain " << domain;
        } else {
          DLOG(INFO) << "Block " << BlockIdUtil::ToString(block) << " existed in Domain " << domain;
        }
        break;
      }
      case kDeleteBlockLocationMessage: {
        serialization::BlockLocationMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        const block_id block = proto.block_id();
        const block_id_domain domain = proto.block_domain();

        bool block_found = false;
        {
          // Lock 'block_locations_shared_mutex_' as briefly as possible to
          // delete an entry for the block location.
          SpinSharedMutexExclusiveLock<false> write_lock(block_locations_shared_mutex_);
          const auto cit = block_locations_[block].find(domain);
          if (cit != block_locations_[block].end()) {
            block_locations_[block].erase(domain);
            block_found = true;
          }
        }

        if (block_found) {
          domain_blocks_[domain].erase(block);
          DLOG(INFO) << "Block " << BlockIdUtil::ToString(block) << " evicted in Domain " << domain;
        } else {
          DLOG(INFO) << "Block " << BlockIdUtil::ToString(block) << " not found in Domain " << domain;
        }
        break;
      }
      case kGetAllDomainNetworkAddressesMessage: {
        processGetAllDomainNetworkAddressesMessage(sender);
        break;
      }
      case kBlockDomainUnregistrationMessage: {
        serialization::BlockDomainMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        const block_id_domain domain = proto.block_domain();

        domain_network_addresses_.erase(domain);

        {
          // Lock 'block_locations_shared_mutex_' as briefly as possible to
          // delete all entry for the block domain.
          SpinSharedMutexExclusiveLock<false> write_lock(block_locations_shared_mutex_);
          for (const block_id block : domain_blocks_[domain]) {
            block_locations_[block].erase(domain);
          }
        }
        domain_blocks_.erase(domain);

        DLOG(INFO) << "Unregistered Domain " << domain;
        break;
      }
      case kPoisonMessage: {
        return;
      }
    }
  }
}

void BlockLocator::processBlockDomainRegistrationMessage(const client_id receiver,
                                                         const std::string &network_address) {
  DCHECK_LT(block_domain_, kMaxDomain);

  domain_network_addresses_.emplace(++block_domain_, network_address);
  domain_blocks_[block_domain_];

  serialization::BlockDomainMessage proto;
  proto.set_block_domain(block_domain_);

  const int proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kBlockDomainRegistrationResponseMessage);
  free(proto_bytes);

  DLOG(INFO) << "BlockLocator with Client " << locator_client_id_
             << " sent BlockDomainRegistrationResponseMessage to Client " << receiver;
  CHECK(tmb::MessageBus::SendStatus::kOK ==
      QueryExecutionUtil::SendTMBMessage(bus_,
                                         locator_client_id_,
                                         receiver,
                                         move(message)));
}

void BlockLocator::processGetAllDomainNetworkAddressesMessage(const client_id receiver) {
  serialization::GetAllDomainNetworkAddressesResponseMessage proto;

  // NOTE(zuyu): We don't need to protect here, as all the writers are in the
  // single thread.
  for (const auto &domain_network_address_pair : domain_network_addresses_) {
    serialization::GetAllDomainNetworkAddressesResponseMessage::DomainNetworkAddress *proto_domain_network_address =
        proto.add_domain_network_addresses();
    proto_domain_network_address->set_block_domain(domain_network_address_pair.first);
    proto_domain_network_address->set_network_address(domain_network_address_pair.second);
  }

  const int proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes), proto_length,
                        kGetAllDomainNetworkAddressesResponseMessage);
  free(proto_bytes);

  DLOG(INFO) << "BlockLocator with Client " << locator_client_id_
             << " sent GetAllDomainNetworkAddressesResponseMessage to StorageManager with Client " << receiver;
  CHECK(tmb::MessageBus::SendStatus::kOK ==
      QueryExecutionUtil::SendTMBMessage(bus_, locator_client_id_, receiver, move(message)));
}

}  // namespace quickstep
