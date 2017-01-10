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

// TODO(chasseur): Better error handling in place of asserts.

#include "tmb/zookeeper_message_bus.h"

#include <algorithm>
#include <cassert>
#include <chrono>              // NOLINT(build/c++11)
#include <condition_variable>  // NOLINT(build/c++11)
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <memory>
#include <mutex>               // NOLINT(build/c++11)
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "tmb/address.h"
#include "tmb/cancellation_token.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/c_string_buffer.h"
#include "tmb/internal/container_pusher.h"
#include "tmb/internal/iterator_adapter.h"
#include "tmb/internal/lock_free_garbage_collector.h"
#include "tmb/internal/memory_mirror_cancellation_set.h"
#include "tmb/internal/memory_mirror_delete_batch.h"
#include "tmb/internal/message_metadata.h"
#include "tmb/internal/persistent_bus_state_interface.h"
#include "tmb/internal/threadsafe_set.h"
#include "tmb/internal/zookeeper_format.h"
#include "tmb/internal/zookeeper_receiver_context.h"
#include "tmb/internal/zookeeper_transaction_batch.h"

#include "zookeeper/zookeeper.h"

namespace {

inline bool CompareCStrings(const char *a, const char *b) {
    return std::strcmp(a, b) < 0;
}

void AppendUintToString(const std::uint32_t value, std::string *str) {
  char buffer[11];
  int written = snprintf(buffer, sizeof(buffer), "%u", value);
  assert((written > 0)
         && (static_cast<std::size_t>(written) < sizeof(buffer)));

  str->append(buffer);
}

void AppendPaddedIntToString(const std::int32_t value, std::string *str) {
  char buffer[11];
  int written = snprintf(buffer, sizeof(buffer), "%010d", value);
  assert(written == 10);

  str->append(buffer);
}

void AppendPriorityToString(const tmb::Priority value, std::string *str) {
  char buffer[4];
  // Flip priorities around and zero-pad so that higher priority comes first in
  // lexicographical order.
  int written = snprintf(buffer, sizeof(buffer), "%03u",
                         std::numeric_limits<tmb::Priority>::max() - value);
  assert(written == 3);

  str->append(buffer);
}

void AppendExpirationTimeToString(
    const std::chrono::time_point<std::chrono::high_resolution_clock>
        &expiration_time,
    std::string *str) {
  if (expiration_time.time_since_epoch().count() == 0) {
    // An unset expiration time should come after any valid expiration time in
    // lexicographical order.
    str->append(tmb::internal::kZookeeperTickDigits, 'x');
  } else {
    // First construct a format string that will print all of the digits,
    // zero-padded.
    char format[9];
    int written = snprintf(format,
                           sizeof(format),
                           "%%0%zulld",
                           tmb::internal::kZookeeperTickDigits);
    assert((written > 0) && (written < 9));

    char buffer[tmb::internal::kZookeeperTickDigits + 1];
    written = snprintf(buffer, sizeof(buffer), format,
                       expiration_time.time_since_epoch().count());
    assert(written == tmb::internal::kZookeeperTickDigits);

    str->append(buffer);
  }
}

tmb::Priority ParsePriority(const char *queued_message_node_name) {
  int priority_buffer;
  int matched = sscanf(queued_message_node_name, "%d-", &priority_buffer);
  assert(matched == 1);

  return std::numeric_limits<tmb::Priority>::max() - priority_buffer;
}

std::chrono::time_point<std::chrono::high_resolution_clock>
    ParseExpirationTime(const char *queued_message_node_name) {
  long long int tick_buffer;  // NOLINT(runtime/int)
  int matched = sscanf(queued_message_node_name + 4, "%lld-", &tick_buffer);
  if (matched == 0) {
    return std::chrono::time_point<std::chrono::high_resolution_clock>();
  } else {
    return std::chrono::time_point<std::chrono::high_resolution_clock>(
        std::chrono::time_point<std::chrono::high_resolution_clock>::duration(
            tick_buffer));
  }
}

int ParseMessageID(const char *queued_message_node_name) {
  int message_id_buffer;
  int matched = sscanf(queued_message_node_name
                           + 5 + tmb::internal::kZookeeperTickDigits,
                       "%d",
                       &message_id_buffer);
  assert(matched == 1);
  return message_id_buffer;
}

class ZookeeperMemoryMirrorCancellationSet
    : public tmb::internal::MemoryMirrorCancellationSet {
 public:
  explicit ZookeeperMemoryMirrorCancellationSet(
      const std::string &message_path)
      : message_path_(message_path) {
  }

  ~ZookeeperMemoryMirrorCancellationSet() {
  }

  inline void AddQueuedMessagePath(const std::string &queued_message_path) {
    queued_message_paths_.emplace_back(queued_message_path);
  }

 private:
  const std::string message_path_;
  std::vector<std::string> queued_message_paths_;

  friend class tmb::ZookeeperMessageBus;
};

class ZookeeperMemoryMirrorDeleteBatch
    : public tmb::internal::MemoryMirrorDeleteBatch {
 public:
  explicit ZookeeperMemoryMirrorDeleteBatch(
      const tmb::client_id receiver_id,
      const std::string &message_queue_basepath)
      : tmb::internal::MemoryMirrorDeleteBatch(receiver_id),
        message_queue_basepath_(message_queue_basepath) {
    message_queue_basepath_.push_back('/');
    AppendUintToString(receiver_id, &message_queue_basepath_);
    message_queue_basepath_.push_back('/');
  }

  ~ZookeeperMemoryMirrorDeleteBatch() {
  }

  void AddMessage(const tmb::internal::QueuedMessage &message) override {
    std::string queued_message_path(message_queue_basepath_);
    AppendPriorityToString(message.GetPriority(), &queued_message_path);
    queued_message_path.push_back('-');
    AppendExpirationTimeToString(message.GetExpirationTime(),
                                 &queued_message_path);
    queued_message_path.push_back('-');
    AppendPaddedIntToString(message.GetMessageID(), &queued_message_path);
    queued_message_paths_.emplace_back(std::move(queued_message_path));
  }

 private:
  std::string message_queue_basepath_;
  std::vector<std::string> queued_message_paths_;

  friend class tmb::ZookeeperMessageBus;
};

class MessageTypeDirectoryDeleter {
 public:
  MessageTypeDirectoryDeleter(
      const std::string &message_type_set_basepath,
      tmb::internal::ZookeeperTransactionBatch *delete_batch)
      : message_type_set_basepath_(message_type_set_basepath),
        delete_batch_(delete_batch) {
  }

  inline void operator()(const tmb::message_type_id message_type) {
    std::string node_path(message_type_set_basepath_);
    node_path.push_back('/');
    AppendUintToString(message_type, &node_path);
    delete_batch_->Delete(std::move(node_path), -1);
  }

 private:
  const std::string &message_type_set_basepath_;
  tmb::internal::ZookeeperTransactionBatch *delete_batch_;
};

class ReceiverDirectoryDeleter {
 public:
  ReceiverDirectoryDeleter(
      const tmb::client_id receiver_id,
      const std::string &receiver_directory_basepath,
      tmb::internal::ZookeeperTransactionBatch *delete_batch)
      : receiver_id_(receiver_id),
        receiver_directory_basepath_(receiver_directory_basepath),
        delete_batch_(delete_batch) {
  }

  inline void operator()(const tmb::message_type_id message_type) {
    std::string node_path(receiver_directory_basepath_);
    node_path.push_back('/');
    AppendUintToString(message_type, &node_path);
    node_path.push_back('/');
    AppendUintToString(receiver_id_, &node_path);
    delete_batch_->Delete(std::move(node_path), -1);
  }

