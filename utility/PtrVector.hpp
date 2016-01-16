/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_UTILITY_PTR_VECTOR_HPP_
#define QUICKSTEP_UTILITY_PTR_VECTOR_HPP_

#include <iterator>
#include <vector>

#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief A vector of pointers to objects, which are automatically deleted when
 *        the PtrVector goes out of scope.
 **/
template <typename T, bool null_allowed = false>
class PtrVector {
 public:
  // TODO(chasseur): Attempt some template magic to get rid of the duplicated
  // code between iterator classes.

  /**
   * @brief Iterator over the contents of a PtrVector.
   * @warning If null_allowed is true, then always check that isNull() is false
   *          before attempting to dereference.
   **/
  class PtrVectorIterator : public std::iterator<std::random_access_iterator_tag, T> {
   public:
    typedef typename std::iterator<std::random_access_iterator_tag, T>::difference_type difference_type;

    PtrVectorIterator() {
    }

    PtrVectorIterator(const PtrVectorIterator& other)
        : internal_iterator_(other.internal_iterator_) {
    }

    PtrVectorIterator& operator=(const PtrVectorIterator& other) {
      if (this != &other) {
        internal_iterator_ = other.internal_iterator_;
      }
      return *this;
    }

    // Comparisons.
    inline bool operator==(const PtrVectorIterator& other) const {
      return internal_iterator_ == other.internal_iterator_;
    }

    inline bool operator!=(const PtrVectorIterator& other) const {
      return internal_iterator_ != other.internal_iterator_;
    }

    inline bool operator<(const PtrVectorIterator& other) const {
      return internal_iterator_ < other.internal_iterator_;
    }

    inline bool operator<=(const PtrVectorIterator& other) const {
      return internal_iterator_ <= other.internal_iterator_;
    }

    inline bool operator>(const PtrVectorIterator& other) const {
      return internal_iterator_ > other.internal_iterator_;
    }

    inline bool operator>=(const PtrVectorIterator& other) const {
      return internal_iterator_ >= other.internal_iterator_;
    }

    // Increment/decrement.
    inline PtrVectorIterator& operator++() {
      ++internal_iterator_;
      return *this;
    }

    PtrVectorIterator operator++(int) {
      PtrVectorIterator result(*this);
      ++(*this);
      return result;
    }

    inline PtrVectorIterator& operator--() {
      --internal_iterator_;
      return *this;
    }

    PtrVectorIterator operator--(int) {
      PtrVectorIterator result(*this);
      --(*this);
      return result;
    }

    // Compound assignment.
    inline PtrVectorIterator& operator+=(difference_type n) {
      internal_iterator_ += n;
      return *this;
    }

    inline PtrVectorIterator& operator-=(difference_type n) {
      internal_iterator_ -= n;
      return *this;
    }

    // Note: + operator with difference_type on the left is defined out-of-line.
    PtrVectorIterator operator+(difference_type n) const {
      return PtrVectorIterator(internal_iterator_ + n);
    }

    PtrVectorIterator operator-(difference_type n) const {
      return PtrVectorIterator(internal_iterator_ - n);
    }

    difference_type operator-(const PtrVectorIterator &other) const {
      return PtrVectorIterator(internal_iterator_ - other.internal_iterator_);
    }

    // Dereference.
    /**
     * @brief Check whether the element at the current iterator position is
     *        NULL.
     * @return Whether the element at the current iterator position is NULL.
     **/
    inline bool isNull() const {
      return (null_allowed && (*internal_iterator_ == nullptr));
    }

    /**
     * @brief Delete the element at the current iterator position and replace
     *        it with a new value.
     *
     * @param value_ptr The new value to place at the current position. The
     *        PtrVector takes ownership of the pointed-to value.
     **/
    void replaceValue(T *value_ptr) {
      if (!null_allowed) {
        DCHECK(value_ptr != nullptr);
      }
      if (!null_allowed || (*internal_iterator_ != nullptr)) {
        delete *internal_iterator_;
      }
      *internal_iterator_ = value_ptr;
    }

