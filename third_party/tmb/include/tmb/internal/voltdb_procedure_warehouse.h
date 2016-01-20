//   Copyright 2014-2015 Quickstep Technologies LLC.
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

#ifndef TMB_INTERNAL_VOLTDB_PROCEDURE_WAREHOUSE_H_
#define TMB_INTERNAL_VOLTDB_PROCEDURE_WAREHOUSE_H_

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>

#include "Procedure.hpp"   // NOLINT(build/include)

#include "tmb/internal/lock_free_stack.h"
#include "tmb/internal/voltdb_procedure_factory.h"

namespace tmb {
namespace internal {

/**
 * @brief Maintains shared pools of each type of procedure used in the VoltDB
 *        TMB implementation. Clients can "check out" a procedure object for
 *        exclusive use by calling GetProcedure(). The procedure is
 *        automatically returned to the appropriate pool when the
 *        ProcedureHandle returned goes out of scope.
 **/
class VoltDBProcedureWarehouse {
 public:
  enum class ProcedureID : std::uint8_t {
    kResetBus = 0,
    kConnectClient,
    kDisconnectClient,
    kRegisterSender,
    kRegisterSenderUnchecked,
    kRegisterReceiver,
    kRegisterReceiverUnchecked,
    kSendToSingleExplicitReceiver,
    kSendToSingleExplicitReceiverUnchecked,
    kSendToExplicitReceivers,
    kSendToExplicitReceiversUnchecked,
    kSendToAny,
    kReceive,
    kReceiveAndDelete,
    kDeleteMessages,
    kDeleteMessagesUnchecked,
    kCancelMessages,
    kLoadState,
    kCountQueuedMessagesForClient,
    kNumProcedureIDs  // Not a real ProcedureID. Used for counting.
  };

  class ProcedureHandle {
   public:
    inline ProcedureHandle()
        : procedure_id_(ProcedureID::kNumProcedureIDs),
          procedure_(nullptr),
          warehouse_(nullptr) {
    }

    inline ~ProcedureHandle() {
      if (valid()) {
        warehouse_->AddToPool(procedure_id_, procedure_);
      }
    }

    inline ProcedureHandle(ProcedureHandle &&orig)  // NOLINT(build/c++11)
        : procedure_id_(orig.procedure_id_),
          procedure_(orig.procedure_),
          warehouse_(orig.warehouse_) {
      orig.procedure_id_ = ProcedureID::kNumProcedureIDs;
      orig.procedure_ = nullptr;
      orig.warehouse_ = nullptr;
    }

    inline ProcedureHandle& operator=(
        ProcedureHandle &&rhs) {  // NOLINT(build/c++11)
      if (this != &rhs) {
        procedure_id_ = rhs.procedure_id_;
        procedure_ = rhs.procedure_;
        warehouse_ = rhs.warehouse_;
        rhs.procedure_id_ = ProcedureID::kNumProcedureIDs;
        rhs.procedure_ = nullptr;
        rhs.warehouse_ = nullptr;
      }
      return *this;
    }

    inline bool valid() const {
      return procedure_ != nullptr;
    }

    inline voltdb::Procedure* get() const {
      return procedure_;
    }

    inline voltdb::Procedure& operator*() const {
      return *procedure_;
    }

    inline voltdb::Procedure* operator->() const {
      return procedure_;
    }

    inline void release() {
      if (valid()) {
        warehouse_->AddToPool(procedure_id_, procedure_);
        procedure_id_ = ProcedureID::kNumProcedureIDs;
        procedure_ = nullptr;
        warehouse_ = nullptr;
      }
    }

   private:
    inline ProcedureHandle(const ProcedureID procedure_id,
                           voltdb::Procedure *procedure,
                           VoltDBProcedureWarehouse *warehouse)
        : procedure_id_(procedure_id),
          procedure_(procedure),
          warehouse_(warehouse) {
    }

    ProcedureID procedure_id_;
    voltdb::Procedure *procedure_;
    VoltDBProcedureWarehouse *warehouse_;

    friend class VoltDBProcedureWarehouse;

    // Disallow copy and assign.
    ProcedureHandle(const ProcedureHandle &orig) = delete;
    ProcedureHandle& operator=(const ProcedureHandle &rhs) = delete;
  };


