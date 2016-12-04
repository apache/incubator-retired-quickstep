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

#include "query_execution/BlockLocatorUtil.hpp"

#include <cstdlib>
#include <string>
#include <utility>

#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"

#include "glog/logging.h"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/tagged_message.h"

using tmb::TaggedMessage;
using tmb::MessageBus;
using tmb::client_id;

namespace quickstep {
namespace block_locator {

namespace S = ::quickstep::serialization;

block_id_domain getBlockDomain(const std::string &network_address,
                               const std::size_t shiftboss_index,
                               const client_id cli_id,
                               client_id *locator_client_id,
                               MessageBus *bus) {
  tmb::Address address;
  address.All(true);
  // NOTE(zuyu): The singleton BlockLocator would need only one copy of the message.
  tmb::MessageStyle style;

  S::BlockDomainRegistrationMessage proto;
  proto.set_domain_network_address(network_address);
  proto.set_shiftboss_index(shiftboss_index);

  const int proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(std::malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage message(static_cast<const void*>(proto_bytes),
                        proto_length,
                        kBlockDomainRegistrationMessage);
  std::free(proto_bytes);

  DLOG(INFO) << "Client (id '" << cli_id
             << "') broadcasts BlockDomainRegistrationMessage (typed '" << kBlockDomainRegistrationMessage
             << "') to BlockLocator.";

  CHECK(MessageBus::SendStatus::kOK ==
      bus->Send(cli_id, address, style, std::move(message)));

  const tmb::AnnotatedMessage annotated_message(bus->Receive(cli_id, 0, true));
  const TaggedMessage &tagged_message = annotated_message.tagged_message;
  CHECK_EQ(kBlockDomainRegistrationResponseMessage, tagged_message.message_type());

  *locator_client_id = annotated_message.sender;

  DLOG(INFO) << "Client (id '" << cli_id
             << "') received BlockDomainRegistrationResponseMessage (typed '"
             << kBlockDomainRegistrationResponseMessage
             << "') from BlockLocator (id '" << *locator_client_id << "').";

  S::BlockDomainMessage response_proto;
  CHECK(response_proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

  return static_cast<block_id_domain>(response_proto.block_domain());
}

}  // namespace block_locator
}  // namespace quickstep