    /**
     * @brief Replace the element at the current iterator position, and
     *        release ownership and return the original value.
     *
     * @param value_ptr The new value to place at the current position. The
     *        PtrVector takes ownership of the pointed-to value.
     * @return A pointer to the original value at the current position. The
     *         PtrVector no longer owns the pointed-to object, which should now
     *         be managed by the caller.
     **/
    T* releaseAndReplaceElement(T *value_ptr) {
      if (!null_allowed) {
        DCHECK(value_ptr != nullptr);
      }
      T *original_value = *internal_iterator_;
      *internal_iterator_ = value_ptr;
      return original_value;
    }

    inline T& operator*() const {
      if (null_allowed) {
        DCHECK(!isNull());
      }
      return **internal_iterator_;
    }

    inline T* operator->() const {
      if (null_allowed) {
        DCHECK(!isNull());
      }
      return *internal_iterator_;
    }

    // Offset dereference. Potentially unsafe if null_allowed.
    T& operator[](difference_type n) const {
      if (null_allowed) {
        DCHECK(internal_iterator_[n] != nullptr);
      }
      return *(internal_iterator_[n]);
    }

   private:
    explicit PtrVectorIterator(const typename std::vector<T*>::iterator &internal_iterator)
        : internal_iterator_(internal_iterator) {
    }

    typename std::vector<T*>::iterator internal_iterator_;

    friend class PtrVector;
    friend class PtrVectorConstIterator;
  };

  /**
   * @brief Const iterator over the contents of a PtrVector.
   * @warning If null_allowed is true, then always check that isNull() is false
   *          before attempting to dereference.
   **/
  class PtrVectorConstIterator : public std::iterator<std::input_iterator_tag, const T> {
   public:
    typedef typename std::iterator<std::input_iterator_tag, T>::difference_type difference_type;

    PtrVectorConstIterator() {
    }

    PtrVectorConstIterator(const PtrVectorConstIterator& other)
        : internal_iterator_(other.internal_iterator_) {
    }

    PtrVectorConstIterator(const PtrVectorIterator& other)  // NOLINT(runtime/explicit) - allow implicit conversion
        : internal_iterator_(other.internal_iterator_) {
    }

    PtrVectorConstIterator& operator=(const PtrVectorConstIterator& other) {
      if (this != &other) {
        internal_iterator_ = other.internal_iterator_;
      }
      return *this;
    }

    PtrVectorConstIterator& operator=(const PtrVectorIterator& other) {
      internal_iterator_ = other.internal_iterator_;
      return *this;
    }

    // Comparisons.
    inline bool operator==(const PtrVectorConstIterator& other) const {
      return internal_iterator_ == other.internal_iterator_;
    }

    inline bool operator!=(const PtrVectorConstIterator& other) const {
      return internal_iterator_ != other.internal_iterator_;
    }

    inline bool operator<(const PtrVectorConstIterator& other) const {
      return internal_iterator_ < other.internal_iterator_;
    }

    inline bool operator<=(const PtrVectorConstIterator& other) const {
      return internal_iterator_ <= other.internal_iterator_;
    }

    inline bool operator>(const PtrVectorConstIterator& other) const {
      return internal_iterator_ > other.internal_iterator_;
    }

    inline bool operator>=(const PtrVectorConstIterator& other) const {
      return internal_iterator_ >= other.internal_iterator_;
    }

    // Increment/decrement.
    inline PtrVectorConstIterator& operator++() {
      ++internal_iterator_;
      return *this;
    }

    PtrVectorConstIterator operator++(int) {
      PtrVectorConstIterator result(*this);
      ++(*this);
      return result;
    }

    inline PtrVectorConstIterator& operator--() {
      --internal_iterator_;
      return *this;
    }

    PtrVectorConstIterator operator--(int) {
      PtrVectorConstIterator result(*this);
      --(*this);
      return result;
    }

    // Compound assignment.
    inline PtrVectorConstIterator& operator+=(difference_type n) {
      internal_iterator_ += n;
      return *this;
    }

    inline PtrVectorConstIterator& operator-=(difference_type n) {
      internal_iterator_ -= n;
      return *this;
    }

    // Note: + operator with difference_type on the left is defined out-of-line.
    PtrVectorConstIterator operator+(difference_type n) const {
      return PtrVectorConstIterator(internal_iterator_ + n);
    }

