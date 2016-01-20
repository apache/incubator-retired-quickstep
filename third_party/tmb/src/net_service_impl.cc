//   Copyright 2014-2015 Quickstep Technologies LLC.
//   Copyright 2015 Pivotal Software, Inc.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#include "tmb/internal/net_service_impl.h"

#include <cassert>
#include <chrono>  // NOLINT(build/c++11)
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "grpc++/server_context.h"
#include "grpc++/support/status.h"
#include "grpc++/support/status_code_enum.h"
#include "grpc++/support/sync_stream.h"

#include "tmb/address.h"
#include "tmb/cancellation_token.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/net_message_removal_interface.h"
#include "tmb/internal/tmb_net.grpc.pb.h"
#include "tmb/internal/tmb_net.pb.h"

namespace tmb {
namespace internal {

namespace {

// Convenience function to get the set of message IDs specified in a
// DeleteOrCancelRequest as a vector.
inline std::vector<std::int64_t> GetMessageIdVector(
    const net::DeleteOrCancelRequest &request) {
  std::vector<std::int64_t> message_ids;
  message_ids.reserve(request.message_id_size());
  for (int message_id_num = 0;
       message_id_num < request.message_id_size();
       ++message_id_num) {
    message_ids.push_back(request.message_id(message_id_num));
  }
  return message_ids;
}

}  // namespace

NetServiceImpl::NetServiceImpl(MessageBus *internal_bus)
    : internal_bus_(internal_bus) {
  assert(internal_bus_->SupportsNetMessageRemovalInterface());
}

grpc::Status NetServiceImpl::ResetBus(
    grpc::ServerContext *context,
    const net::EmptyMessage *request,
    net::EmptyMessage *response) {
  internal_bus_->ResetBus();
  return grpc::Status::OK;
}

grpc::Status NetServiceImpl::Connect(
    grpc::ServerContext *context,
    const net::EmptyMessage *request,
    net::ConnectResponse *response) {
  response->set_client(internal_bus_->Connect());
  return grpc::Status::OK;
}

grpc::Status NetServiceImpl::Disconnect(
    grpc::ServerContext *context,
    const net::DisconnectRequest *request,
    net::BoolStatus *response) {
  response->set_status(internal_bus_->Disconnect(request->client()));
  return grpc::Status::OK;
}

grpc::Status NetServiceImpl::RegisterClientAsSender(
    grpc::ServerContext *context,
    const net::RegistrationRequest *request,
    net::BoolStatus *response) {
  response->set_status(
      internal_bus_->RegisterClientAsSender(request->client(),
                                            request->message_type()));
  return grpc::Status::OK;
}

grpc::Status NetServiceImpl::RegisterClientAsReceiver(
    grpc::ServerContext *context,
    const net::RegistrationRequest *request,
    net::BoolStatus *response) {
  response->set_status(
      internal_bus_->RegisterClientAsReceiver(request->client(),
                                              request->message_type()));
  return grpc::Status::OK;
}

grpc::Status NetServiceImpl::Send(
    grpc::ServerContext *context,
    const net::SendRequest *request,
    net::SendResponse *response) {
  if (!request->has_msg()) {
    return grpc::Status(grpc::INVALID_ARGUMENT, "No message body in request");
  }

  Address address;
  if (request->send_to_all()) {
    address.All(true);
  } else {
    for (int recipient_num = 0;
         recipient_num < request->explicit_recipient_size();
         ++recipient_num) {
      address.AddRecipient(request->explicit_recipient(recipient_num));
    }
  }

  MessageStyle style;
  if (request->broadcast()) {
    style.Broadcast(true);
  }
  if (request->timeout() != 0) {
    style.Timeout(
        std::chrono::time_point<std::chrono::high_resolution_clock>(
            std::chrono::nanoseconds(request->timeout())));
  }

  // TODO(chasseur): Look into hacks that may let us avoid copying the message
  // body out from the request.
  TaggedMessage msg(request->msg().message_body().c_str(),
                    request->msg().message_body().size(),
                    request->msg().message_type());

  CancellationToken token;

  MessageBus::SendStatus status = internal_bus_->Send(
      request->sender(),
      address,
      style,
      std::move(msg),
      request->priority(),
      request->cancellable() ? &token : nullptr);

  switch (status) {
    case MessageBus::SendStatus::kOK: {
      response->set_status(net::SendResponse::OK);
      if (request->cancellable()) {
        response->set_message_id(token.message_id_);
      }
      break;
    }
    case MessageBus::SendStatus::kNoReceivers:
      response->set_status(net::SendResponse::NO_RECEIVERS);
      break;
    case MessageBus::SendStatus::kSenderNotConnected:
      response->set_status(net::SendResponse::SENDER_NOT_CONNECTED);
      break;
    case MessageBus::SendStatus::kSenderNotRegisteredForMessageType:
      response->set_status(
          net::SendResponse::SENDER_NOT_REGISTERED_FOR_MESSAGE_TYPE);
      break;
    case MessageBus::SendStatus::kReceiverNotRegisteredForMessageType:
      response->set_status(
          net::SendResponse::RECEIVER_NOT_REGISTERED_FOR_MESSAGE_TYPE);
      break;
    default:
      return grpc::Status(grpc::INTERNAL,
                          "Unrecognized MessageBus::SendStatus code.");
  }

  return grpc::Status::OK;
}

grpc::Status NetServiceImpl::Receive(
    grpc::ServerContext *context,
    const net::ReceiveRequest *request,
    grpc::ServerWriter<net::AnnotatedTmbMessage> *writer) {
  std::vector<AnnotatedMessage> messages;
  internal_bus_->ReceiveBatchIfAvailable(request->receiver(),
                                         &messages,
                                         request->minimum_priority(),
                                         request->maximum_messages(),
                                         request->delete_immediately());

  for (const AnnotatedMessage &msg : messages) {
    // TODO(chasseur): Look into hacks to avoid deep copy of message body.
    net::AnnotatedTmbMessage msg_proto;
    msg_proto.mutable_tagged_message()->set_message_type(
        msg.tagged_message.message_type());
    msg_proto.mutable_tagged_message()->set_message_body(
        msg.tagged_message.message(),
        msg.tagged_message.message_bytes());
    msg_proto.set_sender(msg.sender);
    msg_proto.set_send_time(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            msg.send_time.time_since_epoch()).count());
    msg_proto.set_message_id(msg.deletion_token.message_id);

    writer->Write(msg_proto);
  }

