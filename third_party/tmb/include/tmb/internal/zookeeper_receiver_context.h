// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_ZOOKEEPER_RECEIVER_CONTEXT_H_
#define TMB_INTERNAL_ZOOKEEPER_RECEIVER_CONTEXT_H_

#include <condition_variable>  // NOLINT(build/c++11)
#include <cstddef>
#include <limits>
#include <mutex>               // NOLINT(build/c++11)

#include "tmb/id_typedefs.h"
#include "tmb/priority.h"

#include "tmb/internal/lock_free_garbage_collector.h"

namespace tmb {

class ZookeeperMessageBus;

namespace internal {

class ContainerPusher;

/**
 * @brief Saved information from a call to ZookeeperMessageBus::ReceiveImpl().
 *        Used for sharing and synchronization with watch callbacks.
 **/
struct ZookeeperReceiverContext {
  ZookeeperReceiverContext(const client_id receiver_id_arg,
                           const Priority minimum_priority_arg,
                           const std::size_t max_messages_arg,
                           const bool delete_immediately_arg,
                           ContainerPusher *pusher_arg,
                           ZookeeperMessageBus *message_bus_arg)
      : message_bus(message_bus_arg),
        receiver_id(receiver_id_arg),
        minimum_priority(minimum_priority_arg),
        max_messages(max_messages_arg),
        delete_immediately(delete_immediately_arg),
        pusher(pusher_arg),
        call_complete(false),
        watch_complete(false),
        spurious_watch_complete(false),
        received_count(0) {
  }

  ZookeeperMessageBus *message_bus;

  // Receive call parameters.
  const client_id receiver_id;
  const Priority minimum_priority;
  const std::size_t max_messages;
  const bool delete_immediately;
  ContainerPusher *pusher;

  bool call_complete;
  bool watch_complete;
  bool spurious_watch_complete;

  std::size_t received_count;

  // Key for deleting from a LockFreeGarbageCollector.
  LockFreeGarbageCollector<ZookeeperReceiverContext>::DeletionToken
      gc_deletion_token;

  std::mutex access_mutex;
  std::condition_variable complete_cv;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_ZOOKEEPER_RECEIVER_CONTEXT_H_
