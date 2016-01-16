// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

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