  VoltDBProcedureWarehouse() {
  }

  ProcedureHandle GetProcedure(const ProcedureID procedure_id) {
    std::unique_ptr<voltdb::Procedure> procedure_buf;
    if (!procedure_pools_[static_cast<std::uint8_t>(procedure_id)].Pop(
            &procedure_buf)) {
      switch (procedure_id) {
        case ProcedureID::kResetBus:
          procedure_buf.reset(factory_.CreateResetBusProcedure());
          break;
        case ProcedureID::kConnectClient:
          procedure_buf.reset(factory_.CreateConnectClientProcedure());
          break;
        case ProcedureID::kDisconnectClient:
          procedure_buf.reset(factory_.CreateDisconnectClientProcedure());
          break;
        case ProcedureID::kRegisterSender:
          procedure_buf.reset(factory_.CreateRegisterSenderProcedure());
          break;
        case ProcedureID::kRegisterSenderUnchecked:
          procedure_buf.reset(
              factory_.CreateRegisterSenderUncheckedProcedure());
          break;
        case ProcedureID::kRegisterReceiver:
          procedure_buf.reset(factory_.CreateRegisterReceiverProcedure());
          break;
        case ProcedureID::kRegisterReceiverUnchecked:
          procedure_buf.reset(
              factory_.CreateRegisterReceiverUncheckedProcedure());
          break;
        case ProcedureID::kSendToSingleExplicitReceiver:
          procedure_buf.reset(
              factory_.CreateSendToSingleExplicitReceiverProcedure());
          break;
        case ProcedureID::kSendToSingleExplicitReceiverUnchecked:
          procedure_buf.reset(
              factory_.CreateSendToSingleExplicitReceiverUncheckedProcedure());
          break;
        case ProcedureID::kSendToExplicitReceivers:
          procedure_buf.reset(
              factory_.CreateSendToExplicitReceiversProcedure());
          break;
        case ProcedureID::kSendToExplicitReceiversUnchecked:
          procedure_buf.reset(
              factory_.CreateSendToExplicitReceiversUncheckedProcedure());
          break;
        case ProcedureID::kSendToAny:
          procedure_buf.reset(factory_.CreateSendToAnyProcedure());
          break;
        case ProcedureID::kReceive:
          procedure_buf.reset(factory_.CreateReceiveProcedure());
          break;
        case ProcedureID::kReceiveAndDelete:
          procedure_buf.reset(factory_.CreateReceiveAndDeleteProcedure());
          break;
        case ProcedureID::kDeleteMessages:
          procedure_buf.reset(factory_.CreateDeleteMessagesProcedure());
          break;
        case ProcedureID::kDeleteMessagesUnchecked:
          procedure_buf.reset(
              factory_.CreateDeleteMessagesUncheckedProcedure());
          break;
        case ProcedureID::kCancelMessages:
          procedure_buf.reset(factory_.CreateCancelMessagesProcedure());
          break;
        case ProcedureID::kLoadState:
          procedure_buf.reset(factory_.CreateLoadStateProcedure());
          break;
        case ProcedureID::kCountQueuedMessagesForClient:
          procedure_buf.reset(
              factory_.CreateCountQueuedMessagesForClientProcedure());
          break;
        default:
          std::fprintf(stderr,
                       "FATAL ERROR: unhandled case in "
                           "VoltDBProcedureWarehouse::GetProcedure()\n");
          std::exit(1);
      }
    }
    return ProcedureHandle(procedure_id, procedure_buf.release(), this);
  }

 private:
  inline void AddToPool(const ProcedureID procedure_id,
                        voltdb::Procedure *procedure) {
    procedure_pools_[static_cast<std::uint8_t>(procedure_id)].Emplace(
        procedure);
  }

  VoltDBProcedureFactory factory_;
  LockFreeStack<std::unique_ptr<voltdb::Procedure>> procedure_pools_[
      static_cast<std::uint8_t>(ProcedureID::kNumProcedureIDs)];

  friend class ProcedureHandle;

  // Disallow copy and assign.
  VoltDBProcedureWarehouse(const VoltDBProcedureWarehouse &orig) = delete;
  VoltDBProcedureWarehouse& operator=(const VoltDBProcedureWarehouse &rhs)
      = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_VOLTDB_PROCEDURE_WAREHOUSE_H_
