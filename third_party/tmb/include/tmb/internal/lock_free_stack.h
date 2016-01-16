// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_LOCK_FREE_STACK_H_
#define TMB_INTERNAL_LOCK_FREE_STACK_H_

#include <atomic>
#include <cstddef>
#include <utility>

#include "tmb/internal/cache_info.h"

namespace tmb {
namespace internal {

/**
 * @brief A stack that allows lock-free pushing and popping of elements.
 *        Internally structured as a singly-linked list.
 **/
template <typename T>
class LockFreeStack {
 public:
  LockFreeStack()
      : top_(nullptr) {
  }

  ~LockFreeStack() {
    ListNode *current = top_.load(std::memory_order_relaxed);
    while (current != nullptr) {
      ListNode *next = current->next;
      delete current;
      current = next;
    }
  }

  inline T* Push(T &&item) {  // NOLINT(build/c++11)
    ListNode *node = new ListNode(std::move(item));
    while (!top_.compare_exchange_weak(node->next,
                                       node,
                                       std::memory_order_relaxed)) {
    }
    return &(node->item);
  }

  template <typename... ArgTypes>
  inline T* Emplace(ArgTypes&&... args) {  // NOLINT(build/c++11)
    ListNode *node = new ListNode(
        std::forward<ArgTypes...>(args...));
    while (!top_.compare_exchange_weak(node->next,
                                       node,
                                       std::memory_order_relaxed)) {
    }
    return &(node->item);
  }

  inline bool Pop(T* out) {
    ListNode *top_node = top_.load(std::memory_order_relaxed);
    for (;;) {
      if (top_node == nullptr) {
        return false;
      } else {
        ListNode *next = top_node->next;
        if (top_.compare_exchange_strong(top_node,
                                         next,
                                         std::memory_order_relaxed)) {
          *out = std::move(top_node->item);
          delete top_node;
          return true;
        }
      }
    }
  }

 private:
  struct ListNode {
    inline explicit ListNode(T &&item_arg)  // NOLINT(build/c++11)
        : item(std::move(item_arg)),
          next(nullptr) {
    }

    template <typename... ArgTypes>
    inline explicit ListNode(ArgTypes&&... args)  // NOLINT(build/c++11)
        : item(std::forward<ArgTypes...>(args...)),
          next(nullptr) {
    }

    T item;
    ListNode *next;
  };

  alignas(kCacheLineBytes) std::atomic<ListNode*> top_;

  // Disallow copy and assign.
  LockFreeStack(const LockFreeStack &orig) = delete;
  LockFreeStack& operator=(const LockFreeStack &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_LOCK_FREE_STACK_H_
