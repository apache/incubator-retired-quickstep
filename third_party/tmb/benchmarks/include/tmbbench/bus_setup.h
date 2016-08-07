//   Copyright 2014-2015 Quickstep Technologies LLC.
// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
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
