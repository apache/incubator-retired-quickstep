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
