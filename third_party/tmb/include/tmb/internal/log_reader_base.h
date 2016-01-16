// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_LOG_READER_BASE_H_
#define TMB_INTERNAL_LOG_READER_BASE_H_

namespace tmb {
namespace internal {

/**
 * @brief Common base class with virtual destructor for LogReaderPosix and
 *        LogReaderStdio.
 **/
class LogReaderBase {
 public:
  LogReaderBase() {
  }

  virtual ~LogReaderBase() {
  }

 private:
  // Disallow copy and assign:
  LogReaderBase(const LogReaderBase &orig) = delete;
  LogReaderBase& operator=(const LogReaderBase &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_LOG_READER_BASE_H_
