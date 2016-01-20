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

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "gflags/gflags.h"
#include "grpc/grpc.h"
#include "grpc++/security/server_credentials.h"
#include "grpc++/server.h"
#include "grpc++/server_builder.h"
#include "tmb/internal/net_service_impl.h"
#include "tmb/message_bus.h"

// ----------------------------------------------------------------------------
// Shared routines for parsing addresses of upstream servers.

struct ServerAddress {
  std::string hostname;
  std::uint16_t port;
};

bool ParseServerAddresses(const std::string &address_string,
                          std::vector<ServerAddress> *addresses) {
  std::size_t pos = 0;
  while (pos < address_string.size()) {
    std::size_t endpos = address_string.find(',', pos);
    if (endpos == pos) {
      // String included double-comma.
      return false;
    }
    std::string host_string = address_string.substr(pos, endpos - pos);
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

    ServerAddress addr;
    addr.hostname = host_string.substr(0, colon_pos);
    addr.port = port;
    addresses->push_back(std::move(addr));

    pos = (endpos == std::string::npos) ? endpos : endpos + 1;
  }

  return true;
}

#if defined(TMB_VOLTDB_ENABLED) || defined(TMB_ZOOKEEPER_ENABLED)
static bool ValidateServerAddresses(const char *flagname,
                                    const std::string &value) {
  // Just do an actual parse to validate the specified servers.
  std::vector<ServerAddress> address_scratch;
  if (ParseServerAddresses(value, &address_scratch)) {
    return true;
  }

  std::fprintf(stderr,
               "Invalid value for --%s: %s\n", flagname, value.c_str());
  return false;
}
#endif  // defined(TMB_VOLTDB_ENABLED) || defined(TMB_ZOOKEEPER_ENABLED)

// ----------------------------------------------------------------------------
// Conditionally-enabled includes and implementation-specific flags for various
// TMB implementations.

#if defined(TMB_LEVELDB_ENABLED) && defined(TMB_MEMORYMIRROR_ENABLED)

#include "tmb/leveldb_message_bus.h"

DEFINE_string(leveldb_path, "tmb_leveldb",
              "[leveldb] Path for LevelDB database.");
DEFINE_bool(leveldb_sync_writes, true,
            "[leveldb] Synchronously write records.");

#endif  // TMB_LEVELDB_ENABLED && TMB_MEMORYMIRROR_ENABLED


#ifdef TMB_MEMORYMIRROR_ENABLED

#include "tmb/memory_mirror_message_bus.h"

DEFINE_bool(memory_mirror, true,
            "Turn on memory mirroring (usually improves performance, but all "
            "connections to the underlying bus must go through this server). "
            "Has no effect if tmb_implementation is purememory or "
            "nativelogging (effectively always on in such cases). This MUST "
            "be turned on if tmb_implementation is leveldb or zookeeper.");

#endif  // TMB_MEMORYMIRROR_ENABLED


#ifdef TMB_NATIVELOG_ENABLED

#include "tmb/native_logging_message_bus.h"

DEFINE_string(nativelog_filename, "tmb.log",
              "[nativelog] Filename for native transaction log.");
DEFINE_bool(nativelog_sync_writes, true,
            "[nativelog] Synchronously write log records.");

#endif  // TMB_NATIVELOG_ENABLED


#ifdef TMB_PUREMEMORY_ENABLED

#include "tmb/pure_memory_message_bus.h"

#endif  // TMB_PUREMEMORY_ENABLED


#ifdef TMB_SQLITE_ENABLED

#include "tmb/sqlite_message_bus.h"

DEFINE_string(sqlite_filename, "tmb.db",
              "[sqlite] Filename for database.");
DEFINE_bool(sqlite_use_wal, true,
            "[sqlite] Use write-ahead logging.");

#endif  // TMB_SQLITE_ENABLED


#ifdef TMB_VOLTDB_ENABLED

#include "tmb/voltdb_message_bus.h"

DEFINE_string(voltdb_servers, "",
              "[voltdb] A comma-separated list of VoltDB servers to connect "
              "to, each of which is given in HOSTNAME:PORT or IP:PORT "
              "format.");
