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
