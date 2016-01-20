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

#include "tmb/internal/leveldb_key_comparator.h"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "leveldb/slice.h"

#include "tmb/id_typedefs.h"
#include "tmb/priority.h"

#include "tmb/internal/leveldb_keys.h"

namespace tmb {
namespace internal {

int LevelDBKeyComparator::Compare(const leveldb::Slice &a,
                                  const leveldb::Slice &b) const {
  assert(a.size() >= sizeof(std::uint32_t));
  assert(b.size() >= sizeof(std::uint32_t));

  // Compare prefix tag.
  if (*reinterpret_cast<const std::uint32_t*>(a.data())
      < *reinterpret_cast<const std::uint32_t*>(b.data())) {
    return -1;
  } else if (*reinterpret_cast<const std::uint32_t*>(a.data())
             > *reinterpret_cast<const std::uint32_t*>(b.data())) {
    return 1;
  }

  if (*reinterpret_cast<const std::uint32_t*>(a.data())
      == kLevelDBKeyTypeClient) {
    // Client entries.
    assert(a.size() == sizeof(LevelDBClientKey));
    assert(b.size() == sizeof(LevelDBClientKey));

    const LevelDBClientKey *key_a
        = reinterpret_cast<const LevelDBClientKey*>(a.data());
    const LevelDBClientKey *key_b
        = reinterpret_cast<const LevelDBClientKey*>(b.data());

    if (key_a->client < key_b->client) {
      return -1;
    } else if (key_a->client > key_b->client) {
      return 1;
    } else {
      return 0;
    }
  } else if (*reinterpret_cast<const std::uint32_t*>(a.data())
             == kLevelDBKeyTypeSenderDirectory) {
    // Sender directory entries.
    assert(a.size() == sizeof(LevelDBSenderDirectoryKey));
    assert(b.size() == sizeof(LevelDBSenderDirectoryKey));

    const LevelDBSenderDirectoryKey *key_a
        = reinterpret_cast<const LevelDBSenderDirectoryKey*>(a.data());
    const LevelDBSenderDirectoryKey *key_b
        = reinterpret_cast<const LevelDBSenderDirectoryKey*>(b.data());

    if (key_a->client < key_b->client) {
      return -1;
    } else if (key_a->client > key_b->client) {
      return 1;
    } else if (key_a->message_type < key_b->message_type) {
      return -1;
    } else if (key_a->message_type > key_b->message_type) {
      return 1;
    } else {
      return 0;
    }
  } else if (*reinterpret_cast<const std::uint32_t*>(a.data())
             == kLevelDBKeyTypeRecieverDirectory) {
    // Receiver directory entries.
    assert(a.size() == sizeof(LevelDBReceiverDirectoryKey));
    assert(b.size() == sizeof(LevelDBReceiverDirectoryKey));

    const LevelDBReceiverDirectoryKey *key_a
        = reinterpret_cast<const LevelDBReceiverDirectoryKey*>(a.data());
    const LevelDBReceiverDirectoryKey *key_b
        = reinterpret_cast<const LevelDBReceiverDirectoryKey*>(b.data());

    if (key_a->message_type < key_b->message_type) {
      return -1;
    } else if (key_a->message_type > key_b->message_type) {
      return 1;
    } else if (key_a->client < key_b->client) {
      return -1;
    } else if (key_a->client > key_b->client) {
      return 1;
    } else {
      return 0;
    }
  } else if (*reinterpret_cast<const std::uint32_t*>(a.data())
             == kLevelDBKeyTypeMessage) {
    // Message entries.
    assert(a.size() == sizeof(LevelDBMessageKey));
    assert(b.size() == sizeof(LevelDBMessageKey));

    const LevelDBMessageKey *key_a
        = reinterpret_cast<const LevelDBMessageKey*>(a.data());
    const LevelDBMessageKey *key_b
        = reinterpret_cast<const LevelDBMessageKey*>(b.data());

    if (key_a->message_id < key_b->message_id) {
      return -1;
    } else if (key_a->message_id > key_b->message_id) {
      return 1;
    } else {
      return 0;
    }
  } else if (*reinterpret_cast<const std::uint32_t*>(a.data())
             == kLevelDBKeyTypeQueuedMessage) {
    // Queued message entries.
    assert(a.size() == sizeof(LevelDBQueuedMessageKey));
    assert(b.size() == sizeof(LevelDBQueuedMessageKey));

    const LevelDBQueuedMessageKey *key_a
        = reinterpret_cast<const LevelDBQueuedMessageKey*>(a.data());
    const LevelDBQueuedMessageKey *key_b
        = reinterpret_cast<const LevelDBQueuedMessageKey*>(b.data());

    // First sort by receiver.
    if (key_a->receiver < key_b->receiver) {
      return -1;
    } else if (key_a->receiver > key_b->receiver) {
      return 1;
    }

    // Sort by DESCENDING priority.
    if (key_a->priority > key_b->priority) {
      return -1;
    } else if (key_a->priority < key_b->priority) {
      return 1;
    }

    // Sort by expiration time, with the epoch indicating no expiration.
    if (key_a->expiration_time.time_since_epoch().count() == 0) {
      if (key_b->expiration_time.time_since_epoch().count() != 0) {
        // 'b' has an expiration time and 'a' does not.
        return 1;
      }
    } else {
      if (key_b->expiration_time.time_since_epoch().count() == 0) {
        // 'a' has an expiration time and 'b' does not.
        return -1;
      } else {
        // Both 'a' and 'b' have expiration times.
        if (key_a->expiration_time < key_b->expiration_time) {
          return -1;
        } else if (key_a->expiration_time > key_b->expiration_time) {
          return 1;
        }
      }
    }

    // Sort by the message sequence number if all else equal.
    if (key_a->message_id < key_b->message_id) {
      return -1;
    } else if (key_a->message_id > key_b->message_id) {
      return 1;
    } else {
      return 0;
    }
  } else {
    // Key is somehow corrupted.
    std::fprintf(stderr, "FATAL ERROR: Corrupt key in LevelDB.\n");
    std::exit(1);
  }
}

}  // namespace internal
}  // namespace tmb
