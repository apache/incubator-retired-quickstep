// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "tmb/internal/voltdb_procedure_factory.h"

#include <vector>

#include "Parameter.hpp"   // NOLINT(build/include)
#include "WireType.h"      // NOLINT(build/include)

namespace tmb {
namespace internal {

VoltDBProcedureFactory::VoltDBProcedureFactory() {
  single_client_id_params_.emplace_back(voltdb::WIRE_TYPE_INTEGER);

  register_params_.emplace_back(voltdb::WIRE_TYPE_INTEGER);
  register_params_.emplace_back(voltdb::WIRE_TYPE_INTEGER);

  send_to_single_explicit_receiver_params_.emplace_back(
      voltdb::WIRE_TYPE_INTEGER);
  send_to_single_explicit_receiver_params_.emplace_back(
      voltdb::WIRE_TYPE_INTEGER);
  send_to_single_explicit_receiver_params_.emplace_back(
      voltdb::WIRE_TYPE_TIMESTAMP);
  send_to_single_explicit_receiver_params_.emplace_back(
      voltdb::WIRE_TYPE_SMALLINT);
  send_to_single_explicit_receiver_params_.emplace_back(
      voltdb::WIRE_TYPE_INTEGER);
  send_to_single_explicit_receiver_params_.emplace_back(
      voltdb::WIRE_TYPE_VARBINARY);

  send_to_single_explicit_receiver_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_INTEGER);
  send_to_single_explicit_receiver_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_INTEGER);
  send_to_single_explicit_receiver_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_TIMESTAMP);
  send_to_single_explicit_receiver_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_TIMESTAMP);
  send_to_single_explicit_receiver_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_SMALLINT);
  send_to_single_explicit_receiver_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_INTEGER);
  send_to_single_explicit_receiver_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_VARBINARY);

  send_to_explicit_receivers_params_.emplace_back(voltdb::WIRE_TYPE_INTEGER);
  send_to_explicit_receivers_params_.emplace_back(voltdb::WIRE_TYPE_INTEGER,
                                                  true);
  send_to_explicit_receivers_params_.emplace_back(voltdb::WIRE_TYPE_TIMESTAMP);
  send_to_explicit_receivers_params_.emplace_back(voltdb::WIRE_TYPE_SMALLINT);
  send_to_explicit_receivers_params_.emplace_back(voltdb::WIRE_TYPE_INTEGER);
  send_to_explicit_receivers_params_.emplace_back(voltdb::WIRE_TYPE_VARBINARY);
  send_to_explicit_receivers_params_.emplace_back(voltdb::WIRE_TYPE_TINYINT);
  send_to_explicit_receivers_params_.emplace_back(voltdb::WIRE_TYPE_TINYINT);

  send_to_explicit_receivers_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_INTEGER);
  send_to_explicit_receivers_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_INTEGER,
      true);
  send_to_explicit_receivers_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_TIMESTAMP);
  send_to_explicit_receivers_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_TIMESTAMP);
  send_to_explicit_receivers_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_SMALLINT);
  send_to_explicit_receivers_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_INTEGER);
  send_to_explicit_receivers_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_VARBINARY);
  send_to_explicit_receivers_unchecked_params_.emplace_back(
      voltdb::WIRE_TYPE_TINYINT);

  send_to_any_params_.emplace_back(voltdb::WIRE_TYPE_INTEGER);
  send_to_any_params_.emplace_back(voltdb::WIRE_TYPE_TIMESTAMP);
  send_to_any_params_.emplace_back(voltdb::WIRE_TYPE_SMALLINT);
  send_to_any_params_.emplace_back(voltdb::WIRE_TYPE_INTEGER);
  send_to_any_params_.emplace_back(voltdb::WIRE_TYPE_VARBINARY);
  send_to_any_params_.emplace_back(voltdb::WIRE_TYPE_TINYINT);
  send_to_any_params_.emplace_back(voltdb::WIRE_TYPE_TINYINT);

  receive_params_.emplace_back(voltdb::WIRE_TYPE_INTEGER);
  receive_params_.emplace_back(voltdb::WIRE_TYPE_SMALLINT);
  receive_params_.emplace_back(voltdb::WIRE_TYPE_BIGINT);

  delete_messages_params_.emplace_back(voltdb::WIRE_TYPE_INTEGER);
  delete_messages_params_.emplace_back(voltdb::WIRE_TYPE_BIGINT, true);

  cancel_messages_params_.emplace_back(voltdb::WIRE_TYPE_BIGINT, true);
}

}  // namespace internal
}  // namespace tmb