 private:
  const tmb::client_id receiver_id_;
  const std::string &receiver_directory_basepath_;
  tmb::internal::ZookeeperTransactionBatch *delete_batch_;
};

}  // namespace

namespace tmb {

const std::uint8_t ZookeeperMessageBus::kExtraSortShift;

ZookeeperMessageBus::ZookeeperMessageBus(const std::string &path_prefix)
    : path_prefix_(path_prefix),
      client_basepath_(path_prefix),
      receiver_directory_basepath_(path_prefix),
      message_basepath_(path_prefix),
      queue_basepath_(path_prefix),
      zookeeper_handle_(nullptr) {
  client_basepath_.append("/clients/client");
  receiver_directory_basepath_.append("/receiver_directory");
  message_basepath_.append("/messages/message");
  queue_basepath_.append("/message_queues");

  zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);
}

ZookeeperMessageBus::~ZookeeperMessageBus() {
  if (zookeeper_handle_ != nullptr) {
    int rc = zookeeper_close(zookeeper_handle_);
    assert(rc == ZOK);
  }
}

void ZookeeperMessageBus::AddServer(const std::string &hostname,
                                    const std::uint16_t port) {
  if (!host_string_.empty()) {
    host_string_.push_back(',');
  }
  host_string_.append(hostname);
  host_string_.push_back(':');
  AppendUintToString(port, &host_string_);
}

// TODO(chasseur): Fix incorrect return of true before network connection
// is actually completed.
bool ZookeeperMessageBus::Initialize() {
  zookeeper_handle_ = zookeeper_init(
      host_string_.c_str(),
      &ZookeeperMessageBus::HandleConnectionEvent,
      2000,
      0,
      this,
      0);
  return (zookeeper_handle_ != nullptr);
}

void ZookeeperMessageBus::ResetBus() {
  assert(zookeeper_handle_ != nullptr);

  internal::ZookeeperTransactionBatch init_batch;
  if (path_prefix_.empty()) {
    RecursivelyDeletePath("/clients", &init_batch);
    RecursivelyDeletePath(receiver_directory_basepath_, &init_batch);
    RecursivelyDeletePath("/messages", &init_batch);
    RecursivelyDeletePath("/message_queues", &init_batch);
  } else {
    RecursivelyDeletePath(path_prefix_, &init_batch);
    init_batch.CreateEmptyNode(path_prefix_, &ZOO_OPEN_ACL_UNSAFE, 0);
  }

  std::string node_path(path_prefix_);
  node_path.append("/clients");
  init_batch.CreateEmptyNode(std::move(node_path), &ZOO_OPEN_ACL_UNSAFE, 0);

  init_batch.CreateEmptyNode(receiver_directory_basepath_,
                             &ZOO_OPEN_ACL_UNSAFE,
                             0);

  node_path.assign(path_prefix_);
  node_path.append("/messages");
  init_batch.CreateEmptyNode(std::move(node_path), &ZOO_OPEN_ACL_UNSAFE, 0);

  init_batch.CreateEmptyNode(queue_basepath_, &ZOO_OPEN_ACL_UNSAFE, 0);

  int rc = init_batch.Commit(zookeeper_handle_);
  assert(rc == ZOK);
}

client_id ZookeeperMessageBus::Connect() {
  assert(zookeeper_handle_ != nullptr);

  // Create sequentially-numbered client node.
  internal::CStringBuffer client_name_buffer(client_basepath_.size() + 11);
  int rc = zoo_create(zookeeper_handle_,
                      client_basepath_.c_str(),
                      nullptr,
                      -1,
                      &ZOO_OPEN_ACL_UNSAFE,
                      ZOO_SEQUENCE,
                      client_name_buffer.Get(),
                      client_basepath_.size() + 11);
  assert(rc == ZOK);

  int client_id_buffer;
  int matched = std::sscanf(client_name_buffer.Get() + client_basepath_.size(),
                            "%d",
                            &client_id_buffer);
  assert(matched == 1);

  // Create children of client node.
  internal::ZookeeperTransactionBatch client_init_batch;

  std::string child_name(client_name_buffer.Get());
  child_name.append("/sendable_types");
  client_init_batch.CreateEmptyNode(std::move(child_name),
                                    &ZOO_OPEN_ACL_UNSAFE,
                                    0);

  child_name.assign(client_name_buffer.Get());
  child_name.append("/receivable_types");
  client_init_batch.CreateEmptyNode(std::move(child_name),
                                    &ZOO_OPEN_ACL_UNSAFE,
                                    0);

  // Create queue node.
  std::string queue_name(queue_basepath_);
  queue_name.push_back('/');
  AppendUintToString(client_id_buffer, &queue_name);
  client_init_batch.CreateEmptyNode(std::move(queue_name),
                                    &ZOO_OPEN_ACL_UNSAFE,
                                    0);

  rc = client_init_batch.Commit(zookeeper_handle_);
  assert(rc == ZOK);

  return client_id_buffer;
}

bool ZookeeperMessageBus::Disconnect(const client_id client) {
  assert(zookeeper_handle_ != nullptr);

  std::string client_path(client_basepath_);
  AppendPaddedIntToString(client, &client_path);

  internal::ZookeeperTransactionBatch disconnect_batch;
  RemoveReceiverDirectoryEntries(client, &disconnect_batch);
  RecursivelyDeletePath(client_path, &disconnect_batch);
  if (disconnect_batch.Empty()) {
    return false;
  } else {
    int rc = disconnect_batch.Commit(zookeeper_handle_);
    assert(rc == ZOK);

    // Now delete any pending undelivered messages.
    //
    // TODO(chasseur): It is possible for a sender to observe this client as
    // connected, then queue up a new message logically after the call to
    // zoo_get_children() below, in which case we can wind up with "leftover"
    // undeleted messages.
    std::string queue_path(queue_basepath_);
    queue_path.push_back('/');
    AppendUintToString(client, &queue_path);

    String_vector queue_children;
    queue_children.count = 0;
    queue_children.data = nullptr;

    rc = zoo_get_children(zookeeper_handle_,
                          queue_path.c_str(),
                          0,
                          &queue_children);
    assert(rc == ZOK);

    internal::ZookeeperTransactionBatch queue_deletion_batch;
    for (int child_num = 0; child_num < queue_children.count; ++child_num) {
      std::string message_path(queue_path);
      message_path.push_back('/');
      message_path.append(queue_children.data[child_num]);

      queue_deletion_batch.Delete(std::move(message_path), -1);
    }

    deallocate_String_vector(&queue_children);

    rc = queue_deletion_batch.Commit(zookeeper_handle_);
    while (rc == ZNONODE) {
      // It is possible for another client to sneak in and cancel a message
      // in the queue, in which case the transaction will fail with ZNONODE.
      // In that case, we prune 'queue_deletion_batch' and try again.
      queue_deletion_batch.RemoveNonexistentNodesFromDeleteBatch(
          zookeeper_handle_);
      rc = queue_deletion_batch.Commit(zookeeper_handle_);
    }
    assert(rc == ZOK);
    return true;
  }
}

bool ZookeeperMessageBus::RegisterClientAsSender(
    const client_id sender_id,
    const message_type_id message_type) {
  assert(zookeeper_handle_ != nullptr);

  std::string path(client_basepath_);
  AppendPaddedIntToString(sender_id, &path);
  path.append("/sendable_types/");
  AppendUintToString(message_type, &path);

  int rc = zoo_create(zookeeper_handle_,
                      path.c_str(),
                      nullptr,
                      -1,
                      &ZOO_OPEN_ACL_UNSAFE,
                      0,
                      nullptr,
                      0);
  if (rc == ZOK) {
    return true;
  } else if ((rc == ZNONODE) || (rc == ZNODEEXISTS)) {
    // Client not connected OR already registered as sender for 'message_type'.
    return false;
  } else {
    // Unhandled error.
    assert(rc == ZOK);
    return false;
  }
}

