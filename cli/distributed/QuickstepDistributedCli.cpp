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

/* A standalone command-line interface to Distributed QuickStep */

#include <memory>
#include <string>

#include "cli/distributed/Cli.hpp"
#include "cli/distributed/Conductor.hpp"
#include "cli/distributed/Executor.hpp"
#include "cli/distributed/Role.hpp"
#include "utility/StringUtil.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "grpc/grpc.h"

using std::make_unique;

namespace quickstep {
namespace optimizer {

DECLARE_bool(use_lip_filters);
DECLARE_bool(use_filter_joins);

}  // namespace optimizer

constexpr char kCliRole[] = "cli";
constexpr char kConductorRole[] = "conductor";
constexpr char kExecutorRole[] = "executor";

DEFINE_string(role, "",
              "The role in the distributed Quickstep: Conductor, Executor, or Cli.");
static bool ValidateRole(const char *flagname,
                         const std::string &value) {
  if (value.empty()) {
    return false;
  }

  FLAGS_role = ToLower(value);
  return FLAGS_role == kCliRole ||
         FLAGS_role == kConductorRole ||
         FLAGS_role == kExecutorRole;
}
static const volatile bool role_dummy
    = gflags::RegisterFlagValidator(&FLAGS_role, &ValidateRole);

}  // namespace quickstep

using quickstep::FLAGS_role;

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);

  // TODO(quickstep-team): Fix JIRA QUICKSTEP-94 for adding LIP filter support
  // with partitioned inputs in the distributed version.
  quickstep::optimizer::FLAGS_use_lip_filters = false;
  quickstep::optimizer::FLAGS_use_filter_joins = false;

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  grpc_init();

  std::unique_ptr<quickstep::Role> role;
  if (FLAGS_role == quickstep::kCliRole) {
    role = make_unique<quickstep::Cli>();
  } else if (FLAGS_role == quickstep::kConductorRole) {
    role = make_unique<quickstep::Conductor>();
  } else if (FLAGS_role == quickstep::kExecutorRole) {
    role = make_unique<quickstep::Executor>();
  }

  role->init();
  LOG(INFO) << FLAGS_role << " is ready";

  role->run();

  return 0;
}
