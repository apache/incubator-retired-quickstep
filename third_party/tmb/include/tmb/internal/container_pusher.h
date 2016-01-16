// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_CONTAINER_PUSHER_H_
#define TMB_INTERNAL_CONTAINER_PUSHER_H_

#include <utility>

#include "tmb/tagged_message.h"

namespace tmb {
namespace internal {

/**
 * @brief A lightweight adapter that allows "pushing" an AnnotatedMessage onto
 *        various containers.
 **/
class ContainerPusher {
 public:
  virtual ~ContainerPusher() {}

  virtual void Push(AnnotatedMessage &&message) = 0;  // NOLINT(build/c++11)
};

template <typename ContainerT>
class ContainerPusherImpl : public ContainerPusher {
 public:
  explicit ContainerPusherImpl(ContainerT *container)
      : container_(container) {
  }

  ~ContainerPusherImpl() override {}

  void Push(AnnotatedMessage &&message) override {  // NOLINT(build/c++11)
    container_->push_back(std::move(message));
  }

 private:
  ContainerT *container_;
};

class SingleElementPusher : public ContainerPusher {
 public:
  explicit SingleElementPusher(AnnotatedMessage *message_ptr)
      : message_ptr_(message_ptr) {
  }

  ~SingleElementPusher() override {}

  void Push(AnnotatedMessage &&message) override {  // NOLINT(build/c++11)
    *message_ptr_ = std::move(message);
  }

 private:
  AnnotatedMessage *message_ptr_;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_CONTAINER_PUSHER_H_