bool ZookeeperMessageBus::RegisterClientAsReceiver(
    const client_id receiver_id,
    const message_type_id message_type) {
  assert(zookeeper_handle_ != nullptr);

  // Create a node for 'message_type' under "/receiver_directory" if one does
  // not yet exist.
  std::string directory_path(receiver_directory_basepath_);
  directory_path.push_back('/');
  AppendUintToString(message_type, &directory_path);

  int rc = zoo_create(zookeeper_handle_,
                      directory_path.c_str(),
                      nullptr,
                      -1,
                      &ZOO_OPEN_ACL_UNSAFE,
                      0,
                      nullptr,
                      0);
  assert((rc == ZOK) || (rc == ZNODEEXISTS));

  internal::ZookeeperTransactionBatch register_batch;

  // Add to the client's set of receivable types.
  std::string path(client_basepath_);
  AppendPaddedIntToString(receiver_id, &path);
  path.append("/receivable_types/");
  AppendUintToString(message_type, &path);

  register_batch.CreateEmptyNode(std::move(path), &ZOO_OPEN_ACL_UNSAFE, 0);

  // Also add to the directory of receivers for the specified type.
  directory_path.push_back('/');
  AppendUintToString(receiver_id, &directory_path);
  register_batch.CreateEmptyNode(std::move(directory_path),
                                 &ZOO_OPEN_ACL_UNSAFE,
                                 0);

  rc = register_batch.Commit(zookeeper_handle_);
  if (rc == ZOK) {
    return true;
  } else if ((rc == ZNONODE) || (rc == ZNODEEXISTS)) {
    // Client not connected OR already registered as receiver for
    // 'message_type'.
    return false;
  } else {
    // Unhandled error.
    assert(rc == ZOK);
    return false;
  }
}

MessageBus::SendStatus ZookeeperMessageBus::Send(
    const client_id sender_id,
    const Address &destination_address,
    const MessageStyle &style,
    TaggedMessage &&message,  // NOLINT(whitespace/operators)
    const Priority priority,
    CancellationToken *cancellation_token) {
  assert(zookeeper_handle_ != nullptr);

  std::string client_path(client_basepath_);

  // Check if sender is connected.
  AppendPaddedIntToString(sender_id, &client_path);

  struct Stat stat_buffer;
  int rc = zoo_exists(zookeeper_handle_, client_path.c_str(), 0, &stat_buffer);
  if (rc == ZNONODE) {
    return SendStatus::kSenderNotConnected;
  }
  assert(rc == ZOK);

  // Check if sender is registered for the given message type.
  client_path.append("/sendable_types/");
  AppendUintToString(message.message_type(), &client_path);

  rc = zoo_exists(zookeeper_handle_, client_path.c_str(), 0, &stat_buffer);
  if (rc == ZNONODE) {
    return SendStatus::kSenderNotRegisteredForMessageType;
  }
  assert(rc == ZOK);

  // Store receiver IDs as strings rather than ints to avoid deserializing and
  // then serializing again.
  std::vector<std::string> receiver_ids;
  if (destination_address.send_to_all_) {
    std::string directory_path(receiver_directory_basepath_);
    directory_path.push_back('/');
    AppendUintToString(message.message_type(), &directory_path);

    String_vector children;
    children.count = 0;
    children.data = nullptr;

    rc = zoo_get_children(zookeeper_handle_,
                          directory_path.c_str(),
                          0,
                          &children);
    if (rc == ZOK) {
      for (int child_num = 0; child_num < children.count; ++child_num) {
        receiver_ids.emplace_back(children.data[child_num]);
      }
    } else if (rc == ZNONODE) {
      // No directory entries.
    } else {
      // Unhandled error.
      assert(rc == ZOK);
    }

    deallocate_String_vector(&children);
  } else {
    for (const client_id receiver : destination_address.explicit_recipients_) {
      client_path.assign(client_basepath_);
      AppendPaddedIntToString(receiver, &client_path);

      rc = zoo_exists(zookeeper_handle_, client_path.c_str(), 0, &stat_buffer);
      if (rc == ZOK) {
        // Check if receiver is registered for the given message type.
        client_path.append("/receivable_types/");
        AppendUintToString(message.message_type(), &client_path);

        rc = zoo_exists(zookeeper_handle_,
                        client_path.c_str(),
                        0,
                        &stat_buffer);
        if (rc == ZNONODE) {
          return SendStatus::kReceiverNotRegisteredForMessageType;
        } else {
          assert(rc == ZOK);
          std::string receiver_str;
          AppendUintToString(receiver, &receiver_str);
          receiver_ids.push_back(std::move(receiver_str));
        }
      } else if (rc == ZNONODE) {
        // This receiver is not connected.
      } else {
        // Unhandled error.
        assert(rc == ZOK);
      }
    }
  }

  if (receiver_ids.empty()) {
    return SendStatus::kNoReceivers;
  }

  // Create the message node.
  internal::CStringBuffer message_name_buffer(message_basepath_.size() + 11);
  rc = zoo_create(zookeeper_handle_,
                  message_basepath_.c_str(),
                  nullptr,
                  -1,
                  &ZOO_OPEN_ACL_UNSAFE,
                  ZOO_SEQUENCE,
                  message_name_buffer.Get(),
                  message_basepath_.size() + 11);
  assert(rc == ZOK);

  int message_id_buffer;
  int matched = std::sscanf(message_name_buffer.Get()
                                + message_basepath_.size(),
                            "%d",
                            &message_id_buffer);
  assert(matched == 1);

  // Enqueue the message.
  // Set up message body.
  // TODO(chasseur): Implement a size-based heuristic to determine whether
  // messages with multiple recipients should be stored multiple times in
  // in queues or once under "/messages".
  internal::MessageMetadata message_metadata;
  message_metadata.message_type = message.message_type();
  message_metadata.sender = sender_id;
  message_metadata.send_time = std::chrono::high_resolution_clock::now();

  const void *message_payload = nullptr;
  char message_payload_inline[sizeof(message.payload_.in_line.data)
                              + sizeof(message_metadata)];
  const std::size_t message_bytes = message.message_bytes();
  if (message.payload_inline_) {
    std::memcpy(message_payload_inline,
                message.message(),
                message_bytes);
    std::memcpy(message_payload_inline + message_bytes,
                &message_metadata,
                sizeof(message_metadata));
    message_payload = message_payload_inline;
  } else {
    message.ExtendOutOfLineAllocation(sizeof(message_metadata));
    std::memcpy(static_cast<char*>(message.payload_.out_of_line.data)
                    + message_bytes,
                &message_metadata,
                sizeof(message_metadata));
    message_payload = message.payload_.out_of_line.data;
  }

  std::string cancellation_list;

  std::string queued_node_name;
  AppendPriorityToString(priority, &queued_node_name);
  queued_node_name.push_back('-');
  AppendExpirationTimeToString(style.expiration_time_, &queued_node_name);
  queued_node_name.push_back('-');
  AppendPaddedIntToString(message_id_buffer, &queued_node_name);

  internal::ZookeeperTransactionBatch send_batch;
  for (const std::string &receiver_id : receiver_ids) {
    std::string queued_node_path(queue_basepath_);
    queued_node_path.push_back('/');
    queued_node_path.append(receiver_id);
    queued_node_path.push_back('/');
    queued_node_path.append(queued_node_name);

    if (cancellation_token != nullptr) {
      cancellation_list.append(queued_node_path);
      cancellation_list.push_back(';');
    }

    send_batch.CreateNodeWithExternalPayload(
        std::move(queued_node_path),
        message_payload,
        message_bytes + sizeof(message_metadata),
        &ZOO_OPEN_ACL_UNSAFE,
        0);
  }

  if (cancellation_token != nullptr) {
    send_batch.Set(message_name_buffer.Get(),
                   cancellation_list.c_str(),
                   cancellation_list.size() + 1,
                   -1);
    cancellation_token->message_id_ = message_id_buffer;
  }

  rc = send_batch.Commit(zookeeper_handle_);
  assert(rc == ZOK);

  return SendStatus::kOK;
}

