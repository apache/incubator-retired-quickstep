// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_C_STRING_BUFFER_H_
#define TMB_INTERNAL_C_STRING_BUFFER_H_

#include <cstddef>
#include <cstdlib>

namespace tmb {
namespace internal {

/**
 * @brief Thin RAII wrapper for a dynamically-allocated C-string.
 **/
class CStringBuffer {
 public:
  inline explicit CStringBuffer(const std::size_t bytes) {
    rep_ = static_cast<char*>(malloc(bytes));
  }

  inline explicit CStringBuffer(char *str)
      : rep_(str) {
  }

  inline ~CStringBuffer() {
    free(rep_);
  }

  inline void Resize(const std::size_t bytes) {
    rep_ = static_cast<char*>(std::realloc(rep_, bytes));
  }

  inline char* Get() const {
    return rep_;
  }

  inline char* Release() {
    char *retval = rep_;
    rep_ = nullptr;
    return retval;
  }

 private:
  char *rep_;

  // Disallow copy and assign:
  CStringBuffer(const CStringBuffer &orig) = delete;
  CStringBuffer& operator=(const CStringBuffer &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_C_STRING_BUFFER_H_
