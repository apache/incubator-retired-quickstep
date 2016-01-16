// Copyright (c) 2015, Pivotal Software, Inc.
// All Rights Reserved.

#ifndef TMBBENCH_MESSAGES_H_
#define TMBBENCH_MESSAGES_H_

#include <cstddef>
#include <limits>

namespace tmbbench {

static constexpr std::size_t kPoisonMessage
    = std::numeric_limits<std::size_t>::max();

struct RunDescription {
  int num_senders;
  int num_receivers;
  std::size_t message_bytes;
  bool delete_immediately;
};

struct ThroughputResult {
  double send_throughput;
  double receive_throughput;
};

}  // namespace tmbbench

#endif  // TMBBENCH_MESSAGES_H_