void ZookeeperMessageBus::CancelMessage(
    const client_id sender_id,
    const CancellationToken &cancellation_token) {
  assert(zookeeper_handle_ != nullptr);

  std::string message_path(message_basepath_);
  AppendPaddedIntToString(cancellation_token.message_id_, &message_path);

  internal::ZookeeperTransactionBatch cancel_batch;
  CancelInternal(message_path.c_str(), &cancel_batch);

  int rc = cancel_batch.Commit(zookeeper_handle_);
  while (rc == ZNONODE) {
    // Interference from concurrent activity (either someone else is
    // cancelling this message, or one of the receivers deleted their
    // copy).
    cancel_batch.RemoveNonexistentNodesFromDeleteBatch(zookeeper_handle_);
    rc = cancel_batch.Commit(zookeeper_handle_);
  }
  assert(rc == ZOK);
}

std::size_t ZookeeperMessageBus::CountQueuedMessagesForClient(
    const client_id receiver_id) {
  assert(zookeeper_handle_ != nullptr);

  // Look up messages under the receiver's queue node.
  std::string queue_path(queue_basepath_);
  queue_path.push_back('/');
  AppendUintToString(receiver_id, &queue_path);

  String_vector children;
  children.count = 0;
  children.data = nullptr;

  int rc = zoo_get_children(zookeeper_handle_,
                            queue_path.c_str(),
                            0,
                            &children);
  assert(rc == ZOK);

  const std::size_t num_messages = children.count;

  deallocate_String_vector(&children);

  return num_messages;
}

void ZookeeperMessageBus::DisconnectClientUnchecked(
    const client_id client,
    const internal::ThreadsafeSet<message_type_id> &sendable_set,
    const internal::ThreadsafeSet<message_type_id> &receivable_set,
    const internal::MemoryMirrorDeleteBatch *queued_message_batch) {
  internal::ZookeeperTransactionBatch disconnect_batch;

  std::string client_node_path(client_basepath_);
  AppendPaddedIntToString(client, &client_node_path);

  // Remove set of sendable types.
  std::string sendable_set_path(client_node_path);
  sendable_set_path.append("/sendable_types");
  sendable_set.for_each(
      MessageTypeDirectoryDeleter(sendable_set_path, &disconnect_batch));
  disconnect_batch.Delete(std::move(sendable_set_path), -1);

  // Remove set of receivable types.
  std::string receivable_set_path(client_node_path);
  receivable_set_path.append("/receivable_types");
  receivable_set.for_each(
      MessageTypeDirectoryDeleter(receivable_set_path, &disconnect_batch));
  disconnect_batch.Delete(std::move(receivable_set_path), -1);

  // Remove entries from receiver directory.
  receivable_set.for_each(ReceiverDirectoryDeleter(
      client,
      receiver_directory_basepath_,
      &disconnect_batch));

  // Finally, remove client node itself.
  disconnect_batch.Delete(std::move(client_node_path), -1);

  int rc = disconnect_batch.Commit(zookeeper_handle_);
  assert(rc == ZOK);

  // Clean up queued messages in seperate transaction, which we may have to
  // retry because of cancellations.
  internal::ZookeeperTransactionBatch queued_message_deletion_batch;
  for (const std::string &queued_message_path
       : static_cast<const ZookeeperMemoryMirrorDeleteBatch*>(
           queued_message_batch)->queued_message_paths_) {
    queued_message_deletion_batch.DeleteWithExternalPath(queued_message_path,
                                                         -1);
  }
  rc = queued_message_deletion_batch.Commit(zookeeper_handle_);
  while (rc == ZNONODE) {
    queued_message_deletion_batch.RemoveNonexistentNodesFromDeleteBatch(
        zookeeper_handle_);
    rc = queued_message_deletion_batch.Commit(zookeeper_handle_);
  }
  assert(rc == ZOK);
}

void ZookeeperMessageBus::InsertMessageTypeUnchecked(
    const message_type_id message_type) {
  std::string directory_path(receiver_directory_basepath_);
  directory_path.push_back('/');
  AppendUintToString(message_type, &directory_path);

  int rc = zoo_create(zookeeper_handle_,
                      directory_path.c_str(),
                      nullptr,
                      -1,
                      &ZOO_OPEN_ACL_UNSAFE,
                      0,
                      nullptr,
                      0);
  assert((rc == ZOK) || (rc == ZNODEEXISTS));
}

void ZookeeperMessageBus::InsertReceivableUnchecked(
    const client_id client,
    const message_type_id message_type,
    const internal::ThreadsafeSet<message_type_id> &receivable_set) {
  internal::ZookeeperTransactionBatch register_batch;

  // Add to the client's set of receivable types.
  std::string path(client_basepath_);
  AppendPaddedIntToString(client, &path);
  path.append("/receivable_types/");
  AppendUintToString(message_type, &path);
  register_batch.CreateEmptyNode(std::move(path), &ZOO_OPEN_ACL_UNSAFE, 0);

  // Also add to the directory of receivers for the specified type.
  std::string directory_path(receiver_directory_basepath_);
  directory_path.push_back('/');
  AppendUintToString(message_type, &directory_path);
  directory_path.push_back('/');
  AppendUintToString(client, &directory_path);
  register_batch.CreateEmptyNode(std::move(directory_path),
                                 &ZOO_OPEN_ACL_UNSAFE,
                                 0);

  int rc = register_batch.Commit(zookeeper_handle_);
  assert(rc == ZOK);
}

