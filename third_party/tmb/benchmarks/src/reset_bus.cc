// Copyright (c) 2014-2015, Quickstep Technologies LLC.
// Copyright (c) 2015, Pivotal Software, Inc.
// All Rights Reserved.

#include <iostream>
#include <memory>

#include "gflags/gflags.h"
#include "tmb/message_bus.h"
#include "tmbbench/bus_setup.h"

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  if (argc != 1) {
    std::cerr << "Unrecognized command-line arguments.\n";
    return 1;
  }

  std::unique_ptr<tmb::MessageBus> message_bus(
      tmbbench::SetupBusAllInOneDistributed());
  if (!message_bus) {
    return 1;
  }

  message_bus->ResetBus();
  return 0;
}