    PtrVectorConstIterator operator-(difference_type n) const {
      return PtrVectorConstIterator(internal_iterator_ - n);
    }

    difference_type operator-(const PtrVectorConstIterator &other) const {
      return PtrVectorConstIterator(internal_iterator_ - other.internal_iterator_);
    }

    // Dereference.
    /**
     * @brief Check whether the element at the current iterator position is
     *        NULL.
     * @return Whether the element at the current iterator position is NULL.
     **/
    inline bool isNull() const {
      return (null_allowed && (*internal_iterator_ == nullptr));
    }

    inline const T& operator*() const {
      if (null_allowed) {
        DCHECK(!isNull());
      }
      return **internal_iterator_;
    }

    inline const T* operator->() const {
      if (null_allowed) {
        DCHECK(!isNull());
      }
      return *internal_iterator_;
    }

    // Offset dereference. Potentially unsafe if null_allowed.
    const T& operator[](difference_type n) const {
      if (null_allowed) {
        DCHECK(internal_iterator_[n] != nullptr);
      }
      return *(internal_iterator_[n]);
    }

   private:
    explicit PtrVectorConstIterator(const typename std::vector<T*>::const_iterator &internal_iterator)
        : internal_iterator_(internal_iterator) {
    }

    typename std::vector<T*>::const_iterator internal_iterator_;

    friend class PtrVector;
  };

  /**
   * @brief Input iterator over the contents of a PtrVector which automatically
   *        skips over NULL entries.
   **/
  class PtrVectorConstSkipIterator : public std::iterator<std::input_iterator_tag, const T> {
   public:
    typedef typename std::iterator<std::input_iterator_tag, T>::difference_type difference_type;

    PtrVectorConstSkipIterator()
        : parent_vector_(nullptr) {
    }

    PtrVectorConstSkipIterator(const PtrVectorConstSkipIterator& other)
        : internal_iterator_(other.internal_iterator_),
          parent_vector_(other.parent_vector_) {
    }

    PtrVectorConstSkipIterator& operator=(const PtrVectorConstSkipIterator& other) {
      if (this != &other) {
        internal_iterator_ = other.internal_iterator_;
        parent_vector_ = other.parent_vector_;
      }
      return *this;
    }

    // Comparisons.
    inline bool operator==(const PtrVectorConstSkipIterator& other) const {
      return internal_iterator_ == other.internal_iterator_;
    }

    inline bool operator!=(const PtrVectorConstSkipIterator& other) const {
      return internal_iterator_ != other.internal_iterator_;
    }

    inline bool operator<(const PtrVectorConstSkipIterator& other) const {
      return internal_iterator_ < other.internal_iterator_;
    }

    inline bool operator<=(const PtrVectorConstSkipIterator& other) const {
      return internal_iterator_ <= other.internal_iterator_;
    }

    inline bool operator>(const PtrVectorConstSkipIterator& other) const {
      return internal_iterator_ > other.internal_iterator_;
    }

    inline bool operator>=(const PtrVectorConstSkipIterator& other) const {
      return internal_iterator_ >= other.internal_iterator_;
    }

    // Increment/decrement.
    inline PtrVectorConstSkipIterator& operator++() {
      ++internal_iterator_;
      DCHECK(parent_vector_ != nullptr);
      while (internal_iterator_ != parent_vector_->end()) {
        if (*internal_iterator_ != nullptr) {
          break;
        }
        ++internal_iterator_;
      }
      return *this;
    }

    PtrVectorConstSkipIterator operator++(int) {
      PtrVectorConstSkipIterator result(*this);
      ++(*this);
      return result;
    }

    inline const T& operator*() const {
      return **internal_iterator_;
    }

    inline const T* operator->() const {
      return *internal_iterator_;
    }

   private:
    explicit PtrVectorConstSkipIterator(const typename std::vector<T*>::const_iterator &internal_iterator,
                                        const std::vector<T*> *parent_vector)
        : internal_iterator_(internal_iterator), parent_vector_(parent_vector) {
      DCHECK(parent_vector_ != nullptr);
      while ((internal_iterator_ != parent_vector_->end()) && (*internal_iterator_ == nullptr)) {
        ++internal_iterator_;
      }
    }