static const bool voltdb_servers_dummy = gflags::RegisterFlagValidator(
    &FLAGS_voltdb_servers,
    &ValidateServerAddresses);

DEFINE_string(voltdb_username, "",
              "[voltdb] Username for connecting to VoltDB servers (if blank, "
              "will attempt to use unauthenticated connection).");
DEFINE_string(voltdb_password, "",
              "[voltdb] Password for connecting to VoltDB servers.");

#endif  // TMB_VOLTDB_ENABLED


#if defined(TMB_ZOOKEEPER_ENABLED) && defined(TMB_MEMORYMIRROR_ENABLED)

#include "tmb/zookeeper_message_bus.h"

DEFINE_string(zookeeper_servers, "",
              "[zookeeper] A comma-separated list of Zookeeper servers to "
              "connect to, each of which is given in HOSTNAME:PORT or IP:PORT "
              "format.");
static const bool zookeeper_servers_dummy = gflags::RegisterFlagValidator(
    &FLAGS_zookeeper_servers,
    &ValidateServerAddresses);

DEFINE_string(zookeeper_path_prefix, "",
              "[zookeeper] The prefix in the zookeeper pseudo-filesystem tree "
              "to store TMB data under.");

#endif  // TMB_ZOOKEEPER_ENABLED && TMB_MEMORYMIRROR_ENABLED

// ----------------------------------------------------------------------------
// Common flags for setting up server.

DEFINE_bool(enable_deletion, true,
            "Enable separate deletion support for bus implementations where "
            "it is optional (purememory, nativelog, or any bus with memory "
            "mirroring enabled). For other busses, separate deletion support "
            "is always available regardless of this flag.");

static bool ValidateTmbImplementation(const char *flagname,
                                      const std::string &value) {
#if defined(TMB_LEVELDB_ENABLED) && defined(TMB_MEMORYMIRROR_ENABLED)
  if (value == "leveldb") {
    return true;
  }
#endif

#ifdef TMB_NATIVELOG_ENABLED
  if (value == "nativelog") {
    return true;
  }
#endif

#ifdef TMB_PUREMEMORY_ENABLED
  if (value == "purememory") {
    return true;
  }
#endif  // TMB_SQLITE_ENABLED

#ifdef TMB_SQLITE_ENABLED
  if (value == "sqlite") {
    return true;
  }
#endif  // TMB_SQLITE_ENABLED

#ifdef TMB_VOLTDB_ENABLED
  if (value == "voltdb") {
    return true;
  }
#endif

#if defined(TMB_ZOOKEEPER_ENABLED) && defined(TMB_MEMORYMIRROR_ENABLED)
  if (value == "zookeeper") {
    return true;
  }
#endif

  std::fprintf(stderr,
               "Invalid value for --%s: %s\n", flagname, value.c_str());
  return false;
}
DEFINE_string(tmb_implementation, "nativelog",
              "Which underlying TMB implementation to use. Valid choices are "
              "leveldb, nativelog, purememory, sqlite, voltdb, and zookeeper. "
              "Depending on how the TMB library was built, some of these may "
              "not be available.");
static const bool tmb_implementation_dummy = gflags::RegisterFlagValidator(
    &FLAGS_tmb_implementation,
    &ValidateTmbImplementation);

// NOTE(chasseur): Port 4575 is towards the middle of a decently-sized block of
// unassigned port numbers (as of 2015-04-21). If we do a public release of
// TMB, it might be worth trying to secure an IANA registration for a port.
DEFINE_string(server_address, "0.0.0.0:4575",
              "The address and port to listen for connections on.");

DEFINE_bool(reset_bus, false,
            "If true, reset the bus and clear any existing state on startup.");

// ----------------------------------------------------------------------------
// Function to actually create a MessageBus instance as specified by flags.

