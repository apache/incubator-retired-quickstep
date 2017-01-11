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

#ifndef QUICKSTEP_UTILITY_PTR_LIST_HPP_
#define QUICKSTEP_UTILITY_PTR_LIST_HPP_

#include <cstddef>
#include <iterator>
#include <list>

#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief A templated STL-style container class which holds a list of pointers
 *        to objects which it owns.
 **/
template <class T>
class PtrList {
 public:
  // TODO(chasseur): Attempt some template magic to get rid of the duplicated
  // code between PtrListIterator and PtrListConstIterator.
  class PtrListIterator : public std::iterator<std::forward_iterator_tag, T, int> {
   public:
    PtrListIterator() {
    }

    PtrListIterator(const PtrListIterator& other)
        : internal_iterator_(other.internal_iterator_) {
    }

    PtrListIterator& operator=(const PtrListIterator& other) {
      if (this != &other) {
        internal_iterator_ = other.internal_iterator_;
      }
      return *this;
    }

    inline PtrListIterator& operator++() {
      ++internal_iterator_;
      return *this;
    }

    PtrListIterator operator++(int) {
      PtrListIterator result(*this);
      ++(*this);
      return result;
    }

    inline PtrListIterator& operator--() {
      --internal_iterator_;
      return *this;
    }

    PtrListIterator operator--(int) {
      PtrListIterator result(*this);
      --(*this);
      return result;
    }

    inline bool operator==(const PtrListIterator& other) const {
      return internal_iterator_ == other.internal_iterator_;
    }

    inline bool operator!=(const PtrListIterator& other) const {
      return internal_iterator_ != other.internal_iterator_;
    }

    inline T& operator*() const {
      return **internal_iterator_;
    }

    inline T* operator->() const {
      return *internal_iterator_;
    }

   private:
    explicit PtrListIterator(typename std::list<T*>::iterator internal_iterator)
        : internal_iterator_(internal_iterator) {
    }

    typename std::list<T*>::iterator internal_iterator_;

    friend class PtrList;
    friend class PtrListConstIterator;
  };


  class PtrListConstIterator : public std::iterator<std::input_iterator_tag, const T, int> {
   public:
    PtrListConstIterator() {
    }

    PtrListConstIterator(const PtrListConstIterator& other)
        : internal_iterator_(other.internal_iterator_) {
    }

    PtrListConstIterator(const PtrListIterator& other)  // NOLINT(runtime/explicit) - allow implicit conversion
        : internal_iterator_(other.internal_iterator_) {
    }

    PtrListConstIterator& operator=(const PtrListConstIterator& other) {
      if (this != &other) {
        internal_iterator_ = other.internal_iterator_;
      }
      return *this;
    }

    PtrListConstIterator& operator=(const PtrListIterator& other) {
      internal_iterator_ = other.internal_iterator_;
      return *this;
    }

    inline PtrListConstIterator& operator++() {
      ++internal_iterator_;
      return *this;
    }

    PtrListConstIterator operator++(int) {
      PtrListConstIterator result(*this);
      ++(*this);
      return result;
    }

    inline PtrListConstIterator& operator--() {
      --internal_iterator_;
      return *this;
    }

    PtrListConstIterator operator--(int) {
      PtrListConstIterator result(*this);
      --(*this);
      return result;
    }

    inline bool operator==(const PtrListConstIterator& other) const {
      return internal_iterator_ == other.internal_iterator_;
    }

    inline bool operator!=(const PtrListConstIterator& other) const {
      return internal_iterator_ != other.internal_iterator_;
    }

    inline const T& operator*() const {
      return **internal_iterator_;
    }

    inline const T* operator->() const {
      return *internal_iterator_;
    }

   private:
    explicit PtrListConstIterator(typename std::list<T*>::const_iterator internal_iterator)
        : internal_iterator_(internal_iterator) {
    }

    typename std::list<T*>::const_iterator internal_iterator_;

    friend class PtrList;
  };

  typedef PtrListIterator iterator;
  typedef PtrListConstIterator const_iterator;

  PtrList() {
  }

  ~PtrList() {
    clear();
  }

  // Add an element
  inline void push_back(T* elt) {
    internal_list_.push_back(elt);
  }

  inline void push_front(T* elt) {
    internal_list_.push_front(elt);
  }

  // Get cardinality of list
  inline std::size_t size() const {
    return internal_list_.size();
  }

  // Test whether list is empty
  inline bool empty() const {
    return internal_list_.empty();
  }

  // Iterators
  iterator begin() {
    return PtrListIterator(internal_list_.begin());
  }

  iterator end() {
    return PtrListIterator(internal_list_.end());
  }

  T& front() {
    return *(internal_list_.front());
  }

  T& back() {
    return *(internal_list_.back());
  }

  const_iterator begin() const {
    return PtrListConstIterator(internal_list_.begin());
  }

  const_iterator end() const {
    return PtrListConstIterator(internal_list_.end());
  }

  const T& front() const {
    return *(internal_list_.front());
  }

  const T& back() const {
    return *(internal_list_.back());
  }

  void splice(iterator position, PtrList<T> &source) {  // NOLINT(runtime/references) - STL-style interface
    internal_list_.splice(position.internal_iterator_, source.internal_list_);
  }

  /**
   * @brief Clear contents but do not release memory. Don't call this unless
   *        you know what you're doing.
   **/
  void clearWithoutRelease() {
    internal_list_.clear();
  }

  /**
   * @brief Clear contents and release memory.
   */
  void clear() {
    for (typename std::list<T*>::iterator it = internal_list_.begin();
         it != internal_list_.end();
         ++it) {
      delete *it;
    }
    internal_list_.clear();
  }

 private:
  std::list<T*> internal_list_;

  DISALLOW_COPY_AND_ASSIGN(PtrList);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_PTR_LIST_HPP_
