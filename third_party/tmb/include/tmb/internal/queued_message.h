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

#ifndef TMB_INTERNAL_QUEUED_MESSAGE_H_
#define TMB_INTERNAL_QUEUED_MESSAGE_H_

#include <chrono>  // NOLINT(build/c++11)
#include <cstdint>
#include <utility>

#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/memory_mirror_cancellation_set.h"
#include "tmb/internal/shared_bool.h"

namespace tmb {
namespace internal {

/**
 * @brief A message queued for delivery. Augments an AnnotatedMessage with a
 *        priority and expiration time, and allows messages to be ranked
 *        according to priority and deadline.
 **/
class QueuedMessage {
 public:
  QueuedMessage(
      const client_id sender,
      const Priority priority,
      const std::chrono::time_point<std::chrono::high_resolution_clock>
          &send_time,
      const std::chrono::time_point<std::chrono::high_resolution_clock>
          &expiration_time,
      const SharedBool &cancel_flag,
      TaggedMessage &&message)  // NOLINT(whitespace/operators)
      : priority_(priority),
        expiration_time_(expiration_time) {
    annotated_message_.sender = sender;
    annotated_message_.send_time = send_time;
    annotated_message_.in_memory_cancel_flag = cancel_flag;
    annotated_message_.tagged_message = std::move(message);
  }

  QueuedMessage(
      const client_id sender,
      const Priority priority,
      const std::chrono::time_point<std::chrono::high_resolution_clock>
          &send_time,
      const std::chrono::time_point<std::chrono::high_resolution_clock>
          &expiration_time,
      const SharedBool &cancel_flag,
      const std::shared_ptr<MemoryMirrorCancellationSet> &cancellation_set,
      TaggedMessage &&message)  // NOLINT(whitespace/operators)
      : priority_(priority),
        expiration_time_(expiration_time) {
    annotated_message_.sender = sender;
    annotated_message_.send_time = send_time;
    annotated_message_.in_memory_cancel_flag = cancel_flag;
    annotated_message_.memory_mirror_cancellation_set = cancellation_set;
    annotated_message_.tagged_message = std::move(message);
  }

  QueuedMessage(const QueuedMessage &orig) = default;
  QueuedMessage(QueuedMessage &&orig) = default;  // NOLINT(build/c++11)

  QueuedMessage& operator=(const QueuedMessage &orig) = default;
  QueuedMessage& operator=(
      QueuedMessage &&orig) = default;  // NOLINT(build/c++11)

  QueuedMessage CopyWithMessageID(const std::int64_t message_id) const {
    QueuedMessage msg_copy(*this);
    msg_copy.annotated_message_.deletion_token.message_id = message_id;
    return msg_copy;
  }

  inline void SetMessageID(const std::int64_t message_id) {
    annotated_message_.deletion_token.message_id = message_id;
  }

  inline void SetMemoryMirrorCancellationSet(
      const std::shared_ptr<MemoryMirrorCancellationSet> &cancellation_set) {
    annotated_message_.memory_mirror_cancellation_set = cancellation_set;
  }

  bool operator<(const QueuedMessage &rhs) const {
    if (priority_ < rhs.priority_) {
      return true;
    } else if (priority_ > rhs.priority_) {
      return false;
    }

    // If priority is the same, order by deadline.
    if (expiration_time_.time_since_epoch().count() == 0) {
      if (rhs.expiration_time_.time_since_epoch().count() != 0) {
        // 'rhs' has a deadline, but this does not.
        return true;
      }
    } else {
      if (rhs.expiration_time_.time_since_epoch().count() == 0) {
        // This message has a deadline, but 'rhs' does not.
        return false;
      } else {
        // Both messages have a deadline, so the later deadline should be
        // ranked lower.
        if (expiration_time_ > rhs.expiration_time_) {
          return true;
        } else if (expiration_time_ < rhs.expiration_time_) {
          return false;
        }
      }
    }

    // Priority and deadline are the same, so earlier send time should come
    // first.
    if (annotated_message_.send_time > rhs.annotated_message_.send_time) {
      return true;
    } else if (annotated_message_.send_time
               < rhs.annotated_message_.send_time) {
      return false;
    }

    return GetMessageID() > rhs.GetMessageID();
  }

  inline bool operator>(const QueuedMessage &rhs) const {
    return !(*this < rhs);
  }

  inline bool Expired() const {
    if (expiration_time_.time_since_epoch().count() == 0) {
      return false;
    }
    if (std::chrono::high_resolution_clock::now() >= expiration_time_) {
      return true;
    } else {
      return false;
    }
  }

  inline bool Cancelled() const {
    return annotated_message_.in_memory_cancel_flag.Valid()
           && annotated_message_.in_memory_cancel_flag.Get();
  }

  inline bool ExpiredOrCancelled() const {
    return Expired() || Cancelled();
  }

  inline Priority GetPriority() const {
    return priority_;
  }

  inline const std::chrono::time_point<std::chrono::high_resolution_clock>&
      GetExpirationTime() const {
    return expiration_time_;
  }

  inline std::int64_t GetMessageID() const {
    return annotated_message_.deletion_token.message_id;
  }

  inline const AnnotatedMessage& GetAnnotatedMessage() const {
    return annotated_message_;
  }

  inline AnnotatedMessage ReleaseAnnotatedMessage() {
    return std::move(annotated_message_);
  }

  inline AnnotatedMessage CopyAnnotatedMessage() const {
    return annotated_message_;
  }

 private:
  AnnotatedMessage annotated_message_;
  Priority priority_;
  std::chrono::time_point<std::chrono::high_resolution_clock> expiration_time_;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_QUEUED_MESSAGE_H_
