// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_CRC32_H_
#define TMB_INTERNAL_CRC32_H_

#include <cstddef>
#include <cstdint>

namespace tmb {
namespace internal {

// Initial value for incremental CRC32 checksum computation.
constexpr std::uint32_t kInitialCrc32 = 0xffffffffU;

// Incrementally update a running CRC32 checksum 'crc' by checksumming 'length'
// bytes at '*data'.
std::uint32_t UpdateCrc32(std::uint32_t crc,
                          const void *data,
                          const std::size_t length);

// Obtain the final CRC32 checksum from a running computation.
inline std::uint32_t FinalizeCrc32(std::uint32_t crc) {
  return ~crc;
}

// Compute CRC32 checksum for a single block of data.
inline std::uint32_t ComputeCrc32(const void *data, const std::size_t length) {
  return FinalizeCrc32(UpdateCrc32(kInitialCrc32, data, length));
}

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_CRC32_H_
