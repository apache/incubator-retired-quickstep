// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_LOG_READ_STATUS_H_
#define TMB_INTERNAL_LOG_READ_STATUS_H_

namespace tmb {
namespace internal {

/**
 * @brief Codes indicating different possible results of a ReadNextRecord()
 *        call.
 **/
enum class LogReadStatus {
  kOK,
  kEndOfFile,
  kIoError,
  kTruncatedRecord,
  // NOTE(chasseur): kOversizeRecord usually means that garbage bytes in a
  // record header were interpreted as some huge length (> kMaxLogRecordSize).
  // Unless the log has been corrupted, it is NOT expected that any good
  // records will follow an oversize record.
  kOversizeRecord,
  kChecksumFailed
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_LOG_READ_STATUS_H_
