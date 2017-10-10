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

#ifndef QUICKSTEP_TYPES_ARRAY_LIT_HPP_
#define QUICKSTEP_TYPES_ARRAY_LIT_HPP_

#include <cstddef>
#include <vector>

#include "types/Type.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

typedef void UntypedLiteral;

class ArrayLit {
 public:
  using const_iterator = std::vector<UntypedLiteral*>::const_iterator;

  ArrayLit(const Type &type)
      : type_(type) {
  }

  ArrayLit(const ArrayLit &other)
      : type_(other.type_) {
    for (const UntypedLiteral *value :other.elements_) {
      elements_.emplace_back(type_.cloneValue(value));
    }
  }

  ArrayLit(ArrayLit &&other)
      : type_(other.type_),
        elements_(std::move(other.elements_)) {
  }

  ~ArrayLit() {
    clear();
  }

  void clear() {
    for (UntypedLiteral *value : elements_) {
      type_.destroyValue(value);
    }
    elements_.clear();
  }

  bool empty() const {
    return elements_.empty();
  }

  std::size_t size() const {
    return elements_.size();
  }


  void push_back(UntypedLiteral *value) {
    elements_.emplace_back(value);
  }

  const UntypedLiteral* operator[](const std::size_t idx) const {
    DCHECK_LT(idx, elements_.size());
    return elements_[idx];
  }

  const_iterator begin() const {
    return elements_.begin();
  }

  const_iterator end() const {
    return elements_.end();
  }

  const UntypedLiteral* front() const {
    return elements_.front();
  }

  const UntypedLiteral* back() const {
    return elements_.back();
  }

 private:
  const Type &type_;
  std::vector<UntypedLiteral*> elements_;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_NULL_LIT_HPP_
