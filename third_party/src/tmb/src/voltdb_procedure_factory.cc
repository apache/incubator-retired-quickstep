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
