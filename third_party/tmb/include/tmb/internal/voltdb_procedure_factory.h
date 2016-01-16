// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_VOLTDB_PROCEDURE_FACTORY_H_
#define TMB_INTERNAL_VOLTDB_PROCEDURE_FACTORY_H_

#include <vector>

#include "Parameter.hpp"   // NOLINT(build/include)
#include "Procedure.hpp"   // NOLINT(build/include)

namespace tmb {
namespace internal {

/**
 * @brief A factory object which creates instances of the TMB VoltDB
 *        procedures.
 **/
class VoltDBProcedureFactory {
 public:
  VoltDBProcedureFactory();

  inline voltdb::Procedure* CreateResetBusProcedure() const {
    return new voltdb::Procedure("ResetBus", empty_params_);
  }

  inline voltdb::Procedure* CreateConnectClientProcedure() const {
    return new voltdb::Procedure("ConnectClient", empty_params_);
  }

  inline voltdb::Procedure* CreateDisconnectClientProcedure() const {
    return new voltdb::Procedure("DisconnectClient", single_client_id_params_);
  }

  inline voltdb::Procedure* CreateRegisterSenderProcedure() const {
    return new voltdb::Procedure("RegisterSender", register_params_);
  }

  inline voltdb::Procedure* CreateRegisterSenderUncheckedProcedure() const {
    return new voltdb::Procedure("RegisterSenderUnchecked", register_params_);
  }

  inline voltdb::Procedure* CreateRegisterReceiverProcedure() const {
    return new voltdb::Procedure("RegisterReceiver", register_params_);
  }

  inline voltdb::Procedure* CreateRegisterReceiverUncheckedProcedure() const {
    return new voltdb::Procedure("RegisterReceiverUnchecked",
                                 register_params_);
  }

  inline voltdb::Procedure* CreateSendToSingleExplicitReceiverProcedure()
      const {
    return new voltdb::Procedure("SendToSingleExplicitReceiver",
                                 send_to_single_explicit_receiver_params_);
  }

  inline voltdb::Procedure*
      CreateSendToSingleExplicitReceiverUncheckedProcedure() const {
    return new voltdb::Procedure(
        "SendToSingleExplicitReceiverUnchecked",
        send_to_single_explicit_receiver_unchecked_params_);
  }

  inline voltdb::Procedure* CreateSendToExplicitReceiversProcedure() const {
    return new voltdb::Procedure("SendToExplicitReceivers",
                                 send_to_explicit_receivers_params_);
  }

  inline voltdb::Procedure* CreateSendToExplicitReceiversUncheckedProcedure()
      const {
    return new voltdb::Procedure("SendToExplicitReceiversUnchecked",
                                 send_to_explicit_receivers_unchecked_params_);
  }

  inline voltdb::Procedure* CreateSendToAnyProcedure() const {
    return new voltdb::Procedure("SendToAny", send_to_any_params_);
  }

  inline voltdb::Procedure* CreateReceiveProcedure() const {
    return new voltdb::Procedure("Receive", receive_params_);
  }

  inline voltdb::Procedure* CreateReceiveAndDeleteProcedure() const {
    return new voltdb::Procedure("ReceiveAndDelete", receive_params_);
  }

  inline voltdb::Procedure* CreateDeleteMessagesProcedure() const {
    return new voltdb::Procedure("DeleteMessages", delete_messages_params_);
  }

  inline voltdb::Procedure* CreateDeleteMessagesUncheckedProcedure() const {
    return new voltdb::Procedure("DeleteMessagesUnchecked",
                                 delete_messages_params_);
  }

  inline voltdb::Procedure* CreateCancelMessagesProcedure() const {
    return new voltdb::Procedure("CancelMessages", cancel_messages_params_);
  }

  inline voltdb::Procedure* CreateLoadStateProcedure() const {
    return new voltdb::Procedure("LoadState", empty_params_);
  }

  inline voltdb::Procedure*
      CreateCountQueuedMessagesForClientProcedure() const {
    return new voltdb::Procedure("CountQueuedMessagesForClient",
                                 single_client_id_params_);
  }

 private:
  std::vector<voltdb::Parameter> empty_params_;
  std::vector<voltdb::Parameter> single_client_id_params_;
  std::vector<voltdb::Parameter> register_params_;
  std::vector<voltdb::Parameter> send_to_single_explicit_receiver_params_;
  std::vector<voltdb::Parameter>
      send_to_single_explicit_receiver_unchecked_params_;
  std::vector<voltdb::Parameter> send_to_explicit_receivers_params_;
  std::vector<voltdb::Parameter> send_to_explicit_receivers_unchecked_params_;
  std::vector<voltdb::Parameter> send_to_any_params_;
  std::vector<voltdb::Parameter> receive_params_;
  std::vector<voltdb::Parameter> delete_messages_params_;
  std::vector<voltdb::Parameter> cancel_messages_params_;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_VOLTDB_PROCEDURE_FACTORY_H_
