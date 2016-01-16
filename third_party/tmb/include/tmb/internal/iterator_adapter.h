// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

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
