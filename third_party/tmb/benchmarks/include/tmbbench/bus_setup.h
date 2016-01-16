// Copyright (c) 2014-2015, Quickstep Technologies LLC.
// Copyright (c) 2015, Pivotal Software, Inc.
// All Rights Reserved.

#ifndef TMBBENCH_BUS_SETUP_H_
#define TMBBENCH_BUS_SETUP_H_

namespace tmb { class MessageBus; }

namespace tmbbench {

// Create a MessageBus object with type and options specified by command-line
// flags. Prints an error message and returns NULL in case of an error.
tmb::MessageBus* CreateBus(const bool delete_messages_immediately);

// Adds servers specified on command-line '*bus' as necessary. Returns true on
// success, prints an error and returns false on failure.
bool AddServersToBus(tmb::MessageBus *bus);

// If command-line flags indicate that a memory mirror should be used, create
// one on top of (and owning) '*bus'. Otherwise just return '*bus' as-is.
tmb::MessageBus* CreateMemoryMirrorIfRequested(
    tmb::MessageBus* bus,
    const bool delete_messages_immediately);

// Do everything necessary to set up a MessageBus according to command-line
// arguments. Creates bus, connects to remote servers if necessary,
// initializes and resets bus, and creates a memory mirror if necessary. If an
// error occurs at any point, an error message is printed and this function
// returns NULL.
tmb::MessageBus* SetupBusAllInOne(const bool delete_messages_immediately);

// Version of SetupBusAllInOne() for distributed experiments. Only allows bus
// to be NativeNet, VoltDB, or Zookeeper without memory mirror.
tmb::MessageBus* SetupBusAllInOneDistributed();

}  // namespace tmbbench

#endif  // TMBBENCH_BUS_SETUP_H_
