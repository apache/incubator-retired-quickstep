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
      : length_(sizeof(T)),
        delete_function_(nullptr) {
    std::memcpy(&value_union_.inline_data, &value, sizeof(T));
  }

  CharStream(std::vector<char> &&value)
      : length_(value.size()),
        delete_function_(&DeleteObject<std::vector<char>>) {
    value_union_.out_of_line_data = new std::vector<char>(std::move(value));
  }

  CharStream(const void *value, const std::size_t length, const bool take_ownership)
      : length_(length),
        delete_function_(std::free) {
    if (take_ownership) {
      value_union_.out_of_line_data = value;
    } else {
      void *copy_of_value = std::malloc(length);
      std::memcpy(copy_of_value, value, length);
      value_union_.out_of_line_data = copy_of_value;
    }
  }

  ~CharStream() {
    if (delete_function_ != nullptr) {
      delete_function_(const_cast<void*>(value_union_.out_of_line_data));
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

  std::size_t length_;
  ValueUnion value_union_;
  DeleterFunction delete_function_;

  DISALLOW_COPY_AND_ASSIGN(CharStream);
};


/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_CHAR_STREAM_HPP_
