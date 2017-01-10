/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef TMB_INTERNAL_CACHE_INFO_H_
#define TMB_INTERNAL_CACHE_INFO_H_

#include <cstddef>

namespace tmb {
namespace internal {

// The assumed size (in bytes) of a cache line on this system. Some shared
// atomic data items will be padded so that they don't reside on the same cache
// line,  minimizing the performance impact of false sharing in cache
// coherency. 64 bytes is the standard on pretty much all x86 processors, and
// many other architectures as well. A CPU with a smaller cache line size
// should still avoid false sharing, but a small amount of space may be wasted.
// A CPU with a larger cache line size will not exhibit any incorrect behavior,
// but false sharing may still occur (causing increased cache coherency traffic
// and cache misses).
constexpr std::size_t kCacheLineBytes = 64;

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_CACHE_INFO_H_
