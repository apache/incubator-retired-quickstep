// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_MESSAGE_METADATA_H_
#define TMB_INTERNAL_MESSAGE_METADATA_H_

#include <chrono>  // NOLINT(build/c++11)

#include "tmb/id_typedefs.h"

namespace tmb {
namespace internal {

// Metadata which is appended to the end of message contents stored in LevelDB
// and Zookeeper.
struct MessageMetadata {
  message_type_id message_type;
  client_id sender;
  std::chrono::time_point<std::chrono::high_resolution_clock> send_time;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_MESSAGE_METADATA_H_
