// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_ZOOKEEPER_FORMAT_H_
#define TMB_INTERNAL_ZOOKEEPER_FORMAT_H_

#include <chrono>  // NOLINT(build/c++11)
#include <cstddef>
#include <limits>

namespace tmb {
namespace internal {

constexpr static std::size_t kZookeeperTickDigits = std::numeric_limits<
    std::chrono::time_point<std::chrono::high_resolution_clock>
        ::duration::rep>::digits10 + 2;

constexpr static std::size_t kZookeeperQueuedMessageNameLength
    = 3                       // Priority
      + 1                     // "-"
      + kZookeeperTickDigits  // Expiration time
      + 1                     // "-"
      + 10                    // Message ID
      + 1;                    // Null-terminator

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_ZOOKEEPER_FORMAT_H_