tmb::MessageBus* CreateMessageBus() {
#if defined(TMB_LEVELDB_ENABLED) && defined(TMB_MEMORYMIRROR_ENABLED)
  if (FLAGS_tmb_implementation == "leveldb") {
    return new tmb::LevelDBMessageBus(FLAGS_leveldb_path,
                                      FLAGS_leveldb_sync_writes);
  }
#endif

#ifdef TMB_NATIVELOG_ENABLED
  if (FLAGS_tmb_implementation == "nativelog") {
    if (FLAGS_enable_deletion) {
      return new tmb::NativeLoggingMessageBus<true>(
          FLAGS_nativelog_filename,
          FLAGS_nativelog_sync_writes,
          true);
    } else {
      return new tmb::NativeLoggingMessageBus<false>(
          FLAGS_nativelog_filename,
          FLAGS_nativelog_sync_writes,
          true);
    }
  }
#endif

#ifdef TMB_PUREMEMORY_ENABLED
  if (FLAGS_tmb_implementation == "purememory") {
    if (FLAGS_enable_deletion) {
      return new tmb::PureMemoryMessageBus<true>(true);
    } else {
      return new tmb::PureMemoryMessageBus<false>(true);
    }
  }
#endif

#ifdef TMB_SQLITE_ENABLED
  if (FLAGS_tmb_implementation == "sqlite") {
    return new tmb::SQLiteMessageBus(
        FLAGS_sqlite_filename,
        FLAGS_sqlite_use_wal);
  }
#endif

#ifdef TMB_VOLTDB_ENABLED
  if (FLAGS_tmb_implementation == "voltdb") {
    tmb::VoltDBMessageBus *bus = FLAGS_voltdb_username.empty()
        ? new tmb::VoltDBMessageBus()
        : new tmb::VoltDBMessageBus(FLAGS_voltdb_username,
                                    FLAGS_voltdb_password);
    std::vector<ServerAddress> addresses;
    const bool parse_ok = ParseServerAddresses(FLAGS_voltdb_servers,
                                               &addresses);
    assert(parse_ok);
    for (const ServerAddress &address : addresses) {
      bus->AddServer(address.hostname, address.port);
    }
    return bus;
  }
#endif

#if defined(TMB_ZOOKEEPER_ENABLED) && defined(TMB_MEMORYMIRROR_ENABLED)
  if (FLAGS_tmb_implementation == "zookeeper") {
    tmb::ZookeeperMessageBus *bus = new tmb::ZookeeperMessageBus(
        FLAGS_zookeeper_path_prefix);
    std::vector<ServerAddress> addresses;
    const bool parse_ok = ParseServerAddresses(FLAGS_zookeeper_servers,
                                               &addresses);
    assert(parse_ok);
    for (const ServerAddress &address : addresses) {
      bus->AddServer(address.hostname, address.port);
    }
    return bus;
  }
#endif

  return nullptr;
}

// ----------------------------------------------------------------------------
// Functions to create a MemoryMirrorMessageBus as needed.

#ifdef TMB_MEMORYMIRROR_ENABLED

bool MemoryMirrorRequired() {
  if (FLAGS_tmb_implementation == "leveldb"
      || FLAGS_tmb_implementation == "zookeeper") {
    return true;
  } else {
    return false;
  }
}

bool MemoryMirrorEnabled() {
  if (FLAGS_tmb_implementation == "nativelog"
      || FLAGS_tmb_implementation == "purememory") {
    return false;
  } else {
    return FLAGS_memory_mirror;
  }
}

