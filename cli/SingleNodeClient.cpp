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

#include "cli/SingleNodeClient.hpp"

#include "gflags/gflags.h"

static bool ValidatePort(const char *flagname, std::int32_t value) {
  if (value > 0 && value < 65536) {
    return true;
  }
  std::printf("Invalid value for --%s: %d\n", flagname, static_cast<int>(value));
  return false;
}

DEFINE_int32(port, 3000, "Listens for TCP connection on this port.");
DEFINE_validator(port, &ValidatePort);