std::int64_t ZookeeperMessageBus::SendUnchecked(
    const internal::QueuedMessage &message,
    const std::vector<client_id> &verified_receivers,
    CancellationToken *cancellation_token,
    std::uint32_t *sender_message_counter) {
  // Create the message node.
  internal::CStringBuffer message_name_buffer(message_basepath_.size() + 11);
  int rc = zoo_create(zookeeper_handle_,
                      message_basepath_.c_str(),
                      nullptr,
                      -1,
                      &ZOO_OPEN_ACL_UNSAFE,
                      ZOO_SEQUENCE,
                      message_name_buffer.Get(),
                      message_basepath_.size() + 11);
  assert(rc == ZOK);

  int message_id_buffer;
  int matched = std::sscanf(message_name_buffer.Get()
                                + message_basepath_.size(),
                            "%d",
                            &message_id_buffer);
  assert(matched == 1);

  // Enqueue the message.
  // Set up message body.
  internal::MessageMetadata message_metadata;
  message_metadata.message_type
      = message.GetAnnotatedMessage().tagged_message.message_type();
  message_metadata.sender = message.GetAnnotatedMessage().sender;
  message_metadata.send_time = message.GetAnnotatedMessage().send_time;

  std::size_t message_bytes
      = message.GetAnnotatedMessage().tagged_message.message_bytes();
  internal::CStringBuffer message_buffer(
      message_bytes + sizeof(message_metadata));
  std::memcpy(message_buffer.Get(),
              message.GetAnnotatedMessage().tagged_message.message(),
              message_bytes);
  std::memcpy(message_buffer.Get() + message_bytes,
              &message_metadata,
              sizeof(message_metadata));

  // Set up cancellation set if necessary.
  std::unique_ptr<ZookeeperMemoryMirrorCancellationSet> my_cancellation_set;
  std::string cancellation_list;
  if (cancellation_token != nullptr) {
    my_cancellation_set.reset(
        new ZookeeperMemoryMirrorCancellationSet(message_name_buffer.Get()));
  }

  std::string queued_node_name;
  AppendPriorityToString(message.GetPriority(), &queued_node_name);
  queued_node_name.push_back('-');
  AppendExpirationTimeToString(message.GetExpirationTime(), &queued_node_name);
  queued_node_name.push_back('-');
  AppendPaddedIntToString(message_id_buffer, &queued_node_name);

  internal::ZookeeperTransactionBatch send_batch;
  for (const client_id receiver_id : verified_receivers) {
    std::string queued_node_path(queue_basepath_);
    queued_node_path.push_back('/');
    AppendUintToString(receiver_id, &queued_node_path);
    queued_node_path.push_back('/');
    queued_node_path.append(queued_node_name);

    if (cancellation_token != nullptr) {
      my_cancellation_set->AddQueuedMessagePath(queued_node_path);
      cancellation_list.append(queued_node_path);
      cancellation_list.push_back(';');
    }

    send_batch.CreateNodeWithExternalPayload(
        std::move(queued_node_path),
        message_buffer.Get(),
        message_bytes + sizeof(message_metadata),
        &ZOO_OPEN_ACL_UNSAFE,
        0);
  }

  if (cancellation_token != nullptr) {
    send_batch.Set(message_name_buffer.Get(),
                   cancellation_list.c_str(),
                   cancellation_list.size() + 1,
                   -1);
    cancellation_token->memory_mirror_cancellation_set_.reset(
        my_cancellation_set.release());
  }

  rc = send_batch.Commit(zookeeper_handle_);
  assert(rc == ZOK);

  return message_id_buffer;
}

internal::MemoryMirrorDeleteBatch* ZookeeperMessageBus::CreateDeleteBatch(
    const client_id receiver_id) const {
  return new ZookeeperMemoryMirrorDeleteBatch(receiver_id, queue_basepath_);
}

void ZookeeperMessageBus::DeleteMessagesUnchecked(
    const internal::MemoryMirrorDeleteBatch &delete_batch) {
  internal::ZookeeperTransactionBatch delete_xact_batch;
  for (const std::string &node_path
       : static_cast<const ZookeeperMemoryMirrorDeleteBatch&>(delete_batch)
           .queued_message_paths_) {
    delete_xact_batch.DeleteWithExternalPath(node_path, -1);
  }

  int rc = delete_xact_batch.Commit(zookeeper_handle_);
  while (rc == ZNONODE) {
    delete_xact_batch.RemoveNonexistentNodesFromDeleteBatch(zookeeper_handle_);
    rc = delete_xact_batch.Commit(zookeeper_handle_);
  }
  assert(rc == ZOK);
}

void ZookeeperMessageBus::CancelMessageUnchecked(
    const CancellationToken &cancellation_token) {
  const ZookeeperMemoryMirrorCancellationSet &cancellation_set_cast
      = static_cast<const ZookeeperMemoryMirrorCancellationSet&>(
          *(cancellation_token.memory_mirror_cancellation_set_));

  internal::ZookeeperTransactionBatch cancel_batch;
  cancel_batch.DeleteWithExternalPath(cancellation_set_cast.message_path_, -1);
  for (const std::string &node_path
       : cancellation_set_cast.queued_message_paths_) {
    cancel_batch.DeleteWithExternalPath(node_path, -1);
  }

  int rc = cancel_batch.Commit(zookeeper_handle_);
  while (rc == ZNONODE) {
    cancel_batch.RemoveNonexistentNodesFromDeleteBatch(zookeeper_handle_);
    rc = cancel_batch.Commit(zookeeper_handle_);
  }
  assert(rc == ZOK);
}

void ZookeeperMessageBus::CancelMessagesUnchecked(
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  internal::ZookeeperTransactionBatch cancel_batch;

  while (adapter->Valid()) {
    if ((*adapter)->memory_mirror_cancellation_set) {
      const ZookeeperMemoryMirrorCancellationSet *cancellation_set_cast
          = static_cast<const ZookeeperMemoryMirrorCancellationSet*>(
              (*adapter)->memory_mirror_cancellation_set.get());
      cancel_batch.DeleteWithExternalPath(
          cancellation_set_cast->message_path_,
          -1);
      for (const std::string &node_path
           : cancellation_set_cast->queued_message_paths_) {
        cancel_batch.DeleteWithExternalPath(node_path, -1);
      }
    }
    adapter->Next();
  }

  int rc = cancel_batch.Commit(zookeeper_handle_);
  while (rc == ZNONODE) {
    cancel_batch.RemoveNonexistentNodesFromDeleteBatch(zookeeper_handle_);
    rc = cancel_batch.Commit(zookeeper_handle_);
  }
  assert(rc == ZOK);
}

