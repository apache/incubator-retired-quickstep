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

#ifndef TMB_INTERNAL_LEVELDB_KEYS_H_
#define TMB_INTERNAL_LEVELDB_KEYS_H_

#include <chrono>  // NOLINT(build/c++11)
#include <cstddef>
#include <cstdint>
#include <limits>

#include "tmb/id_typedefs.h"
#include "tmb/priority.h"

namespace tmb {
namespace internal {

// All of the key structs in this file have a uint32_t 'key_type' at offset 0.
// This value can be checked to determine the type of a key in order to allow
// casting to the appropriate type to allow inspecting the other fields.

// Per-client records. Associated values are arrays of message_type_id, which
// are the types for which a client is registered as a receiver.
static constexpr std::uint32_t kLevelDBKeyTypeClient = 0;
struct LevelDBClientKey {
  const std::uint32_t key_type = kLevelDBKeyTypeClient;
  client_id client;
};
static_assert(offsetof(LevelDBClientKey, key_type) == 0,
              "LevelDBClientKey.key_type not at offset 0 as expected.");
static_assert(sizeof(LevelDBClientKey) ==
              sizeof(LevelDBClientKey::key_type)
                  + sizeof(LevelDBClientKey::client),
              "LevelDBClientKey is larger than the sum of its parts (i.e. the "
              "compiler has inserted gaps/padding between members because of "
              "alignment requirements). Explicit zero-initialized padding "
              "should be added.");


// Sender directory. Each client has an entry for each type for which it is
// registered as a sender. Mapped values are empty.
static constexpr std::uint32_t kLevelDBKeyTypeSenderDirectory = 1;
struct LevelDBSenderDirectoryKey {
  const std::uint32_t key_type = kLevelDBKeyTypeSenderDirectory;
  client_id client;
  message_type_id message_type;

  static LevelDBSenderDirectoryKey MinKeyForClient(const client_id client) {
    LevelDBSenderDirectoryKey key;
    key.client = client;
    key.message_type = std::numeric_limits<message_type_id>::min();
    return key;
  }

  static LevelDBSenderDirectoryKey LimitKeyForClient(const client_id client) {
    return MinKeyForClient(client + 1);
  }
};
static_assert(offsetof(LevelDBSenderDirectoryKey, key_type) == 0,
              "LevelDBSenderDirectoryKey.key_type not at offset 0 as "
              "expected.");
static_assert(sizeof(LevelDBSenderDirectoryKey) ==
              sizeof(LevelDBSenderDirectoryKey::key_type)
                  + sizeof(LevelDBSenderDirectoryKey::client)
                  + sizeof(LevelDBSenderDirectoryKey::message_type),
              "LevelDBSenderDirectoryKey is larger than the sum of its parts "
              "(i.e. the compiler has inserted gaps/padding between members "
              "because of alignment requirements). Explicit zero-initialized "
              "padding should be added.");


// Receiver directory.  Each client has an entry for each type for which it is
// registered as a receiver. Mapped values are empty. Note that this is ordered
// on 'message_type' BEFORE 'client_id', so that all of the receivers for a
// particular message type can be found quickly.
static constexpr std::uint32_t kLevelDBKeyTypeRecieverDirectory = 2;
struct LevelDBReceiverDirectoryKey {
  const std::uint32_t key_type = kLevelDBKeyTypeRecieverDirectory;
  message_type_id message_type;
  client_id client;

  static LevelDBReceiverDirectoryKey MinKeyForMessageType(
      const message_type_id message_type) {
    LevelDBReceiverDirectoryKey key;
    key.message_type = message_type;
    key.client = std::numeric_limits<client_id>::min();
    return key;
  }

