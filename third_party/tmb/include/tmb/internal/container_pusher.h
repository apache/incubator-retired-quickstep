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
