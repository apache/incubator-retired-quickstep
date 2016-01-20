//   Copyright 2014-2015 Quickstep Technologies LLC.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#ifndef TMB_TAGGED_MESSAGE_H_
#define TMB_TAGGED_MESSAGE_H_

#include <cassert>
#include <chrono>  // NOLINT(build/c++11)
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "tmb/id_typedefs.h"

#include "tmb/internal/leveldb_keys.h"
#include "tmb/internal/memory_mirror_cancellation_set.h"
#include "tmb/internal/shared_bool.h"
#include "tmb/internal/zookeeper_format.h"

namespace tmb {

template <bool enable_deletion_support> class MemoryMirrorMessageBus;
template <bool enable_deletion_support> class PureMemoryMessageBus;

namespace internal {
class NativeTransactionLog;
class NetMemoryContainerPusher;
class NetServiceImpl;
class QueuedMessage;
template <bool memory_mirror_version> class TreeReceiverMessageQueue;
}  // namespace internal

/** \addtogroup TMB
 *  @{
 */

/**
 * @brief An array of bytes with a message_type_id tag to indicate how the
 *        bytes are to be interpreted.
 **/
class TaggedMessage {
 public:
  /**
   * @brief Default constructor which creates an empty, invalid message.
   **/
  TaggedMessage()
      : payload_inline_(true),
        message_type_(0) {
    payload_.in_line.size = 0;
  }

  /**
   * @brief Constructor.
   *
   * @param msg A pointer to the message contents in memory, which will be
   *        copied into this TaggedMessage.
   * @param bytes The size of the message at msg in bytes.
   * @param message_type The application-defined type ID of the message.
   **/
  TaggedMessage(const void *msg,
                const std::size_t bytes,
                const message_type_id message_type)
      : message_type_(message_type) {
    if (bytes <= sizeof(payload_.in_line.data)) {
      std::memcpy(payload_.in_line.data, msg, bytes);
      payload_.in_line.size = static_cast<unsigned char>(bytes);
      payload_inline_ = true;
    } else {
      payload_.out_of_line.data = std::malloc(bytes);
      std::memcpy(payload_.out_of_line.data, msg, bytes);
      payload_.out_of_line.size = bytes;
      payload_inline_ = false;
    }
  }

  /**
   * @brief Destructor.
   **/
  ~TaggedMessage() {
    if (!payload_inline_) {
      std::free(payload_.out_of_line.data);
    }
  }

  /**
   * @brief Copy constructor.
   **/
  TaggedMessage(const TaggedMessage &orig) {
    std::memcpy(this, &orig, sizeof(TaggedMessage));
    if (!payload_inline_) {
      payload_.out_of_line.data = std::malloc(payload_.out_of_line.size);
      std::memcpy(payload_.out_of_line.data,
                  orig.payload_.out_of_line.data,
                  payload_.out_of_line.size);
    }
  }

  /**
   * @brief Move constructor.
   **/
  TaggedMessage(TaggedMessage &&orig) {  // NOLINT(build/c++11)
    std::memcpy(this, &orig, sizeof(TaggedMessage));
    orig.payload_inline_ = true;
  }

  /**
   * @brief Copy-assignment operator.
   **/
  TaggedMessage& operator=(const TaggedMessage &orig) {
    if (this != &orig) {
      if (!payload_inline_) {
        std::free(payload_.out_of_line.data);
      }

      std::memcpy(this, &orig, sizeof(TaggedMessage));
      if (!payload_inline_) {
        payload_.out_of_line.data = std::malloc(payload_.out_of_line.size);
        std::memcpy(payload_.out_of_line.data,
                    orig.payload_.out_of_line.data,
                    payload_.out_of_line.size);
      }
    }

    return *this;
  }

  /**
   * @brief Move-assignment operator.
   **/
  TaggedMessage& operator=(TaggedMessage &&orig) {  // NOLINT(build/c++11)
    if (this != &orig) {
      if (!payload_inline_) {
        std::free(payload_.out_of_line.data);
      }

      std::memcpy(this, &orig, sizeof(TaggedMessage));
      orig.payload_inline_ = true;
    }

    return *this;
  }

  /**
   * @brief Overwrite this TaggedMessage with new contents.
   *
   * @param msg A pointer to the message contents in memory, which will be
   *        copied into this TaggedMessage.
   * @param bytes The size of the message at msg in bytes.
   * @param message_type The application-defined type ID of the message.
   **/
  void set_message(const void *msg,
                   const std::size_t bytes,
                   const message_type_id message_type) {
    if (!payload_inline_) {
      std::free(payload_.out_of_line.data);
    }

    if (bytes <= sizeof(payload_.in_line.data)) {
      std::memcpy(payload_.in_line.data, msg, bytes);
      payload_.in_line.size = static_cast<unsigned char>(bytes);
      payload_inline_ = true;
    } else {
      payload_.out_of_line.data = std::malloc(bytes);
      std::memcpy(payload_.out_of_line.data, msg, bytes);
      payload_.out_of_line.size = bytes;
      payload_inline_ = false;
    }

    message_type_ = message_type;
  }

