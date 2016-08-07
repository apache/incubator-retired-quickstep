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

#ifndef TMB_INTERNAL_ITERATOR_ADAPTER_H_
#define TMB_INTERNAL_ITERATOR_ADAPTER_H_

#include <type_traits>
#include <utility>

namespace tmb {
namespace internal {

/**
 * @brief A lightweight adapter for late-binding of iterators.
 **/
template <typename T>
class IteratorAdapter {
 public:
  virtual ~IteratorAdapter() {}

  virtual IteratorAdapter<T>* Clone() const = 0;

  virtual bool Valid() const = 0;

  virtual void Next() = 0;

  virtual T& operator*() const = 0;

  virtual T* operator->() const = 0;
};

template <typename IteratorT>
class IteratorAdapterImpl : public IteratorAdapter<
    typename std::add_const<typename std::remove_reference<
        decltype(*std::declval<IteratorT>())>::type>::type> {
 public:
  typedef typename std::add_const<
              typename std::remove_reference<
                  decltype(*std::declval<IteratorT>())>::type>::type
          ValueT;

  IteratorAdapterImpl(const IteratorT &begin, const IteratorT &end)
      : current_(begin),
        end_(end) {
  }

  inline IteratorAdapter<ValueT>* Clone() const override {
    return new IteratorAdapterImpl<IteratorT>(current_, end_);
  }

  inline bool Valid() const override {
    return current_ != end_;
  }

  inline void Next() override {
    ++current_;
  }

  inline ValueT& operator*() const override {
    return *current_;
  }

  inline ValueT* operator->() const override {
    return &(*current_);
  }

 private:
  IteratorT current_;
  const IteratorT end_;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_ITERATOR_ADAPTER_H_