std::unordered_map<client_id,
                   internal::PersistentBusStateInterface::TempClientHandle>*
    ZookeeperMessageBus::LoadState() {
  std::unique_ptr<std::unordered_map<client_id, TempClientHandle>> client_map(
      new std::unordered_map<client_id, TempClientHandle>());

  // Scan through all connected clients.
  std::string clients_node_path(path_prefix_);
  clients_node_path.append("/clients");

  String_vector clients_children;
  clients_children.count = 0;
  clients_children.data = nullptr;
  int rc = zoo_get_children(zookeeper_handle_,
                            clients_node_path.c_str(),
                            0,
                            &clients_children);

  if (rc == ZNONODE) {
    // Bus structure not yet initialized.
    return client_map.release();
  }
  assert(rc == ZOK);

  for (int child_num = 0; child_num < clients_children.count; ++child_num) {
    int client_id_buffer;
    int matched = std::sscanf(clients_children.data[child_num],
                              "client%d",
                              &client_id_buffer);
    assert(matched == 1);

    std::unordered_map<client_id, TempClientHandle>::iterator client_it
        = client_map->emplace(client_id_buffer, TempClientHandle()).first;
    assert(client_it != client_map->end());

    // Scan through sendable types for this client.
    std::string sendable_node_path(clients_node_path);
    sendable_node_path.push_back('/');
    sendable_node_path.append(clients_children.data[child_num]);
    sendable_node_path.append("/sendable_types");

    String_vector sendable_children;
    sendable_children.count = 0;
    sendable_children.data = nullptr;
    rc = zoo_get_children(zookeeper_handle_,
                          sendable_node_path.c_str(),
                          0,
                          &sendable_children);
    assert(rc == ZOK);
    for (int sendable_child_num = 0;
         sendable_child_num < sendable_children.count;
         ++sendable_child_num) {
      message_type_id message_type_buffer;
      int matched = std::sscanf(sendable_children.data[sendable_child_num],
                                "%u",
                                &message_type_buffer);
      assert(matched == 1);
      client_it->second.sendable->emplace(message_type_buffer);
    }
    deallocate_String_vector(&sendable_children);

    // Scan through receivable types for this client.
    std::string receivable_node_path(clients_node_path);
    receivable_node_path.push_back('/');
    receivable_node_path.append(clients_children.data[child_num]);
    receivable_node_path.append("/receivable_types");

    String_vector receivable_children;
    receivable_children.count = 0;
    receivable_children.data = nullptr;
    rc = zoo_get_children(zookeeper_handle_,
                          receivable_node_path.c_str(),
                          0,
                          &receivable_children);
    assert(rc == ZOK);
    for (int receivable_child_num = 0;
         receivable_child_num < receivable_children.count;
         ++receivable_child_num) {
      message_type_id message_type_buffer;
      int matched = std::sscanf(receivable_children.data[receivable_child_num],
                                "%u",
                                &message_type_buffer);
      assert(matched == 1);
      client_it->second.receivable->emplace(message_type_buffer);
    }
    deallocate_String_vector(&receivable_children);
  }
  deallocate_String_vector(&clients_children);

  // Scan through messages and find those which are cancellable.
  std::unordered_map<std::int64_t, internal::SharedBool> cancellation_flags;
  std::unordered_map<std::int64_t,
                     std::shared_ptr<internal::MemoryMirrorCancellationSet>>
      cancellation_sets;

  std::string messages_node_path(path_prefix_);
  messages_node_path.append("/messages");
  String_vector messages_children;
  messages_children.count = 0;
  messages_children.data = nullptr;
  rc = zoo_get_children(zookeeper_handle_,
                        messages_node_path.c_str(),
                        0,
                        &messages_children);
  for (int child_num = 0; child_num < messages_children.count; ++child_num) {
    std::string message_path(messages_node_path);
    message_path.push_back('/');
    message_path.append(messages_children.data[child_num]);

    struct Stat message_stat;
    rc = zoo_exists(zookeeper_handle_, message_path.c_str(), 0, &message_stat);
    assert(rc == ZOK);

    if (message_stat.dataLength != 0) {
      int message_id_buffer;
      int matched = std::sscanf(messages_children.data[child_num],
                                "message%d",
                                &message_id_buffer);
      assert(matched == 1);

      cancellation_flags.emplace(message_id_buffer,
                                 internal::SharedBool(false));
      // We do not actually fill in the cancellation set with queued message
      // paths until we scan them below.
      cancellation_sets.emplace(
          message_id_buffer,
          std::shared_ptr<internal::MemoryMirrorCancellationSet>(
              new ZookeeperMemoryMirrorCancellationSet(message_path)));
    }
  }
  deallocate_String_vector(&messages_children);

  // Now, scan through each connected client's queue and load messages there.
  for (std::pair<const client_id, TempClientHandle> &client_entry
       : *client_map) {
    std::string queue_node_path(queue_basepath_);
    queue_node_path.push_back('/');
    AppendUintToString(client_entry.first, &queue_node_path);

    String_vector queue_children;
    queue_children.count = 0;
    queue_children.data = nullptr;
    rc = zoo_get_children(zookeeper_handle_,
                          queue_node_path.c_str(),
                          0,
                          &queue_children);
    assert(rc == ZOK);
    for (int child_num = 0; child_num < queue_children.count; ++child_num) {
      // Parse metadata encoded in node name.
      Priority priority = ParsePriority(queue_children.data[child_num]);
      std::chrono::time_point<std::chrono::high_resolution_clock>
          expiration_time = ParseExpirationTime(
              queue_children.data[child_num]);
      std::int64_t message_id = ParseMessageID(queue_children.data[child_num]);

      // Load the actual message.
      std::string message_path(queue_node_path);
      message_path.push_back('/');
      message_path.append(queue_children.data[child_num]);

      struct Stat message_stat;
      rc = zoo_exists(zookeeper_handle_,
                      message_path.c_str(),
                      0,
                      &message_stat);
      assert(rc == ZOK);

      internal::CStringBuffer message_buffer(message_stat.dataLength);
      int message_buffer_length = message_stat.dataLength;
      rc = zoo_get(zookeeper_handle_,
                   message_path.c_str(),
                   0,
                   message_buffer.Get(),
                   &message_buffer_length,
                   nullptr);
      assert(rc == ZOK);
      assert(message_buffer_length == message_stat.dataLength);
      assert(static_cast<std::size_t>(message_buffer_length)
             >= sizeof(internal::MessageMetadata));

      // Copy the metadata and then trim the buffer to just the actual message
      // payload.
      internal::MessageMetadata message_metadata
          = *reinterpret_cast<const internal::MessageMetadata*>(
              message_buffer.Get() + message_buffer_length
                                   - sizeof(internal::MessageMetadata));
      message_buffer.Resize(message_buffer_length - sizeof(message_metadata));

      TaggedMessage msg;
      msg.acquire_message(message_buffer.Release(),
                          message_buffer_length - sizeof(message_metadata),
                          message_metadata.message_type);

      // Check if the message is cancellable.
      internal::SharedBool cancellation_flag;
      std::unordered_map<std::int64_t, internal::SharedBool>::const_iterator
          cancellation_flag_it = cancellation_flags.find(message_id);
      if (cancellation_flag_it != cancellation_flags.end()) {
        cancellation_flag = cancellation_flag_it->second;
      }

      std::shared_ptr<internal::MemoryMirrorCancellationSet> cancellation_set;
      std::unordered_map<std::int64_t,
                         std::shared_ptr<internal::MemoryMirrorCancellationSet>>
          ::const_iterator cancellation_set_it
              = cancellation_sets.find(message_id);
      if (cancellation_set_it != cancellation_sets.end()) {
        cancellation_set = cancellation_set_it->second;
      }

      // If message is cancellable, add this node to the cancellation set.
      if (cancellation_set) {
        static_cast<ZookeeperMemoryMirrorCancellationSet*>(
            cancellation_set.get())->AddQueuedMessagePath(message_path);
      }

      // Finally, build the QueuedMessage.
      internal::QueuedMessage queued_msg(message_metadata.sender,
                                         priority,
                                         message_metadata.send_time,
                                         expiration_time,
                                         cancellation_flag,
                                         cancellation_set,
                                         std::move(msg));
      queued_msg.SetMessageID(message_id);
      client_entry.second.queued_messages.emplace_back(std::move(queued_msg));
    }
    deallocate_String_vector(&queue_children);
  }

  return client_map.release();
}

std::size_t ZookeeperMessageBus::ReceiveImpl(
    const client_id receiver_id,
    const Priority minimum_priority,
    const std::size_t max_messages,
    const bool delete_immediately,
    internal::ContainerPusher *pusher) {
  assert(zookeeper_handle_ != nullptr);

  // Create a shared, lockable context for this call and the watcher callback
  // we will set on the Zookeeper server.
  internal::ZookeeperReceiverContext *context
      = new internal::ZookeeperReceiverContext(receiver_id,
                                               minimum_priority,
                                               max_messages,
                                               delete_immediately,
                                               pusher,
                                               this);

  // Look up messages under the receiver's queue node.
  std::string queue_path(queue_basepath_);
  queue_path.push_back('/');
  AppendUintToString(receiver_id, &queue_path);

  String_vector children;
  children.count = 0;
  children.data = nullptr;

  std::size_t retval = 0;
  bool all_complete = false;
  {
    // Lock '*context' and get children with watcher callback.
    std::unique_lock<std::mutex> lock(context->access_mutex);

    int rc = zoo_wget_children(zookeeper_handle_,
                               queue_path.c_str(),
                               &ZookeeperMessageBus::QueueWatchCallback,
                               context,
                               &children);
    assert(rc == ZOK);

    context->received_count = ReceiveInternal(receiver_id,
                                              minimum_priority,
                                              max_messages,
                                              delete_immediately,
                                              queue_path.c_str(),
                                              pusher,
                                              &children);

    // If we succeeded on retrieving messages on the first try, then the
    // "useful" watch is considered done.
    if (context->received_count != 0) {
      context->watch_complete = true;
      // The spurious watch is still outstanding, so put '*context' in the set
      // of contexts to be garbage collected when this ZookeeperMessageBus
      // instance is destroyed.
      context->gc_deletion_token = dead_watch_contexts_.Insert(receiver_id,
                                                               context);
    }

    // Wait for callback to signal CV.
    while (context->received_count == 0) {
      context->complete_cv.wait(lock);
    }

    context->call_complete = true;
    retval = context->received_count;
    if (context->spurious_watch_complete) {
      // By this point, the call is completing, and the "useful" watch is
      // complete. If the follow-on spurious watch is also complete, then there
      // is no longer any outstanding watch which requires '*context', and we
      // can delete it below.
      all_complete = true;
    }
  }

  if (all_complete) {
    dead_watch_contexts_.Delete(context->gc_deletion_token);
  }

  return retval;
}

