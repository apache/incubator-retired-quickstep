// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_LOG_WRITER_BASE_H_
#define TMB_INTERNAL_LOG_WRITER_BASE_H_

namespace tmb {
namespace internal {

/**
 * @brief Common base class with virtual destructor for LogWriterPosix and
 *        LogWriterStdio.
 **/
class LogWriterBase {
 public:
  LogWriterBase() {
  }

  virtual ~LogWriterBase() {
  }

 private:
  // Disallow copy and assign:
  LogWriterBase(const LogWriterBase &orig) = delete;
  LogWriterBase& operator=(const LogWriterBase &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_LOG_WRITER_BASE_H_
