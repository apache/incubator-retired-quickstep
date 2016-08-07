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

#ifndef TMB_INTERNAL_NET_MEMORY_CONTAINER_PUSHER_H_
#define TMB_INTERNAL_NET_MEMORY_CONTAINER_PUSHER_H_

#include <cstdint>
#include <unordered_map>
#include <utility>

#include "tmb/cancellation_token.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/container_pusher.h"

namespace tmb {
namespace internal {

/**
 * @brief Adapter for another ContainerPusher that also records cancellation
 *        tokens, if any, in a ClientHandle's received token set.
 **/
class NetMemoryContainerPusher : public ContainerPusher {
 public:
  NetMemoryContainerPusher(
      ContainerPusher *wrapped_pusher,
      std::unordered_map<std::int64_t, CancellationToken>
          *receiver_cancel_tokens)
      : wrapped_pusher_(wrapped_pusher),
        receiver_cancel_tokens_(receiver_cancel_tokens) {
  }

  ~NetMemoryContainerPusher() override {}

  void Push(AnnotatedMessage &&message) override {  // NOLINT(build/c++11)
    if (message.in_memory_cancel_flag.Valid()) {
      CancellationToken token;
      token.in_memory_cancel_flag_ = message.in_memory_cancel_flag;
      token.memory_mirror_cancellation_set_
          = message.memory_mirror_cancellation_set;
      token.message_id_ = message.deletion_token.message_id;
      receiver_cancel_tokens_->emplace(message.deletion_token.message_id,
                                       std::move(token));
    }
    wrapped_pusher_->Push(std::move(message));
  }

 private:
  ContainerPusher *wrapped_pusher_;
  std::unordered_map<std::int64_t, CancellationToken> *receiver_cancel_tokens_;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_NET_MEMORY_CONTAINER_PUSHER_H_
