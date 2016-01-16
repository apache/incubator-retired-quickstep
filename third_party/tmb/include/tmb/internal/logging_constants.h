// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_LOGGING_CONSTANTS_H_
#define TMB_INTERNAL_LOGGING_CONSTANTS_H_

#include <cstddef>

namespace tmb {
namespace internal {

// The LogWriterPosix class will grow log files in increments of
// 'kLogSegmentSize' bytes instead of always appending to the log file with
// each record written. This has positive implications for performance: it
// means that log writes (even synchronous ones) generally do NOT need to
// update the file's metadata, and it can help avoid fragmentation from many
// small writes.
constexpr std::size_t kLogSegmentSize = 0x1000000;  // 16 MB

// This is the maximum allowed size of an individual log record. Attempting to
// write a record larger than this size with LogWriter::WriteRecord() or
// LogWriter::WriteMultipartRecord() will fail and return false. When reading
// a log with LogReader, any record that appears to be larger than this limit
// will be considered invalid, and the kOversizeRecord error code will be
// returned.
constexpr std::size_t kMaxLogRecordSize = 0x4000000;  // 64 MB

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_LOGGING_CONSTANTS_H_
