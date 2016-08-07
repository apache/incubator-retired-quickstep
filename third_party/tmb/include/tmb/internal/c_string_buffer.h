/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

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
