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

#ifndef QUICKSTEP_STORAGE_COUNTED_REFERENCE_HPP_
#define QUICKSTEP_STORAGE_COUNTED_REFERENCE_HPP_

#include <type_traits>

#include "storage/EvictionPolicy.hpp"
#include "storage/StorageBlockBase.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A managed pointer to a StorageBlockBase. StorageBlockBase::ref() and
 *        StorageBlockBase::unref(), as well as EvictionPolicy::blockReferenced()
 *        and EvictionPolicy::blockUnreferenced(), are called when
 *        appropriate. This class is a template to allow different type
 *        modifiers to be applied; however, a static assertion prevents it
 *        from being used for anything other than a StorageBlockBase.
 *
 **/
template<class T>
class CountedReference {
  static_assert(std::is_base_of<StorageBlockBase, typename std::remove_cv<T>::type>::value,
                "CountedReference is for use only with StorageBlockBase and its subclasses");

 public:
  /**
   * @brief Default constructor. Initializes the object without a StorageBlockBase
   *     to manage.
   **/
  CountedReference()
      : block_(nullptr),
        eviction_policy_(nullptr) {}

  /**
   * @brief Construct a new CountedReference to manage a reference to an
   *     underlying StorageBlockBase. Calls StorageBlockBase::ref() and
   *     EvictionPolicy::blockReferenced().
   *
   * @param block The StorageBlockBase to manage.
   * @param eviction_policy The EvictionPolicy inform when this
   *     CountedReference is constructor or destroyed.
   **/
  CountedReference(T *block, EvictionPolicy *eviction_policy)
      : block_(block), eviction_policy_(eviction_policy) {
//    eviction_policy_->blockReferenced(block_->getID());
#ifdef QUICKSTEP_DEBUG
    block_->ref();
#endif
  }

  /**
   * @brief Move constructor. Transfers management of a block reference away
   *     from another block. The other reference will not longer be valid.
   * @param other The other block.
   **/
  /* implicit */
  CountedReference(CountedReference<T> &&other)
      : block_(other.block_), eviction_policy_(other.eviction_policy_) {
    other.block_ = nullptr;
    other.eviction_policy_ = nullptr;
  }

  /**
   * @brief This is a move constructor that lets you move from a
   *        MutableBlockReference to a BlockReference. The weird template stuff is to
   *        ensure that this code is only generated when T = const StorageBlockBase or const
   *        StorageBlob. See
   *        http://stackoverflow.com/questions/17842478/select-class-constructor-using-
   *        enable-if.
   * @param other The other block.
   **/
  /* implicit */
  template<typename U = T>
  CountedReference(CountedReference<typename std::remove_cv<T>::type> &&other,
                   typename std::enable_if<std::is_same<U, const T>::value>::type* = nullptr)
      : block_(other.block_), eviction_policy_(other.eviction_policy_) {
    other.block_ = nullptr;
    other.eviction_policy_ = nullptr;
  }

  /**
   * @brief Unrefs the StorageBlockBase before destruction and called
   *     EvictionPolicy::blockUnreferenced().
   **/
  ~CountedReference() {
      if (block_ != nullptr) {
#ifdef QUICKSTEP_DEBUG
        block_->unref();
#endif
//        eviction_policy_->blockUnreferenced(block_->getID());
      }
  }

  /**
   * @brief Check whether this CountedReference is valid.
   *
   * @return true if this object is managing a StorageBlockBase reference false
   *     otherwise.
   **/
  inline bool valid() { return block_ != nullptr; }

  /**
   * @brief Provide pointer access to underlying StorageBlockBase.
   *
   * @return Pointer to managed StorageBlockBase.
   **/
  inline T *operator->() const { return block_; }

  /**
   * @brief Provide reference access to underlying StorageBlockBase.
   *
   * @return Reference to managed StorageBlockBase.
   **/
  inline T &operator*() const { return *block_; }

  /**
   * @brief Move assignment operator for CountedReference. Releases the
   *     managed StorageBlockBase from other and takes over its management.
   *     After calling this, other.valid() will return false.
   **/
  inline CountedReference &operator=(CountedReference &&other) {
    if (this == &other) {
      return *this;
    }
    this->~CountedReference();
    block_ = other.block_;
    eviction_policy_ = other.eviction_policy_;
    other.block_ = nullptr;
    other.eviction_policy_ = nullptr;
    return *this;
  }

  /**
   * @brief Releases management of the underlying StorageBlockBase.
   **/
  inline void release() {
    this->~CountedReference();
    block_ = nullptr;
    eviction_policy_ = nullptr;
  }

 private:
  friend CountedReference<const T>;  // Needed to move from mutable to non-mutable CountedReference objects.

  T *block_;
  EvictionPolicy *eviction_policy_;
  DISALLOW_COPY_AND_ASSIGN(CountedReference<T>);
};

}  // namespace quickstep

/** @} */

#endif  // QUICKSTEP_STORAGE_COUNTED_REFERENCE_HPP_
