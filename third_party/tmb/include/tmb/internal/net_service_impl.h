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

#ifndef TMB_INTERNAL_NET_SERVICE_IMPL_H_
#define TMB_INTERNAL_NET_SERVICE_IMPL_H_

#include <memory>

#include "grpc++/server_context.h"
#include "grpc++/support/status.h"
#include "grpc++/support/sync_stream.h"

#include "tmb/message_bus.h"

#include "tmb/internal/tmb_net.grpc.pb.h"
#include "tmb/internal/tmb_net.pb.h"

namespace tmb {
namespace internal {

/**
 * @brief Implementation of TMB GRPC service that wraps an internal MessageBus.
 **/
class NetServiceImpl : public net::MessageBus::Service {
 public:
  explicit NetServiceImpl(MessageBus *internal_bus);

  ~NetServiceImpl() override {
  }

  grpc::Status ResetBus(grpc::ServerContext *context,
                        const net::EmptyMessage *request,
                        net::EmptyMessage *response) override;

  grpc::Status Connect(grpc::ServerContext *context,
                       const net::EmptyMessage *request,
                       net::ConnectResponse *response) override;

  grpc::Status Disconnect(grpc::ServerContext *context,
                          const net::DisconnectRequest *request,
                          net::BoolStatus *response) override;

  grpc::Status RegisterClientAsSender(grpc::ServerContext *context,
                                      const net::RegistrationRequest *request,
                                      net::BoolStatus *response) override;

  grpc::Status RegisterClientAsReceiver(
      grpc::ServerContext *context,
      const net::RegistrationRequest *request,
      net::BoolStatus *response) override;

  grpc::Status Send(grpc::ServerContext *context,
                    const net::SendRequest *request,
                    net::SendResponse *response) override;

  grpc::Status Receive(
      grpc::ServerContext *context,
      const net::ReceiveRequest *request,
      grpc::ServerWriter<net::AnnotatedTmbMessage> *writer) override;

  grpc::Status Delete(grpc::ServerContext *context,
                      const net::DeleteOrCancelRequest *request,
                      net::EmptyMessage *response) override;

  grpc::Status SenderCancel(grpc::ServerContext *context,
                            const net::DeleteOrCancelRequest *request,
                            net::EmptyMessage *response) override;

  grpc::Status ReceiverCancel(grpc::ServerContext *context,
                              const net::DeleteOrCancelRequest *request,
                              net::EmptyMessage *response) override;

  grpc::Status CountQueuedMessages(
      grpc::ServerContext *context,
      const net::CountQueuedMessagesRequest *request,
      net::CountQueuedMessagesResponse *response) override;

 private:
  std::unique_ptr<MessageBus> internal_bus_;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_NET_SERVICE_IMPL_H_
