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

#include "cli/distributed/Role.hpp"

#include <cstdio>
#include <cstdint>

#include "gflags/gflags.h"

#include "tmb/native_net_client_message_bus.h"

namespace quickstep {

DEFINE_string(tmb_server_ip, "127.0.0.1", "IP Address of the TMB Server.");

static bool ValidateTmbServerPort(const char *flagname,
                                 std::int32_t value) {
  if (value > 0 && value < 65536) {
    return true;
  } else {
    std::fprintf(stderr, "--%s must be between 1 and 65535 (inclusive)\n", flagname);
    return false;
  }
}
DEFINE_int32(tmb_server_port, 4575, "Port of the TMB Server.");
static const bool tmb_server_port_dummy
    = gflags::RegisterFlagValidator(&FLAGS_tmb_server_port, &ValidateTmbServerPort);

Role::Role() {
  bus_.AddServer(FLAGS_tmb_server_ip, FLAGS_tmb_server_port);
  bus_.Initialize();
}

}  // namespace quickstep