std::size_t ZookeeperMessageBus::ReceiveIfAvailableImpl(
    const client_id receiver_id,
    const Priority minimum_priority,
    const std::size_t max_messages,
    const bool delete_immediately,
    internal::ContainerPusher *pusher) {
  assert(zookeeper_handle_ != nullptr);

  // Look up messages under the receiver's queue node.
  std::string queue_path(queue_basepath_);
  queue_path.push_back('/');
  AppendUintToString(receiver_id, &queue_path);

  String_vector children;
  children.count = 0;
  children.data = nullptr;

  int rc = zoo_get_children(zookeeper_handle_,
                            queue_path.c_str(),
                            0,
                            &children);
  assert(rc == ZOK);

  return ReceiveInternal(receiver_id,
                         minimum_priority,
                         max_messages,
                         delete_immediately,
                         queue_path.c_str(),
                         pusher,
                         &children);
}

void ZookeeperMessageBus::DeleteImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  assert(zookeeper_handle_ != nullptr);

  std::string queue_path(queue_basepath_);
  queue_path.push_back('/');
  AppendUintToString(receiver_id, &queue_path);
  queue_path.push_back('/');

  internal::ZookeeperTransactionBatch delete_batch;
  while (adapter->Valid()) {
    std::string queued_message_path(queue_path);
    queued_message_path.append((*adapter)->deletion_token.zookeeper_nodename);
    delete_batch.Delete(std::move(queued_message_path), -1);

    adapter->Next();
  }

  int rc = delete_batch.Commit(zookeeper_handle_);
  while (rc == ZNONODE) {
    delete_batch.RemoveNonexistentNodesFromDeleteBatch(zookeeper_handle_);
    rc = delete_batch.Commit(zookeeper_handle_);
  }
  assert(rc == ZOK);
}

void ZookeeperMessageBus::CancelImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  assert(zookeeper_handle_ != nullptr);

  internal::ZookeeperTransactionBatch cancel_batch;
  while (adapter->Valid()) {
    std::string message_path(message_basepath_);
    message_path.append((*adapter)->deletion_token.zookeeper_nodename
                            + 5 + internal::kZookeeperTickDigits);
    CancelInternal(message_path.c_str(), &cancel_batch);
    adapter->Next();
  }

  int rc = cancel_batch.Commit(zookeeper_handle_);
  while (rc == ZNONODE) {
    // Interference from concurrent activity (either someone else is
    // cancelling one of these messages, or one of the receivers deleted its
    // copy).
    cancel_batch.RemoveNonexistentNodesFromDeleteBatch(zookeeper_handle_);
    rc = cancel_batch.Commit(zookeeper_handle_);
  }
  assert(rc == ZOK);
}

void ZookeeperMessageBus::HandleConnectionEvent(zhandle_t *connection_handle,
                                                int event_type,
                                                int connection_state,
                                                const char *node_path,
                                                void *watcher_context) {
}

void ZookeeperMessageBus::QueueWatchCallback(_zhandle *connection_handle,
                                             int event_type,
                                             int connection_state,
                                             const char *node_path,
                                             void *watcher_context) {
  assert(event_type == ZOO_CHILD_EVENT);
  internal::ZookeeperReceiverContext *context
      = static_cast<internal::ZookeeperReceiverContext*>(watcher_context);
  context->message_bus->HandleQueueChange(node_path, context);
}

void ZookeeperMessageBus::RecursivelyDeletePath(
    const std::string &path,
    internal::ZookeeperTransactionBatch *batch) {
  assert(zookeeper_handle_ != nullptr);

  String_vector children;
  children.count = 0;
  children.data = nullptr;

  int rc = zoo_get_children(zookeeper_handle_, path.c_str(), 0, &children);
  if (rc == ZOK) {
    // Recursively delete each child, then delete the node.
    for (int child_num = 0; child_num < children.count; ++child_num) {
      std::string child_path(path);
      child_path.push_back('/');
      child_path.append(children.data[child_num]);
      RecursivelyDeletePath(child_path, batch);
    }
    batch->Delete(path, -1);
  } else if (rc == ZNONODE) {
    // Node doesn't exist, so do nothing.
  } else {
    // Unhandled error.
    assert(rc == ZOK);
  }

  deallocate_String_vector(&children);
}

void ZookeeperMessageBus::RemoveReceiverDirectoryEntries(
    const client_id client,
    internal::ZookeeperTransactionBatch *batch) {
  assert(zookeeper_handle_ != nullptr);

  std::string client_receivable_root(client_basepath_);
  AppendPaddedIntToString(client, &client_receivable_root);
  client_receivable_root.append("/receivable_types");

  String_vector children;
  children.count = 0;
  children.data = nullptr;

  int rc = zoo_get_children(zookeeper_handle_,
                            client_receivable_root.c_str(),
                            0,
                            &children);
  if (rc == ZOK) {
    for (int child_num = 0; child_num < children.count; ++child_num) {
      std::string node_path(receiver_directory_basepath_);
      node_path.push_back('/');
      node_path.append(children.data[child_num]);
      node_path.push_back('/');
      AppendUintToString(client, &node_path);

      batch->Delete(std::move(node_path), -1);
    }
  } else if (rc == ZNONODE) {
    // Node doesn't exist. Do nothing.
  } else {
    // Unhandled error.
    assert(rc == ZOK);
  }

  deallocate_String_vector(&children);
}

