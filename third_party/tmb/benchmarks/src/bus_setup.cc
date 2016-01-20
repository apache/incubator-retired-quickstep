//   Copyright 2014-2015 Quickstep Technologies LLC.
//   Copyright 2015 Pivotal Software, Inc.
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

#include "tmbbench/bus_setup.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "gflags/gflags.h"
#include "tmb/message_bus.h"

// Conditionally include MessageBus implementations that are enabled in
// TMB library.
#ifdef TMB_LEVELDB_ENABLED
#include "tmb/leveldb_message_bus.h"
#endif

#ifdef TMB_MEMORYMIRROR_ENABLED
#include "tmb/memory_mirror_message_bus.h"
#endif

#ifdef TMB_NATIVELOG_ENABLED
#include "tmb/native_logging_message_bus.h"
#endif

#ifdef TMB_NATIVENET_ENABLED
#include "tmb/native_net_client_message_bus.h"
#endif

#ifdef TMB_PUREMEMORY_ENABLED
#include "tmb/pure_memory_message_bus.h"
#endif

#ifdef TMB_SQLITE_ENABLED
#include "tmb/sqlite_message_bus.h"
#endif

#ifdef TMB_VOLTDB_ENABLED
#include "tmb/voltdb_message_bus.h"
#endif

#ifdef TMB_ZOOKEEPER_ENABLED
#include "tmb/zookeeper_message_bus.h"
#endif

