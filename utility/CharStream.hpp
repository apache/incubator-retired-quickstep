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

#ifndef QUICKSTEP_UTILITY_CHAR_STREAM_HPP_
#define QUICKSTEP_UTILITY_CHAR_STREAM_HPP_

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <type_traits>

#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

class CharStream {
 public:
  template <typename T>
  CharStream(const T &value,
             std::enable_if_t<std::is_pod<T>::value && sizeof(T) <= sizeof(std::uint64_t)> * = 0)
      : object_(nullptr),
        length_(sizeof(T)),
        delete_function_(nullptr) {
    std::memcpy(&value_union_.inline_data, &value, sizeof(T));
  }

  CharStream(const std::vector<char> *value)
      : object_(const_cast<std::vector<char>*>(value)),
        length_(value->size()),
        delete_function_(&DeleteObject<std::vector<char>>) {
    value_union_.out_of_line_data = value->data();
  }

  CharStream(const void *value,
             const std::size_t length,
             const bool take_ownership)
      : length_(length),
        delete_function_(std::free) {
    if (take_ownership) {
      object_ = const_cast<void*>(value);
    } else {
      object_ = std::malloc(length);
      std::memcpy(object_, value, length);
    }
    value_union_.out_of_line_data = object_;
  }

  CharStream(CharStream &&other)
      : object_(other.object_),
        length_(other.length_),
        value_union_(other.value_union_),
        delete_function_(other.delete_function_) {
    other.delete_function_ = nullptr;
  }

  ~CharStream() {
    if (delete_function_ != nullptr) {
      DCHECK(object_ != nullptr);
      delete_function_(object_);
    }
  }

  std::size_t length() const {
    return length_;
  }

  const void* getDataPtr() const {
    return delete_function_ == nullptr ? &value_union_.inline_data
                                       : value_union_.out_of_line_data;
  }

 private:
  union ValueUnion {
    std::uint64_t inline_data;
    const void *out_of_line_data;
  };

  typedef void (*DeleterFunction)(void*);

  template <typename T>
  static void DeleteObject(void *object) {
    delete static_cast<T*>(object);
  }

  void *object_;
  std::size_t length_;
  ValueUnion value_union_;
  DeleterFunction delete_function_;

  DISALLOW_COPY_AND_ASSIGN(CharStream);
};


/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_CHAR_STREAM_HPP_
