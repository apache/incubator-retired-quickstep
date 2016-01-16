// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_VOLTDB_CONNECTION_POOL_H_
#define TMB_INTERNAL_VOLTDB_CONNECTION_POOL_H_

#include <cassert>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Client.h"                // NOLINT(build/include)
#include "ClientConfig.h"          // NOLINT(build/include)
#include "InvocationResponse.hpp"  // NOLINT(build/include)

#include "tmb/internal/lock_free_stack.h"

namespace voltdb { class Procedure; }

namespace tmb {
namespace internal {

/**
 * @brief A shared pool of reusable VoltDB client connections. A client may
 *        call InvokeProcedure() to synchronously invoke a VoltDB procedure
 *        using one of the connections in the pool (if no connection is
 *        available at the moment InvokeProcedure() is called, a new one will
 *        be created and added to the pool).
 **/
class VoltDBConnectionPool {
 public:
  VoltDBConnectionPool()
      : initialized_(false) {
  }

  VoltDBConnectionPool(const std::string &username,
                       const std::string &password)
      : config_(username, password),
        initialized_(false) {
  }

  inline void AddServer(const std::string &hostname,
                        const std::uint16_t port) {
    assert(!initialized_);
    servers_.emplace_back(hostname, port);
  }

  bool Initialize();

  inline voltdb::InvocationResponse InvokeProcedure(
      voltdb::Procedure *procedure) {
    assert(initialized_);

    std::unique_ptr<voltdb::Client> client;
    if (!clients_.Pop(&client)) {
      client.reset(new voltdb::Client(CreateClient()));
    }
    voltdb::InvocationResponse response(client->invoke(*procedure));
    clients_.Push(std::move(client));
    return std::move(response);
  }

 private:
  struct ServerDescription {
    inline ServerDescription(const std::string &hostname_arg,
                             const std::uint16_t port_arg)
        : hostname(hostname_arg),
          port(port_arg) {
    }

    const std::string hostname;
    const std::uint16_t port;
  };

  voltdb::Client CreateClient() const;

  voltdb::ClientConfig config_;
  std::vector<ServerDescription> servers_;
  bool initialized_;

  LockFreeStack<std::unique_ptr<voltdb::Client>> clients_;

  // Disallow copy and assign:
  VoltDBConnectionPool(const VoltDBConnectionPool &orig) = delete;
  VoltDBConnectionPool& operator=(const VoltDBConnectionPool &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_VOLTDB_CONNECTION_POOL_H_
