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

#ifndef TMB_INTERNAL_MEMORY_MIRROR_DELETE_BATCH_H_
#define TMB_INTERNAL_MEMORY_MIRROR_DELETE_BATCH_H_

#include "tmb/id_typedefs.h"

namespace tmb {
namespace internal {

class QueuedMessage;

/**
 * @brief An object which tracks the information needed to efficiently delete
 *        a set of messages in the PersistentMessageBus underlying a
 *        MemoryMirrorMessageBus.
 **/
class MemoryMirrorDeleteBatch {
 public:
  explicit MemoryMirrorDeleteBatch(const client_id receiver_id)
      : receiver_id_(receiver_id) {
  }

  virtual ~MemoryMirrorDeleteBatch() {
  }

  virtual void AddMessage(const QueuedMessage &message) = 0;

 protected:
  const client_id receiver_id_;

 private:
  // Disallow copy and assign:
  MemoryMirrorDeleteBatch(const MemoryMirrorDeleteBatch &orig) = delete;
  MemoryMirrorDeleteBatch& operator=(const MemoryMirrorDeleteBatch &rhs)
      = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_MEMORY_MIRROR_DELETE_BATCH_H_
