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

#ifndef TMB_INTERNAL_SHARED_BOOL_H_
#define TMB_INTERNAL_SHARED_BOOL_H_

#include <atomic>
#include <cassert>
#include <cstddef>

namespace tmb {
namespace internal {

/**
 * @brief A shared boolean flag that can be used by multiple threads and
 *        atomically updated and read. Underyling shared storage is reference
 *        counted and automatically garbage collected when no longer in use.
 **/
class SharedBool {
 public:
  inline SharedBool()
      : shared_atom_(nullptr) {
  }

  inline explicit SharedBool(const bool initial_value)
      : shared_atom_(new std::atomic<std::size_t>(initial_value ? 3 : 2)) {
  }

  inline SharedBool(const SharedBool &orig)
      : shared_atom_(orig.shared_atom_) {
    if (shared_atom_ != nullptr) {
      shared_atom_->fetch_add(2, std::memory_order_relaxed);
    }
  }

  inline SharedBool(SharedBool &&orig)  // NOLINT(build/c++11)
      : shared_atom_(orig.shared_atom_) {
    orig.shared_atom_ = nullptr;
  }

  inline ~SharedBool() {
    if ((shared_atom_ != nullptr)
        && (shared_atom_->fetch_sub(2, std::memory_order_relaxed) < 4)) {
      delete shared_atom_;
    }
  }

  inline SharedBool& operator=(const SharedBool &rhs) {
    if (this != &rhs) {
      if ((shared_atom_ != nullptr)
          && (shared_atom_->fetch_sub(2, std::memory_order_relaxed) < 4)) {
        delete shared_atom_;
      }

      shared_atom_ = rhs.shared_atom_;
      if (shared_atom_ != nullptr) {
        shared_atom_->fetch_add(2, std::memory_order_relaxed);
      }
    }

    return *this;
  }

  inline SharedBool& operator=(SharedBool &&rhs) {  // NOLINT(build/c++11)
    if (this != &rhs) {
      if ((shared_atom_ != nullptr)
          && (shared_atom_->fetch_sub(2, std::memory_order_relaxed) < 4)) {
        delete shared_atom_;
      }

      shared_atom_ = rhs.shared_atom_;
      rhs.shared_atom_ = nullptr;
    }

    return *this;
  }

  inline bool Valid() const {
    return shared_atom_ != nullptr;
  }

  inline bool Get() const {
    assert(Valid());
    return (shared_atom_->load(std::memory_order_relaxed) & 0x1);
  }

  // Returns value held before call.
  inline bool Set(const bool value) {
    assert(Valid());
    if (value) {
      return shared_atom_->fetch_or(0x1, std::memory_order_relaxed)
             & 0x1;
    } else {
      return shared_atom_->fetch_and(~static_cast<std::size_t>(0x1),
                                     std::memory_order_relaxed)
             & 0x1;
    }
  }

  inline bool OnlyCopy() const {
    assert(Valid());
    return shared_atom_->load(std::memory_order_relaxed) < 4;
  }

 private:
  std::atomic<std::size_t> *shared_atom_;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_SHARED_BOOL_H_