  /**
   * @brief Overwrite this TaggedMessage with new contents, taking ownership
   *        of a memory block allocated elsewhere.
   * @warning msg must have been allocated with one of the C standard library
   *          functions malloc(), calloc(), or realloc() (NOT C++ new). It will
   *          eventually be deallocated with free(). See also set_message(),
   *          which is safer (but copies the message).
   *
   * @param msg A pointer to the message contents in memory, which this
   *        TaggedMessage takes ownership of.
   * @param bytes The size of the message at msg in bytes.
   * @param message_type The application-defined type ID of the message.
   **/
  void acquire_message(void *msg,
                       const std::size_t bytes,
                       const message_type_id message_type) {
    if (!payload_inline_) {
      std::free(payload_.out_of_line.data);
    }

    if (bytes <= sizeof(payload_.in_line.data)) {
      std::memcpy(payload_.in_line.data, msg, bytes);
      std::free(msg);
      payload_.in_line.size = static_cast<unsigned char>(bytes);
      payload_inline_ = true;
    } else {
      payload_.out_of_line.data = msg;
      payload_.out_of_line.size = bytes;
      payload_inline_ = false;
    }

    message_type_ = message_type;
  }

  /**
   * @brief Get this message's contents.
   *
   * @return A pointer to this message's content as raw bytes.
   **/
  inline const void* message() const {
    return payload_inline_ ? payload_.in_line.data : payload_.out_of_line.data;
  }

  /**
   * @brief Get the size of this message.
   *
   * @return The size of the message returned by message() in bytes.
   **/
  inline std::size_t message_bytes() const {
    return payload_inline_ ? payload_.in_line.size : payload_.out_of_line.size;
  }

  /**
   * @brief Get the type of this message.
   *
   * @return The application-defined type of this message.
   **/
  inline message_type_id message_type() const {
    return message_type_;
  }

 private:
  inline void ExtendOutOfLineAllocation(const std::size_t extra_bytes) {
    assert(!payload_inline_);
    payload_.out_of_line.data
        = std::realloc(payload_.out_of_line.data,
                       payload_.out_of_line.size + extra_bytes);
  }

  union Payload {
    struct OutOfLine {
      void *data;
      std::size_t size;
    } out_of_line;
    struct Inline {
      char data[sizeof(OutOfLine) - 1];
      unsigned char size;
    } in_line;
  } payload_;
  bool payload_inline_;

  message_type_id message_type_;

  friend class LevelDBMessageBus;
  friend class ZookeeperMessageBus;
};

/**
 * @brief A TaggedMessage with additional annotations to indicate who sent the
 *        message and when.
 **/
struct AnnotatedMessage {
  /**
   * @brief Default constructor which creates an empty, invalid
   *        AnnotatedMessage.
   **/
  AnnotatedMessage() {
    deletion_token.message_id = -1;
  }

  AnnotatedMessage(const AnnotatedMessage &orig) = default;
  AnnotatedMessage(AnnotatedMessage &&orig) = default;

  AnnotatedMessage& operator=(const AnnotatedMessage &orig) = default;
  AnnotatedMessage& operator=(
      AnnotatedMessage &&orig) = default;  // NOLINT(build/c++11)

  client_id sender;
  std::chrono::time_point<std::chrono::high_resolution_clock> send_time;
  TaggedMessage tagged_message;

 private:
  union DeletionToken {
    char leveldb_key[sizeof(internal::LevelDBQueuedMessageKey)];
    std::int64_t message_id;
    char zookeeper_nodename[internal::kZookeeperQueuedMessageNameLength];
  } deletion_token;

  mutable internal::SharedBool in_memory_cancel_flag;
  std::shared_ptr<internal::MemoryMirrorCancellationSet>
      memory_mirror_cancellation_set;

  friend class internal::NativeTransactionLog;
  friend class internal::NetMemoryContainerPusher;
  friend class internal::NetServiceImpl;
  friend class internal::QueuedMessage;
  friend class internal::TreeReceiverMessageQueue<false>;
  friend class internal::TreeReceiverMessageQueue<true>;
  friend class LevelDBMessageBus;
  friend class MemoryMirrorMessageBus<false>;
  friend class MemoryMirrorMessageBus<true>;
  friend class NativeNetClientMessageBus;
  friend class PureMemoryMessageBus<false>;
  friend class PureMemoryMessageBus<true>;
  friend class SQLiteMessageBus;
  friend class VoltDBMessageBus;
  friend class ZookeeperMessageBus;
};

/** @} */

}  // namespace tmb

#endif  // TMB_TAGGED_MESSAGE_H_
