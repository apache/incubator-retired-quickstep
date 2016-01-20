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

// TODO(chasseur): Better error handling in place of asserts.

#include "tmb/native_net_client_message_bus.h"

#include <cassert>
#include <chrono>  // NOLINT(build/c++11)
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <utility>

#include "grpc++/client_context.h"
#include "grpc++/create_channel.h"
#include "grpc++/security/credentials.h"
#include "grpc++/support/status.h"
#include "grpc++/support/sync_stream.h"

#include "tmb/address.h"
#include "tmb/cancellation_token.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/tagged_message.h"
#include "tmb/priority.h"

#include "tmb/internal/container_pusher.h"
#include "tmb/internal/iterator_adapter.h"
#include "tmb/internal/tmb_net.grpc.pb.h"
#include "tmb/internal/tmb_net.pb.h"

namespace tmb {

void NativeNetClientMessageBus::AddServer(const std::string &hostname,
                                          const std::uint16_t port) {
  server_address_ = hostname;
  server_address_.push_back(':');
  server_address_.append(std::to_string(static_cast<unsigned>(port)));
}

bool NativeNetClientMessageBus::Initialize() {
  stub_ = internal::net::MessageBus::NewStub(
      grpc::CreateChannel(server_address_,
                          grpc::InsecureCredentials()));

  return (stub_.get() != nullptr);
}

void NativeNetClientMessageBus::ResetBus() {
  internal::net::EmptyMessage request;
  internal::net::EmptyMessage response;
  grpc::ClientContext context;

  grpc::Status status = stub_->ResetBus(&context, request, &response);
  assert(status.ok());
}

client_id NativeNetClientMessageBus::Connect() {
  internal::net::EmptyMessage request;
  internal::net::ConnectResponse response;
  grpc::ClientContext context;

  grpc::Status status = stub_->Connect(&context, request, &response);
  assert(status.ok());

  return response.client();
}

bool NativeNetClientMessageBus::Disconnect(const client_id client) {
  internal::net::DisconnectRequest request;
  request.set_client(client);

  internal::net::BoolStatus response;
  grpc::ClientContext context;

  grpc::Status status = stub_->Disconnect(&context, request, &response);
  assert(status.ok());

  return response.status();
}

bool NativeNetClientMessageBus::RegisterClientAsSender(
    const client_id sender_id,
    const message_type_id message_type) {
  internal::net::RegistrationRequest request;
  request.set_client(sender_id);
  request.set_message_type(message_type);

  internal::net::BoolStatus response;
  grpc::ClientContext context;

  grpc::Status status = stub_->RegisterClientAsSender(&context,
                                                      request,
                                                      &response);
  assert(status.ok());

  return response.status();
}

bool NativeNetClientMessageBus::RegisterClientAsReceiver(
    const client_id receiver_id,
    const message_type_id message_type) {
  internal::net::RegistrationRequest request;
  request.set_client(receiver_id);
  request.set_message_type(message_type);

  internal::net::BoolStatus response;
  grpc::ClientContext context;

  grpc::Status status = stub_->RegisterClientAsReceiver(&context,
                                                        request,
                                                        &response);
  assert(status.ok());

  return response.status();
}

MessageBus::SendStatus NativeNetClientMessageBus::Send(
    const client_id sender_id,
    const Address &destination_address,
    const MessageStyle &style,
    TaggedMessage &&message,  // NOLINT(whitespace/operators)
    const Priority priority,
    CancellationToken *cancellation_token) {
  internal::net::SendRequest request;
  request.set_sender(sender_id);
  request.set_send_to_all(destination_address.send_to_all_);
  for (const client_id recipient : destination_address.explicit_recipients_) {
    request.add_explicit_recipient(recipient);
  }
  request.set_broadcast(style.broadcast_);
  request.set_timeout(std::chrono::duration_cast<std::chrono::nanoseconds>(
      style.expiration_time_.time_since_epoch()).count());
  request.mutable_msg()->set_message_type(message.message_type());
  // TODO(chasseur): Protobuf API supports passing in an externally allocated
  // std::string to avoid a copy here, but we manage TaggedMessage's body at a
  // lower level than that using malloc/free plus our own version of the
  // short-string optimization. If message copying here and in NetServiceImpl
  // becomes a performance issue, we might consider changing the internal
  // representation of TaggedMessage to use a std::string so that we avoid a
  // copy into/out of protos.
  request.mutable_msg()->set_message_body(message.message(),
                                          message.message_bytes());
  request.set_priority(priority);
  request.set_cancellable(cancellation_token != nullptr);

  internal::net::SendResponse response;
  grpc::ClientContext context;

  grpc::Status status = stub_->Send(&context, request, &response);
  assert(status.ok());

  switch (response.status()) {
    case internal::net::SendResponse::OK:
      if (cancellation_token != nullptr) {
        cancellation_token->message_id_ = response.message_id();
      }
      return MessageBus::SendStatus::kOK;
    case internal::net::SendResponse::NO_RECEIVERS:
      return MessageBus::SendStatus::kNoReceivers;
    case internal::net::SendResponse::SENDER_NOT_CONNECTED:
      return MessageBus::SendStatus::kSenderNotConnected;
    case internal::net::SendResponse::SENDER_NOT_REGISTERED_FOR_MESSAGE_TYPE:
      return MessageBus::SendStatus::kSenderNotRegisteredForMessageType;
    case internal::net::SendResponse::RECEIVER_NOT_REGISTERED_FOR_MESSAGE_TYPE:
      return MessageBus::SendStatus::kReceiverNotRegisteredForMessageType;
    default:
      std::fprintf(
          stderr,
          "FATAL ERROR: Unrecognized value of status enum in SendResponse\n");
      std::exit(1);
  }
}

void NativeNetClientMessageBus::CancelMessage(
    const client_id sender_id,
    const CancellationToken &cancellation_token) {
  internal::net::DeleteOrCancelRequest request;
  request.set_client(sender_id);
  request.add_message_id(cancellation_token.message_id_);

  internal::net::EmptyMessage response;
  grpc::ClientContext context;

  grpc::Status status = stub_->SenderCancel(&context, request, &response);
  assert(status.ok());
}

std::size_t NativeNetClientMessageBus::CountQueuedMessagesForClient(
    const client_id receiver_id) {
  internal::net::CountQueuedMessagesRequest request;
  request.set_client(receiver_id);

  internal::net::CountQueuedMessagesResponse response;
  grpc::ClientContext context;

  grpc::Status status = stub_->CountQueuedMessages(&context,
                                                   request,
                                                   &response);
  assert(status.ok());

  return response.message_count();
}

std::size_t NativeNetClientMessageBus::ReceiveIfAvailableImpl(
    const client_id receiver_id,
    const Priority minimum_priority,
    const std::size_t max_messages,
    const bool delete_immediately,
    internal::ContainerPusher *pusher) {
  internal::net::ReceiveRequest request;
  request.set_receiver(receiver_id);
  request.set_minimum_priority(minimum_priority);
  request.set_maximum_messages(max_messages);
  request.set_delete_immediately(delete_immediately);

  internal::net::AnnotatedTmbMessage msg_proto;
  grpc::ClientContext context;

  std::unique_ptr<grpc::ClientReader<internal::net::AnnotatedTmbMessage>>
      reader(stub_->Receive(&context, request));
  std::size_t num_received = 0;
  while (reader->Read(&msg_proto)) {
    assert(msg_proto.has_tagged_message());
    ++num_received;

    AnnotatedMessage msg;
    msg.sender = msg_proto.sender();
    msg.send_time
        = std::chrono::time_point<std::chrono::high_resolution_clock>(
            std::chrono::nanoseconds(msg_proto.send_time()));
    msg.deletion_token.message_id = msg_proto.message_id();
    msg.tagged_message.set_message(
        msg_proto.tagged_message().message_body().c_str(),
        msg_proto.tagged_message().message_body().size(),
        msg_proto.tagged_message().message_type());

    pusher->Push(std::move(msg));
  }
  grpc::Status status = reader->Finish();
  assert(status.ok());

  return num_received;
}

void NativeNetClientMessageBus::DeleteImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  internal::net::DeleteOrCancelRequest request;
  request.set_client(receiver_id);
  while (adapter->Valid()) {
    request.add_message_id((*adapter)->deletion_token.message_id);
    adapter->Next();
  }

  internal::net::EmptyMessage response;
  grpc::ClientContext context;

  grpc::Status status = stub_->Delete(&context, request, &response);
  assert(status.ok());
}

void NativeNetClientMessageBus::CancelImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  internal::net::DeleteOrCancelRequest request;
  request.set_client(receiver_id);
  while (adapter->Valid()) {
    request.add_message_id((*adapter)->deletion_token.message_id);
    adapter->Next();
  }

  internal::net::EmptyMessage response;
  grpc::ClientContext context;

  grpc::Status status = stub_->ReceiverCancel(&context, request, &response);
  assert(status.ok());
}

}  // namespace tmb
