// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "tmb/internal/voltdb_connection_pool.h"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "Client.h"        // NOLINT(build/include)

#include "tmb/internal/lock_free_stack.h"

namespace tmb {
namespace internal {

bool VoltDBConnectionPool::Initialize() {
  if (!initialized_) {
    if (servers_.empty()) {
      return false;
    }
    try {
      voltdb::Client client(CreateClient());
      clients_.Emplace(new voltdb::Client(client));
    } catch (...) {
      return false;
    }
    initialized_ = true;
  }
  return true;
}

voltdb::Client VoltDBConnectionPool::CreateClient() const {
  voltdb::Client client(voltdb::Client::create(config_));
  for (const ServerDescription &desc : servers_) {
    client.createConnection(desc.hostname, desc.port);
  }
  return std::move(client);
}

}  // namespace internal
}  // namespace tmb
