// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

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