  static LevelDBReceiverDirectoryKey LimitKeyForMessageType(
      const message_type_id message_type) {
    return MinKeyForMessageType(message_type + 1);
  }
};
static_assert(offsetof(LevelDBReceiverDirectoryKey, key_type) == 0,
              "LevelDBReceiverDirectoryKey.key_type not at offset 0 as "
              "expected.");
static_assert(sizeof(LevelDBReceiverDirectoryKey) ==
              sizeof(LevelDBReceiverDirectoryKey::key_type)
                  + sizeof(LevelDBReceiverDirectoryKey::message_type)
                  + sizeof(LevelDBReceiverDirectoryKey::client),
              "LevelDBReceiverDirectoryKey is larger than the sum of its "
              "parts (i.e. the compiler has inserted gaps/padding between "
              "members because of alignment requirements). Explicit "
              "zero-initialized padding should be added.");


// Messages. For now, this is merely a sequence of ids which map to empty
// values or, if a message is cancellable, to an array of
// LevelDBQueuedMessageKeys. Actual message contents are stored in the
// queued message range.
static constexpr std::uint32_t kLevelDBKeyTypeMessage = 3;
struct LevelDBMessageKey {
  const std::uint32_t key_type = kLevelDBKeyTypeMessage;
  // Alignment requirements on many 64-bit architectures will put a 4-byte
  // gap between 'key_type' and 'message_id'. We explicitly fill this gap with
  // zeroes so that LevelDB will not use uninitialized bytes when computing
  // CRC32 checksums or Bloom filters.
  const char _padding[sizeof(std::int64_t) - sizeof(std::uint32_t)] = {};
  std::int64_t message_id;
};
static_assert(offsetof(LevelDBMessageKey, key_type) == 0,
              "LevelDBMessageKey.key_type not at offset 0 as expected.");
static_assert(sizeof(LevelDBMessageKey) ==
              sizeof(LevelDBMessageKey::key_type)
                  + sizeof(LevelDBMessageKey::_padding)
                  + sizeof(LevelDBMessageKey::message_id),
              "LevelDBMessageKey is larger than the sum of its parts (i.e. "
              "the compiler has inserted gaps/padding between members because "
              "of alignment requirements). Explicit zero-initialized padding "
              "should be added.");


// Messages queued for individual receivers. Ordered by receiver's ID, then
// in descending order of priority, then by expiration time (messages with no
// expiration time come last), and finally by the message ID. The effect of
// this ordering is that the next message which should be received by a
// particular client is the first-ordered message with that client's ID that is
// not yet expired. Mapped values are messages concatenated with
// MessageMetadata (see tmb/internal/message_metadata.h).
static constexpr std::uint32_t kLevelDBKeyTypeQueuedMessage = 4;
struct LevelDBQueuedMessageKey {
  const std::uint32_t key_type = kLevelDBKeyTypeQueuedMessage;
  client_id receiver;
  Priority priority;
  // As with LevelDBMessageKey above, we add explicit zero-padding so that
  // LevelDB doesn't read uninitialized bytes.
  const char _padding[
      sizeof(std::chrono::time_point<std::chrono::high_resolution_clock>)
      - sizeof(Priority)] = {};
  std::chrono::time_point<std::chrono::high_resolution_clock> expiration_time;
  std::int64_t message_id;

  static LevelDBQueuedMessageKey MinKeyForClient(const client_id client) {
    LevelDBQueuedMessageKey key;
    key.receiver = client;
    key.priority = std::numeric_limits<Priority>::max();
    if (std::numeric_limits<
        std::chrono::time_point<std::chrono::high_resolution_clock>::rep>
            ::is_signed) {
      // If ticks are signed, this is the point furthest in the past. Most
      // sensible C++ standard library implementations are like this.
      key.expiration_time
          = std::chrono::time_point<std::chrono::high_resolution_clock>::min();
    } else if (std::numeric_limits<
        std::chrono::time_point<std::chrono::high_resolution_clock>::rep>
            ::is_integer) {
      // If ticks are unsigned integers, go one after the epoch (which
      // represents no expiration time and is ordered AFTER messages which can
      // expire).
      key.expiration_time
          = std::chrono::time_point<std::chrono::high_resolution_clock>(
              std::chrono::time_point<std::chrono::high_resolution_clock>
                  ::duration(1));
    } else {
      // Ticks are an unsigned floating-point type (WEIRD!). Use the smallest
      // nonzero number of ticks.
      key.expiration_time
          = std::chrono::time_point<std::chrono::high_resolution_clock>(
            std::chrono::time_point<std::chrono::high_resolution_clock>
                ::duration(
                    std::numeric_limits<std::chrono::time_point<
                    std::chrono::high_resolution_clock>::rep>::denorm_min()));
    }
    key.message_id = std::numeric_limits<std::int64_t>::min();
    return key;
  }

  static LevelDBQueuedMessageKey LimitKeyForClient(const client_id client) {
    return MinKeyForClient(client + 1);
  }
};
static_assert(offsetof(LevelDBQueuedMessageKey, key_type) == 0,
              "LevelDBQueuedMessageKey.key_type not at offset 0 as expected.");
static_assert(sizeof(LevelDBQueuedMessageKey) ==
              sizeof(LevelDBQueuedMessageKey::key_type)
                  + sizeof(LevelDBQueuedMessageKey::receiver)
                  + sizeof(LevelDBQueuedMessageKey::priority)
                  + sizeof(LevelDBQueuedMessageKey::_padding)
                  + sizeof(LevelDBQueuedMessageKey::expiration_time)
                  + sizeof(LevelDBQueuedMessageKey::message_id),
              "LevelDBQueuedMessageKey is larger than the sum of its parts "
              "(i.e. the compiler has inserted gaps/padding between members "
              "because of alignment requirements). Explicit zero-initialized "
              "padding should be added.");

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_LEVELDB_KEYS_H_