tmb::MessageBus* CreateMemoryMirror(tmb::MessageBus *underlying_bus) {
#ifdef TMB_LEVELDB_ENABLED
  if (FLAGS_tmb_implementation == "leveldb") {
    if (FLAGS_enable_deletion) {
      return new tmb::MemoryMirrorMessageBus<true>(
          static_cast<tmb::LevelDBMessageBus*>(underlying_bus),
          true);
    } else {
      return new tmb::MemoryMirrorMessageBus<false>(
          static_cast<tmb::LevelDBMessageBus*>(underlying_bus),
          true);
    }
  }
#endif

#ifdef TMB_SQLITE_ENABLED
  if (FLAGS_tmb_implementation == "sqlite") {
    if (FLAGS_enable_deletion) {
      return new tmb::MemoryMirrorMessageBus<true>(
          static_cast<tmb::SQLiteMessageBus*>(underlying_bus),
          true);
    } else {
      return new tmb::MemoryMirrorMessageBus<false>(
          static_cast<tmb::SQLiteMessageBus*>(underlying_bus),
          true);
    }
  }
#endif

#ifdef TMB_VOLTDB_ENABLED
  if (FLAGS_tmb_implementation == "voltdb") {
    if (FLAGS_enable_deletion) {
      return new tmb::MemoryMirrorMessageBus<true>(
          static_cast<tmb::VoltDBMessageBus*>(underlying_bus),
          true);
    } else {
      return new tmb::MemoryMirrorMessageBus<false>(
          static_cast<tmb::VoltDBMessageBus*>(underlying_bus),
          true);
    }
  }
#endif

#ifdef TMB_ZOOKEEPER_ENABLED
  if (FLAGS_tmb_implementation == "zookeeper") {
    if (FLAGS_enable_deletion) {
      return new tmb::MemoryMirrorMessageBus<true>(
          static_cast<tmb::ZookeeperMessageBus*>(underlying_bus),
          true);
    } else {
      return new tmb::MemoryMirrorMessageBus<false>(
          static_cast<tmb::ZookeeperMessageBus*>(underlying_bus),
          true);
    }
  }
#endif

  return nullptr;
}

#endif  // TMB_MEMORYMIRROR_ENABLED

// ----------------------------------------------------------------------------
// Server main()

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  if (argc != 1) {
    std::fprintf(
        stderr,
        "Unrecognized command-line arguments. Try running %s --help\n",
        argv[0]);
    return 1;
  }

#ifdef TMB_MEMORYMIRROR_ENABLED
  if (MemoryMirrorRequired() && !MemoryMirrorEnabled()) {
    std::fprintf(stderr,
                 "You have selected tmb_implementation %s, which requires "
                     "memory_mirror set to true\n",
                 FLAGS_tmb_implementation.c_str());
    return 1;
  }
#endif  // TMB_MEMORYMIRROR_ENABLED

  grpc_init();

  std::printf("Creating message bus... ");
  std::fflush(stdout);
  tmb::MessageBus *bus = CreateMessageBus();
  if (bus != nullptr) {
    std::printf("OK\n");
  } else {
    std::printf("FAILED\n");
    return 1;
  }

  std::printf("Initializing bus... ");
  std::fflush(stdout);
  if (bus->Initialize()) {
    std::printf("OK\n");
  } else {
    std::printf("FAILED\n");
    delete bus;
    return 1;
  }

  if (FLAGS_reset_bus) {
    std::printf("Resetting bus... ");
    std::fflush(stdout);
    bus->ResetBus();
    std::printf("OK\n");
  }

#ifdef TMB_MEMORYMIRROR_ENABLED
  if (MemoryMirrorEnabled()) {
    std::printf("Creating memory mirror... ");
    std::fflush(stdout);
    tmb::MessageBus *mirror = CreateMemoryMirror(bus);
    if (mirror != nullptr) {
      std::printf("OK\n");
      bus = mirror;
    } else {
      std::printf("FAILED\n");
      delete bus;
      return 1;
    }

    std::printf("Initializing memory mirror (loading state)... ");
    std::fflush(stdout);
    if (bus->Initialize()) {
      std::printf("OK\n");
    } else {
      std::printf("FAILED\n");
      delete bus;
      return 1;
    }
  }
#endif  // TMB_MEMORYMIRROR_ENABLED

  tmb::internal::NetServiceImpl tmb_service(bus);

  // TODO(chasseur): Currently we just start the server with insecure
  // credentials. We should add support for GRPC's built-in TLS encryption and
  // authentication.
  grpc::ServerBuilder builder;
  builder.RegisterService(&tmb_service);
  builder.AddListeningPort(FLAGS_server_address,
                           grpc::InsecureServerCredentials());
  std::printf("Starting server on %s ... ", FLAGS_server_address.c_str());
  std::fflush(stdout);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  if (bus->Initialize()) {
    std::printf("OK\n");
  } else {
    std::printf("FAILED\n");
    return 1;
  }

  server->Wait();

  grpc_shutdown();

  return 0;
}