namespace tmbbench {

// Command-line flags to select bus type and options.
DEFINE_string(tmb_implementation, "PureMemory",
              "Which TMB implementation should be used. Options are "
              "PureMemory, NativeLog, NativeNet, SQLite, LevelDB, Zookeeper, "
              "or VoltDB.");

#ifdef TMB_LEVELDB_ENABLED
DEFINE_bool(leveldb_sync_writes, true,
            "Whether to use synchronous writes (safer, but slower) with "
            "LevelDB. Has no effect with other TMB implementations.");
#endif

#ifdef TMB_NATIVELOG_ENABLED
DEFINE_bool(native_log_sync_writes, true,
            "Whether to use synchronous writes (safer, but slower) with the "
            "native transaction log. Has no effect with other TMB "
            "implementations.");
#endif

#ifdef TMB_MEMORYMIRROR_ENABLED
DEFINE_bool(memory_mirror, false,
            "Whether to use a MemoryMirrorMessageBus as in an-memory cache "
            "in front of a persistent message bus.");
#endif

#if defined(TMB_NATIVENET_ENABLED) \
    || defined(TMB_VOLTDB_ENABLED) \
    || defined(TMB_ZOOKEEPER_ENABLED)
DEFINE_string(servers, "",
              "For NativeNet, VoltDB, and Zookeeper implementations, "
              "specifies a comma-separated list of servers to connect to, "
              "each of which is given in HOSTNAME:PORT or IP:PORT format. "
              "Has no effect for other TMB implementations.");
#endif

namespace {

struct ServerDescription {
  std::string hostname;
  std::uint16_t port;
};

bool ParseServersString(const std::string &servers_string,
                        std::vector<ServerDescription> *descriptions) {
  std::size_t pos = 0;
  while (pos < servers_string.size()) {
    std::size_t endpos = servers_string.find(',', pos);
    if (endpos == pos) {
      // String included double-comma.
      return false;
    }
    std::string host_string = servers_string.substr(pos, endpos - pos);
    std::size_t colon_pos = host_string.find(':');
    if (colon_pos == 0) {
      // Empty hostname.
      return false;
    }
    if (colon_pos == std::string::npos) {
      // Improperly formatted server string (no ":" character).
      return false;
    }

    char *endptr = nullptr;
    long parsed_port  // NOLINT(runtime/int)
        = std::strtol(host_string.c_str() + colon_pos + 1,
                      &endptr,
                      10);
    if ((endptr == host_string.c_str() + colon_pos + 1) || (*endptr != '\0')) {
      // Improperly formatted port number.
      return false;
    }

    std::uint16_t port = parsed_port;
    if (parsed_port != port) {
      // Port number out of range.
      return false;
    }

    ServerDescription desc;
    desc.hostname = host_string.substr(0, colon_pos);
    desc.port = port;
    descriptions->push_back(std::move(desc));

    pos = (endpos == std::string::npos) ? endpos : endpos + 1;
  }

  return true;
}

}  // namespace

tmb::MessageBus* CreateBus(const bool delete_messages_immediately) {
#ifdef TMB_MEMORYMIRROR_ENABLED
  const bool use_memory_mirror = FLAGS_memory_mirror;
#else
  const bool use_memory_mirror = false;
#endif

#ifdef TMB_LEVELDB_ENABLED
  if (FLAGS_tmb_implementation == "LevelDB") {
    return new tmb::LevelDBMessageBus("_benchmark_leveldb",
                                      FLAGS_leveldb_sync_writes);
  }
#endif

#ifdef TMB_NATIVELOG_ENABLED
  if (FLAGS_tmb_implementation == "NativeLog") {
    if (use_memory_mirror) {
      std::cerr << "Can't use memory_mirror with NativeLoggingMessageBus (it "
                   "already is one, effectively).\n";
      return nullptr;
    }

    if (delete_messages_immediately) {
      return new tmb::NativeLoggingMessageBus<false>(
          "_benchmark_log.tmb",
          FLAGS_native_log_sync_writes);
    } else {
      return new tmb::NativeLoggingMessageBus<true>(
          "_benchmark_log.tmb",
          FLAGS_native_log_sync_writes);
    }
  }
#endif

#ifdef TMB_NATIVENET_ENABLED
  if (FLAGS_tmb_implementation == "NativeNet") {
    if (use_memory_mirror) {
      std::cerr << "Can't use memory_mirror with NativeNetClientMessageBus.\n";
      return nullptr;
    }

    return new tmb::NativeNetClientMessageBus();
  }
#endif

#ifdef TMB_PUREMEMORY_ENABLED
  if (FLAGS_tmb_implementation == "PureMemory") {
    if (use_memory_mirror) {
      std::cerr << "Can't use memory_mirror with PureMemoryMessageBus.\n";
      return nullptr;
    }

    if (delete_messages_immediately) {
      return new tmb::PureMemoryMessageBus<false>();
    } else {
      return new tmb::PureMemoryMessageBus<true>();
    }
  }
#endif

#ifdef TMB_SQLITE_ENABLED
  if (FLAGS_tmb_implementation == "SQLite") {
    return new tmb::SQLiteMessageBus("_benchmark.db");
  }
#endif

#ifdef TMB_VOLTDB_ENABLED
  if (FLAGS_tmb_implementation == "VoltDB") {
    return new tmb::VoltDBMessageBus();
  }
#endif

#ifdef TMB_ZOOKEEPER_ENABLED
  if (FLAGS_tmb_implementation == "Zookeeper") {
    return new tmb::ZookeeperMessageBus("/_tmb_benchmark");
  }
#endif

  std::cerr << "Unrecognized option for -tmb_implementation\n";
  return nullptr;
}

bool AddServersToBus(tmb::MessageBus *bus) {
#if defined(TMB_NATIVENET_ENABLED) \
    || defined(TMB_VOLTDB_ENABLED) \
    || defined(TMB_ZOOKEEPER_ENABLED)
  std::vector<ServerDescription> server_descriptions;
  if (!ParseServersString(FLAGS_servers, &server_descriptions)) {
    std::cerr << "Unable to parse servers (expected a list of HOST:PORT pairs "
                 "separated by commas).\n";
    return false;
  }

  for (const tmbbench::ServerDescription &desc : server_descriptions) {
    bus->AddServer(desc.hostname, desc.port);
  }
#endif

  return true;
}

tmb::MessageBus* CreateMemoryMirrorIfRequested(
    tmb::MessageBus* bus,
    const bool delete_messages_immediately) {
#ifdef TMB_MEMORYMIRROR_ENABLED
  if (FLAGS_memory_mirror) {
    tmb::internal::PersistentBusStateInterface *persistent_bus
        = dynamic_cast<tmb::internal::PersistentBusStateInterface*>(bus);
    tmb::MessageBus *mirror
        = delete_messages_immediately
          ? static_cast<tmb::MessageBus*>(
              new tmb::MemoryMirrorMessageBus<false>(persistent_bus))
          : static_cast<tmb::MessageBus*>(
              new tmb::MemoryMirrorMessageBus<true>(persistent_bus));
    mirror->Initialize();
    return mirror;
  }
#endif

  return bus;
}

tmb::MessageBus* SetupBusAllInOne(const bool delete_messages_immediately) {
  tmb::MessageBus *bus = CreateBus(delete_messages_immediately);
  if (bus == nullptr) {
    return nullptr;
  }

  if (!AddServersToBus(bus)) {
    delete bus;
    return nullptr;
  }

  if (!bus->Initialize()) {
    std::cerr << "ERROR: Unable to connect to or initialize bus. Check that "
                 "you have write permissions in this directory and that you "
                 "correctly specified servers, if applicable.\n";
    delete bus;
    return nullptr;
  }

  bus->ResetBus();

  return CreateMemoryMirrorIfRequested(bus, delete_messages_immediately);
}

tmb::MessageBus* SetupBusAllInOneDistributed() {
  if (!((FLAGS_tmb_implementation == "NativeNet")
        || (FLAGS_tmb_implementation == "VoltDB")
        || (FLAGS_tmb_implementation == "Zookeeper"))) {
    std::cerr << "ERROR: Distributed experiments must have tmb_implementation "
                 "set to one of NativeNet, VoltDB, or Zookeeper.\n";
    return nullptr;
  }

#ifdef TMB_MEMORYMIRROR_ENABLED
  if (FLAGS_memory_mirror) {
    std::cerr << "ERROR: Can not use memory_mirror in distributed "
                 "experiments\n";
    return nullptr;
  }
#endif

  tmb::MessageBus *bus = CreateBus(false);
  if (bus == nullptr) {
    return nullptr;
  }

  if (!AddServersToBus(bus)) {
    delete bus;
    return nullptr;
  }

  if (!bus->Initialize()) {
    std::cerr << "ERROR: Unable to connect to or initialize bus. Check that "
                 "you correctly specified servers.\n";
    delete bus;
    return nullptr;
  }

  return bus;
}

}  // namespace tmbbench
