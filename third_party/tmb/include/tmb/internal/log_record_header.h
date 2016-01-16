// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_LOG_RECORD_HEADER_H_
#define TMB_INTERNAL_LOG_RECORD_HEADER_H_

#include <cstddef>
#include <cstdint>

namespace tmb {
namespace internal {

// Header for all log records written by LogWriter and read by LogReader.
struct LogRecordHeader {
  std::size_t length;
  std::uint32_t crc32;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_LOG_RECORD_HEADER_H_