std::size_t ZookeeperMessageBus::ReceiveInternal(
    const client_id receiver_id,
    const Priority minimum_priority,
    const std::size_t max_messages,
    const bool delete_immediately,
    const char *queue_node_path,
    internal::ContainerPusher *pusher,
    String_vector *queue_node_children) {
  assert(zookeeper_handle_ != nullptr);

  int rc;
  std::chrono::time_point<std::chrono::high_resolution_clock> now
      = std::chrono::high_resolution_clock::now();

  // NOTE(chasseur): Unfortunately, we must do this ad-hoc sort, since the
  // various forms of zoo_get_children() list children in no particular order.
  //
  // We sort some "extra" messages beyond what is strictly required to account
  // for expired/cancelled messages.
  std::size_t messages_with_extra
      = max_messages + (max_messages >> kExtraSortShift);
  char **sorted_threshold;
  if ((max_messages == 0)
      || (messages_with_extra
          > static_cast<std::size_t>(queue_node_children->count))) {
    sorted_threshold = queue_node_children->data + queue_node_children->count;
    std::sort(queue_node_children->data,
              queue_node_children->data + queue_node_children->count,
              CompareCStrings);
  } else {
    sorted_threshold = queue_node_children->data + messages_with_extra;
    std::partial_sort(queue_node_children->data,
                      sorted_threshold,
                      queue_node_children->data + queue_node_children->count,
                      CompareCStrings);
  }

  std::size_t received = 0;
  // Transaction batch for any queued messages we may delete (either because
  // they were received and 'delete_immediately' is true, or because they are
  // expired).
  internal::ZookeeperTransactionBatch delete_batch;
  for (int child_num = 0;
       child_num < queue_node_children->count
           && ((max_messages == 0) || (received < max_messages));
       ++child_num) {
    if (queue_node_children->data + child_num == sorted_threshold) {
      // Sort 'max_messages' - 'received', plus some extra, more messages.
      std::size_t additional_required_messages = max_messages - received;
      char **next_threshold
          = sorted_threshold
            + additional_required_messages
            + (additional_required_messages >> kExtraSortShift);
      if (next_threshold
          >= queue_node_children->data + queue_node_children->count) {
        std::sort(sorted_threshold,
                  queue_node_children->data + queue_node_children->count,
                  CompareCStrings);
        sorted_threshold = queue_node_children->data
                           + queue_node_children->count;
      } else {
        std::partial_sort(sorted_threshold,
                          next_threshold,
                          queue_node_children->data
                              + queue_node_children->count,
                          CompareCStrings);
        sorted_threshold = next_threshold;
      }
    }

    assert(std::strlen(queue_node_children->data[child_num])
           == internal::kZookeeperQueuedMessageNameLength - 1);

    // Check if we have gone past all of the messages which satisfy
    // 'minimum_priority'.
    Priority priority = ParsePriority(queue_node_children->data[child_num]);
    if (priority < minimum_priority) {
      break;
    }

    // Construct the full path of the queued message.
    std::string message_path(queue_node_path);
    message_path.push_back('/');
    message_path.append(queue_node_children->data[child_num]);

    std::chrono::time_point<std::chrono::high_resolution_clock> expiration_time
        = ParseExpirationTime(queue_node_children->data[child_num]);
    if ((expiration_time.time_since_epoch().count() != 0)
        && (now > expiration_time)) {
      // If message is expired, remember to delete it and move on.
      delete_batch.Delete(std::move(message_path), -1);
      continue;
    }

    // Use exists call to look up Stat for queued message node.
    struct Stat message_stat;
    rc = zoo_exists(zookeeper_handle_, message_path.c_str(), 0, &message_stat);
    if (rc == ZNONODE) {
      // Message disappeared (i.e. was cancelled).
      continue;
    }
    assert(rc == ZOK);

    // Allocate buffer to hold message contents.
    internal::CStringBuffer message_buffer(message_stat.dataLength);
    int message_buffer_length = message_stat.dataLength;
    rc = zoo_get(zookeeper_handle_,
                 message_path.c_str(),
                 0,
                 message_buffer.Get(),
                 &message_buffer_length,
                 nullptr);
    if (rc == ZNONODE) {
      // Message disappeared (i.e. was cancelled).
      continue;
    }
    assert(rc == ZOK);
    assert(message_buffer_length == message_stat.dataLength);
    assert(static_cast<std::size_t>(message_buffer_length)
           >= sizeof(internal::MessageMetadata));

    // Copy out message.
    const internal::MessageMetadata *metadata_ptr
        = reinterpret_cast<const internal::MessageMetadata*>(
            message_buffer.Get() + message_buffer_length
                                 - sizeof(internal::MessageMetadata));

    AnnotatedMessage received_message;
    received_message.sender = metadata_ptr->sender;
    received_message.send_time = metadata_ptr->send_time;
    message_type_id message_type = metadata_ptr->message_type;
    // Trim metadata off the end of message_buffer, then transfer ownership to
    // the TaggedMessage.
    message_buffer.Resize(message_buffer_length
                          - sizeof(internal::MessageMetadata));
    received_message.tagged_message.acquire_message(
        message_buffer.Release(),
        message_buffer_length - sizeof(internal::MessageMetadata),
        message_type);
    std::memcpy(received_message.deletion_token.zookeeper_nodename,
                queue_node_children->data[child_num],
                internal::kZookeeperQueuedMessageNameLength);
    pusher->Push(std::move(received_message));

    ++received;

    if (delete_immediately) {
      delete_batch.Delete(std::move(message_path), -1);
    }
  }

  deallocate_String_vector(queue_node_children);

  // Wrap up by deleting appropriate messages.
  rc = delete_batch.Commit(zookeeper_handle_);
  while (rc == ZNONODE) {
    delete_batch.RemoveNonexistentNodesFromDeleteBatch(zookeeper_handle_);
    rc = delete_batch.Commit(zookeeper_handle_);
  }
  assert(rc == ZOK);

  return received;
}

void ZookeeperMessageBus::HandleQueueChange(
    const char *queue_node_path,
    internal::ZookeeperReceiverContext *context) {
  bool delete_context = false;
  bool signal = false;
  {
    std::lock_guard<std::mutex> lock(context->access_mutex);
    if (context->call_complete) {
      // The original call has returned, so this must be the follow-on spurious
      // watch, in which case we should delete '*context', as no one else will
      // need it.
      assert(context->watch_complete);
      delete_context = true;
    } else if (context->watch_complete) {
      // The "useful" watch finished, but the original call hasn't finished
      // yet. Mark that the spurious watch is finished, so that when the thread
      // blocking on receive wakes up, it can delete '*context'.
      context->spurious_watch_complete = true;
    } else {
      // Look up messages under the receiver's queue node.
      String_vector children;
      children.count = 0;
      children.data = nullptr;

      int rc = zoo_wget_children(zookeeper_handle_,
                                 queue_node_path,
                                 &ZookeeperMessageBus::QueueWatchCallback,
                                 context,
                                 &children);
      assert(rc == ZOK);

      context->received_count = ReceiveInternal(context->receiver_id,
                                                context->minimum_priority,
                                                context->max_messages,
                                                context->delete_immediately,
                                                queue_node_path,
                                                context->pusher,
                                                &children);

      if (context->received_count != 0) {
        // Successfully received at least one message, so the "useful" watch
        // is now complete and we should signal the CV.
        context->watch_complete = true;
        signal = true;

        // Put '*context' in the set of contexts to be garbage collected when
        // this instance of ZookeeperMessageBus is destroyed.
        context->gc_deletion_token
            = dead_watch_contexts_.Insert(context->receiver_id, context);
      }
    }
  }

  if (signal) {
    context->complete_cv.notify_all();
  } else if (delete_context) {
    dead_watch_contexts_.Delete(context->gc_deletion_token);
  }
}

void ZookeeperMessageBus::CancelInternal(
    const char *message_path,
    internal::ZookeeperTransactionBatch *batch) {
  assert(zookeeper_handle_ != nullptr);

  // Use exists call to look up Stat for message node.
  struct Stat message_stat;
  int rc = zoo_exists(zookeeper_handle_, message_path, 0, &message_stat);
  if (rc == ZNONODE) {
    // Message disappeared (i.e. was already cancelled).
    return;
  }
  assert(rc == ZOK);

  if (message_stat.dataLength == 0) {
    // Message was not created as cancellable.
    return;
  }

  // Allocate buffer to hold message data.
  internal::CStringBuffer message_buffer(message_stat.dataLength);
  int message_buffer_length = message_stat.dataLength;
  rc = zoo_get(zookeeper_handle_,
               message_path,
               0,
               message_buffer.Get(),
               &message_buffer_length,
               nullptr);
  if (rc == ZNONODE) {
    // Message disappeared (i.e. was cancelled concurrently by another client).
    return;
  }
  assert(rc == ZOK);
  assert(message_buffer_length == message_stat.dataLength);

  batch->Delete(message_path, -1);

  char *pos = message_buffer.Get();
  char *end = message_buffer.Get() + message_buffer_length;
  while (pos < end) {
    char *terminator = std::strchr(pos, ';');
    if (terminator != nullptr) {
      *terminator = '\0';
      batch->Delete(pos, -1);
      pos = terminator + 1;
    } else {
      pos = end;
    }
  }
}

}  // namespace tmb
