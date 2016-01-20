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
