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