    typename std::vector<T*>::const_iterator internal_iterator_;
    const std::vector<T*> *parent_vector_;

    friend class PtrVector;
  };

  typedef typename std::vector<T*>::size_type size_type;
  typedef T value_type;
  typedef PtrVectorIterator iterator;
  typedef PtrVectorConstIterator const_iterator;
  typedef PtrVectorConstSkipIterator const_skip_iterator;

  PtrVector() {
  }

  ~PtrVector() {
    for (typename std::vector<T*>::iterator it = internal_vector_.begin();
         it != internal_vector_.end();
         ++it) {
      if (!null_allowed || (*it != nullptr)) {
        delete *it;
      }
    }
  }

  inline size_type size() const {
    return internal_vector_.size();
  }

  inline size_type max_size() const {
    return internal_vector_.max_size();
  }

  inline size_type capacity() const {
    return internal_vector_.capacity();
  }

  inline bool empty() const {
    return internal_vector_.empty();
  }

  /**
   * @brief Check whether this vector contains any actual objects. Unlike
   *        empty(), this returns true if the vector has some elements, but
   *        they are all NULL.
   *
   * @return Whether this PtrVector is empty of actual objects.
   **/
  bool emptyNullCheck() const {
    if (null_allowed) {
      for (typename std::vector<T*>::const_iterator it = internal_vector_.begin();
           it != internal_vector_.end();
           ++it) {
        if (*it != nullptr) {
          return false;
        }
      }
      return true;
    } else {
      return empty();
    }
  }

  inline void reserve(size_type n) {
    internal_vector_.reserve(n);
  }

  // Iterators
  iterator begin() {
    return iterator(internal_vector_.begin());
  }

  iterator end() {
    return iterator(internal_vector_.end());
  }

  const_iterator begin() const {
    return const_iterator(internal_vector_.begin());
  }

  const_iterator end() const {
    return const_iterator(internal_vector_.end());
  }

  /**
   * @brief Get an iterator at the beginning of this PtrVector which
   *        automatically skips over NULL entries.
   *
   * @return An iterator at the beginning of this PtrVector which automatically
   *         skips over NULL entries.
   **/
  const_skip_iterator begin_skip() const {
    return const_skip_iterator(internal_vector_.begin(), &internal_vector_);
  }

  /**
   * @brief Get an iterator at one past the end of this PtrVector which
   *        automatically skips over NULL entries.
   *
   * @return An iterator at one past the end of this PtrVector which
   *         automatically skips over NULL entries.
   **/
  const_skip_iterator end_skip() const {
    return const_skip_iterator(internal_vector_.end(), &internal_vector_);
  }

  /**
   * @brief Check whether the element at the specified position is NULL.
   *
   * @param n The position in this PtrVector to check.
   * @return Whether the element at position n is NULL.
   **/
  inline bool elementIsNull(const size_type n) const {
    if (null_allowed && (internal_vector_[n] == nullptr)) {
      return true;
    } else {
      return false;
    }
  }

  /**
   * @brief Check whether the element at the specified position is NULL.
   * @note This is similar to elementIsNull(), but uses std::vector::at(),
   *       which throws std::out_of_range exceptions.
   *
   * @param n The position in this PtrVector to check.
   * @return Whether the element at position n is NULL.
   **/
  inline bool elementIsNullAt(const size_type n) const {
    if (null_allowed && (internal_vector_.at(n) == nullptr)) {
      return true;
    } else {
      return false;
    }
  }

  inline T& front() {
    if (null_allowed) {
      typename std::vector<T*>::iterator it = internal_vector_.begin();
      while ((it != internal_vector_.end()) && (*it == nullptr)) {
        ++it;
      }
      return **it;
    } else {
      return *(internal_vector_.front());
    }
  }

  inline const T& front() const {
    if (null_allowed) {
      typename std::vector<T*>::const_iterator it = internal_vector_.begin();
      while ((it != internal_vector_.end()) && (*it == nullptr)) {
        ++it;
      }
      return **it;
    } else {
      return *(internal_vector_.front());
    }
  }

  inline T& back() {
    if (null_allowed) {
      typename std::vector<T*>::reverse_iterator it = internal_vector_.rbegin();
      while ((it != internal_vector_.rend()) && (*it == nullptr)) {
        ++it;
      }
      return **it;
    } else {
      return *(internal_vector_.back());
    }
  }