  return grpc::Status::OK;
}

grpc::Status NetServiceImpl::Delete(
    grpc::ServerContext *context,
    const net::DeleteOrCancelRequest *request,
    net::EmptyMessage *response) {
  if (!internal_bus_->SupportsNetMessageRemovalInterface()) {
    return grpc::Status(
        grpc::INTERNAL,
        "Net service was started with bus that does not support "
            "NetMessageRemovalInterface");
  }
  internal_bus_->GetNetMessageRemovalInterface()->DeleteById(
      request->client(),
      GetMessageIdVector(*request));
  return grpc::Status::OK;
}

grpc::Status NetServiceImpl::SenderCancel(
    grpc::ServerContext *context,
    const net::DeleteOrCancelRequest *request,
    net::EmptyMessage *response) {
  if (!internal_bus_->SupportsNetMessageRemovalInterface()) {
    return grpc::Status(
        grpc::INTERNAL,
        "Net service was started with bus that does not support "
            "NetMessageRemovalInterface");
  }
  internal_bus_->GetNetMessageRemovalInterface()->SenderCancelById(
      request->client(),
      GetMessageIdVector(*request));
  return grpc::Status::OK;
}

grpc::Status NetServiceImpl::ReceiverCancel(
    grpc::ServerContext *context,
    const net::DeleteOrCancelRequest *request,
    net::EmptyMessage *response) {
  if (!internal_bus_->SupportsNetMessageRemovalInterface()) {
    return grpc::Status(
        grpc::INTERNAL,
        "Net service was started with bus that does not support "
            "NetMessageRemovalInterface");
  }
  internal_bus_->GetNetMessageRemovalInterface()->ReceiverCancelById(
      request->client(),
      GetMessageIdVector(*request));
  return grpc::Status::OK;
}

grpc::Status NetServiceImpl::CountQueuedMessages(
    grpc::ServerContext *context,
    const net::CountQueuedMessagesRequest *request,
    net::CountQueuedMessagesResponse *response) {
  response->set_message_count(
      internal_bus_->CountQueuedMessagesForClient(request->client()));
  return grpc::Status::OK;
}

}  // namespace internal
}  // namespace tmb