  inline const T& back() const {
    if (null_allowed) {
      typename std::vector<T*>::const_reverse_iterator it = internal_vector_.rbegin();
      while ((it != internal_vector_.rend()) && (*it == nullptr)) {
        ++it;
      }
      return **it;
    } else {
      return *(internal_vector_.back());
    }
  }

  inline T& operator[](const size_type n) {
    if (null_allowed) {
      DCHECK(!elementIsNull(n));
    }
    return *(internal_vector_[n]);
  }

  inline const T& operator[](const size_type n) const {
    if (null_allowed) {
      DCHECK(!elementIsNull(n));
    }
    return *(internal_vector_[n]);
  }

  inline T& at(const size_type n) {
    if (null_allowed) {
      DCHECK(!elementIsNullAt(n));
    }
    return *(internal_vector_.at(n));
  }

  inline const T& at(const size_type n) const {
    if (null_allowed) {
      DCHECK(!elementIsNullAt(n));
    }
    return *(internal_vector_.at(n));
  }

  inline void push_back(T *value_ptr) {
    if (!null_allowed) {
      DCHECK(value_ptr != nullptr);
    }
    internal_vector_.push_back(value_ptr);
  }

  /**
   * @brief Delete an element at the specified index and replace it with a new
   *        value.
   *
   * @param n The position of the element to replace.
   * @param value_ptr The new value to place at position n. This PtrVector
   *        takes ownership of the pointed-to value.
   **/
  void replaceElement(const size_type n, T *value_ptr) {
    if (!null_allowed) {
      DCHECK(value_ptr != nullptr);
    }
    if (!null_allowed || (internal_vector_[n] != nullptr)) {
      delete internal_vector_[n];
    }
    internal_vector_[n] = value_ptr;
  }

  /**
   * @brief Replace an element at the specified index with a new value, and
   *        release ownership and return the original value.
   *
   * @param n The position of the element to replace.
   * @param value_ptr The new value to place at position n. This PtrVector
   *        takes ownership of the pointed-to value.
   * @return A pointer to the original value at position n. This PtrVector no
   *         longer owns the pointed-to object, which should now be managed
   *         by the caller.
   **/
  T* releaseAndReplaceElement(const size_type n, T *value_ptr) {
    if (!null_allowed) {
      DCHECK(value_ptr != nullptr);
    }
    T *original_value = internal_vector_[n];
    internal_vector_[n] = value_ptr;
    return original_value;
  }

  /**
   * @brief Delete an element and set it to null. Only usable if null_allowed
   *        is true.
   *
   * @param n The position of the element to delete.
   **/
  void deleteElement(const size_type n) {
    DCHECK(null_allowed);
    if (internal_vector_[n] != nullptr) {
      delete internal_vector_[n];
      internal_vector_[n] = nullptr;
    }
  }

  /**
   * @brief Delete the last element and truncate the vector. Only usable if
   *        null_allowed is false.
   **/
  void removeBack() {
    DCHECK(!null_allowed);
    DCHECK(!internal_vector_.empty());
    delete internal_vector_.back();
    internal_vector_.resize(internal_vector_.size() - 1);
  }

  /**
   * @brief Get a const reference to the internal vector of pointers.
   *
   * @return A const reference to the internal vector of pointers.
   **/
  const std::vector<T*>& getInternalVector() const {
    return internal_vector_;
  }

  /**
   * @brief Get a mutable pointer to the internal vector of pointers.
   * @warning Only call this if you really know what you are doing.
   *
   * @return A mutable pointer to the internal vector of pointers.
   **/
  std::vector<T*>* getInternalVectorMutable() {
    return &internal_vector_;
  }

 private:
  std::vector<T*> internal_vector_;

  DISALLOW_COPY_AND_ASSIGN(PtrVector);
};

template <typename T>
typename PtrVector<T>::PtrVectorIterator operator+(
    const typename PtrVector<T>::PtrVectorIterator::difference_type n,
    const typename PtrVector<T>::PtrVectorIterator &it) {
  return it + n;
}

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_PTR_VECTOR_HPP_
